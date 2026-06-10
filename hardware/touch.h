#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "sys.h"
#include "lvgl.h"

/* XPT2046 pin definitions — all on GPIOC */
#define TP_PEN  PCin(1)    /* PC1  Touch IRQ (active low) */
#define TP_DOUT PCin(2)    /* PC2  MISO */
#define TP_TDIN PCout(3)   /* PC3  MOSI */
#define TP_TCLK PCout(0)   /* PC0  SCLK */
#define TP_TCS  PCout(4)   /* PC4  CS   */

/* XPT2046 commands — swapped for this panel's orientation */
#define CMD_RDX  0x90    /* reads Y */
#define CMD_RDY  0xD0    /* reads X */

/* Calibration: both axes inverted, raw range ~230-4000 */
#define TP_XFAC  (-0.064f)
#define TP_XOFF  254
#define TP_YFAC  (-0.085f)
#define TP_YOFF  339

void Touch_Init(void);
void Touch_LVGL_Init(void);
u16  tp_read_xoy(u8 cmd);   /* exposed for direct test */

#endif
