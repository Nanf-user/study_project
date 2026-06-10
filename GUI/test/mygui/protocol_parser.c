/**
 * protocol_parser.c — ESP32 串口 JSON 协议解析实现
 *
 * 轻量级实现，不依赖第三方 JSON 库。
 * 用 strstr 定位关键字段再提取值。
 */

#include "protocol_parser.h"
#include <stdio.h>
#include <string.h>

/* ---- 全局存储 ---- */
static ParsedWeather g_weather;
static ParsedStatus  g_status;
static bool          g_has_time = false;
static int           g_wifi_cfg_result = 0;  /* 0=未收到, 1=ok, -1=fail */

/* ---- 辅助：从 JSON 中提取一对引号内的值 ---- */
static bool extract_str(const char *json, const char *key, char *out, size_t out_len)
{
    char search[64];
    sprintf(search, "\"%s\":\"", key);

    const char *pos = strstr(json, search);
    if (!pos) return false;

    pos += strlen(search);          // 跳过 "key":"
    size_t i = 0;
    while (*pos && *pos != '"' && i < out_len - 1) {
        out[i++] = *pos++;
    }
    out[i] = '\0';
    return true;
}

/* ---- 辅助：从 JSON 中提取整数值 ---- */
static bool extract_int(const char *json, const char *key, int *out)
{
    char search[64];
    sprintf(search, "\"%s\":", key);

    const char *pos = strstr(json, search);
    if (!pos) return false;

    pos += strlen(search);
    *out = atoi(pos);
    return true;
}

/* ---- 核心：解析一行 JSON ---- */
void protocol_parse_line(const char *json)
{
    if (!json || strlen(json) < 10) return;

    /* 识别包类型 */
    if (strstr(json, "\"t\":\"w\"")) {
        /* ---- 天气包 ---- */
        memset(&g_weather, 0, sizeof(g_weather));

        if (extract_str(json, "time", g_weather.time_str, sizeof(g_weather.time_str))) {
            g_has_time = true;
        }
        extract_str(json, "city", g_weather.city, sizeof(g_weather.city));
        extract_str(json, "wth",  g_weather.weather_en, sizeof(g_weather.weather_en));

        int tmp;
        if (extract_int(json, "tmp", &tmp)) {
            g_weather.temperature = tmp;
        }
        int hum;
        if (extract_int(json, "hum", &hum)) {
            g_weather.humidity = hum;
        }
        int rssi;
        if (extract_int(json, "rssi", &rssi)) {
            g_weather.rssi = rssi;
        }
        int wifi;
        if (extract_int(json, "wifi", &wifi)) {
            g_weather.wifi_connected = (wifi != 0);
        }

        g_weather.valid = true;

    } else if (strstr(json, "\"t\":\"s\"")) {
        /* ---- 状态包 ---- */
        memset(&g_status, 0, sizeof(g_status));

        int wifi;
        if (extract_int(json, "wifi", &wifi)) {
            g_status.wifi_connected = (wifi != 0);
        }
        extract_str(json, "ip", g_status.ip, sizeof(g_status.ip));
        extract_str(json, "fw", g_status.fw_version, sizeof(g_status.fw_version));

        int uptime;
        if (extract_int(json, "uptime", (int *)&g_status.uptime)) {
            // uptime 已赋值
        }
        g_status.valid = true;

        // 状态包不包含时间，不更新 g_has_time
    } else if (strstr(json, "\"t\":\"wifi_status\"")) {
        /* ---- WiFi 配置结果包 ---- */
        char result[8] = {0};
        if (extract_str(json, "result", result, sizeof(result))) {
            if (strcmp(result, "ok") == 0) {
                g_wifi_cfg_result = 1;
            } else {
                g_wifi_cfg_result = -1;
            }
        }
    }
}

/* ---- 访问器 ---- */
const char* protocol_get_time(void)
{
    return g_weather.time_str;
}

bool protocol_has_time(void)
{
    return g_has_time;
}

const ParsedWeather* protocol_get_weather(void)
{
    return &g_weather;
}

const ParsedStatus* protocol_get_status(void)
{
    return &g_status;
}

int protocol_get_wifi_cfg_result(void)
{
    return g_wifi_cfg_result;
}

void protocol_clear_wifi_cfg_result(void)
{
    g_wifi_cfg_result = 0;
}
