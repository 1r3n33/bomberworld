#include <snes.h>
#include "bomb.h"
#include "egypt.h"
#include "graphics.h"
#include "vfx.h"

// backgrounds
extern char egypt_bg0_til_begin, egypt_bg0_til_end;
extern char egypt_bg0_pal_begin, egypt_bg0_pal_end;

extern char egypt_bg1_til_begin, egypt_bg1_til_end;
extern char egypt_bg1_pal_begin, egypt_bg1_pal_end;
extern char egypt_bg1_map_begin, egypt_bg1_map_end;

u16 egypt_bg0_scroll_x = 0;
u16 egypt_bg1_scroll_x = 0;
u16 egypt_bg2_scroll_x = 0;
u16 egypt_bg3_scroll_x = 0;

u16 egypt_block_count = 0;

u8 egypt_min_building_size = 2;
u8 egypt_max_building_size = 8;

u16 egypt_level_tilemap[32][32];

void init_egypt_level_state(u8 level)
{
}

void init_egypt_level_gfx()
{
    REG_HDMAEN = 0;

    // Init backgrounds
    bgInitTileSet(
        0,
        &egypt_bg0_til_begin,
        &egypt_bg0_pal_begin,
        0,
        (&egypt_bg0_til_end - &egypt_bg0_til_begin),
        (&egypt_bg0_pal_end - &egypt_bg0_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG0_GFX
    );
    bgInitTileSet(
        1,
        &egypt_bg1_til_begin,
        &egypt_bg1_pal_begin,
        0,
        (&egypt_bg1_til_end - &egypt_bg1_til_begin),
        (&egypt_bg1_pal_end - &egypt_bg1_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG1_GFX
    );
    bgInitMapSet(
        1,
        &egypt_bg1_map_begin,
        (&egypt_bg1_map_end - &egypt_bg1_map_begin),
        SC_32x32,
        VRAM_ADDR_BG1_MAP
    );

    setMode(BG_MODE0, 0);

    // Important to disable non-used bkg to avoid artefacts
    bgSetDisable(2);
    bgSetDisable(3);

    // Because the SNES loads OBJ data for each scanline during the previous scanline.
    // The very first line, though, wouldn't have any OBJ data loaded.
    // So the SNES doesn't actually output scanline 0, although it does everything to render it.
    // I want the first line of the BGs to be visible so set offset -1.
    bgSetScroll(0, egypt_bg0_scroll_x >> 4, 0xFF);
    bgSetScroll(1, egypt_bg1_scroll_x >> 4, 0xFF);
    bgSetScroll(2, egypt_bg2_scroll_x >> 4, 0xFF);
    bgSetScroll(3, egypt_bg3_scroll_x >> 4, 0xFF);

    // Heat fx
    init_vfx_bkg_waves(0);
}

u8 egypt_mega_bomb_collision(u8 top, u8 bottom, u8 left, u8 right)
{
    u8 map_x = (right-1) / 8;
    u8 map_y = (bottom-1) / 8;

    s8 i;
    s8 j;
    for (j=0; j<11; j++)
    {
        for (i=0; i<11; i++)
        {
            s8 xx = (map_x-5)+i;
            s8 yy = (map_y-5)+j;

            if (xx >= 0 && xx < 32 && yy >= 0 && yy < 32 && get_mega_bomb_mask(i, j))
            {
                if (egypt_level_tilemap[yy][xx] > 0)
                {
                    egypt_level_tilemap[yy][xx] = 0;
                    egypt_block_count--;
                }
            }
        }
    }

    bgInitMapSet(
        0,
        (u8*)egypt_level_tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );

    return 0;
}

void build_egypt_level_tilemap()
{
    u8 i, j;
    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            egypt_level_tilemap[j][i] = 0;
        }
    }
    egypt_block_count = 0;

    u8 height = 0;
    for (i=2; i<16; i++)
    {
        for (j=0; j<height; j++)
        {
            if (j < 10)
            {
                egypt_level_tilemap[26-j][i] = TIL_PYRAMID_BLOCK_NORMAL | TIL_PYRAMID_PALETTE_1_FLAG;
                egypt_level_tilemap[26-j][31-i] = TIL_PYRAMID_BLOCK_NORMAL | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG;
            }
            else
            {
                egypt_level_tilemap[26-j][i] = TIL_PYRAMID_BLOCK_GOLD;
                egypt_level_tilemap[26-j][31-i] = TIL_PYRAMID_BLOCK_GOLD | TIL_PYRAMID_MIRROR_X_FLAG;
            }
        }

        if (j < 10)
        {
            egypt_level_tilemap[26-j][i] = TIL_PYRAMID_SIDE_NORMAL | TIL_PYRAMID_PALETTE_1_FLAG;
            egypt_level_tilemap[26-j][31-i] = TIL_PYRAMID_SIDE_NORMAL | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG;
        }
        else
        {
            egypt_level_tilemap[26-j][i] = TIL_PYRAMID_SIDE_GOLD;
            egypt_level_tilemap[26-j][31-i] = TIL_PYRAMID_SIDE_GOLD | TIL_PYRAMID_MIRROR_X_FLAG;
        }

        egypt_block_count += (2*(height+1));
        height++;
    }

    bgInitMapSet(
        0,
        (u8*)egypt_level_tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );
}

