#ifndef SETTING_H
#define SETTING_H

// overload subscript
// value, key
#define SIGNALS {                     \
    {{8, 0, 0, 240}, "speed"},        \
    {{7, 8, -60, 60}, "temperature"}, \
    {{7, 15, 0, 100}, "battery"},     \
    {{1, 22, 0, 1}, "left_light"},    \
    {{1, 23, 0, 1}, "right_light"}}

#ifdef __cplusplus

#include <string>
#include <map>
#include <stdexcept>

namespace Settings
{

    constexpr int INTERVAL{40};

    struct signal_value
    {
        int bit_size;
        int bit_offset;
        int min;
        int max;

        constexpr signal_value(int bit_size, int bit_offset, int min, int max)
            : bit_size(bit_size), bit_offset(bit_offset), min(min), max(max) {}
    };

    struct signal_type
    {
        signal_value value;
        const char *key;

        constexpr signal_type(signal_value value, const char *key)
            : value(value), key(key) {}
    };

    class Signal
    {
    public:
        /**
         * @brief Get the singleton instance of Settings
         *
         * @return Settings&
         */
        static Signal &getInstance()
        {
            static Signal instance;
            return instance;
        }

        /**
         * @brief get the signal value by key using subscript operator
         *
         * @param key the key of the signal
         * @return const signal_value&
         */
        const signal_value &operator[](const char *key) const
        {
            auto it = signalMap.find(key);
            if (it != signalMap.end())
            {
                return it->second;
            }
            throw std::out_of_range("Signal key not found");
        }

        // Delete copy constructor and assignment operator to enforce singleton pattern
        Signal(const Signal &) = delete;
        void operator=(const Signal &) = delete;

    private:
        // Private constructor to prevent instantiation
        Signal()
        {
            // Initialize the signal map
            for (const auto &signal : signal_list)
            {
                signalMap.insert({signal.key, signal.value});
            }
        };

        constexpr static signal_type signal_list[] = SIGNALS;
        std::map<const char *, const signal_value> signalMap;
    };
}
#endif

#endif
