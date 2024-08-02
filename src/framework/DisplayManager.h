#pragma once

#include "User_Setup.h"
#include <TFT_eSPI.h>
#include <lvgl.h>

#define SLICES 10

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void update();

private:

    const uint16_t screenWidth = 320;
    const uint16_t screenHeight = 480;
    lv_color_t buf[320 * 480 / SLICES];

    TFT_eSPI tft;
    lv_disp_draw_buf_t draw_buf;

    static void displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
};
