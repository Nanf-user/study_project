#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

/* Key pin definitions */
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define WK_UP 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2

/* Key return values */
#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

void KEY_Init(void);
u8 KEY_Scan(u8);

/* LVGL input device adapter (optional) */
#define KEY_USE_LVGL_INDEV 1

#if KEY_USE_LVGL_INDEV
#include "lvgl.h"
void KEY_LVGL_IndevInit(void);
lv_indev_t * KEY_GetLvglIndev(void);
#endif

#endif
