#include <snes.h>
#include "graphics.h"
#include "sea.h"

// backgrounds
extern char sea_bg0_til_begin, sea_bg0_til_end;
extern char sea_bg0_pal_begin, sea_bg0_pal_end;
extern char sea_bg0_map_begin, sea_bg0_map_end;

extern char sea_bg1_til_begin, sea_bg1_til_end;
extern char sea_bg1_pal_begin, sea_bg1_pal_end;
extern char sea_bg1_map_begin, sea_bg1_map_end;

extern char sea_bg2_til_begin, sea_bg2_til_end;
extern char sea_bg2_pal_begin, sea_bg2_pal_end;
extern char sea_bg2_map_begin, sea_bg2_map_end;

u16 sea_block_count = 0;

u16 sea_level_tilemap[32][32];

u8 sea_level_frame = 0;

u8 ship_scroll_x = 0;
u8 ship_scroll_y = 0;

s8 scroll_offset[2] = { 1, -1 };

void init_sea_level_state(u8 level)
{
    sea_level_frame = 0;

    ship_scroll_x = 0;
    ship_scroll_y = 0;
}

void init_sea_level_gfx()
{
    REG_HDMAEN = 0;

    // Init backgrounds
    bgInitTileSet(
        0,
        &sea_bg0_til_begin,
        &sea_bg0_pal_begin,
        0,
        (&sea_bg0_til_end - &sea_bg0_til_begin),
        (&sea_bg0_pal_end - &sea_bg0_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG0_GFX
    );

    bgInitMapSet(
        0,
        &sea_bg0_map_begin,
        (&sea_bg0_map_end - &sea_bg0_map_begin),
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );

    bgInitTileSet(
        1,
        &sea_bg1_til_begin,
        &sea_bg1_pal_begin,
        0,
        (&sea_bg1_til_end - &sea_bg1_til_begin),
        (&sea_bg1_pal_end - &sea_bg1_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG1_GFX
    );

    bgInitTileSet(
        2,
        &sea_bg2_til_begin,
        &sea_bg2_pal_begin,
        0,
        (&sea_bg2_til_end - &sea_bg2_til_begin),
        (&sea_bg2_pal_end - &sea_bg2_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG2_GFX
    );

    bgInitMapSet(
        2,
        &sea_bg2_map_begin,
        (&sea_bg2_map_end - &sea_bg2_map_begin),
        SC_32x32,
        VRAM_ADDR_BG2_MAP
    );

    setMode(BG_MODE0, 0);

    // Important to disable non-used bkg to avoid artefacts
    bgSetDisable(3);
}

void build_sea_level_tilemap(u16 tilemap[32][32])
{
    u16 i,j;
    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            u16 tile = ((u16*)&sea_bg1_map_begin)[j*32+i];
            sea_level_tilemap[j][i] = tile;

            if (tile > 0)
            {
                sea_block_count++;
            }
        }
    }

    bgInitMapSet(
        1,
        (u8*)sea_level_tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG1_MAP
    );
}

void build_sea_level_editor_tilemap(u16 * tilemap)
{
    u16 i,j;
    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            u16 tile = tilemap[j*32+i];
            sea_level_tilemap[j][i] = tile;

            if (tile > 0)
            {
                sea_block_count++;
            }
        }
    }

    bgInitMapSet(
        1,
        (u8*)sea_level_tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG1_MAP
    );
}

void update_sea_level_gfx(u8 frame)
{
    if ((sea_level_frame&7)==7) // Every 7 frames
    {
        u8 x = (sea_level_frame >> 7) & 1;
        ship_scroll_x += scroll_offset[x];

        u8 y = (sea_level_frame >> 5) & 1;
        ship_scroll_y += scroll_offset[y];

        bgSetScroll(1, ship_scroll_x, ship_scroll_y);
    }

    bgSetScroll(0, sea_level_frame>>1, (sea_level_frame>>5)&1);
    bgSetScroll(2, sea_level_frame>>2, (sea_level_frame>>6)&1);

    sea_level_frame++;
}

u8 check_sea_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right)
{
    u8 map_min_x = (left+ship_scroll_x) / 8;
    u8 map_max_x = ((right+ship_scroll_x)-1) / 8;
    u8 map_max_y = ((bottom+ship_scroll_y)-1) / 8;

    u8 x = map_min_x;
    u8 y = map_max_y;
    for (; x<=map_max_x; x++)
    {
        u16 tile = sea_level_tilemap[y][x];
        if (tile > 0)
        {
            sea_block_count--;
            sea_level_tilemap[y][x] = 0;
            bgInitMapSet(
                1,
                (u8*)sea_level_tilemap,
                32*32*2,
                SC_32x32,
                VRAM_ADDR_BG1_MAP
            );

            return 2;
        }
    }

    return 0;
}

u8 check_sea_level_pilot_collision(u8 x, u8 y)
{
    u8 map_x = (x+ship_scroll_x) / 8;
    u8 map_y = (y+ship_scroll_y) / 8;

    u8 tile = sea_level_tilemap[map_y][map_x];

    return tile;
}

u8 check_sea_level_done()
{
    return sea_block_count < 16;
}
