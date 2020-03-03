#include <snes.h>
#include "bomb.h"
#include "graphics.h"

#define GAMEPLAY_BOMB_POWER         2 // the number of building blocks the bomb can destroy
#define GAMEPLAY_BOMB_DROP_SPEED    1
#define GAMEPLAY_BOMB_GROUND_Y_POS  (224-12)

struct bomb_t bombs[4];

u8 bomb_oam_ids[4] = { OBJ_P0_BOMB_0, OBJ_P0_BOMB_1, OBJ_P1_BOMB_0, OBJ_P1_BOMB_1 };

struct bomb_t * get_bomb(u8 id)
{
    return &bombs[id];
}

void init_bomb(u8 id)
{
    bombs[id].spr = bomb_oam_ids[id];

    bombs[id].x = 0xFF;
    bombs[id].y = 0xFF;
    bombs[id].dropped = 0;

    oamSet(bombs[id].spr, 0xFF, 0xFF, 3, 0, 0, SPR_BOMB, 0);
    oamSetEx(bombs[id].spr, OBJ_LARGE, OBJ_HIDE);
}

void drop_bomb(u8 id, u16 x, u16 y)
{
    //if (bombs[id].dropped) Checked in update_bomb
    {
        bombs[id].x = x;
        bombs[id].y = y;
        bombs[id].dropped = GAMEPLAY_BOMB_POWER;

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
