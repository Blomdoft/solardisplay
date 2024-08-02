#pragma once

#include <lvgl.h>
#include "../logic/WifiSetup.h"

class UIManager {
public:
    UIManager(WiFiSetup &wifiSetup);
    void begin();
    void update();
    void showTouchTest();

private:
    WiFiSetup &wifiSetup;

    // UI elements
    lv_obj_t *main_screen;
    lv_obj_t *settings_screen;
    lv_obj_t *status_label;
    lv_obj_t *connect_btn;
    lv_obj_t *chart;
    lv_obj_t *back_btn;

    void createMainScreen();
    void createSettingsScreen();

    static void connectBtnEventHandler(lv_event_t *e);
    static void setupBtnEventHandler(lv_event_t *e);
    static void backBtnEventHandler(lv_event_t *e);
};
