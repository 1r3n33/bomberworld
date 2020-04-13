#include <snes.h>
#include "graphics.h"

char credits[32][33] =
{
//---0123456789ABCDEF0123456789ABCDEF"
    " Thanks for playing Bomberworld ",
    "",
    "",
    "",
//---"0123456789ABCDEF0123456789ABCDEF"
    " Design, Prog, Gfx and Sound by ",
    "        Ulysse Caroulle",
    "        Irenee Caroulle",
    "",
//---"0123456789ABCDEF0123456789ABCDEF"
    "",
    "",
    "       Special Thanks to",
    "            Alekmaul",
//---"0123456789ABCDEF0123456789ABCDEF"
    "            Dave Town",
    "         Juhani Junkala",
    "      Olivier Guittonneau",
    "             NoCash",
//---"0123456789ABCDEF0123456789ABCDEF"
    "              Near",
    "",
    "",
    "",
//---"0123456789ABCDEF0123456789ABCDEF"
    "            The End",
    "",
    "",
    "",
//---"0123456789ABCDEF0123456789ABCDEF"
    "",
    "",
    "",
    "",
//---"0123456789ABCDEF0123456789ABCDEF"
    "",
    "",
    "",
    "",
//---"0123456789ABCDEF0123456789ABCDEF"
};

extern char ending_bg0_til_begin, ending_bg0_til_end;
extern char ending_bg0_pal_begin, ending_bg0_pal_end;

extern char ending_bg1_til_begin, ending_bg1_til_end;
extern char ending_bg1_pal_begin, ending_bg1_pal_end;
extern char ending_bg1_map_begin, ending_bg1_map_end;

u16 ending_level_tilemap[64][32];
u16 ending_block_count = 0;

u16 ending_bg0_scroll_y = 0;

void init_ending_level_state(u8 level)
{

}

void init_ending_level_gfx()
{
    REG_HDMAEN = 0;

    // Init backgrounds
    bgInitTileSet(
        0,
        &ending_bg0_til_begin,
        &ending_bg0_pal_begin,
        0,
        (&ending_bg0_til_end - &ending_bg0_til_begin),
        (&ending_bg0_pal_end - &ending_bg0_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG0_GFX
    );
    bgInitTileSet(
        1,
        &ending_bg1_til_begin,
        &ending_bg1_pal_begin,
        0,
        (&ending_bg1_til_end - &ending_bg1_til_begin),
        (&ending_bg1_pal_end - &ending_bg1_pal_begin),
        BG_4COLORS0,
        VRAM_ADDR_BG1_GFX
    );
    bgInitMapSet(
        1,
        &ending_bg1_map_begin,
        (&ending_bg1_map_end - &ending_bg1_map_begin),
        SC_32x32,
        VRAM_ADDR_BG1_MAP
    );

    setMode(BG_MODE0, 0);

    ending_bg0_scroll_y = 280;

    bgSetDisable(2);
    bgSetDisable(3);
    bgSetScroll(0, 0, ending_bg0_scroll_y);
    bgSetScroll(1, 0, 247);
    bgSetScroll(2, 0, 0xFF);
    bgSetScroll(3, 0, 0xFF);
}

void build_ending_level_tilemap()
{
    u8 i, j;
    for (j=0; j<64; j++)
    {
        for (i=0; i<32; i++)
        {
            ending_level_tilemap[j][i] = 0;
        }
    }
    ending_block_count = 0;

    for (j=0; j<32; j++)
    {
        char * str = credits[j];
        for (i=0; str[i] != 0; i++)
        {
            u8 tile = str[i]-32;
            if (tile > 0)
            {
                ending_level_tilemap[j][i] = tile;
                ending_block_count++;
            }
        }
    }

    bgInitMapSet(
        0,
        (u8*)ending_level_tilemap,
        64*32*2,
        SC_32x64,
        VRAM_ADDR_BG0_MAP
    );
}

void update_ending_level_gfx(u8 frame)
{
    if ((frame & 15) == 15)
    {
        ending_bg0_scroll_y++;
        bgSetScroll(0, 0, ending_bg0_scroll_y);
    }
}

u8 check_ending_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right)
{
    u8 map_min_x = (left) / 8;
    u8 map_max_x = ((right)-1) / 8;

    u16 real_bottom = (u16)bottom + ending_bg0_scroll_y;
    u8 map_max_y = (((real_bottom)-1)%512) / 8;

    u8 x = map_min_x;
    u8 y = map_max_y;
    for (; x<=map_max_x; x++)
    {
        u16 tile = ending_level_tilemap[y][x];
        if (tile > 0)
        {
            ending_block_count--;
            ending_level_tilemap[y][x] = 0;

            bgInitMapSet(
                0,
                (u8*)ending_level_tilemap,
                64*32*2,
                SC_32x64,
                VRAM_ADDR_BG0_MAP
            );

            return 2;
        }
    }

    return 0;
}

u8 check_ending_level_pilot_collision(u8 x, u8 y)
{
    return 0;
}

u8 check_ending_level_done()
{
    return (ending_bg0_scroll_y > 680) || (ending_block_count == 0);
}