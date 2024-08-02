#ifndef TOUCH_H
#define TOUCH_H

#include "Arduino.h"
#include "lvgl.h"

// I2C读写命令
#define GT_CMD_WR           0XBA         // 写命令0xBA
#define GT_CMD_RD           0XBB         // 读命令0XBB

#define GT911_MAX_WIDTH     320          // Touchscreen pad max width
#define GT911_MAX_HEIGHT    480          // Touchscreen pad max height

// GT911 部分寄存器定义
#define GT_CTRL_REG         0X8040       // GT911控制寄存器
#define GT_CFGS_REG         0X8047       // GT911配置起始地址寄存器
#define GT_CHECK_REG        0X80FF       // GT911校验和寄存器
#define GT_PID_REG          0X8140       // GT911产品ID寄存器

#define GT_GSTID_REG        0X814E       // GT911当前检测到的触摸情况
#define GT911_READ_XY_REG   0x814E       // 坐标寄存器
#define CT_MAX_TOUCH        5            // 电容触摸屏最大支持的点数

// Declare the variables as extern
extern int IIC_SCL;
extern int IIC_SDA;
extern int IIC_RST;

#define IIC_SCL_0  digitalWrite(IIC_SCL,LOW)
#define IIC_SCL_1  digitalWrite(IIC_SCL,HIGH)

#define IIC_SDA_0  digitalWrite(IIC_SDA,LOW)
#define IIC_SDA_1  digitalWrite(IIC_SDA,HIGH)

#define IIC_RST_0  digitalWrite(IIC_RST,LOW)
#define IIC_RST_1  digitalWrite(IIC_RST,HIGH)

#define READ_SDA   digitalRead(IIC_SDA)

typedef struct {
    uint8_t Touch;
    uint8_t TouchpointFlag;
    uint8_t TouchCount;
    uint8_t Touchkeytrackid[CT_MAX_TOUCH];
    uint16_t X[CT_MAX_TOUCH];
    uint16_t Y[CT_MAX_TOUCH];
    uint16_t S[CT_MAX_TOUCH];
} GT911_Dev;

extern GT911_Dev Dev_Now, Dev_Backup;
extern uint8_t s_GT911_CfgParams[];
extern const uint8_t GT9111_CFG_TBL[];

void gt911_int_();

void GT911_Int();
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

#endif // TOUCH_H
