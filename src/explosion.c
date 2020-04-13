#include "explosion.h"
#include "graphics.h"

struct explosion_t explosions[6];

u8 explosion_oam_ids[] =
{
    OBJ_BOMB_EXPLOSION_0,
    OBJ_BOMB_EXPLOSION_1,
    OBJ_BOMB_EXPLOSION_2,
    OBJ_BOMB_EXPLOSION_3,
    OBJ_PILOT_EXPLOSION_0,
    OBJ_PILOT_EXPLOSION_1,
};

u8 explosion_gfx_frames[] =
{
    SPR_EXPLOSION_FRAME_0, SPR_EXPLOSION_FRAME_0,
    SPR_EXPLOSION_FRAME_1, SPR_EXPLOSION_FRAME_1,
    SPR_EXPLOSION_FRAME_2, SPR_EXPLOSION_FRAME_2,
    SPR_EXPLOSION_FRAME_3, SPR_EXPLOSION_FRAME_3,
    SPR_EXPLOSION_FRAME_2, SPR_EXPLOSION_FRAME_2,
    SPR_EXPLOSION_FRAME_1, SPR_EXPLOSION_FRAME_1,
    SPR_EXPLOSION_FRAME_0, SPR_EXPLOSION_FRAME_0,
};

void init_explosion(u8 id, u16 x, u16 y)
{
    explosions[id].x = x;
    explosions[id].y = y;
    explosions[id].frame = sizeof(explosion_gfx_frames)-1;

    oamSet(explosion_oam_ids[id], 0xFF, 0xFF, 3, 0, 0, explosion_gfx_frames[explosions[id].frame], 0);
    oamSetEx(explosion_oam_ids[id], OBJ_LARGE, OBJ_SHOW);
    oamSetXY(explosion_oam_ids[id], explosions[id].x, explosions[id].y);
}

u8 update_explosion(u8 id)
{
    if (explosions[id].frame)
    {
        explosions[id].frame--;
        oamSetGfxOffset(explosion_oam_ids[id], explosion_gfx_frames[explosions[id].frame]);
        return 1;
    }
    else
    {
        oamSetEx(explosion_oam_ids[id], OBJ_LARGE, OBJ_HIDE);
        return 0;
    }
}

void disable_explosion(u8 id)
{
    explosions[id].frame = 0;
    oamSetEx(explosion_oam_ids[id], OBJ_LARGE, OBJ_HIDE);
}
