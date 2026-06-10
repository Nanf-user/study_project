/**
 * time_display.h — 时间显示模块
 *
 * 在 Screen1 顶部居中创建 LVGL 时间标签，并提供更新函数。
 */

#ifndef TIME_DISPLAY_H
#define TIME_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化时间显示（在 Screen1 顶部居中创建 label）
 *        必须在 ui_init() 之后调用。
 */
void time_display_init(void);

/**
 * @brief 从协议解析模块读取最新时间并更新 label
 *        在主循环中调用，有新数据时更新。
 */
void time_display_update(void);

#ifdef __cplusplus
}
#endif

#endif /* TIME_DISPLAY_H */
