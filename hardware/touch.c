/**
 * XPT2046 touch driver for STM32F407 + LVGL v9
 * Adapted from QDtech F103 example — GPIO on GPIOC
 */
#include "touch.h"
#include "delay.h"
#include <stdio.h>

/*--- hardware SPI bit-bang (compatible with any GPIO) ---*/

/* NOP delay for XPT2046 timing (~200ns @168MHz) */
static void tp_delay(void) {
    volatile int i;
    for (i = 0; i < 5; i++) __NOP();
}

static void tp_write_byte(u8 num)
{
    u8 count;
    for (count = 0; count < 8; count++) {
        if (num & 0x80) TP_TDIN = 1;
        else            TP_TDIN = 0;
        num <<= 1;
        tp_delay();
        TP_TCLK = 0;
        tp_delay();
        TP_TCLK = 1;          /* data latched on rising edge */
    }
    tp_delay();
}

static u16 tp_read_ad(u8 cmd)
{
    u8 count;
    u16 num = 0;

    TP_TCLK = 0;
    TP_TDIN = 0;
    tp_delay();
    TP_TCS  = 0;              /* select XPT2046 */
    tp_delay();
    tp_write_byte(cmd);
    delay_us(10);             /* ADC conversion time (need ~6us min) */

    TP_TCLK = 0;
    delay_us(2);
    TP_TCLK = 1;              /* dummy clock to clear BUSY */
    tp_delay();
    TP_TCLK = 0;
    tp_delay();

    for (count = 0; count < 16; count++) {
        num <<= 1;
        TP_TCLK = 0;
        tp_delay();
        TP_TCLK = 1;
        tp_delay();
        if (TP_DOUT) num++;
    }
    num >>= 4;                /* keep upper 12 bits */
    TP_TCS = 1;               /* deselect */
    tp_delay();
    return num;
}

/*--- filtered coordinate read (median of 5, discard 2 outliers) ---*/

#define READ_TIMES  5
#define LOST_VAL    1

u16 tp_read_xoy(u8 cmd)
{
    u16 i, j;
    u16 buf[READ_TIMES];
    u16 sum = 0;
    u16 temp;

    for (i = 0; i < READ_TIMES; i++) buf[i] = tp_read_ad(cmd);

    /* bubble sort */
    for (i = 0; i < READ_TIMES - 1; i++) {
        for (j = i + 1; j < READ_TIMES; j++) {
            if (buf[i] > buf[j]) {
                temp = buf[i]; buf[i] = buf[j]; buf[j] = temp;
            }
        }
    }
    /* average middle values */
    for (i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++) sum += buf[i];
    return sum / (READ_TIMES - 2 * LOST_VAL);
}

/* double-sample with jitter check */
static u8 tp_read_xy(u16 *x, u16 *y)
{
    u16 x1, y1, x2, y2;

    x1 = tp_read_xoy(CMD_RDX); y1 = tp_read_xoy(CMD_RDY);
    x2 = tp_read_xoy(CMD_RDX); y2 = tp_read_xoy(CMD_RDY);

    if ((x1 > x2 ? x1 - x2 : x2 - x1) > 80) return 0;
    if ((y1 > y2 ? y1 - y2 : y2 - y1) > 80) return 0;

    *x = (x1 + x2) / 2;
    *y = (y1 + y2) / 2;
    return 1;
}

/*--- GPIO init for F407 ---*/

void Touch_Init(void)
{
    GPIO_InitTypeDef gpio;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* CLK(PC0), MOSI(PC3), CS(PC13) — push-pull outputs */
    gpio.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gpio);

    /* MISO(PC2), IRQ(PC1) — inputs with pull-up */
    gpio.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2;
    gpio.GPIO_Mode  = GPIO_Mode_IN;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gpio);
}

/*--- LVGL pointer indev ---*/

static lv_indev_t * touch_indev = NULL;

static void touch_lvgl_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    static u16 last_x = 0, last_y = 0;

    if (TP_PEN == 0) {                     /* PEN low = pressed */
        u16 raw_x = tp_read_xoy(CMD_RDX); /* median-filtered read */
        u16 raw_y = tp_read_xoy(CMD_RDY);

        last_x = (u16)(raw_x * TP_XFAC + TP_XOFF);
        last_y = (u16)(raw_y * TP_YFAC + TP_YOFF);
        data->point.x = last_x;
        data->point.y = last_y;
        data->state   = LV_INDEV_STATE_PRESSED;
    } else {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state   = LV_INDEV_STATE_RELEASED;
    }
}

void Touch_LVGL_Init(void)
{
    Touch_Init();
    touch_indev = lv_indev_create();
    lv_indev_set_type(touch_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(touch_indev, touch_lvgl_read);
}
