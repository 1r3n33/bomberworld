#include <snes.h>
#include "graphics.h"

// sprites
extern char spr_game_til_begin, spr_game_til_end;
extern char spr_game_pal_begin, spr_game_pal_end;

void init_graphics()
{
    // Init sprites. Inspired from oamInitGfxSet. Large mode for sprites.
    WaitForVBlank();

    // Sprites tiles
    dmaCopyVram(&spr_game_til_begin, VRAM_ADDR_SPR, (&spr_game_til_end-&spr_game_til_begin));

    // Sprites palettes
    dmaCopyCGram(&spr_game_pal_begin, 128, (&spr_game_pal_end-&spr_game_pal_begin));

    REG_OBSEL = (0<<5) | (VRAM_ADDR_SPR >> 13); // 0<<5 is for 8x8+16x16 mode
}
