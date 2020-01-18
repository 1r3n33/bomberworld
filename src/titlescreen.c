#include <snes.h>
#include "graphics.h"
#include "pilot.h"

// sprites
extern char gfxpsrite, gfxpsrite_end;
extern char palsprite, palsprite_end;

extern char bkg_titlescreen_til_begin, bkg_titlescreen_til_end;
extern char bkg_titlescreen_pal_begin, bkg_titlescreen_pal_end;
extern char bkg_titlescreen_map_begin, bkg_titlescreen_map_end;

extern char bkg_sky_16_til_begin, bkg_sky_16_til_end;
extern char bkg_sky_16_pal_begin, bkg_sky_16_pal_end;
extern char bkg_sky_16_map_begin, bkg_sky_16_map_end;

void init_titlescreen()
{
	setMode(BG_MODE1, 0);

    bgInitTileSet(
		0,
		&bkg_titlescreen_til_begin,
		&bkg_titlescreen_pal_begin,
		0,
		(&bkg_titlescreen_til_end - &bkg_titlescreen_til_begin),
		(&bkg_titlescreen_pal_end - &bkg_titlescreen_pal_begin),
		BG_16COLORS,
		VRAM_BG0_GFX
	);
	bgInitMapSet(
		0,
		&bkg_titlescreen_map_begin,
		(&bkg_titlescreen_map_end - &bkg_titlescreen_map_begin),
		SC_32x32,
		VRAM_BG0_MAP
	);

	bgInitTileSet(
		1,
		&bkg_sky_16_til_begin,
		&bkg_sky_16_pal_begin,
		2,
		(&bkg_sky_16_til_end - &bkg_sky_16_til_begin),
		(&bkg_sky_16_pal_end - &bkg_sky_16_pal_begin),
		BG_16COLORS,
		VRAM_BG1_GFX
	);
	bgInitMapSet(
		1,
		&bkg_sky_16_map_begin,
		(&bkg_sky_16_map_end - &bkg_sky_16_map_begin),
		SC_32x32,
		VRAM_BG1_MAP
	);

    // Important to disable non-used bkg to avoid artefacts
	bgSetDisable(2);
	bgSetDisable(3);

	bgSetScroll(0, 0, 8);
	bgSetScroll(1, 0, 160);

	// Init Sprites gfx and palette with default size of 32x32.
	oamInitGfxSet(
		&gfxpsrite,
		(&gfxpsrite_end-&gfxpsrite),
		&palsprite,
		(&palsprite_end-&palsprite),
		0,
		VRAM_SPR_32x32,
		OBJ_SIZE32
	); 

    init_pilot(0);
    set_pilot_pos(0, 128-16, 112+16);

    setScreenOn();
}

u8 run_titlescreen()
{
    u16 i = 0;
    u16 scroll = 0;

    init_titlescreen();

    for (i=0; i<60*1; i++)
    {
	    bgSetScroll(1, scroll>>4, 160);
        WaitForVBlank();
        scroll += 8;
    }

    setFadeEffect(FADE_OUT);
    return 0;
}
