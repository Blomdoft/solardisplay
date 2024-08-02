#include <Arduino.h>
#include "logic/WiFiSetup.h"
#include "logic/Fronius.h"
#include "logic/Hue.h"
#include "framework/Logger.h"
#include "framework/DisplayManager.h"
#include "framework/touch.h"
#include "ui/UIManager.h"
#include "lv_conf.h"

DisplayManager displayManager;

WiFiSetup wifi;
Fronius fronius;
Hue hue;

UIManager uiManager(wifi);

void setup() {
    Serial.begin(115200);
    delay(1000); // Give some time for the Serial to initialize
    
    // original touch code
    gt911_int_();

    displayManager.begin();

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    wifi.begin();
    fronius.begin();
    hue.begin();

    uiManager.begin();
}

void loop() {
    
    displayManager.update();
    uiManager.update();
    wifi.loop();
    fronius.loop();
    hue.loop();

    // Delay between loops
    delay(10);
}


