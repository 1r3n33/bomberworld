#include <snes.h>
#include "graphics.h"
#include "sea.h"

// backgrounds
extern char bkg_sea_til_begin, bkg_sea_til_end;
extern char bkg_sea_pal_begin, bkg_sea_pal_end;
extern char bkg_sea_map_begin, bkg_sea_map_end;

extern char bkg_ship_til_begin, bkg_ship_til_end;
extern char bkg_ship_pal_begin, bkg_ship_pal_end;
extern char bkg_ship_map_begin, bkg_ship_map_end;

extern char bkg_sea_back_til_begin, bkg_sea_back_til_end;
extern char bkg_sea_back_pal_begin, bkg_sea_back_pal_end;
extern char bkg_sea_back_map_begin, bkg_sea_back_map_end;

u16 sea_block_count = 0;

u16 * sea_level_tilemap = 0;

u8 ship_scroll_x = 0;
u8 ship_scroll_y = 0;

s8 scroll_offset[2] = { 1, -1 };

void init_sea_level_state(u8 level)
{
    ship_scroll_x = 0;
    ship_scroll_y = 0;
}

void init_sea_level_gfx()
{
    REG_HDMAEN = 0;

    // Init backgrounds
    bgInitTileSet(
        0,
        &bkg_sea_til_begin,
        &bkg_sea_pal_begin,
        0,
        (&bkg_sea_til_end - &bkg_sea_til_begin),
        (&bkg_sea_pal_end - &bkg_sea_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG0_GFX
    );

    bgInitMapSet(
        0,
        &bkg_sea_map_begin,
        (&bkg_sea_map_end - &bkg_sea_map_begin),
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );

    bgInitTileSet(
        1,
        &bkg_ship_til_begin,
        &bkg_ship_pal_begin,
        0,
        (&bkg_ship_til_end - &bkg_ship_til_begin),
        (&bkg_ship_pal_end - &bkg_ship_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG1_GFX
    );

    bgInitTileSet(
        2,
        &bkg_sea_back_til_begin,
        &bkg_sea_back_pal_begin,
        0,
        (&bkg_sea_back_til_end - &bkg_sea_back_til_begin),
        (&bkg_sea_back_pal_end - &bkg_sea_back_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG2_GFX
    );

    bgInitMapSet(
        2,
        &bkg_sea_back_map_begin,
        (&bkg_sea_back_map_end - &bkg_sea_back_map_begin),
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
            u16 tile = ((u16*)&bkg_ship_map_begin)[j*32+i];
            tilemap[j][i] = tile;

            if (tile > 0)
            {
                sea_block_count++;
            }
        }
    }

    sea_level_tilemap = (u16*)tilemap;

    bgInitMapSet(
        1,
        (u8*)tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG1_MAP
    );
}

void update_sea_level_gfx(u8 frame)
{
    if ((frame&7)==7) // Every 7 frames
    {
        u8 x = (frame >> 7) & 1;
        ship_scroll_x += scroll_offset[x];

        u8 y = (frame >> 5) & 1;
        ship_scroll_y += scroll_offset[y];

        bgSetScroll(1, ship_scroll_x, ship_scroll_y);
    }

    bgSetScroll(0, frame>>1, (frame>>5)&1);
    bgSetScroll(2, frame>>2, (frame>>6)&1);
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
        u16 tile = sea_level_tilemap[y*32+x];
        if (tile > 0)
        {
            sea_block_count--;
            sea_level_tilemap[y*32+x] = 0;
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

    u8 tile = sea_level_tilemap[(map_y*32)+map_x];

    return tile;
}

u8 check_sea_level_done()
{
    return sea_block_count < 16;
}
