/**
 * time_display.c — 时间显示模块实现
 *
 * 依赖: LVGL 9.3, Screen1 和 protocol_parser
 */

#include "time_display.h"
#include "protocol_parser.h"
#include "ui.h"                     /* ui_Screen1 等全局变量 */
#include "lvgl.h"

/* ---- 内部变量 ---- */
static lv_obj_t *time_label = NULL;

/* ---- 初始化 ---- */
void time_display_init(void)
{
    if (ui_Screen1 == NULL) return;

    /* 在 Screen1 顶部居中创建时间 label */
    time_label = lv_label_create(ui_Screen1);

    /* 样式: 18号字体，白色文字 */
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_18, LV_PART_MAIN);
    lv_obj_set_style_text_color(time_label, lv_color_black(), LV_PART_MAIN);

    /* 顶部居中，y 偏移 10px */
    lv_obj_align(time_label, LV_ALIGN_TOP_MID, 0, 10);

    /* 初始文本（无数据时显示） */
    lv_label_set_text(time_label, "--:--");
}

/* ---- 更新 ---- */
void time_display_update(void)
{
    if (time_label == NULL) return;

    if (protocol_has_time()) {
        lv_label_set_text(time_label, protocol_get_time());
    }
}
