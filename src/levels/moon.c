#include <snes.h>
#include "graphics.h"
#include "moon.h"
#include "vfx.h"

extern char moon_bg0_til_begin, moon_bg0_til_end;
extern char moon_bg0_pal_begin, moon_bg0_pal_end;
extern char moon_bg0_map_begin, moon_bg0_map_end;

extern char moon_bg1_til_begin, moon_bg1_til_end;
extern char moon_bg1_pal_begin, moon_bg1_pal_end;

extern char moon_bg2_til_begin, moon_bg2_til_end;
extern char moon_bg2_pal_begin, moon_bg2_pal_end;
extern char moon_bg2_map_begin, moon_bg2_map_end;

#define TENTACLE_BLANK 0
#define TENTACLE_HEAD  1
#define TENTACLE_BODY  2

u16 moon_block_count = 0;

u16 moon_level_tilemap[32][32];

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

    bgSetScroll(0, 0, 0xFF);
    bgSetScroll(1, 0, 0xFF);
    bgSetScroll(2, 0, 0xFF);
    bgSetScroll(3, 0, 0xFF);

    init_vfx_moon_bkg();
}

void build_moon_level_tilemap(u16 tilemap[32][32])
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
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG1_MAP
    );
}

void update_moon_level_gfx(u8 frame)
{
    update_vfx_moon_bkg(frame);
}

u8 check_moon_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right)
{
    u8 map_min_x = (left) / 8;
    u8 map_max_x = ((right)-1) / 8;
    u8 map_max_y = ((bottom)-1) / 8;

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
                32*32*2,
                SC_32x32,
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
    u8 map_y = (y) / 8;

    u8 tile = moon_level_tilemap[map_y][map_x];

    return tile;
}

u8 check_moon_level_done()
{
    return moon_block_count == 0;
}