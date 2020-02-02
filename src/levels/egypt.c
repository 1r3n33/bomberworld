#include <snes.h>
#include "egypt.h"
#include "graphics.h"

// backgrounds
extern char bkg_egypt_til_begin, bkg_egypt_til_end;
extern char bkg_egypt_pal_begin, bkg_egypt_pal_end;
extern char bkg_egypt_map_begin, bkg_egypt_map_end;

extern char til_pyramid_til_begin, til_pyramid_til_end;
extern char til_pyramid_pal_begin, til_pyramid_pal_end;

u16 egypt_bg0_scroll_x = 0;
u16 egypt_bg1_scroll_x = 0;
u16 egypt_bg2_scroll_x = 0;
u16 egypt_bg3_scroll_x = 0;

u16 egypt_block_count = 0;

u8 egypt_min_building_size = 2;
u8 egypt_max_building_size = 8;

u16 * egypt_level_tilemap = 0;

void init_egypt_level_state(u8 level)
{
}

void init_egypt_level_gfx()
{
	// Init backgrounds
	bgInitTileSet(
		0,
		&til_pyramid_til_begin,
		&til_pyramid_pal_begin,
		0,
		(&til_pyramid_til_end - &til_pyramid_til_begin),
		(&til_pyramid_pal_end - &til_pyramid_pal_begin),
		BG_4COLORS0,
		VRAM_ADDR_BG0_GFX
	);
	bgInitTileSet(
		1,
		&bkg_egypt_til_begin,
		&bkg_egypt_pal_begin,
		0,
		(&bkg_egypt_til_end - &bkg_egypt_til_begin),
		(&bkg_egypt_pal_end - &bkg_egypt_pal_begin),
		BG_4COLORS0,
		VRAM_ADDR_BG1_GFX
	);
	bgInitMapSet(
		1,
		&bkg_egypt_map_begin,
		(&bkg_egypt_map_end - &bkg_egypt_map_begin),
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
}

void build_egypt_level_tilemap(u16 tilemap[32][32])
{
    const u16 mask_palette_0 = 0x0000;
    const u16 mask_palette_1 = 0x0400;
    const u16 mask_mirror_x  = 0x4000;

	u8 i, j, height;

	egypt_block_count = 0;

    height = 0;
	for (i=2; i<16; i++)
    {
        for (j=0; j<height; j++)
        {
            tilemap[26-j][i] = 2 | mask_palette_1;
            tilemap[26-j][31-i] = 2 | mask_palette_1 | mask_mirror_x;
        }
        tilemap[26-j][i] = 1 | mask_palette_1;
        tilemap[26-j][31-i] = 1 | mask_palette_1 | mask_mirror_x;

        egypt_block_count += (2*(height+1));
        height++;
    }

	egypt_level_tilemap = (u16*)tilemap;
}

void update_egypt_level_gfx()
{
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
		u8 tile = egypt_level_tilemap[(y*32)+x];
		u8 tile_id = tile & TIL_PYRAMID_ID_MASK;

		if (tile_id > 0 && tile_id < TIL_PYRAMID_BROKEN_FIRST_ID)
		{
			egypt_block_count--;

			// Change the look of the surrounding tiles
			u16 tile_left  = egypt_level_tilemap[(y*32)+(x-1)];
			u16 tile_right = egypt_level_tilemap[(y*32)+(x+1)];
			u16 tile_up    = egypt_level_tilemap[((y-1)*32)+x];

			u8 tile_left_solid = (tile_left > 0 && (tile_left&TIL_PYRAMID_ID_MASK) < TIL_PYRAMID_BROKEN_FIRST_ID) ? 1 : 0;
			u8 tile_right_solid = (tile_right > 0 && (tile_right&TIL_PYRAMID_ID_MASK) < TIL_PYRAMID_BROKEN_FIRST_ID) ? 1 : 0;

			if (tile_left_solid == 0 && tile_right_solid == 0)
			{
				egypt_level_tilemap[(y*32)+x] = TIL_PYRAMID_BROKEN_010 | TIL_PYRAMID_PALETTE_1_FLAG;
			}
			else if (tile_left_solid == 1 && tile_right_solid == 1)
			{
				egypt_level_tilemap[(y*32)+x] = TIL_PYRAMID_BROKEN_111 | TIL_PYRAMID_PALETTE_1_FLAG;
			}
			else if (tile_left_solid == 0 && tile_right_solid == 1)
			{
				egypt_level_tilemap[(y*32)+x] = TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG;
			}
			else if (tile_left_solid == 1 && tile_right_solid == 0)
			{
				egypt_level_tilemap[(y*32)+x] = TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG;
			}
			
			u8 tile_left_broken = ((tile_left&TIL_PYRAMID_ID_MASK) >= TIL_PYRAMID_BROKEN_FIRST_ID) ? 1 : 0;
			u8 tile_right_broken  = ((tile_right&TIL_PYRAMID_ID_MASK) >= TIL_PYRAMID_BROKEN_FIRST_ID) ? 1 : 0;

			if (tile_left_broken)
			{
				if (tile_left == (TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG))
				{
					egypt_level_tilemap[(y*32)+(x-1)] = TIL_PYRAMID_BROKEN_010 | TIL_PYRAMID_PALETTE_1_FLAG;
				} 
				else if (tile_left == (TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG))
				{
					egypt_level_tilemap[(y*32)+(x-1)] = TIL_PYRAMID_BLANK;
				} 
				else if (tile_left == (TIL_PYRAMID_BROKEN_111 | TIL_PYRAMID_PALETTE_1_FLAG))
				{
					egypt_level_tilemap[(y*32)+(x-1)] = TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG;
				}
				else if (tile_left == (TIL_PYRAMID_BROKEN_101 | TIL_PYRAMID_PALETTE_1_FLAG))
				{
					egypt_level_tilemap[(y*32)+(x-1)] = TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG;
				}
			}

			if (tile_right_broken)
			{
				if (tile_right == (TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG))
				{
					egypt_level_tilemap[(y*32)+(x+1)] = TIL_PYRAMID_BROKEN_010 | TIL_PYRAMID_PALETTE_1_FLAG;
				} 
				else if (tile_right == (TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG))
				{
					egypt_level_tilemap[(y*32)+(x+1)] = TIL_PYRAMID_BLANK;
				} 
				else if (tile_right == (TIL_PYRAMID_BROKEN_111 | TIL_PYRAMID_PALETTE_1_FLAG))
				{
					egypt_level_tilemap[(y*32)+(x+1)] = TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG;
				}
				else if (tile_right == (TIL_PYRAMID_BROKEN_101 | TIL_PYRAMID_PALETTE_1_FLAG))
				{
					egypt_level_tilemap[(y*32)+(x+1)] = TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG;
				}
			}

			u8 tile_up_broken = ((tile_up&TIL_PYRAMID_ID_MASK) >= TIL_PYRAMID_BROKEN_FIRST_ID) ? 1 : 0;
			if (tile_up_broken)
			{
				if (tile_up == (TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG))
				{
					egypt_level_tilemap[((y-1)*32)+x] = TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG;
				} 
				else if (tile_up == (TIL_PYRAMID_BROKEN_011 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG))
				{
					egypt_level_tilemap[((y-1)*32)+x] = TIL_PYRAMID_BROKEN_001 | TIL_PYRAMID_PALETTE_1_FLAG | TIL_PYRAMID_MIRROR_X_FLAG;
				} 
				else if (tile_up == (TIL_PYRAMID_BROKEN_111 | TIL_PYRAMID_PALETTE_1_FLAG))
				{
					egypt_level_tilemap[((y-1)*32)+x] = TIL_PYRAMID_BROKEN_101 | TIL_PYRAMID_PALETTE_1_FLAG;
				} 
				else if (tile_up == (TIL_PYRAMID_BROKEN_010 | TIL_PYRAMID_PALETTE_1_FLAG))
				{
					egypt_level_tilemap[((y-1)*32)+x] = TIL_PYRAMID_BLANK;
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

	u8 tile = egypt_level_tilemap[(map_y*32)+map_x];
	u8 tile_id = tile & 0xF;
	u8 tile_mirror_x = (tile & 0x4000) ? 1 : 0;

	if (tile_id == 1)
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

	if (tile_id == 2)
	{
		return 1;
	}

	return 0;
}

u8 check_egypt_level_done()
{
	return (egypt_block_count == 0);
}
