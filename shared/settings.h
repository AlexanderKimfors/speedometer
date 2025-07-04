#ifndef SETTINGS_H
#define SETTINGS_H

#ifdef __cplusplus

#include <string>
#include <map>
#include <stdexcept>
#include <climits>

#define SIGNALS {                     \
    {{8, 0, 0, 240}, "speed"},        \
    {{7, 8, -60, 60}, "temperature"}, \
    {{7, 15, 0, 100}, "battery"},     \
    {{1, 22, 0, 1}, "left_light"},    \
    {{1, 23, 0, 1}, "right_light"}}
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

    template <size_t Size>
    static constexpr size_t buffer_size_calculator(const signal_type (&arr)[Size])
    {
        size_t sum_bits{0};
        for (signal_type signal : arr)
        {
            sum_bits += signal.value.bit_size;
        }

        size_t sum_bytes{(sum_bits / CHAR_BIT) + ((sum_bits % CHAR_BIT != 0) ? 1 : 0)};

        return sum_bytes;
    }

    class Signal
    {
    private:
        static constexpr signal_type signal_list[] = SIGNALS;
        std::map<const char *, const signal_value> signalMap;

        // Private constructor to prevent instantiation
        Signal()
        {
            // Initialize the signal map
            for (const auto &signal : signal_list)
            {
                signalMap.insert({signal.key, signal.value});
            }
        };
        // Delete copy constructor and assignment operator to enforce singleton pattern
        Signal(const Signal &) = delete;
        void operator=(const Signal &) = delete;

    public:
        static constexpr size_t BUFFER_SIZE{buffer_size_calculator(signal_list)};

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
    };
}
#endif

#endif
