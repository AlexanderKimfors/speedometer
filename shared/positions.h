#ifndef POSITIONS_H
#define POSITIONS_H

#include "config.h"

namespace Positions
{
    inline constexpr int PADDING{20};

    inline constexpr int TEMP_WIDTH{55};
    inline constexpr int TEMP_ICON_HEIGHT{60};
    inline constexpr int TEMP_TEXT_HEIGHT{20};
    inline constexpr int TEMP_X = Config::WINDOW_WIDTH - TEMP_WIDTH - PADDING;
    inline constexpr int TEMP_Y = Config::WINDOW_HEIGHT - TEMP_ICON_HEIGHT - TEMP_TEXT_HEIGHT - PADDING;

    inline constexpr int BATTERY_WIDTH{TEMP_WIDTH};
    inline constexpr int BATTERY_ICON_HEIGHT{110};
    inline constexpr int BATTERY_TEXT_HEIGHT{20};
    inline constexpr int BATTERY_X{TEMP_X};
    inline constexpr int BATTERY_Y{TEMP_Y - PADDING - BATTERY_TEXT_HEIGHT - BATTERY_ICON_HEIGHT};
};

#endif