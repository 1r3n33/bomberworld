#include <snes.h>
#include <string.h>
#include "graphics.h"
#include "text.h"

void reset_text()
{
    u16 text_char_id = OBJ_TEXT;

    u16 i=0;
    for (i=0; i<64; i++)
    {
        oamSet(OBJ_TEXT+(i*4), 0xFF, 0xFF, 3, 0, 0, SPR_ASCIITABLE+(' '-32), 1);
        oamSetEx(OBJ_TEXT+(i*4), OBJ_SMALL, OBJ_HIDE);
    }
}

void set_text(u16 id, u8 * text, u8 x, u8 y)
{
    u8 len = strlen(text);

    u8 i=0;
    for (i=0; i<len; i++)
    {
        oamSet(id, x+(i*8), y, 3, 0, 0, SPR_ASCIITABLE+(text[i]-32), 1);
        oamSetEx(id, OBJ_SMALL, OBJ_SHOW);

        id += 4;
    }
}

void set_text_number(u16 id, u16 number, u8 x, u8 y)
{
    char text[8];

    u8 i = 7;
    do
    {
        u16 div = number/10;
        u16 rem = number-(div*10);

        oamSet(id, x+(i*8), y, 3, 0, 0, SPR_ASCIITABLE+((48+rem)-32), 1);
        oamSetEx(id, OBJ_SMALL, OBJ_SHOW);
        id += 4;
        i--;

        number = div;

    } while ((number > 0) && (i != 0xFF));

    while (i != 0xFF)
    {
        oamSet(id, x+(i*8), y, 3, 0, 0, SPR_ASCIITABLE+('0'-32), 1);
        oamSetEx(id, OBJ_SMALL, OBJ_SHOW);
        id += 4;
        i--;
    }
}
