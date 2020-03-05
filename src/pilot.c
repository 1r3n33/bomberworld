#include "graphics.h"
#include "pilot.h"

// TODO: smaller struct for pilot: spr & y could be 1 byte

struct pilot_t pilots[2];

#define GAMEPLAY_PILOT_ID1_OFFSET 			0
#define GAMEPLAY_PILOT_EOL_ALTITUDE_DROP	8

struct pilot_t * get_pilot(u8 id)
{
    return &pilots[id];
}

int drop_count = 0;

void init_pilot(u8 id)
{
    if (id == 0)
    {
        pilots[id].spr = OBJ_PILOT_0;
        pilots[id].x = ((512-16) << 4); // behind the left border
        pilots[id].y_baseline = 16;
        pilots[id].y = pilots[id].y_baseline;
        pilots[id].hovering_count = 1;

        // Define sprites parameters
        oamSet(pilots[id].spr, 0xFF, 0xFF, 3, 0, 0, SPR_PILOT_0_FRAME_0, 0);
        oamSetEx(pilots[id].spr, OBJ_LARGE, OBJ_SHOW);
    }
    else
    {
        pilots[id].spr = OBJ_PILOT_1;
        pilots[id].x = ((512+256) << 4); // behind the right border
        pilots[id].y_baseline = 16;
        pilots[id].y = pilots[id].y_baseline;
        pilots[id].hovering_count = 0;

        // Define sprites parameters, mirrored.
        oamSet(pilots[id].spr, 0xFF, 0xFF, 3, 1, 0, SPR_PILOT_1_FRAME_0, 0);
        oamSetEx(pilots[id].spr, OBJ_LARGE, OBJ_SHOW);
    }
}

void move_pilot(u8 id, u8 speed, u8 second_player)
{
    if (id == 0)
    {
        pilots[id].x += speed;
    }
    else
    {
        pilots[id].x -= speed;
    }

    u16 x = pilots[id].x>>4;

    if (second_player)
    {
        if (pilots[id].hovering_count&1)
        {
            if (x >= 512 && x < (512+16))
            {
                pilots[id].y = pilots[id].y_baseline - (x-512);
            }
            if (x >= (512+226) && x < (512+240))
            {
                pilots[id].y = pilots[id].y_baseline - ((512+240)-x);
            }
        }
        else
        {
            if (x >= 512 && x < (512+16))
            {
                pilots[id].y = pilots[id].y_baseline + (x-512);
            }
            if (x >= (512+226) && x < (512+240))
            {
                pilots[id].y = pilots[id].y_baseline + ((512+240)-x);
            }
        }
    }

    if (id == 0)
    {
        if (x >= (512+256))
        {
            pilots[id].x = ((512-16) << 4);
            pilots[id].y_baseline += GAMEPLAY_PILOT_EOL_ALTITUDE_DROP;
            pilots[id].y = pilots[id].y_baseline;
            pilots[id].hovering_count++;
        }
    }
    else
    {
        if (x <= (512-16))
        {
            pilots[id].x = ((512+256) << 4);
            pilots[id].y_baseline += GAMEPLAY_PILOT_EOL_ALTITUDE_DROP;
            pilots[id].y = pilots[id].y_baseline;
            pilots[id].hovering_count++;
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
