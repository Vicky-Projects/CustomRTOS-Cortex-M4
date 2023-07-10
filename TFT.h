
#ifndef __tft_H
#define __tft_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "Fonts.h"
#include "stm32f4xx_hal.h"
#include "string.h"
#include "Functions.h"
#include "USER_settings.h"
#include "stdlib.h"
#include "BSP.h"



//  
#define true	1
#define false	0




#define MIPI_DCS_REV1   (1<<0)
#define AUTO_READINC    (1<<1)
#define READ_BGR        (1<<2)
#define READ_LOWHIGH    (1<<3)
#define READ_24BITS     (1<<4)
#define XSA_XEA_16BIT   (1<<5)
#define READ_NODUMMY    (1<<6)
#define INVERT_GS       (1<<8)
#define INVERT_SS       (1<<9)
#define MV_AXIS         (1<<10)
#define INVERT_RGB      (1<<11)
#define REV_SCREEN      (1<<12)
#define FLIP_VERT       (1<<13)
#define FLIP_HORIZ      (1<<14)





void tft_init(uint16_t ID);
void reset(void);
uint16_t readID(void);

void setRotation(uint8_t r);
void invertDisplay(uint8_t i);
void vertScroll(int16_t top, int16_t scrollines, int16_t offset);



void setFont(const GFXfont *f);

void setTextWrap(uint8_t w);

void setTextColor (uint16_t color);

void setTextSize (uint8_t size);

void setCursor(int16_t x, int16_t y);

void printnewtstr (int row, int column, uint16_t txtcolor, const GFXfont *f, uint8_t txtsize, uint8_t *str);

void printstr (uint8_t *str);
void TFT_Setup(void);


#ifdef __cplusplus
}
#endif

#endif /* __HX8347G_H */