void build_egypt_level_editor_tilemap(u16 * tilemap)
{
    egypt_block_count = 0;

    u8 i, j;
    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            u16 tile = tilemap[(j*32)+i];
            egypt_level_tilemap[j][i] = tile;

            if (tile > 0 && tile < TIL_PYRAMID_BROKEN_FIRST_ID)
            {
                egypt_block_count++;
            }
        }
    }

    bgInitMapSet(
        0,
        (u8*)egypt_level_tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );
}

void update_egypt_level_gfx(u8 frame)
{
    update_vfx_bkg_waves(frame);
}

u8 check_egypt_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right)
{
    u8 map_min_x = left / 8;
    u8 map_max_x = (right-1) / 8;
    u8 map_max_y = (bottom-1) / 8;

    u8 x = map_min_x;
    u8 y = map_max_y;
    for (; x<=map_max_x; x++)
    {
        u8 tile = egypt_level_tilemap[y][x];
        u8 tile_id = tile & TIL_PYRAMID_ID_MASK;

        if (tile_id > 0 && tile_id < TIL_PYRAMID_BROKEN_FIRST_ID)
        {
            // Collision against the gold cap
            switch (tile_id)
            {
            case TIL_PYRAMID_SIDE_GOLD:
            case TIL_PYRAMID_BLOCK_GOLD:
                egypt_level_tilemap[y][x] += 2; // offset by 2 in the tilemap gets the corresponding cracked tile
                bgInitMapSet(
                    0,
                    (u8*)egypt_level_tilemap,
                    32*32*2,
                    SC_32x32,
                    VRAM_ADDR_BG0_MAP
                );
                return 2;

            case TIL_PYRAMID_SIDE_GOLD_CRACK:
            case TIL_PYRAMID_BLOCK_GOLD_CRACK:
                egypt_level_tilemap[y][x] -= 4; // offset by -4 in the tilemap gets the corresponding brick tile
                egypt_level_tilemap[y][x] |= TIL_PYRAMID_PALETTE_1_FLAG;
                bgInitMapSet(
                    0,
                    (u8*)egypt_level_tilemap,
                    32*32*2,
                    SC_32x32,
                    VRAM_ADDR_BG0_MAP
                );
                return 2;

            default:
                break;
            }

            egypt_block_count--;

            // Change the look of the surrounding tiles
            u16 tile_left  = egypt_level_tilemap[y][x-1];
            u16 tile_right = egypt_level_tilemap[y][x+1];
            u16 tile_up    = egypt_level_tilemap[y-1][x];

            u8 tile_left_solid = (tile_left > 0 && (tile_left&TIL_PYRAMID_ID_MASK) < TIL_PYRAMID_BROKEN_FIRST_ID) ? 1 : 0;
            u8 tile_right_solid = (tile_right > 0 && (tile_right&TIL_PYRAMID_ID_MASK) < TIL_PYRAMID_BROKEN_FIRST_ID) ? 1 : 0;

            if (tile_left_solid == 0 && tile_right_solid == 0)
            {
                egypt_level_tilemap[y][x] = TIL_PYRAMID_BROKEN_010 | TIL_PYRAMID_PALETTE_1_FLAG;
            }
            else if (tile_left_solid == 1 && tile_right_solid == 1)
            {
                egypt_level_tilemap[y][x] = TIL_PYRAMID_BROKEN_111 | TIL_PYRAMID_PALETTE_1_FLAG;
            }
            else if (tile_left_solid == 0 && tile_right_solid == 1)
            {
                egypt_level_tilemap[y][x] = TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG;
            }
            else if (tile_left_solid == 1 && tile_right_solid == 0)
            {
                egypt_level_tilemap[y][x] = TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG;
            }

            u8 tile_left_broken = ((tile_left&TIL_PYRAMID_ID_MASK) >= TIL_PYRAMID_BROKEN_FIRST_ID) ? 1 : 0;
            u8 tile_right_broken  = ((tile_right&TIL_PYRAMID_ID_MASK) >= TIL_PYRAMID_BROKEN_FIRST_ID) ? 1 : 0;

            if (tile_left_broken)
            {
                if (tile_left == (TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG))
                {
                    egypt_level_tilemap[y][x-1] = TIL_PYRAMID_BROKEN_010 | TIL_PYRAMID_PALETTE_1_FLAG;
                }
                else if (tile_left == (TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG))
                {
                    egypt_level_tilemap[y][x-1] = TIL_PYRAMID_BLANK;
                }
                else if (tile_left == (TIL_PYRAMID_BROKEN_111 | TIL_PYRAMID_PALETTE_1_FLAG))
                {
                    egypt_level_tilemap[y][x-1] = TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG;
                }
                else if (tile_left == (TIL_PYRAMID_BROKEN_101 | TIL_PYRAMID_PALETTE_1_FLAG))
                {
                    egypt_level_tilemap[y][x-1] = TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG;
                }
            }

            if (tile_right_broken)
            {
                if (tile_right == (TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG))
                {
                    egypt_level_tilemap[y][x+1] = TIL_PYRAMID_BROKEN_010 | TIL_PYRAMID_PALETTE_1_FLAG;
                }
                else if (tile_right == (TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG))
                {
                    egypt_level_tilemap[y][x+1] = TIL_PYRAMID_BLANK;
                }
                else if (tile_right == (TIL_PYRAMID_BROKEN_111 | TIL_PYRAMID_PALETTE_1_FLAG))
                {
                    egypt_level_tilemap[y][x+1] = TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG;
                }
                else if (tile_right == (TIL_PYRAMID_BROKEN_101 | TIL_PYRAMID_PALETTE_1_FLAG))
                {
                    egypt_level_tilemap[y][x+1] = TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG;
                }
            }

            u8 tile_up_broken = ((tile_up&TIL_PYRAMID_ID_MASK) >= TIL_PYRAMID_BROKEN_FIRST_ID) ? 1 : 0;
            if (tile_up_broken)
            {
                if (tile_up == (TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG))
                {
                    egypt_level_tilemap[y-1][x] = TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG;
                }
                else if (tile_up == (TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG))
                {
                    egypt_level_tilemap[y-1][x] = TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG;
                }
                else if (tile_up == (TIL_PYRAMID_BROKEN_111 | TIL_PYRAMID_PALETTE_1_FLAG))
                {
                    egypt_level_tilemap[y-1][x] = TIL_PYRAMID_BROKEN_101 | TIL_PYRAMID_PALETTE_1_FLAG;
                }
                else if (tile_up == (TIL_PYRAMID_BROKEN_010 | TIL_PYRAMID_PALETTE_1_FLAG))
                {
                    egypt_level_tilemap[y-1][x] = TIL_PYRAMID_BLANK;
                }
            }

            // Refresh BG
            bgInitMapSet(
                0,
                (u8*)egypt_level_tilemap,
                32*32*2,
                SC_32x32,
                VRAM_ADDR_BG0_MAP
            );

            return 1;
        }
    }

    return 0;
}

