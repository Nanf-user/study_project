/**
 * protocol_parser.h — ESP32 串口 JSON 协议解析
 *
 * 解析 ESP32 通过 USART2 发送的 JSON 数据包：
 *   天气: {"t":"w","city":"...","tmp":28,"wth":"...","hum":65,"time":"2026-06-03 14:30","rssi":-45,"wifi":1}
 *   状态: {"t":"s","wifi":1,"ip":"...","uptime":3600,"fw":"1.0.0"}
 */

#ifndef PROTOCOL_PARSER_H
#define PROTOCOL_PARSER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- 天气数据结构 ---- */
typedef struct {
    char city[32];
    char weather_cn[16];
    char weather_en[16];
    int  temperature;
    int  humidity;
    char time_str[20];      // "YYYY-MM-DD HH:MM"
    int  rssi;
    bool wifi_connected;
    bool valid;
} ParsedWeather;

/* ---- 状态数据结构 ---- */
typedef struct {
    bool wifi_connected;
    char ip[16];
    unsigned long uptime;
    char fw_version[8];
    bool valid;
} ParsedStatus;

/**
 * @brief 解析一行 JSON 数据，自动识别类型并填充对应结构体
 * @param json 以 \0 结尾的 JSON 字符串（不含 \r\n）
 */
void protocol_parse_line(const char *json);

/**
 * @brief 获取最近一次解析的时间字符串
 * @return "YYYY-MM-DD HH:MM" 或 ""（无数据）
 */
const char* protocol_get_time(void);

/**
 * @brief 是否有有效的时间数据
 */
bool protocol_has_time(void);

/**
 * @brief 获取最近一次解析的天气数据（只读）
 */
const ParsedWeather* protocol_get_weather(void);

/**
 * @brief 获取最近一次解析的状态数据（只读）
 */
const ParsedStatus* protocol_get_status(void);

/**
 * @brief 获取 WiFi 配置结果
 * @return 0=未收到, 1=连接成功, -1=连接失败
 */
int protocol_get_wifi_cfg_result(void);

/**
 * @brief 清除 WiFi 配置结果（用于重新连接时重置状态）
 */
void protocol_clear_wifi_cfg_result(void);

#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_PARSER_H */
