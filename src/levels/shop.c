#include <snes.h>
#include "graphics.h"
#include "shop.h"

#define SHOP_SIMPLE_BOMB_FLAG     0x01
#define SHOP_EXTRA_LIFE_FLAG      0x02
#define SHOP_POWER_PROPELANT_FLAG 0x04
#define SHOP_MEGA_BOMB_FLAG       0x08
#define SHOP_UFO_FLAG             0x10

extern char shop_bg0_til_begin, shop_bg0_til_end;
extern char shop_bg0_pal_begin, shop_bg0_pal_end;

extern char shop_bg1_til_begin, shop_bg1_til_end;
extern char shop_bg1_pal_begin, shop_bg1_pal_end;
extern char shop_bg1_map_begin, shop_bg1_map_end;

u16 shop_bg0_map[32][32];

u8 shop_current_selection;
u8 shop_items_mask;
u8 shop_input_throttle;

void init_shop_gfx()
{
    REG_HDMAEN = 0;

    // Init backgrounds
    bgInitTileSet(
        0,
        &shop_bg0_til_begin,
        &shop_bg0_pal_begin,
        0,
        (&shop_bg0_til_end - &shop_bg0_til_begin),
        (&shop_bg0_pal_end - &shop_bg0_pal_begin),
        BG_16COLORS,
        VRAM_ADDR_BG0_GFX
    );

    bgInitTileSet(
        1,
        &shop_bg1_til_begin,
        &shop_bg1_pal_begin,
        1,
        (&shop_bg1_til_end - &shop_bg1_til_begin),
        (&shop_bg1_pal_end - &shop_bg1_pal_begin),
        BG_16COLORS,
        VRAM_ADDR_BG1_GFX
    );
    bgInitMapSet(
        1,
        &shop_bg1_map_begin,
        (&shop_bg1_map_end - &shop_bg1_map_begin),
        SC_32x32,
        VRAM_ADDR_BG1_MAP
    );

    setMode(BG_MODE1, 0);

    bgSetDisable(2);
    bgSetDisable(3);

    bgSetScroll(0, 0, 0xFF);
    bgSetScroll(1, 0, 0xFF);
    bgSetScroll(2, 0, 0xFF);
    bgSetScroll(3, 0, 0xFF);
}

void init_shop_state(u8 level)
{
    shop_current_selection = 0;
    shop_items_mask = SHOP_SIMPLE_BOMB_FLAG | SHOP_EXTRA_LIFE_FLAG | SHOP_POWER_PROPELANT_FLAG;
    shop_input_throttle = 0;

    // Set items
    u8 i, j;
    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            shop_bg0_map[j][i] = 0;
        }
    }

    for (i=0; i<5; i++)
    {
        u8 flag = 1<<i;
        if ((shop_items_mask & flag) == 0)
        {
            shop_bg0_map[16][(i*6)+2] = 4;
            shop_bg0_map[16][(i*6)+3] = 5;
            shop_bg0_map[16][(i*6)+4] = 6;
            shop_bg0_map[16][(i*6)+5] = 7;
            shop_bg0_map[17][(i*6)+2] = 8;
            shop_bg0_map[17][(i*6)+3] = 9;
            shop_bg0_map[17][(i*6)+4] = 10;
            shop_bg0_map[17][(i*6)+5] = 11;
            shop_bg0_map[18][(i*6)+2] = 12;
            shop_bg0_map[18][(i*6)+3] = 13;
            shop_bg0_map[18][(i*6)+4] = 14;
            shop_bg0_map[18][(i*6)+5] = 15;
            shop_bg0_map[19][(i*6)+2] = 16;
            shop_bg0_map[19][(i*6)+3] = 17;
            shop_bg0_map[19][(i*6)+4] = 18;
            shop_bg0_map[19][(i*6)+5] = 19;
        }
    }

    bgInitMapSet(
        0,
        (u8*)shop_bg0_map,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );
}

u8 shop_selection()
{
    u16 pad0 = padsCurrent(0);

    if (shop_input_throttle == 0)
    {
        if (pad0 & KEY_LEFT)
        {
            if (shop_current_selection > 0)
            {
                shop_current_selection--;
                shop_input_throttle = 16;
            }
        }

        if (pad0 & KEY_RIGHT)
        {
            if (shop_current_selection < 4)
            {
                shop_current_selection++;
                shop_input_throttle = 16;
            }
        }

        if (pad0 & KEY_DOWN)
        {
            if (shop_current_selection != 0xFF)
            {
                shop_current_selection = 0xFF;
                shop_input_throttle = 16;
            }
        }

        if (pad0 & KEY_UP)
        {
            if (shop_current_selection == 0xFF)
            {
                shop_current_selection = 4;
                shop_input_throttle = 16;
            }
        }

        if (pad0 & (KEY_A | KEY_START))
        {
            return 1;
        }
    }
    else
    {
        if (pad0 == 0)
        {
            shop_input_throttle = 0;
        }
        else
        {
            shop_input_throttle--;
        }
    }

    return 0;
}

