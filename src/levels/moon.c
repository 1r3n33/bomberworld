#include <snes.h>
#include "graphics.h"
#include "moon.h"
#include "pilot.h"
#include "vfx.h"

extern char moon_bg0_til_begin, moon_bg0_til_end;
extern char moon_bg0_pal_begin, moon_bg0_pal_end;
extern char moon_bg0_map_begin, moon_bg0_map_end;

extern char moon_bg1_til_begin, moon_bg1_til_end;
extern char moon_bg1_pal_begin, moon_bg1_pal_end;

extern char moon_boss_bg1_til_begin, moon_boss_bg1_til_end;
extern char moon_boss_bg1_pal_begin, moon_boss_bg1_pal_end;
extern char moon_boss_bg1_map_begin, moon_boss_bg1_map_end;

extern char moon_bg2_til_begin, moon_bg2_til_end;
extern char moon_bg2_pal_begin, moon_bg2_pal_end;
extern char moon_bg2_map_begin, moon_bg2_map_end;

#define TENTACLE_BLANK 0
#define TENTACLE_HEAD  1
#define TENTACLE_BODY  2

u16 moon_block_count = 0;

u16 moon_level_tilemap[64][32];

u16 moon_bg1_scroll_x = 0;
u16 moon_bg1_scroll_y = 0;

u8 boss_eyes_oam_ids[] = { OBJ_BOSS_EYE_0, OBJ_BOSS_EYE_1, OBJ_BOSS_EYE_2, OBJ_BOSS_EYE_3, OBJ_BOSS_EYE_4 };
u8 boss_eyes_spr_ids[] = { SPR_BOSS_EYE, SPR_BOSS_EYE, SPR_BOSS_EYE, SPR_BOSS_EYE, SPR_BOSS_EYE };

struct simple_sprite_t
{
    u8 x, y;
};

struct simple_sprite_t boss_eyes[4];

void init_moon_level_state(u8 level)
{
}

