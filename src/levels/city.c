#include <snes.h>
#include "city.h"
#include "graphics.h"

// backgrounds
extern char city_bg0_til_begin, city_bg0_til_end;
extern char city_bg0_pal_begin, city_bg0_pal_end;

extern char city_bg1_til_begin, city_bg1_til_end;
extern char city_bg1_pal_begin, city_bg1_pal_end;
extern char city_bg1_map_begin, city_bg1_map_end;

extern char city_bg2_til_begin, city_bg2_til_end;
extern char city_bg2_pal_begin, city_bg2_pal_end;
extern char city_bg2_map_begin, city_bg2_map_end;

u16 city_bg0_scroll_x = 0;
u16 city_bg1_scroll_x = 0;
u16 city_bg2_scroll_x = 0;
u16 city_bg3_scroll_x = 0;

u16 city_block_count = 0;

u8 city_min_building_size = 2;
u8 city_max_building_size = 8;

u16 city_level_tilemap[32][32];

void init_city_level_state(u8 level)
{
    switch (level)
    {
    case 0:
        city_min_building_size = 2;
        city_max_building_size = 8;
        break;

    case 1:
        city_min_building_size = 6;
        city_max_building_size = 12;
        break;

    case 2:
        city_min_building_size = 10;
        city_max_building_size = 16;
        break;

    default:
        city_min_building_size = 8;
        city_max_building_size = 8;
        break;
    }
}

void init_city_level_gfx()
{
    REG_HDMAEN = 0;

    // Init backgrounds
    bgInitTileSet(
        0,
        &city_bg0_til_begin,
        &city_bg0_pal_begin,
        0,
        (&city_bg0_til_end - &city_bg0_til_begin),
        (&city_bg0_pal_end - &city_bg0_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG0_GFX
    );
    bgInitTileSet(
        1,
        &city_bg1_til_begin,
        &city_bg1_pal_begin,
        0,
        (&city_bg1_til_end - &city_bg1_til_begin),
        (&city_bg1_pal_end - &city_bg1_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG1_GFX
    );
    bgInitMapSet(
        1,
        &city_bg1_map_begin,
        (&city_bg1_map_end - &city_bg1_map_begin),
        SC_32x32,
        VRAM_ADDR_BG1_MAP
    );
    bgInitTileSet(
        2,
        &city_bg2_til_begin,
        &city_bg2_pal_begin,
        0,
        (&city_bg2_til_end - &city_bg2_til_begin),
        (&city_bg2_pal_end - &city_bg2_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG2_GFX
    );
    bgInitMapSet(
        2,
        &city_bg2_map_begin,
        (&city_bg2_map_end - &city_bg2_map_begin),
        SC_32x32,
        VRAM_ADDR_BG2_MAP
    );

    setMode(BG_MODE0, 0);

    // Important to disable non-used bkg to avoid artefacts
    bgSetDisable(3);

    // Because the SNES loads OBJ data for each scanline during the previous scanline.
    // The very first line, though, wouldn't have any OBJ data loaded.
    // So the SNES doesn't actually output scanline 0, although it does everything to render it.
    // I want the first line of the BGs to be visible so set offset -1.
    bgSetScroll(0, city_bg0_scroll_x >> 4, 0xFF);
    bgSetScroll(1, city_bg1_scroll_x >> 4, 0xFF);
    bgSetScroll(2, city_bg2_scroll_x >> 4, 0xFF);
    bgSetScroll(3, city_bg3_scroll_x >> 4, 0xFF);
}

void build_city_level_tilemap()
{
    u8 i, j;
    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            city_level_tilemap[j][i] = 0;
        }
    }
    city_block_count = 0;

    for (i=0; i<32; i+=4)
    {
        u8 rnd = (rand()%(city_max_building_size-city_min_building_size))+city_min_building_size;

        for (j=0; j<rnd; j++)
        {
            if (j == 0)
            {
                city_level_tilemap[26-j][i+1] = TIL_BUILDING_FLOOR_SIDE_DOOR;
                city_level_tilemap[26-j][i+2] = TIL_BUILDING_FLOOR_SIDE_DOOR | 0x4000;
                city_block_count += 2;
            }
            else if (j == (rnd-1))
            {
                city_level_tilemap[26-j][i+1] = TIL_BUILDING_CEILING_SIDE_WINDOW_0;
                city_level_tilemap[26-j][i+2] = TIL_BUILDING_CEILING_SIDE_WINDOW_0 | 0x4000;
                city_block_count += 2;
            }
            else
            {
                city_level_tilemap[26-j][i+1] = TIL_BUILDING_BODY_SIDE_WINDOW_0;
                city_level_tilemap[26-j][i+2] = TIL_BUILDING_BODY_SIDE_WINDOW_0 | 0x4000;
                city_block_count += 2;
            }
        }
    }

    bgInitMapSet(
        0,
        (u8*)city_level_tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );
}

void build_city_level_editor_tilemap(u16 * tilemap)
{
    city_block_count = 0;

    u8 i, j;
    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            u16 tile = tilemap[(j*32)+i];
            city_level_tilemap[j][i] = tile;

            if (tile != 0)
            {
                city_block_count++;
            }
        }
    }

    bgInitMapSet(
        0,
        (u8*)city_level_tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );
}

void update_city_level_gfx(u8 frame)
{
    city_bg2_scroll_x += 4;
    bgSetScroll(2, city_bg2_scroll_x >> 4, 0xFF);
}

