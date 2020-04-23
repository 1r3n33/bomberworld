#include <snes.h>
#include "debris.h"
#include "graphics.h"

struct debris_t debris[8];

u8 debris_oam_ids[] =
{
    OBJ_DEBRIS_0,
    OBJ_DEBRIS_1,
    OBJ_DEBRIS_2,
    OBJ_DEBRIS_3,
    OBJ_DEBRIS_4,
    OBJ_DEBRIS_5,
    OBJ_DEBRIS_6,
    OBJ_DEBRIS_7,
    OBJ_DEBRIS_8,
    OBJ_DEBRIS_9,
    OBJ_DEBRIS_10,
    OBJ_DEBRIS_11,
    OBJ_DEBRIS_12,
    OBJ_DEBRIS_13,
    OBJ_DEBRIS_14,
    OBJ_DEBRIS_15,
};

u8 debris_counter = 0;

void reset_debris()
{
    u8 i=0;
    for (i=0; i<16; i++)
    {
        debris[i].x = 256;
        debris[i].y = 256;
        oamSet(debris_oam_ids[i], 0xFF, 0xFF, 3, 0, 0, SPR_DEBRIS, 0);
        oamSetEx(debris_oam_ids[i], OBJ_LARGE, OBJ_SHOW);
        oamSetXY(debris_oam_ids[i], debris[i].x, debris[i].y);
    }
}

void init_debris(u16 x, u16 y)
{
    u8 i = (debris_counter++)&15;

    debris[i].x = x;
    debris[i].y = y;

    oamSetXY(debris_oam_ids[i], debris[i].x, debris[i].y);
}

void update_debris(u8 frame)
{
    u8 i=0;
    for (i=0; i<16; i++)
    {
        if (debris[i].y < 202)
        {
            debris[i].y += 2;
            oamSetXY(debris_oam_ids[i], debris[i].x, debris[i].y);
        }
    }
}
