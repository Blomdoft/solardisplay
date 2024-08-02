#include "DisplayManager.h"

DisplayManager::DisplayManager() : tft(TFT_eSPI()) {}

void DisplayManager::begin() {
    Serial.println("Initializing display...");
    tft.begin();
    tft.setRotation(1); // Set rotation to horizontal mode

    lv_init();

    Serial.println("Initializing LVGL draw buffer...");
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / SLICES);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenHeight; // Adjust horizontal resolution
    disp_drv.ver_res = screenWidth;  // Adjust vertical resolution
    disp_drv.flush_cb = displayFlush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.user_data = &tft;
    lv_disp_drv_register(&disp_drv);

    Serial.println("Display initialization complete.");
}

void DisplayManager::update() {
    lv_timer_handler();
}

void DisplayManager::displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    TFT_eSPI &tft = *(TFT_eSPI *)disp->user_data;
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}
