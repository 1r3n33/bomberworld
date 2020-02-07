#include "graphics.h"
#include "pilot.h"

// TODO: smaller struct for pilot: spr & y could be 1 byte

struct pilot_t pilots[2];

#define GAMEPLAY_PILOT_ID1_OFFSET 			32
#define GAMEPLAY_PILOT_EOL_ALTITUDE_DROP	8

struct pilot_t * get_pilot(u8 id)
{
    return &pilots[id];
}

void init_pilot(u8 id)
{
    if (id == 0)
    {
        pilots[id].spr = OBJ_PILOT_0;
        pilots[id].x = ((512-16) << 4); // behind the left border
        pilots[id].y = 0;

        // Define sprites parameters
        oamSet(pilots[id].spr, 0xFF, 0xFF, 3, 0, 0, SPR_PILOT_0_FRAME_0, 0);
        oamSetEx(pilots[id].spr, OBJ_LARGE, OBJ_SHOW);
    }
    else
    {
        pilots[id].spr = OBJ_PILOT_1;
        pilots[id].x = ((512+256) << 4); // behind the right border
        pilots[id].y = GAMEPLAY_PILOT_ID1_OFFSET;

        // Define sprites parameters, mirrored.
        oamSet(pilots[id].spr, 0xFF, 0xFF, 3, 1, 0, SPR_PILOT_1_FRAME_0, 0);
        oamSetEx(pilots[id].spr, OBJ_LARGE, OBJ_SHOW);
    }
}

void move_pilot(u8 id, u8 speed)
{
    if (id == 0)
    {
        pilots[id].x += speed;
        if (pilots[id].x >= ((512+256) << 4))
        {
            pilots[id].x = ((512-16) << 4);
            pilots[id].y += GAMEPLAY_PILOT_EOL_ALTITUDE_DROP;
        }
    }
    else
    {
        pilots[id].x -= speed;
        if (pilots[id].x <= ((512-16) << 4))
        {
            pilots[id].x = ((512+256) << 4);
            pilots[id].y += GAMEPLAY_PILOT_EOL_ALTITUDE_DROP;
        }
    }

    oamSetXY(pilots[id].spr, pilots[id].x>>4, pilots[id].y);
}

void set_pilot_pos(u8 id, u16 x, u16 y)
{
    pilots[id].x = (x << 4);
    pilots[id].y = y;
    oamSetXY(pilots[id].spr, x, y);
}

u8 is_pilot_entirely_on_screen(u8 id, u8 off_left, u8 off_right)
{
    u16 x, left, right;

    x = (pilots[id].x>>4);

    left = x + off_left;
    right = x + off_right;

    return (left >= 512) && (right < 512+256);
}