void shop_clear_cursor()
{
    if (shop_current_selection == 0xFF)
    {
        shop_bg0_map[21][25] = 0;
        shop_bg0_map[21][26] = 0;
        shop_bg0_map[21][27] = 0;
        shop_bg0_map[21][28] = 0;
        shop_bg0_map[21][29] = 0;
        shop_bg0_map[21][30] = 0;
        shop_bg0_map[22][25] = 0;
        shop_bg0_map[22][30] = 0;
        shop_bg0_map[23][25] = 0;
        shop_bg0_map[23][30] = 0;
        shop_bg0_map[24][25] = 0;
        shop_bg0_map[24][30] = 0;
        shop_bg0_map[25][25] = 0;
        shop_bg0_map[25][30] = 0;
        shop_bg0_map[26][25] = 0;
        shop_bg0_map[26][26] = 0;
        shop_bg0_map[26][27] = 0;
        shop_bg0_map[26][28] = 0;
        shop_bg0_map[26][29] = 0;
        shop_bg0_map[26][30] = 0;
    }
    else
    {
        shop_bg0_map[15][(shop_current_selection*6)+1] = 0;
        shop_bg0_map[15][(shop_current_selection*6)+2] = 0;
        shop_bg0_map[15][(shop_current_selection*6)+3] = 0;
        shop_bg0_map[15][(shop_current_selection*6)+4] = 0;
        shop_bg0_map[15][(shop_current_selection*6)+5] = 0;
        shop_bg0_map[15][(shop_current_selection*6)+6] = 0;
        shop_bg0_map[16][(shop_current_selection*6)+1] = 0;
        shop_bg0_map[16][(shop_current_selection*6)+6] = 0;
        shop_bg0_map[17][(shop_current_selection*6)+1] = 0;
        shop_bg0_map[17][(shop_current_selection*6)+6] = 0;
        shop_bg0_map[18][(shop_current_selection*6)+1] = 0;
        shop_bg0_map[18][(shop_current_selection*6)+6] = 0;
        shop_bg0_map[19][(shop_current_selection*6)+1] = 0;
        shop_bg0_map[19][(shop_current_selection*6)+6] = 0;
        shop_bg0_map[20][(shop_current_selection*6)+1] = 0;
        shop_bg0_map[20][(shop_current_selection*6)+2] = 0;
        shop_bg0_map[20][(shop_current_selection*6)+3] = 0;
        shop_bg0_map[20][(shop_current_selection*6)+4] = 0;
        shop_bg0_map[20][(shop_current_selection*6)+5] = 0;
        shop_bg0_map[20][(shop_current_selection*6)+6] = 0;
    }
}

void shop_set_cursor()
{
    if (shop_current_selection == 0xFF)
    {
        shop_bg0_map[21][25] = 1;
        shop_bg0_map[21][26] = 3;
        shop_bg0_map[21][27] = 3;
        shop_bg0_map[21][28] = 3;
        shop_bg0_map[21][29] = 3;
        shop_bg0_map[21][30] = 1 | 0x4000;
        shop_bg0_map[22][25] = 2;
        shop_bg0_map[22][30] = 2 | 0x4000;
        shop_bg0_map[23][25] = 2;
        shop_bg0_map[23][30] = 2 | 0x4000;
        shop_bg0_map[24][25] = 2;
        shop_bg0_map[24][30] = 2 | 0x4000;
        shop_bg0_map[25][25] = 2;
        shop_bg0_map[25][30] = 2 | 0x4000;
        shop_bg0_map[26][25] = 1 | 0x8000;
        shop_bg0_map[26][26] = 3 | 0x8000;
        shop_bg0_map[26][27] = 3 | 0x8000;
        shop_bg0_map[26][28] = 3 | 0x8000;
        shop_bg0_map[26][29] = 3 | 0x8000;
        shop_bg0_map[26][30] = 1 | 0x4000 | 0x8000;
    }
    else
    {
        shop_bg0_map[15][(shop_current_selection*6)+1] = 1;
        shop_bg0_map[15][(shop_current_selection*6)+2] = 3;
        shop_bg0_map[15][(shop_current_selection*6)+3] = 3;
        shop_bg0_map[15][(shop_current_selection*6)+4] = 3;
        shop_bg0_map[15][(shop_current_selection*6)+5] = 3;
        shop_bg0_map[15][(shop_current_selection*6)+6] = 1 | 0x4000;
        shop_bg0_map[16][(shop_current_selection*6)+1] = 2;
        shop_bg0_map[16][(shop_current_selection*6)+6] = 2 | 0x4000;
        shop_bg0_map[17][(shop_current_selection*6)+1] = 2;
        shop_bg0_map[17][(shop_current_selection*6)+6] = 2 | 0x4000;
        shop_bg0_map[18][(shop_current_selection*6)+1] = 2;
        shop_bg0_map[18][(shop_current_selection*6)+6] = 2 | 0x4000;
        shop_bg0_map[19][(shop_current_selection*6)+1] = 2;
        shop_bg0_map[19][(shop_current_selection*6)+6] = 2 | 0x4000;
        shop_bg0_map[20][(shop_current_selection*6)+1] = 1 | 0x8000;
        shop_bg0_map[20][(shop_current_selection*6)+2] = 3 | 0x8000;
        shop_bg0_map[20][(shop_current_selection*6)+3] = 3 | 0x8000;
        shop_bg0_map[20][(shop_current_selection*6)+4] = 3 | 0x8000;
        shop_bg0_map[20][(shop_current_selection*6)+5] = 3 | 0x8000;
        shop_bg0_map[20][(shop_current_selection*6)+6] = 1 | 0x4000 | 0x8000;
    }
}

u8 shop_loop()
{
    while(1)
    {
        spcProcess();
        WaitForVBlank();

        shop_clear_cursor();

        u8 exit = shop_selection();
        if (exit)
        {
            return 1;
        }

        shop_set_cursor();

        bgInitMapSet(
            0,
            (u8*)shop_bg0_map,
            32*32*2,
            SC_32x32,
            VRAM_ADDR_BG0_MAP
        );
    }
}
