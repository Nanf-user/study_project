#ifndef LV_PORT_DISP_H
#define LV_PORT_DISP_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the LVGL display driver with the ST7789 LCD.
 * Must call ST7789_Init() and lv_init() before this function.
 * Configures a single-buffer partial-render display (240x320).
 */
void lv_port_disp_init(void);

#ifdef __cplusplus
}
#endif

#endif /* LV_PORT_DISP_H */