void init_moon_level_gfx()
{
    REG_HDMAEN = 0;

    // Init backgrounds
    bgInitTileSet(
        0,
        &moon_bg0_til_begin,
        &moon_bg0_pal_begin,
        0,
        (&moon_bg0_til_end - &moon_bg0_til_begin),
        (&moon_bg0_pal_end - &moon_bg0_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG0_GFX
    );

    bgInitMapSet(
        0,
        &moon_bg0_map_begin,
        (&moon_bg0_map_end - &moon_bg0_map_begin),
        SC_64x32,
        VRAM_ADDR_BG0_MAP
    );

    bgInitTileSet(
        1,
        &moon_bg1_til_begin,
        &moon_bg1_pal_begin,
        0,
        (&moon_bg1_til_end - &moon_bg1_til_begin),
        (&moon_bg1_pal_end - &moon_bg1_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG1_GFX
    );

    bgInitTileSet(
        2,
        &moon_bg2_til_begin,
        &moon_bg2_pal_begin,
        0,
        (&moon_bg2_til_end - &moon_bg2_til_begin),
        (&moon_bg2_pal_end - &moon_bg2_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG2_GFX
    );

    bgInitMapSet(
        2,
        &moon_bg2_map_begin,
        (&moon_bg2_map_end - &moon_bg2_map_begin),
        SC_64x32,
        VRAM_ADDR_BG2_MAP
    );

    setMode(BG_MODE0, 0);

    // Important to disable non-used bkg to avoid artefacts
    bgSetDisable(3);

    moon_bg1_scroll_y = 0;

    bgSetScroll(0, 0, 255);
    bgSetScroll(1, 0, 511);
    bgSetScroll(2, 0, 255);
    bgSetScroll(3, 0, 255);

    init_vfx_moon_bkg();
}

void init_moon_boss_level_gfx()
{
    REG_HDMAEN = 0;

    // Init backgrounds
    bgInitTileSet(
        0,
        &moon_bg0_til_begin,
        &moon_bg0_pal_begin,
        0,
        (&moon_bg0_til_end - &moon_bg0_til_begin),
        (&moon_bg0_pal_end - &moon_bg0_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG0_GFX
    );

    bgInitMapSet(
        0,
        &moon_bg0_map_begin,
        (&moon_bg0_map_end - &moon_bg0_map_begin),
        SC_64x32,
        VRAM_ADDR_BG0_MAP
    );

    bgInitTileSet(
        1,
        &moon_boss_bg1_til_begin,
        &moon_boss_bg1_pal_begin,
        0,
        (&moon_boss_bg1_til_end - &moon_boss_bg1_til_begin),
        (&moon_boss_bg1_pal_end - &moon_boss_bg1_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG1_GFX
    );

    bgInitTileSet(
        2,
        &moon_bg2_til_begin,
        &moon_bg2_pal_begin,
        0,
        (&moon_bg2_til_end - &moon_bg2_til_begin),
        (&moon_bg2_pal_end - &moon_bg2_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG2_GFX
    );

    bgInitMapSet(
        2,
        &moon_bg2_map_begin,
        (&moon_bg2_map_end - &moon_bg2_map_begin),
        SC_64x32,
        VRAM_ADDR_BG2_MAP
    );

    setMode(BG_MODE0, 0);

    // Important to disable non-used bkg to avoid artefacts
    bgSetDisable(3);

    moon_bg1_scroll_y = 80;

    bgSetScroll(0, 256, 0xFF);
    bgSetScroll(1, 0,   moon_bg1_scroll_y);
    bgSetScroll(2, 256, 0xFF);
    bgSetScroll(3, 0,   0xFF);

    boss_eyes[0].x = 72;
    boss_eyes[0].y = 112;

    boss_eyes[1].x = 100;
    boss_eyes[1].y = 116;

    boss_eyes[2].x = 124;
    boss_eyes[2].y = 108;

    boss_eyes[3].x = 148;
    boss_eyes[3].y = 108;

    boss_eyes[4].x = 172;
    boss_eyes[4].y = 116;

    u8 i=0;
    for (i=0; i<5; i++)
    {
        oamSet(boss_eyes_oam_ids[i], 0xFF, 0xFF, 2, 0, 0, boss_eyes_spr_ids[i], 0);
        oamSetEx(boss_eyes_oam_ids[i], OBJ_LARGE, OBJ_SHOW);
    }
}

void build_moon_level_tilemap()
{
    u8 i, j;
    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            moon_level_tilemap[j][i] = 0;
        }
    }
    moon_block_count = 0;

    // Tentacle 1
    for (i=25; i>16; i--)
    {
        moon_level_tilemap[i][3] = TENTACLE_BODY;
        moon_block_count++;
    }
    moon_level_tilemap[i][3] = TENTACLE_HEAD;
    moon_block_count++;

    // Tentacle 2
    for (i=26; i>13; i--)
    {
        moon_level_tilemap[i][8] = TENTACLE_BODY;
        moon_block_count++;
    }
    moon_level_tilemap[i][8] = TENTACLE_HEAD;
    moon_block_count++;

    // Tentacle 3
    for (i=24; i>19; i--)
    {
        moon_level_tilemap[i][13] = TENTACLE_BODY;
        moon_block_count++;
    }
    moon_level_tilemap[i][13] = TENTACLE_HEAD;
    moon_block_count++;

    // Tentacle 4
    for (i=26; i>17; i--)
    {
        moon_level_tilemap[i][17] = TENTACLE_BODY;
        moon_block_count++;
    }
    moon_level_tilemap[i][17] = TENTACLE_HEAD;
    moon_block_count++;

    // Tentacle 5
    for (i=25; i>14; i--)
    {
        moon_level_tilemap[i][23] = TENTACLE_BODY;
        moon_block_count++;
    }
    moon_level_tilemap[i][23] = TENTACLE_HEAD;
    moon_block_count++;

    // Tentacle 6
    for (i=24; i>16; i--)
    {
        moon_level_tilemap[i][28] = TENTACLE_BODY;
        moon_block_count++;
    }
    moon_level_tilemap[i][28] = TENTACLE_HEAD;
    moon_block_count++;

    bgInitMapSet(
        1,
        (u8*)moon_level_tilemap,
        64*32*2,
        SC_32x64,
        VRAM_ADDR_BG1_MAP
    );
}

void build_moon_boss_level_tilemap()
{
    REG_HDMAEN = 0;

    u8 i, j;
    for (j=0; j<64; j++)
    {
        for (i=0; i<32; i++)
        {
            u16 tile = ((u16*)&moon_boss_bg1_map_begin)[j*32+i];
            moon_level_tilemap[j][i] = tile;
        }
    }

    moon_block_count = 4; // boss heart

    bgInitMapSet(
        1,
        (u8*)moon_level_tilemap,
        64*32*2,
        SC_32x64,
        VRAM_ADDR_BG1_MAP
    );
}

void update_moon_level_gfx(u8 frame)
{
    update_vfx_moon_bkg(frame);
}

void update_eye_pos(u8 id, u16 px, u16 py, u8 max)
{
    u16 ex = boss_eyes[id].x+512;
    u16 ey = boss_eyes[id].y+(192-moon_bg1_scroll_y);

    u16 dx = 0;
    if (px > ex)
    {
        dx = px - ex;
        dx = dx / 8;
        if (dx > max) dx = max;
        dx = dx + ex;
    }
    else
    {
        dx = ex - px;
        dx = dx / 8;
        if (dx > max) dx = max;
        dx = ex - dx;
    }

    u16 dy = 0;
    dy = ey - py;
    dy = dy / 8;
    if (dy > max) dy = max;
    dy = ey - dy;

    oamSetXY(boss_eyes_oam_ids[id], dx, dy);
}

void update_moon_boss_level_gfx(u8 frame)
{
    struct pilot_t * p = get_pilot(0);

    u16 px = (p->x >> 4);
    u16 py = (p->y);

    update_eye_pos(0, px, py, 6);
    update_eye_pos(1, px, py, 8);
    update_eye_pos(2, px, py, 3);
    update_eye_pos(3, px, py, 8);
    update_eye_pos(4, px, py, 3);

    if ((moon_bg1_scroll_y < 191) && ((frame & 15) == 15))
    {
        moon_bg1_scroll_y++;
        bgSetScroll(1, 0, moon_bg1_scroll_y);
    }
}

u8 check_moon_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right)
{
    u8 map_min_x = (left) / 8;
    u8 map_max_x = ((right)-1) / 8;

    u16 real_bottom = (u16)bottom + moon_bg1_scroll_y;
    u8 map_max_y = ((real_bottom)-1) / 8;

    u8 x = map_min_x;
    u8 y = map_max_y;
    for (; x<=map_max_x; x++)
    {
        u16 tile = moon_level_tilemap[y][x];
        if (tile > 0)
        {
            moon_block_count--;
            moon_level_tilemap[y][x] = 0;
            if (moon_level_tilemap[y+1][x] == TENTACLE_BODY)
            {
                moon_level_tilemap[y+1][x] = TENTACLE_HEAD;
            }
            bgInitMapSet(
                1,
                (u8*)moon_level_tilemap,
                64*32*2,
                SC_32x64,
                VRAM_ADDR_BG1_MAP
            );

            return 2;
        }
    }

    return 0;
}

u8 check_moon_boss_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right)
{
    u8 map_min_x = (left) / 8;
    u8 map_max_x = ((right)-1) / 8;

    u16 real_bottom = (u16)bottom + moon_bg1_scroll_y;
    u8 map_max_y = ((real_bottom)-1) / 8;

    u8 x = map_min_x;
    u8 y = map_max_y;
    for (; x<=map_max_x; x++)
    {
        u16 tile = moon_level_tilemap[y][x];
        if (tile > 0)
        {
            moon_level_tilemap[y][x] = 0;

            if (x == 15 && y == 46) moon_block_count--;
            if (x == 15 && y == 47) moon_block_count--;
            if (x == 16 && y == 46) moon_block_count--;
            if (x == 16 && y == 47) moon_block_count--;

            bgInitMapSet(
                1,
                (u8*)moon_level_tilemap,
                64*32*2,
                SC_32x64,
                VRAM_ADDR_BG1_MAP
            );

            return 2;
        }
    }

    return 0;
}

u8 check_moon_level_pilot_collision(u8 x, u8 y)
{
    u8 map_x = (x) / 8;

    u16 real_y = (u16)y + moon_bg1_scroll_y;
    u8 map_y = (real_y) / 8;

    u8 tile = moon_level_tilemap[map_y][map_x];

    return tile;
}

u8 check_moon_level_done()
{
    return moon_block_count == 0;
}