#include "UIManager.h"

UIManager::UIManager(WiFiSetup &wifiSetup) : wifiSetup(wifiSetup), main_screen(nullptr), settings_screen(nullptr), status_label(nullptr), connect_btn(nullptr), chart(nullptr), back_btn(nullptr) {}

void UIManager::begin() {
    createMainScreen();
    createSettingsScreen();

    // Load the main screen by default
    lv_scr_load(main_screen);
}

void UIManager::update() {
    // This function can be used to update the UI elements if needed
}

void UIManager::showTouchTest() {
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Touch the screen...");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

    // Create a style for the touch point
    static lv_style_t style_touch_point;
    lv_style_init(&style_touch_point);
    lv_style_set_bg_color(&style_touch_point, lv_color_hex(0xFF0000));
    lv_style_set_border_color(&style_touch_point, lv_color_hex(0xFFFFFF));
    lv_style_set_radius(&style_touch_point, LV_RADIUS_CIRCLE);

    // Create a small object to show the touch point
    lv_obj_t *touch_point = lv_obj_create(lv_scr_act());
    lv_obj_add_style(touch_point, &style_touch_point, 0);
    lv_obj_set_size(touch_point, 20, 20);
    lv_obj_align(touch_point, LV_ALIGN_CENTER, 0, 0);

    while (true) {
        lv_task_handler();
        lv_timer_handler();
        delay(10);

        // Update the touch point position
        if (lv_indev_get_act() != NULL && lv_indev_get_act()->proc.state == LV_INDEV_STATE_PR) {
            lv_indev_data_t data;
            lv_indev_get_act()->driver->read_cb(lv_indev_get_act()->driver, &data);
            lv_obj_set_pos(touch_point, data.point.x - 10, data.point.y - 10); // Center the touch point
        }
    }
}

void UIManager::createMainScreen() {
    main_screen = lv_obj_create(NULL);

    lv_obj_t *label = lv_label_create(main_screen);
    lv_label_set_text(label, "Main Screen");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *setup_btn = lv_btn_create(main_screen);
    lv_obj_set_size(setup_btn, 40, 40);
    lv_obj_align(setup_btn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(setup_btn, setupBtnEventHandler, LV_EVENT_CLICKED, this);
    lv_obj_t *btn_label = lv_label_create(setup_btn);
    lv_label_set_text(btn_label, LV_SYMBOL_SETTINGS);
}

void UIManager::createSettingsScreen() {
    settings_screen = lv_obj_create(NULL);

    // Create back button
    back_btn = lv_btn_create(settings_screen);
    lv_obj_set_size(back_btn, 40, 40);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, backBtnEventHandler, LV_EVENT_CLICKED, this);
    lv_obj_t *btn_label = lv_label_create(back_btn);
    lv_label_set_text(btn_label, LV_SYMBOL_LEFT);

    // Create tabs
    lv_obj_t *tabview = lv_tabview_create(settings_screen, LV_DIR_TOP, 50);
    lv_obj_set_size(tabview, LV_PCT(100), LV_PCT(85)); // Adjust size to fit horizontal mode
    lv_obj_align(tabview, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *tab_wifi = lv_tabview_add_tab(tabview, "WiFi");
    lv_obj_t *tab_inverter = lv_tabview_add_tab(tabview, "Inverter");
    lv_obj_t *tab_sockets = lv_tabview_add_tab(tabview, "Sockets");

    // Add content to tabs (currently empty)
}

void UIManager::connectBtnEventHandler(lv_event_t *e) {
    UIManager *ui = static_cast<UIManager *>(lv_event_get_user_data(e));
    // Attempt to connect to WiFi
    ui->wifiSetup.connectToNetwork("your-ssid", "your-password"); // Replace with actual SSID and password
    if (ui->wifiSetup.isConnected()) {
        lv_label_set_text(ui->status_label, "Connected to WiFi");
    } else {
        lv_label_set_text(ui->status_label, "Failed to connect to WiFi");
    }
}

void UIManager::setupBtnEventHandler(lv_event_t *e) {
    UIManager *ui = static_cast<UIManager *>(lv_event_get_user_data(e));
    lv_scr_load(ui->settings_screen);
}

void UIManager::backBtnEventHandler(lv_event_t *e) {
    UIManager *ui = static_cast<UIManager *>(lv_event_get_user_data(e));
    lv_scr_load(ui->main_screen);
}
