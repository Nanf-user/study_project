/**
 * wifi_config.c — WiFi 凭据存储模块实现
 *
 * 使用 STM32F4 内部 Flash Sector 11 保存 WiFi 凭据。
 * 写入流程: Unlock → Erase Sector → Program Word × N → Lock
 */

#include "wifi_config.h"
#include "stm32f4xx_flash.h"
#include <string.h>

/* ================================================================
 *  简单校验和 (rotate-left + XOR)
 *  只计算 magic + ssid + password 部分，不包含 crc 自身
 * ================================================================ */
static uint32_t calc_checksum(const WifiCredentials *cfg)
{
    uint32_t sum = 0;
    const uint8_t *p = (const uint8_t *)cfg;

    /* crc 字段之前的所有字节 */
    size_t len = (size_t)((uint8_t *)&cfg->crc - (uint8_t *)cfg);
    for (size_t i = 0; i < len; i++) {
        sum = (sum << 1) | (sum >> 31);   /* rotate left 1 bit */
        sum ^= p[i];
    }
    return sum;
}

/* ================================================================
 *  公开接口
 * ================================================================ */

/**
 * @brief 检查是否已有有效凭据
 */
bool wifi_config_has_saved(void)
{
    const WifiCredentials *cfg = (const WifiCredentials *)WIFI_CFG_FLASH_ADDR;

    /* 未写入的 Flash 是 0xFF，magic 肯定不匹配 */
    if (cfg->magic != WIFI_CFG_MAGIC) return false;

    /* 校验 CRC */
    if (cfg->crc != calc_checksum(cfg)) return false;

    return true;
}

/**
 * @brief 读取凭据
 */
bool wifi_config_load(char *ssid, uint32_t ssid_size,
                      char *password, uint32_t pwd_size)
{
    if (!wifi_config_has_saved()) return false;

    const WifiCredentials *cfg = (const WifiCredentials *)WIFI_CFG_FLASH_ADDR;

    strncpy(ssid, cfg->ssid, ssid_size - 1);
    ssid[ssid_size - 1] = '\0';

    strncpy(password, cfg->password, pwd_size - 1);
    password[pwd_size - 1] = '\0';

    return true;
}

/**
 * @brief 保存凭据到 Flash
 */
bool wifi_config_save(const char *ssid, const char *password)
{
    WifiCredentials cfg;

    /* 先全部填 0xFF (模拟擦除态)，然后填充实际数据 */
    memset(&cfg, 0xFF, sizeof(cfg));

    cfg.magic = WIFI_CFG_MAGIC;
    strncpy(cfg.ssid, ssid, sizeof(cfg.ssid) - 1);
    cfg.ssid[sizeof(cfg.ssid) - 1] = '\0';
    strncpy(cfg.password, password, sizeof(cfg.password) - 1);
    cfg.password[sizeof(cfg.password) - 1] = '\0';
    cfg.crc = calc_checksum(&cfg);

    /* Flash 操作 */
    FLASH_Unlock();
    if (FLASH_EraseSector(WIFI_CFG_FLASH_SECTOR, VoltageRange_3) != FLASH_COMPLETE) {
        FLASH_Lock();
        return false;
    }

    /* 逐字写入 */
    uint32_t *src = (uint32_t *)&cfg;
    uint32_t addr = WIFI_CFG_FLASH_ADDR;
    uint32_t num_words = sizeof(WifiCredentials) / 4;

    for (uint32_t i = 0; i < num_words; i++) {
        if (FLASH_ProgramWord(addr + i * 4, src[i]) != FLASH_COMPLETE) {
            FLASH_Lock();
            return false;
        }
    }

    FLASH_Lock();
    return true;
}

/**
 * @brief 清除凭据
 */
void wifi_config_clear(void)
{
    FLASH_Unlock();
    FLASH_EraseSector(WIFI_CFG_FLASH_SECTOR, VoltageRange_3);
    FLASH_Lock();
}

/* ---- JSON 辅助：写入一个转义后的 JSON 字符串值 ---- */
static int json_append_str(char *buf, uint32_t buf_size, int pos, const char *str)
{
    if (pos + 1 >= (int)buf_size) return pos;
    buf[pos++] = '"';

    while (*str) {
        /* 检查缓冲区还剩至少 3 字节（字符 + 转义 + \0） */
        if (pos + 3 >= (int)buf_size) break;

        if (*str == '"') {
            buf[pos++] = '\\';
            buf[pos++] = '"';
        } else if (*str == '\\') {
            buf[pos++] = '\\';
            buf[pos++] = '\\';
        } else {
            buf[pos++] = *str;
        }
        str++;
    }

    if (pos + 1 >= (int)buf_size) return pos;
    buf[pos++] = '"';
    return pos;
}

/**
 * @brief 构建 WiFi 配置 JSON
 *        格式: {"t":"wifi_cfg","ssid":"...","pwd":"..."}\r\n
 */
int wifi_config_build_json(const char *ssid, const char *password,
                           char *buf, uint32_t buf_size)
{
    int pos = 0;

    /* {"t":"wifi_cfg","ssid": */
    const char *prefix = "{\"t\":\"wifi_cfg\",\"ssid\":";
    while (*prefix && pos < (int)buf_size - 1) {
        buf[pos++] = *prefix++;
    }

    /* SSID 值（已转义） */
    pos = json_append_str(buf, buf_size, pos, ssid);

    /* ,"pwd": */
    const char *mid = ",\"pwd\":";
    while (*mid && pos < (int)buf_size - 1) {
        buf[pos++] = *mid++;
    }

    /* 密码值（已转义） */
    pos = json_append_str(buf, buf_size, pos, password);

    /* }\r\n */
    const char *suffix = "}\r\n";
    while (*suffix && pos < (int)buf_size - 1) {
        buf[pos++] = *suffix++;
    }

    buf[pos] = '\0';
    return pos;
}
