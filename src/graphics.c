#include <snes.h>
#include "building.h"
#include "graphics.h"

// sprites
extern char gfxpsrite, gfxpsrite_end;
extern char palsprite, palsprite_end;

// title screen backgrounds

// game backgrounds
extern char bkg_city0_til_begin, bkg_city0_til_end;
extern char bkg_city0_pal_begin, bkg_city0_pal_end;
extern char bkg_city0_map_begin, bkg_city0_map_end;

extern char bkg_sky_4_til_begin, bkg_sky_4_til_end;
extern char bkg_sky_4_pal_begin, bkg_sky_4_pal_end;
extern char bkg_sky_4_map_begin, bkg_sky_4_map_end;

extern char bkg_buildings_til_begin, bkg_buildings_til_end;
extern char bkg_buildings_pal_begin, bkg_buildings_pal_end;

u16 bg0_scroll_x = 0;
u16 bg1_scroll_x = 0;
u16 bg2_scroll_x = 0;
u16 bg3_scroll_x = 0;

void init_graphics()
{
    u16 i, j;

	u16 * map;

	// Init backgrounds
	bgInitTileSet(
		0,
		&bkg_buildings_til_begin,
		&bkg_buildings_pal_begin,
		0,
		(&bkg_buildings_til_end - &bkg_buildings_til_begin),
		(&bkg_buildings_pal_end - &bkg_buildings_pal_begin),
		BG_4COLORS0,
		VRAM_ADDR_BG0_GFX
	);
	bgInitTileSet(
		1,
		&bkg_city0_til_begin,
		&bkg_city0_pal_begin,
		0,
		(&bkg_city0_til_end - &bkg_city0_til_begin),
		(&bkg_city0_pal_end - &bkg_city0_pal_begin),
		BG_4COLORS0,
		VRAM_ADDR_BG1_GFX
	);
	bgInitMapSet(
		1,
		&bkg_city0_map_begin,
		(&bkg_city0_map_end - &bkg_city0_map_begin),
		SC_32x32,
		VRAM_ADDR_BG1_MAP
	);
	bgInitTileSet(
		2,
		&bkg_sky_4_til_begin,
		&bkg_sky_4_pal_begin,
		0,
		(&bkg_sky_4_til_end - &bkg_sky_4_til_begin),
		(&bkg_sky_4_pal_end - &bkg_sky_4_pal_begin),
		BG_4COLORS0,
		VRAM_ADDR_BG2_GFX
	);
	bgInitMapSet(
		2,
		&bkg_sky_4_map_begin,
		(&bkg_sky_4_map_end - &bkg_sky_4_map_begin),
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
	bgSetScroll(0, bg0_scroll_x >> 4, 0xFF);
	bgSetScroll(1, bg1_scroll_x >> 4, 0xFF);
	bgSetScroll(2, bg2_scroll_x >> 4, 0xFF);
	bgSetScroll(3, bg3_scroll_x >> 4, 0xFF);

	// Init Sprites gfx and palette with default size of 32x32.
	oamInitGfxSet(
		&gfxpsrite,
		(&gfxpsrite_end-&gfxpsrite),
		&palsprite,
		(&palsprite_end-&palsprite),
		0,
		VRAM_ADDR_SPR,
		OBJ_SIZE32
	);
}

void update_graphics()
{
	bg2_scroll_x += 4;
	bgSetScroll(2, bg2_scroll_x >> 4, 0xFF);
}