u8 check_city_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right)
{
    u8 map_min_x = left / 8;
    u8 map_max_x = (right-1) / 8;
    u8 map_max_y = (bottom-1) / 8;

    u8 x = map_min_x;
    u8 y = map_max_y;
    for (; x<=map_max_x; x++)
    {
        u8 tile = city_level_tilemap[y][x]&0xFF;
        // Check collision against solid building block
        if (tile != 0 && tile < TIL_BUILDING_24)
        {
            city_block_count--;

            // Change the look of the surrounding tiles
            u16 tile_left  = city_level_tilemap[y][x-1];
            u16 tile_right = city_level_tilemap[y][x+1];
            u16 tile_up    = city_level_tilemap[y-1][x];

            u8 tile_left_solid = (tile_left != 0 && (tile_left&0xFF) < TIL_BUILDING_24) ? 1 : 0;
            u8 tile_right_solid = (tile_right != 0 && (tile_right&0xFF) < TIL_BUILDING_24) ? 1 : 0;

            if (tile_left_solid == 0 && tile_right_solid == 0)
            {
                city_level_tilemap[y][x] = TIL_BUILDING_CEILING_TOWER_BROKEN;
            }
            else if (tile_left_solid == 1 && tile_right_solid == 1)
            {
                city_level_tilemap[y][x] = TIL_BUILDING_BODY_CENTER_BROKEN_1;
            }
            else if (tile_left_solid == 0 && tile_right_solid == 1)
            {
                city_level_tilemap[y][x] = TIL_BUILDING_CEILING_SIDE_BROKEN;
            }
            else if (tile_left_solid == 1 && tile_right_solid == 0)
            {
                city_level_tilemap[y][x] = TIL_BUILDING_CEILING_SIDE_BROKEN | 0x4000;
            }

            u8 tile_left_broken = ((tile_left&0xFF) > TIL_BUILDING_27) ? 1 : 0;
            u8 tile_right_broken  = ((tile_right&0xFF) > TIL_BUILDING_27) ? 1 : 0;

            if (tile_left_broken)
            {
                if (tile_left == TIL_BUILDING_CEILING_SIDE_BROKEN)
                {
                    city_level_tilemap[y][x-1] = TIL_BUILDING_CEILING_TOWER_BROKEN;
                }
                else if (tile_left == TIL_BUILDING_BODY_SIDE_BROKEN)
                {
                    city_level_tilemap[y][x-1] = 0;
                }
                else if (tile_left == TIL_BUILDING_BODY_CENTER_BROKEN_1)
                {
                    city_level_tilemap[y][x-1] = TIL_BUILDING_CEILING_SIDE_BROKEN | 0x4000;
                }
                else if (tile_left == TIL_BUILDING_BODY_CENTER_BROKEN_0)
                {
                    city_level_tilemap[y][x-1] = TIL_BUILDING_BODY_SIDE_BROKEN | 0x4000;
                }
            }

            if (tile_right_broken)
            {
                if (tile_right == (TIL_BUILDING_CEILING_SIDE_BROKEN | 0x4000))
                {
                    city_level_tilemap[y][x+1] = TIL_BUILDING_CEILING_TOWER_BROKEN;
                }
                else if (tile_right == (TIL_BUILDING_BODY_SIDE_BROKEN | 0x4000))
                {
                    city_level_tilemap[y][x+1] = 0;
                }
                else if (tile_right == TIL_BUILDING_BODY_CENTER_BROKEN_1)
                {
                    city_level_tilemap[y][x+1] = TIL_BUILDING_CEILING_SIDE_BROKEN;
                }
                else if (tile_right == TIL_BUILDING_BODY_CENTER_BROKEN_0)
                {
                    city_level_tilemap[y][x+1] = TIL_BUILDING_BODY_SIDE_BROKEN;
                }
            }

            u8 tile_up_broken = ((tile_up&0xFF) > TIL_BUILDING_27) ? 1 : 0;
            if (tile_up_broken)
            {
                if (tile_up == TIL_BUILDING_CEILING_SIDE_BROKEN)
                {
                    city_level_tilemap[y-1][x] = TIL_BUILDING_BODY_SIDE_BROKEN;
                }
                else if (tile_up == (TIL_BUILDING_CEILING_SIDE_BROKEN | 0x4000))
                {
                    city_level_tilemap[y-1][x] = TIL_BUILDING_BODY_SIDE_BROKEN | 0x4000;
                }
                else if (tile_up == TIL_BUILDING_BODY_CENTER_BROKEN_1)
                {
                    city_level_tilemap[y-1][x] = TIL_BUILDING_BODY_CENTER_BROKEN_0;
                }
                else if (tile_up == TIL_BUILDING_CEILING_TOWER_BROKEN)
                {
                    city_level_tilemap[y-1][x] = 0;
                }
            }

            // Refresh BG
            bgInitMapSet(
                0,
                (u8*)city_level_tilemap,
                32*32*2,
                SC_32x32,
                VRAM_ADDR_BG0_MAP
            );

            return 1;
        }
    }

    return 0;
}

u8 check_city_level_pilot_collision(u8 x, u8 y)
{
    u8 map_x = x / 8;
    u8 map_y = y / 8;

    u8 tile = city_level_tilemap[map_y][map_x]&0xFF;
    // Check collision against solid building block
    if (tile != 0 && (tile&0x0FFF) < TIL_BUILDING_24)
    {
        return 1;
    }

    return 0;
}

u8 check_city_level_done()
{
    return (city_block_count == 0);
}
