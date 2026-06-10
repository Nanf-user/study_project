/**
 * wifi_config.h — WiFi 凭据存储模块
 *
 * 将 SSID 和密码保存在 STM32 内部 Flash 的最后一个扇区
 * (Sector 11, 0x080E0000, 128KB)，断电后不丢失。
 */

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Flash 存储地址 ---- */
#define WIFI_CFG_FLASH_ADDR     0x080E0000
#define WIFI_CFG_FLASH_SECTOR   FLASH_Sector_11
#define WIFI_CFG_MAGIC          0x57494649  /* "WIFI" */

/* ---- 凭据结构体 (必须 4 字节对齐) ---- */
typedef struct {
    uint32_t magic;              /* 魔数，标识 "已写入" */
    char     ssid[32];           /* WiFi SSID，最多 31 字符 */
    char     password[64];       /* WiFi 密码，最多 63 字符 */
    uint32_t crc;                /* 校验和 */
} WifiCredentials;

/**
 * @brief 检查是否已有保存的 WiFi 凭据
 * @return true = 有有效凭据
 */
bool wifi_config_has_saved(void);

/**
 * @brief 读取保存的 WiFi 凭据
 * @param ssid       输出 SSID 缓冲区
 * @param ssid_size  缓冲区大小
 * @param password   输出密码缓冲区
 * @param pwd_size   缓冲区大小
 * @return true = 读取成功
 */
bool wifi_config_load(char *ssid, uint32_t ssid_size,
                      char *password, uint32_t pwd_size);

/**
 * @brief 保存 WiFi 凭据到 Flash
 * @param ssid      SSID 字符串
 * @param password  密码字符串
 * @return true = 保存成功
 */
bool wifi_config_save(const char *ssid, const char *password);

/**
 * @brief 清除已保存的凭据（擦除扇区）
 */
void wifi_config_clear(void);

/**
 * @brief 构建发送给 ESP32 的 WiFi 配置 JSON 字符串
 *        自动转义 SSID/密码中的特殊字符 (\" 和 \\).
 * @param ssid      SSID 字符串
 * @param password  密码字符串
 * @param buf       输出缓冲区
 * @param buf_size  缓冲区大小
 * @return 实际写入的 JSON 长度（不含 \\0）
 */
int wifi_config_build_json(const char *ssid, const char *password,
                           char *buf, uint32_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_CONFIG_H */
