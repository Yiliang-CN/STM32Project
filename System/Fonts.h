#ifndef __FONTS_H
#define __FONTS_H

typedef struct FONT
{
    const uint8_t *table;
    uint16_t Width;
    uint16_t Height;
} FONT;

extern FONT Font8x16;

#endif
