#include "explosion.h"
#include "graphics.h"

struct explosion_t explosions[4];

u8 explosion_oam_ids[] =
{
    OBJ_EXPLOSION_0,
    OBJ_EXPLOSION_1,
    OBJ_EXPLOSION_2,
    OBJ_EXPLOSION_3,
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

void init_explosion(u8 id, u8 x, u8 y)
{
    explosions[id].x = x;
    explosions[id].y = y;
    explosions[id].frame = sizeof(explosion_gfx_frames)-1;

    oamSet(explosion_oam_ids[id], explosions[id].x, explosions[id].y, 3, 0, 0, explosion_gfx_frames[explosions[id].frame], 0);
    oamSetEx(explosion_oam_ids[id], OBJ_LARGE, OBJ_SHOW);
}

void update_explosion(u8 id)
{
    if (explosions[id].frame)
    {
        explosions[id].frame--;
        oamSetGfxOffset(explosion_oam_ids[id], explosion_gfx_frames[explosions[id].frame]);
    }
    else
    {
        oamSetEx(explosion_oam_ids[id], OBJ_LARGE, OBJ_HIDE);
    }
}

void disable_explosion(u8 id)
{
    explosions[id].frame = 0;
    oamSetEx(explosion_oam_ids[id], OBJ_LARGE, OBJ_HIDE);
}
