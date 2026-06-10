#include "key.h"
#include "delay.h"

// Key hardware initialization
void KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);

    // KEY0 - PE4, pull-down (active high when pressed)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // KEY1, KEY2 - PE3, PE2, pull-up (active low when pressed)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // WK_UP - PA0, pull-down (active high when pressed)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// Key scan function
// mode: 0=no repeat, 1=support repeat
// Priority: KEY0 > WK_UP > KEY1 > KEY2
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1;
    if (mode) key_up = 1;

    if (key_up && (KEY0 == 1 || WK_UP == 1 || KEY1 == 0 || KEY2 == 0))
    {
        delay_ms(20);  // debounce
        key_up = 0;

        if (KEY0 == 1)      return KEY0_PRES;
        else if (WK_UP == 1) return WKUP_PRES;
        else if (KEY1 == 0)  return KEY1_PRES;
        else if (KEY2 == 0)  return KEY2_PRES;
    }
    else if (KEY0 == 0 && WK_UP == 0 && KEY1 == 1 && KEY2 == 1)
        key_up = 1;

    return 0;
}

#if KEY_USE_LVGL_INDEV

static lv_indev_t * key_indev = NULL;

/* LVGL keypad read callback — called ~30ms by LVGL */
static void key_lvgl_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    uint8_t key = KEY_Scan(0);   // 0 = no repeat

    switch (key)
    {
        case KEY0_PRES:  data->key = LV_KEY_ENTER; break;   // Enter/Click
        case WKUP_PRES:  data->key = LV_KEY_ESC;   break;   // Back
        case KEY1_PRES:  data->key = LV_KEY_PREV;  break;   // Previous
        case KEY2_PRES:  data->key = LV_KEY_NEXT;  break;   // Next
        default:         data->key = 0;            break;
    }

    data->state = key ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

/* Register LVGL keypad input device — must be called AFTER lv_init() */
void KEY_LVGL_IndevInit(void)
{
    key_indev = lv_indev_create();
    lv_indev_set_type(key_indev, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(key_indev, key_lvgl_read);
}

lv_indev_t * KEY_GetLvglIndev(void)
{
    return key_indev;
}

#endif /* KEY_USE_LVGL_INDEV */
