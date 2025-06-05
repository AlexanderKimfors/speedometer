#ifndef SETTING_H
#define SETTING_H

// overload subscript
// value, key
#define SIGNALS {                     \
    {{8, 0, 0, 240}, "speed"},        \
    {{7, 8, -60, 60}, "temperature"}, \
    {{7, 15, 0, 100}, "battery"},     \
    {{1, 22, 0, 1}, "left_light"},    \
    {{1, 23, 0, 1}, "right_light"},   \
}

#define NUM_SIGNALS 5

#define BUFFLEN 3
#define BAUDRATE 1048576

#ifdef __cplusplus

#include <string>
#include <map>

namespace settings {
    struct signal_value
        {
        int bit_size;
        int bit_offset;
        int min;
        int max;
    };

    struct signal_type {
        signal_value value;
        const char* key;
    };

    class Settings {
        public:

        /**
         * @brief Get the singleton instance of Settings
         * 
         * @return Settings& 
         */
        static Settings& getInstance() {
            static Settings instance;
            return instance;
        }

        /**
         * @brief get the signal value by key using subscript operator
         * 
         * @param key the key of the signal
         * @return const signal_value& 
         */
        const signal_value& operator[](const char* key) const {
            auto it = signalMap.find(key);
            if (it != signalMap.end()) {
                return it->second;
            }
            throw std::out_of_range("Signal key not found");
        }
        
        // Delete copy constructor and assignment operator to enforce singleton pattern
        Settings(const Settings&) = delete;
        void operator=(const Settings&) = delete;

        private:
        // Private constructor to prevent instantiation
        Settings() {
            // Initialize the signal map
            for (int i = 0; i < NUM_SIGNALS; ++i) {
                signalMap.insert({signal_list[i].key, signal_list[i].value});
            }
        };

        signal_type const signal_list[NUM_SIGNALS] = SIGNALS;
        std::map<const char*, const signal_value> signalMap;
           
    };
}
#endif

#endif
