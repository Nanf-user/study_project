/**
 * weather_display.c — 天气显示模块实现
 *
 * 依赖: LVGL 9.3, Screen4, protocol_parser
 *
 * 注意: 当前 LCD 字体（Montserrat）不支持中文。
 *       如需中文显示，需要用 SquareLine Studio 导出包含
 *       "晴多云阴雨雪雾霾"等汉字的 LVGL 字体文件。
 */

#include "weather_display.h"
#include "protocol_parser.h"
#include "ui.h"
#include "lvgl.h"
#include <string.h>
#include <stdio.h>

/* ================================================================
 *  天气图标映射表
 *  TODO: 图片需要用 SquareLine Studio 导入 PNG 并导出，
 *        这样生成的 .c 文件带 LV_COLOR_FORMAT_NATIVE_WITH_ALPHA
 *        才能正确显示透明背景的天气图标。
 *        在线转换器生成的 RGB565 不带 alpha，透明部分会变黑。
 * ================================================================ */
typedef struct {
    const char            *weather_en;
    const lv_image_dsc_t  *image;
} WeatherIconEntry;

static const WeatherIconEntry weather_icons[] = {
    {"Sunny",      &ui_img_image_qing_png        },  /* Sunny = qing.png */
    /* 继续用 SquareLine 导入更多天气图标后添加:*/
    {"Cloudy",     &ui_img_image_duoyun_png    },
    
    { NULL, NULL }
};

static const lv_image_dsc_t * lookup_weather_icon(const char *weather_en)
{
    for (int i = 0; weather_icons[i].weather_en != NULL; i++) {
        if (strstr(weather_en, weather_icons[i].weather_en)) {
            return weather_icons[i].image;
        }
    }
    return &ui_img_image_xiaoyu_png;  /* 默认图 */
}

/* ================================================================
 *  内部 UI 对象句柄
 * ================================================================ */
static lv_obj_t *city_label        = NULL;
static lv_obj_t *weather_img       = NULL;
static lv_obj_t *temp_label        = NULL;
static lv_obj_t *weather_label     = NULL;  /* 天气描述（英文，Montserrat 可显示） */
static lv_obj_t *humidity_label    = NULL;
static lv_obj_t *rssi_label        = NULL;
static lv_obj_t *update_time_label = NULL;

/* ---- 辅助 ---- */
static lv_obj_t * create_label(lv_obj_t *parent, const char *text,
                                const lv_font_t *font, lv_color_t color,
                                lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, color, LV_PART_MAIN);
    lv_obj_align(label, align, x_ofs, y_ofs);
    lv_label_set_text(label, text);
    return label;
}

/* ================================================================
 *  初始化
 * ================================================================ */
void weather_display_init(void)
{
    if (ui_Screen4 == NULL) return;

    /* 白色背景 */
    lv_obj_set_style_bg_color(ui_Screen4, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_Screen4, 255, LV_PART_MAIN);

    /* 删除 SquareLine 旧图片 */
    if (ui_Image3 != NULL) {
        lv_obj_del(ui_Image3);
        ui_Image3 = NULL;
    }

    /* 城市名 */
    city_label = create_label(ui_Screen4, "----",
                              &lv_font_montserrat_18,
                              lv_color_hex(0x333333),
                              LV_ALIGN_TOP_MID, 0, 12);

    /* 天气图标 */
    weather_img = lv_image_create(ui_Screen4);
    lv_image_set_src(weather_img, &ui_img_image_qing_png);
    lv_obj_set_width(weather_img, 100);
    lv_obj_set_height(weather_img, 100);
    lv_obj_align(weather_img, LV_ALIGN_CENTER, 0, -70);

    /* 温度 */
    temp_label = create_label(ui_Screen4, "--\xc2\xb0",
                              &lv_font_montserrat_32,
                              lv_color_hex(0x222222),
                              LV_ALIGN_CENTER, 0, 35);

    /* 天气描述 — 18号英文 */
    weather_label = create_label(ui_Screen4, "----",
                                  &lv_font_montserrat_18,
                                  lv_color_hex(0x444444),
                                  LV_ALIGN_CENTER, 0, 75);

    /* 湿度 */
    humidity_label = create_label(ui_Screen4, "Humidity: --%",
                                   &lv_font_montserrat_14,
                                   lv_color_hex(0x666666),
                                   LV_ALIGN_CENTER, 0, 110);

    /* WiFi 信号 */
    rssi_label = create_label(ui_Screen4, "WiFi: -- dBm",
                               &lv_font_montserrat_14,
                               lv_color_hex(0x666666),
                               LV_ALIGN_CENTER, 0, 128);

    /* 更新时间 */
    update_time_label = create_label(ui_Screen4, "Updated: --:--",
                                      &lv_font_montserrat_14,
                                      lv_color_hex(0xAAAAAA),
                                      LV_ALIGN_BOTTOM_MID, 0, -8);
}

/* ================================================================
 *  更新
 * ================================================================ */
void weather_display_update(void)
{
    const ParsedWeather *w = protocol_get_weather();
    if (!w || !w->valid) return;

    char buf[64];

    /* 城市名 */
    if (city_label && w->city[0]) {
        lv_label_set_text(city_label, w->city);
    }

    /* 天气图标 */
    if (weather_img && w->weather_en[0]) {
        lv_image_set_src(weather_img, lookup_weather_icon(w->weather_en));
    }

    /* 温度 */
    if (temp_label) {
        sprintf(buf, "%d\xc2\xb0", w->temperature);
        lv_label_set_text(temp_label, buf);
    }

    /* 天气英文 */
    if (weather_label && w->weather_en[0]) {
        lv_label_set_text(weather_label, w->weather_en);
    }

    /* 湿度 */
    if (humidity_label && w->humidity > 0) {
        sprintf(buf, "Humidity: %d%%", w->humidity);
        lv_label_set_text(humidity_label, buf);
    }

    /* WiFi */
    if (rssi_label) {
        sprintf(buf, "WiFi: %d dBm", w->rssi);
        lv_label_set_text(rssi_label, buf);
    }

    /* 更新时间 */
    if (update_time_label && w->time_str[0]) {
        sprintf(buf, "Updated: %s", w->time_str);
        lv_label_set_text(update_time_label, buf);
    }
}
