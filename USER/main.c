/**
 * @file main.c
 * @brief  LVGL widgets demo with XPT2046 touch input
 */
#include "stm32f4xx.h"
#include "mylcd.h"
#include "delay.h"
#include "touch.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "ui.h"
#include "usart2.h"
#include "protocol_parser.h"
#include "time_display.h"
#include "weather_display.h"
#include "wifi_config.h"
#include <stdio.h>

/**
 * TIM7 — 1ms tick for lv_tick_inc()
 * APB1 timer clock = 84 MHz, Prescaler 83 → 1 MHz, Period 999 → 1 ms
 */
static void TIM7_Init(void)
{
    TIM_TimeBaseInitTypeDef tim;
    NVIC_InitTypeDef nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    tim.TIM_Prescaler         = 84 - 1;
    tim.TIM_CounterMode       = TIM_CounterMode_Up;
    tim.TIM_Period            = 1000 - 1;
    tim.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM7, &tim);

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE);

    nvic.NVIC_IRQChannel                   = TIM7_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority        = 0;
    nvic.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic);
}

int main(void)
{
    delay_init(168);
    TIM7_Init();

    ST7789_Init(WHITE, BLACK);

    lv_init();
    lv_port_disp_init();
    Touch_LVGL_Init();

    ui_init();

    /* 初始化与 ESP32 的通信串口 */
    USART2_Init(9600);
    /* 创建时间显示 label（Screen1 顶部居中） */
    time_display_init();
    /* 创建天气界面（Screen4） */
    weather_display_init();

    /* 开机自动连接 WiFi（如果已保存凭据） */
    {
        char saved_ssid[33], saved_pwd[65];
        if (wifi_config_load(saved_ssid, sizeof(saved_ssid),
                             saved_pwd, sizeof(saved_pwd))) {
            char buf[192];
            wifi_config_build_json(saved_ssid, saved_pwd, buf, sizeof(buf));
            USART2_SendString(buf);
            delay_ms(200);  /* 给 ESP32 一点时间处理 */
        }
    }

    /* 主动向 ESP32 请求天气数据（包含时间） */
    delay_ms(500);
    USART2_SendString("$FETCH\r\n");

    while (1)
    {
        /* 检查 ESP32 是否发来数据 */
        if (USART2_RxComplete()) {
            protocol_parse_line((const char *)USART2_RX_BUF);
            time_display_update();
            weather_display_update();
            USART2_RxReset();
        }

        /* 如果还没有收到时间，每 5 秒重试一次请求 */
        static uint32_t last_fetch = 0;
        if (!protocol_has_time()) {
            uint32_t now = lv_tick_get();
            if (now - last_fetch > 5000) {
                USART2_SendString("$FETCH\r\n");
                last_fetch = now;
            }
        }

        lv_timer_handler();
        delay_ms(1);
    }
}