u8 check_egypt_level_pilot_collision(u8 x, u8 y)
{
    u8 map_x = x / 8;
    u8 map_y = y / 8;

    u8 tile = egypt_level_tilemap[map_y][map_x];
    u8 tile_id = tile & 0xF;
    u8 tile_mirror_x = (tile & 0x4000) ? 1 : 0;

    switch (tile_id)
    {
    case TIL_PYRAMID_SIDE_NORMAL:
    case TIL_PYRAMID_SIDE_GOLD:
    case TIL_PYRAMID_SIDE_GOLD_CRACK:
        {
            //(x)
            // 0 [ ][ ][ ][ ][ ][ ][ ][X]	X are the solid pixels of the tile.
            // 1 [ ][ ][ ][ ][ ][ ][X][X] 	There is a collision if (7-y) >= x
            // 2 [ ][ ][ ][ ][ ][X][X][X]
            // 3 [ ][ ][ ][ ][X][X][X][X]
            // 4 [ ][ ][ ][X][X][X][X][X]
            // 5 [ ][ ][X][X][X][X][X][X]
            // 6 [ ][X][X][X][X][X][X][X]
            // 7 [X][X][X][X][X][X][X][X]
            //    0  1  2  3  4  5  6  7 (y)

            u8 rem_x = x & 7;
            u8 rem_y = y & 7;

            u8 collision = 0;
            if (tile_mirror_x == 0)
            {
                collision = (7-rem_y) >= rem_x ? 1 : 0;
            }
            else
            {
                collision = rem_x <= rem_y ? 1 : 0;
            }

            return collision;
        }

    case TIL_PYRAMID_BLOCK_NORMAL:
    case TIL_PYRAMID_BLOCK_GOLD:
    case TIL_PYRAMID_BLOCK_GOLD_CRACK:
        return 1;
    }

    return 0;
}

u8 check_egypt_level_done()
{
    return (egypt_block_count == 0);
}
