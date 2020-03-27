#include <snes.h>
#include "bomb.h"
#include "graphics.h"

#define GAMEPLAY_BOMB_DROP_SPEED    1
#define GAMEPLAY_BOMB_GROUND_Y_POS  (224-16)

struct bomb_t bombs[8];

u8 bomb_oam_ids[] = { OBJ_P0_BOMB_0, OBJ_P0_BOMB_1, OBJ_P0_BOMB_MEGA, OBJ_P1_BOMB_0, OBJ_P1_BOMB_1, OBJ_P1_BOMB_MEGA };
u8 bomb_gfx_ids[] = { SPR_BOMB, SPR_BOMB, SPR_MEGA_BOMB, SPR_BOMB, SPR_BOMB, SPR_MEGA_BOMB };
u8 bomb_hits[] = { 2, 2, 5, 2, 2, 5 }; // The number of building blocks the bomb can destroy.

struct bomb_t * get_bomb(u8 id)
{
    return &bombs[id];
}

// Do not set x & y position.
// This method is used to clear the bomb struct after collision.
// By not resetting x & y, we keep access to the last known position.
// The last know position is used to spawn explosions.
void init_bomb(u8 id)
{
    bombs[id].spr = bomb_oam_ids[id];
    bombs[id].dropped = 0;

    oamSet(bombs[id].spr, 0xFF, 0xFF, 3, 0, 0, bomb_gfx_ids[id], 0);
    oamSetEx(bombs[id].spr, OBJ_LARGE, OBJ_HIDE);
}

void drop_bomb(u8 id, u16 x, u16 y)
{
    //if (bombs[id].dropped) Checked in update_bomb
    {
        bombs[id].x = x;
        bombs[id].y = y;
        bombs[id].dropped = bomb_hits[id];

        oamSetXY(bombs[id].spr, bombs[id].x, bombs[id].y); // Force the sprite to OBJ_SHOW
    }
}

void move_bomb(u8 id)
{
    //if (bombs[id].dropped) Checked in update_bomb
    {
        bombs[id].y += GAMEPLAY_BOMB_DROP_SPEED;
        if (bombs[id].y >= GAMEPLAY_BOMB_GROUND_Y_POS)
        {
            init_bomb(id);
        }
        else
        {
            oamSetXY(bombs[id].spr, bombs[id].x, bombs[id].y); // Force the sprite to OBJ_SHOW
        }
    }
}
