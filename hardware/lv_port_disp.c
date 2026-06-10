/**
 * @file lv_port_disp.c
 *
 * LVGL v9 display port for STM32F407 + ST7789 SPI LCD.
 * Uses the existing SPL-based mylcd.c driver (ST7789_Cursor + ST7789_WriteColor).
 */

#include "lv_port_disp.h"
#include "mylcd.h"

/*********************
 *      DEFINES
 *********************/

/** Display resolution (portrait, matches USE_HORIZONTAL=0 in mylcd.h) */
#define MY_DISP_HOR_RES  240
#define MY_DISP_VER_RES  320

/**
 * Draw buffer size in bytes.
 * Partial render mode: one buffer of 10 lines.
 * LV_COLOR_FORMAT_GET_SIZE(RGB565) = 2 → 240 * 10 * 2 = 4800 bytes.
 * LV_DISPLAY_RENDER_MODE_PARTIAL requires buffer >= 1/10 of screen area.
 */
#define BYTE_PER_PIXEL LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)
#define MY_DISP_BUF_SIZE (MY_DISP_HOR_RES * 40 * BYTE_PER_PIXEL)

/**********************
 *  STATIC VARIABLES
 **********************/

/** Pixel buffer for partial rendering */
static LV_ATTRIBUTE_MEM_ALIGN uint8_t disp_buf[MY_DISP_BUF_SIZE];

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /* Create display with LVGL v9 API */
    lv_display_t * disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);

    /* Set flush callback that uses the SPL-based ST7789 driver */
    lv_display_set_flush_cb(disp, disp_flush);

    /* Set draw buffers - single buffer, partial render mode */
    lv_display_set_buffers(disp, disp_buf, NULL, sizeof(disp_buf),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Flush callback called by LVGL when a display area needs to be updated.
 *
 * For each dirty area, LVGL calls this with:
 *   - area: the rectangle to update (x1, y1, x2, y2)
 *   - px_map: pixel data in RGB565 format
 *
 * Implementation strategy:
 *   1. Set the ST7789 write window to cover the dirty area
 *   2. Push each pixel into the LCD GRAM via SPI
 *   3. Signal completion to LVGL via lv_display_flush_ready()
 */
static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * px_map)
{
    uint32_t pixel_count = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);

    ST7789_Cursor(area->x1, area->y1, area->x2, area->y2);
    ST7789_BulkWrite((const uint16_t *)px_map, pixel_count);

    lv_display_flush_ready(disp_drv);
}
