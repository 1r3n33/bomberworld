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
    const u16 mask_palette0 = 0x0000;
    const u16 mask_palette1 = 0x0400;
    const u16 mask_mirrorx  = 0x4000;

	u8 i, j, height;

	egypt_block_count = 0;

    height = 0;
	for (i=2; i<16; i++)
    {
        for (j=0; j<height; j++)
        {
            tilemap[26-j][i] = 2 | mask_palette1;
            tilemap[26-j][31-i] = 2 | mask_palette1 | mask_mirrorx;
        }
        tilemap[26-j][i] = 1 | mask_palette1;
        tilemap[26-j][31-i] = 1 | mask_palette1 | mask_mirrorx;

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
	return 0;
}

u8 check_egypt_level_pilot_collision(u8 x, u8 y)
{
	return 0;
}

u8 check_egypt_level_done()
{
	return 0;
}
