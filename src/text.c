#include <snes.h>
#include <string.h>
#include "graphics.h"
#include "text.h"

u16 text_char_id = OBJ_TEXT;

void reset_text()
{
    text_char_id = OBJ_TEXT;

    u16 i=0;
    for (i=0; i<64; i++)
    {
        oamSet(OBJ_TEXT+(i*4), 0xFF, 0xFF, 3, 0, 0, SPR_ASCIITABLE+(' '-32), 1);
        oamSetEx(OBJ_TEXT+(i*4), OBJ_SMALL, OBJ_HIDE);
    }
}

void set_text(char * text, u8 x, u8 y)
{
    u8 len = strlen(text);

    u8 i=0;
    for (i=0; i<len; i++)
    {
        oamSet(text_char_id, x+(i*8), y, 3, 0, 0, SPR_ASCIITABLE+(text[i]-32), 1);
        oamSetEx(text_char_id, OBJ_SMALL, OBJ_SHOW);

        text_char_id += 4;
    }
}
