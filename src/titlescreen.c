#include <snes.h>
#include "graphics.h"
#include "pilot.h"
#include "text.h"

extern char spr_game_til_begin, spr_game_til_end;
extern char spr_game_pal_begin, spr_game_pal_end;

extern char bkg_titlescreen_til_begin, bkg_titlescreen_til_end;
extern char bkg_titlescreen_pal_begin, bkg_titlescreen_pal_end;
extern char bkg_titlescreen_map_begin, bkg_titlescreen_map_end;

extern char bkg_sky_16_til_begin, bkg_sky_16_til_end;
extern char bkg_sky_16_pal_begin, bkg_sky_16_pal_end;
extern char bkg_sky_16_map_begin, bkg_sky_16_map_end;

extern char fonts_til_begin, fonts_til_end;
extern char fonts_pal_begin, fonts_pal_end;

void init_titlescreen()
{
    REG_HDMAEN = 0;
    setMode(BG_MODE1, 0);

    bgInitTileSet(
        0,
        &bkg_titlescreen_til_begin,
        &bkg_titlescreen_pal_begin,
        1,
        (&bkg_titlescreen_til_end - &bkg_titlescreen_til_begin),
        (&bkg_titlescreen_pal_end - &bkg_titlescreen_pal_begin),
        BG_16COLORS,
        VRAM_ADDR_BG0_GFX
    );
    bgInitMapSet(
        0,
        &bkg_titlescreen_map_begin,
        (&bkg_titlescreen_map_end - &bkg_titlescreen_map_begin),
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );

    bgInitTileSet(
        1,
        &bkg_sky_16_til_begin,
        &bkg_sky_16_pal_begin,
        3,
        (&bkg_sky_16_til_end - &bkg_sky_16_til_begin),
        (&bkg_sky_16_pal_end - &bkg_sky_16_pal_begin),
        BG_16COLORS,
        VRAM_ADDR_BG1_GFX
    );
    bgInitMapSet(
        1,
        &bkg_sky_16_map_begin,
        (&bkg_sky_16_map_end - &bkg_sky_16_map_begin),
        SC_32x32,
        VRAM_ADDR_BG1_MAP
    );

    // Important to disable non-used bkg to avoid artefacts
    bgSetDisable(2);
    bgSetDisable(3);

    bgSetScroll(0, 0, 8);
    bgSetScroll(1, 0, 160);

    // Init sprites. Inspired from oamInitGfxSet, use two sources of sprites.
    WaitForVBlank();

    // Sprites tiles
    dmaCopyVram(&spr_game_til_begin, VRAM_ADDR_SPR+0x0000, (&spr_game_til_end-&spr_game_til_begin));
    dmaCopyVram(&fonts_til_begin, VRAM_ADDR_SPR+0x1000, (&fonts_til_end-&fonts_til_begin));

    // Sprites palettes
    dmaCopyCGram(&spr_game_pal_begin, 128+(0*16), (&spr_game_pal_end-&spr_game_pal_begin));
    dmaCopyCGram(&fonts_pal_begin, 128+(1*16), (&fonts_pal_end-&fonts_pal_begin));

    REG_OBSEL = (0<<5) | (VRAM_ADDR_SPR >> 13); // 0<<5 is for 8x8+16x16 mode

    oamClear(0, 0);

    // Set text for menu
    reset_text();
    set_text(OBJ_TEXT,    "1 player",   (SCREEN_WIDTH/2)-32, (SCREEN_HEIGHT/2)+(0*16));
    set_text(OBJ_TEXT+32, "2 players",  (SCREEN_WIDTH/2)-32, (SCREEN_HEIGHT/2)+(1*16));
    set_text(OBJ_TEXT+68, "Map Editor", (SCREEN_WIDTH/2)-32, (SCREEN_HEIGHT/2)+(2*16));

    // Use pilot as menu cursor
    init_pilot(0);

    // Reset color math
    REG_CGWSEL = 0x00;
    REG_CGADSUB = 0x00;

    setScreenOn();
}

// Execute the titlescreen and return the selection.
u8 run_titlescreen()
{
    u16 bkg_scroll = 0;
    u8 input_throttle = 0; // prevent taking inputs into consideration for a number of frames
    u8 selection = 0;
    u8 selected = 0;

    init_titlescreen();

    while(selected == 0)
    {
        u16 pad0 = padsCurrent(0);

        if (input_throttle == 0)
        {
            if (pad0 & KEY_UP)
            {
                if (selection > 0)
                {
                    selection--;
                    input_throttle = 16;
                }
            }

            if (pad0 & KEY_DOWN)
            {
                if (selection < 2)
                {
                    selection++;
                    input_throttle = 16;
                }
            }

            if (pad0 & (KEY_A | KEY_START))
            {
                selected = 1;
            }
        }
        else
        {
            if (pad0 == 0)
            {
                input_throttle = 0;
            }
            else
            {
                input_throttle--;
            }
        }

        // Move cursor according to the selection.
        set_pilot_pos(0, (SCREEN_WIDTH/2)-64, (SCREEN_HEIGHT/2)+(selection*16)-4);

        // Animate.
        animate_pilot(0);
        bgSetScroll(1, bkg_scroll>>1, 160);
        bkg_scroll += 1;

        WaitForVBlank();
    }

    // Leave gracefully.
    setFadeEffect(FADE_OUT);
    reset_text();

    return selection;
}
