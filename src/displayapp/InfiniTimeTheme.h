#pragma once

#include <lvgl/lvgl.h>

namespace Colors {
    static constexpr lv_color_t deepOrange = LV_COLOR_MAKE(0xff, 0x40, 0x0);
    static constexpr lv_color_t orange = LV_COLOR_MAKE(0xff, 0xb0, 0x0);
    static constexpr lv_color_t green = LV_COLOR_MAKE(0x0, 0xb0, 0x0);
    static constexpr lv_color_t blue = LV_COLOR_MAKE(0x0, 0x50, 0xff);
    static constexpr lv_color_t lightGray = LV_COLOR_MAKE(0xb0, 0xb0, 0xb0);

    static constexpr lv_color_t bg = LV_COLOR_MAKE(0x5d, 0x69, 0x7e);
    static constexpr lv_color_t bgAlt = LV_COLOR_MAKE(0x38, 0x38, 0x38);
    static constexpr lv_color_t bgDark = LV_COLOR_MAKE(0x18, 0x18, 0x18);
    static constexpr lv_color_t highlight = green;
};

lv_theme_t* lv_pinetime_theme_init();
