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

u8 debris_spr_ids[] =
{
    SPR_DEBRIS_CITY,
    SPR_DEBRIS_EGYPT,
    SPR_DEBRIS_SHIP,
    SPR_DEBRIS_CITY,
    SPR_DEBRIS_CITY,
    SPR_DEBRIS_CITY,
};

u8 debris_counter = 0;

void reset_debris(u8 level)
{
    u8 i=0;
    for (i=0; i<16; i++)
    {
        debris[i].x = SCREEN_WIDTH;
        debris[i].y = SCREEN_HEIGHT;
        oamSet(debris_oam_ids[i], 0xFF, 0xFF, 3, 0, 0, debris_spr_ids[level], 0);
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

void update_debris(u8 frame, u8 ground_y_pos)
{
    u8 i = 0;

    // If there is an actual ground stop a little before for better looking vfx.
    u8 y = ground_y_pos == SCREEN_HEIGHT ? SCREEN_HEIGHT : ground_y_pos-6;

    for (i=0; i<16; i++)
    {
        if (debris[i].y < ground_y_pos)
        {
            debris[i].y += 2;
            oamSetXY(debris_oam_ids[i], debris[i].x, debris[i].y);
        }
    }
}
