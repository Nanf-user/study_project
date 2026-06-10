/**
 * weather_display.h — 天气显示模块
 *
 * 在 Screen4 上创建手机 App 风格的天气界面，
 * 并根据 ESP32 发来的天气数据动态更新。
 */

#ifndef WEATHER_DISPLAY_H
#define WEATHER_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化天气 UI（在 Screen4 上创建标签和图片）
 *        必须在 ui_init() 之后调用。
 */
void weather_display_init(void);

/**
 * @brief 从 protocol_parser 读取最新天气数据并更新所有 UI
 *        在主循环中调用。
 */
void weather_display_update(void);

#ifdef __cplusplus
}
#endif

#endif /* WEATHER_DISPLAY_H */
