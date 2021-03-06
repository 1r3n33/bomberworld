#include <snes.h>
#include "bomb.h"
#include "graphics.h"
#include "shop.h"
#include "state.h"
#include "text.h"

#define SHOP_EXTRA_BOMB_FLAG      0x01
#define SHOP_EXTRA_LIFE_FLAG      0x02
#define SHOP_POWER_PROPELANT_FLAG 0x04
#define SHOP_MEGA_BOMB_FLAG       0x08
#define SHOP_UFO_FLAG             0x10

#define SHOP_GFX_BLANK                  0
#define SHOP_GFX_CURSOR_CORNER          1
#define SHOP_GFX_CURSOR_VERTICAL        2
#define SHOP_GFX_CURSOR_HORIZONTAL      3
#define SHOP_GFX_INTERROGATION_MARK_4X4 4
#define SHOP_GFX_GREEN_CHECK_MARK_4X4   20

extern char shop_bg0_til_begin, shop_bg0_til_end;
extern char shop_bg0_pal_begin, shop_bg0_pal_end;

extern char shop_bg1_til_begin, shop_bg1_til_end;
extern char shop_bg1_pal_begin, shop_bg1_pal_end;
extern char shop_bg1_map_begin, shop_bg1_map_end;

u16 shop_bg0_map[32][32];

u8 shop_current_selection;
u8 shop_available_items_mask;
u8 shop_selected_items_mask;
u8 shop_input_throttle;

u8 shop_has_moved = 0;

u16 shop_prices[] = { 200, 400, 500, 800, 1000 };

char ** shop_current_text = 0;

char * shop_str_greetings[3] =
{
    "Hi. My name is Grimo.",
    "What can I do",
    "for you Today?",
};

char * shop_str_welcomeback[3] =
{
    "Welcome back.",
};

char * shop_str_bomb[3] =
{
    "Second bomb?",
    "$200",
};

char * shop_str_life[3] =
{
    "Extra life?",
    "$400",
};

char * shop_str_propellant[3] =
{
    "Propellant?",
    "It has Guitonnol!",
    "$500",
};

char * shop_str_megabomb[3] =
{
    "The PGB bomb?",
    "$800",
};

char * shop_str_ufo[3] =
{
    "Flying saucer?",
    "$1000",
};

char * shop_str_not_yet_available[3] =
{
    "This item is not",
    "yet available.",
};

char * shop_str_already_bought[3] =
{
    "No more in stock!",
};

char * shop_str_exit[3] =
{
    "Already leaving?",
};

void print_text(char ** text)
{
    u16 id = OBJ_TEXT+128;
    id = set_text(id, text[0], 18, 18);
    id = set_text(id, text[1], 18, 28);
    id = set_text(id, text[2], 18, 38);
    for (; id <512; id+=4)
    {
        oamSetEx(id, OBJ_SMALL, OBJ_HIDE);
    }
};

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

void set_selected(u8 id)
{
    shop_bg0_map[16][(id*6)+2] = SHOP_GFX_GREEN_CHECK_MARK_4X4+0;
    shop_bg0_map[16][(id*6)+3] = SHOP_GFX_GREEN_CHECK_MARK_4X4+1;
    shop_bg0_map[16][(id*6)+4] = SHOP_GFX_GREEN_CHECK_MARK_4X4+2;
    shop_bg0_map[16][(id*6)+5] = SHOP_GFX_GREEN_CHECK_MARK_4X4+3;
    shop_bg0_map[17][(id*6)+2] = SHOP_GFX_GREEN_CHECK_MARK_4X4+4;
    shop_bg0_map[17][(id*6)+3] = SHOP_GFX_GREEN_CHECK_MARK_4X4+5;
    shop_bg0_map[17][(id*6)+4] = SHOP_GFX_GREEN_CHECK_MARK_4X4+6;
    shop_bg0_map[17][(id*6)+5] = SHOP_GFX_GREEN_CHECK_MARK_4X4+7;
    shop_bg0_map[18][(id*6)+2] = SHOP_GFX_GREEN_CHECK_MARK_4X4+8;
    shop_bg0_map[18][(id*6)+3] = SHOP_GFX_GREEN_CHECK_MARK_4X4+9;
    shop_bg0_map[18][(id*6)+4] = SHOP_GFX_GREEN_CHECK_MARK_4X4+10;
    shop_bg0_map[18][(id*6)+5] = SHOP_GFX_GREEN_CHECK_MARK_4X4+11;
    shop_bg0_map[19][(id*6)+2] = SHOP_GFX_GREEN_CHECK_MARK_4X4+12;
    shop_bg0_map[19][(id*6)+3] = SHOP_GFX_GREEN_CHECK_MARK_4X4+13;
    shop_bg0_map[19][(id*6)+4] = SHOP_GFX_GREEN_CHECK_MARK_4X4+14;
    shop_bg0_map[19][(id*6)+5] = SHOP_GFX_GREEN_CHECK_MARK_4X4+15;
}

void shop_clear_cursor()
{
    if (shop_current_selection == 0xFF)
    {
        shop_bg0_map[21][25] = SHOP_GFX_BLANK;
        shop_bg0_map[21][26] = SHOP_GFX_BLANK;
        shop_bg0_map[21][27] = SHOP_GFX_BLANK;
        shop_bg0_map[21][28] = SHOP_GFX_BLANK;
        shop_bg0_map[21][29] = SHOP_GFX_BLANK;
        shop_bg0_map[21][30] = SHOP_GFX_BLANK;
        shop_bg0_map[22][25] = SHOP_GFX_BLANK;
        shop_bg0_map[22][30] = SHOP_GFX_BLANK;
        shop_bg0_map[23][25] = SHOP_GFX_BLANK;
        shop_bg0_map[23][30] = SHOP_GFX_BLANK;
        shop_bg0_map[24][25] = SHOP_GFX_BLANK;
        shop_bg0_map[24][30] = SHOP_GFX_BLANK;
        shop_bg0_map[25][25] = SHOP_GFX_BLANK;
        shop_bg0_map[25][30] = SHOP_GFX_BLANK;
        shop_bg0_map[26][25] = SHOP_GFX_BLANK;
        shop_bg0_map[26][26] = SHOP_GFX_BLANK;
        shop_bg0_map[26][27] = SHOP_GFX_BLANK;
        shop_bg0_map[26][28] = SHOP_GFX_BLANK;
        shop_bg0_map[26][29] = SHOP_GFX_BLANK;
        shop_bg0_map[26][30] = SHOP_GFX_BLANK;
    }
    else
    {
        shop_bg0_map[15][(shop_current_selection*6)+1] = SHOP_GFX_BLANK;
        shop_bg0_map[15][(shop_current_selection*6)+2] = SHOP_GFX_BLANK;
        shop_bg0_map[15][(shop_current_selection*6)+3] = SHOP_GFX_BLANK;
        shop_bg0_map[15][(shop_current_selection*6)+4] = SHOP_GFX_BLANK;
        shop_bg0_map[15][(shop_current_selection*6)+5] = SHOP_GFX_BLANK;
        shop_bg0_map[15][(shop_current_selection*6)+6] = SHOP_GFX_BLANK;
        shop_bg0_map[16][(shop_current_selection*6)+1] = SHOP_GFX_BLANK;
        shop_bg0_map[16][(shop_current_selection*6)+6] = SHOP_GFX_BLANK;
        shop_bg0_map[17][(shop_current_selection*6)+1] = SHOP_GFX_BLANK;
        shop_bg0_map[17][(shop_current_selection*6)+6] = SHOP_GFX_BLANK;
        shop_bg0_map[18][(shop_current_selection*6)+1] = SHOP_GFX_BLANK;
        shop_bg0_map[18][(shop_current_selection*6)+6] = SHOP_GFX_BLANK;
        shop_bg0_map[19][(shop_current_selection*6)+1] = SHOP_GFX_BLANK;
        shop_bg0_map[19][(shop_current_selection*6)+6] = SHOP_GFX_BLANK;
        shop_bg0_map[20][(shop_current_selection*6)+1] = SHOP_GFX_BLANK;
        shop_bg0_map[20][(shop_current_selection*6)+2] = SHOP_GFX_BLANK;
        shop_bg0_map[20][(shop_current_selection*6)+3] = SHOP_GFX_BLANK;
        shop_bg0_map[20][(shop_current_selection*6)+4] = SHOP_GFX_BLANK;
        shop_bg0_map[20][(shop_current_selection*6)+5] = SHOP_GFX_BLANK;
        shop_bg0_map[20][(shop_current_selection*6)+6] = SHOP_GFX_BLANK;
    }
}

void shop_set_cursor()
{
    if (shop_current_selection == 0xFF)
    {
        shop_bg0_map[21][25] = SHOP_GFX_CURSOR_CORNER;
        shop_bg0_map[21][26] = SHOP_GFX_CURSOR_HORIZONTAL;
        shop_bg0_map[21][27] = SHOP_GFX_CURSOR_HORIZONTAL;
        shop_bg0_map[21][28] = SHOP_GFX_CURSOR_HORIZONTAL;
        shop_bg0_map[21][29] = SHOP_GFX_CURSOR_HORIZONTAL;
        shop_bg0_map[21][30] = SHOP_GFX_CURSOR_CORNER | 0x4000;
        shop_bg0_map[22][25] = SHOP_GFX_CURSOR_VERTICAL;
        shop_bg0_map[22][30] = SHOP_GFX_CURSOR_VERTICAL | 0x4000;
        shop_bg0_map[23][25] = SHOP_GFX_CURSOR_VERTICAL;
        shop_bg0_map[23][30] = SHOP_GFX_CURSOR_VERTICAL | 0x4000;
        shop_bg0_map[24][25] = SHOP_GFX_CURSOR_VERTICAL;
        shop_bg0_map[24][30] = SHOP_GFX_CURSOR_VERTICAL | 0x4000;
        shop_bg0_map[25][25] = SHOP_GFX_CURSOR_VERTICAL;
        shop_bg0_map[25][30] = SHOP_GFX_CURSOR_VERTICAL | 0x4000;
        shop_bg0_map[26][25] = SHOP_GFX_CURSOR_CORNER | 0x8000;
        shop_bg0_map[26][26] = SHOP_GFX_CURSOR_HORIZONTAL | 0x8000;
        shop_bg0_map[26][27] = SHOP_GFX_CURSOR_HORIZONTAL | 0x8000;
        shop_bg0_map[26][28] = SHOP_GFX_CURSOR_HORIZONTAL | 0x8000;
        shop_bg0_map[26][29] = SHOP_GFX_CURSOR_HORIZONTAL | 0x8000;
        shop_bg0_map[26][30] = SHOP_GFX_CURSOR_CORNER | 0x4000 | 0x8000;
    }
    else
    {
        shop_bg0_map[15][(shop_current_selection*6)+1] = SHOP_GFX_CURSOR_CORNER;
        shop_bg0_map[15][(shop_current_selection*6)+2] = SHOP_GFX_CURSOR_HORIZONTAL;
        shop_bg0_map[15][(shop_current_selection*6)+3] = SHOP_GFX_CURSOR_HORIZONTAL;
        shop_bg0_map[15][(shop_current_selection*6)+4] = SHOP_GFX_CURSOR_HORIZONTAL;
        shop_bg0_map[15][(shop_current_selection*6)+5] = SHOP_GFX_CURSOR_HORIZONTAL;
        shop_bg0_map[15][(shop_current_selection*6)+6] = SHOP_GFX_CURSOR_CORNER | 0x4000;
        shop_bg0_map[16][(shop_current_selection*6)+1] = SHOP_GFX_CURSOR_VERTICAL;
        shop_bg0_map[16][(shop_current_selection*6)+6] = SHOP_GFX_CURSOR_VERTICAL | 0x4000;
        shop_bg0_map[17][(shop_current_selection*6)+1] = SHOP_GFX_CURSOR_VERTICAL;
        shop_bg0_map[17][(shop_current_selection*6)+6] = SHOP_GFX_CURSOR_VERTICAL | 0x4000;
        shop_bg0_map[18][(shop_current_selection*6)+1] = SHOP_GFX_CURSOR_VERTICAL;
        shop_bg0_map[18][(shop_current_selection*6)+6] = SHOP_GFX_CURSOR_VERTICAL | 0x4000;
        shop_bg0_map[19][(shop_current_selection*6)+1] = SHOP_GFX_CURSOR_VERTICAL;
        shop_bg0_map[19][(shop_current_selection*6)+6] = SHOP_GFX_CURSOR_VERTICAL | 0x4000;
        shop_bg0_map[20][(shop_current_selection*6)+1] = SHOP_GFX_CURSOR_CORNER | 0x8000;
        shop_bg0_map[20][(shop_current_selection*6)+2] = SHOP_GFX_CURSOR_HORIZONTAL | 0x8000;
        shop_bg0_map[20][(shop_current_selection*6)+3] = SHOP_GFX_CURSOR_HORIZONTAL | 0x8000;
        shop_bg0_map[20][(shop_current_selection*6)+4] = SHOP_GFX_CURSOR_HORIZONTAL | 0x8000;
        shop_bg0_map[20][(shop_current_selection*6)+5] = SHOP_GFX_CURSOR_HORIZONTAL | 0x8000;
        shop_bg0_map[20][(shop_current_selection*6)+6] = SHOP_GFX_CURSOR_CORNER | 0x4000 | 0x8000;
    }
}

void init_shop_state(u8 level)
{
    u8 available_items[] =
    {
        SHOP_EXTRA_BOMB_FLAG | SHOP_EXTRA_LIFE_FLAG | SHOP_POWER_PROPELANT_FLAG,
        SHOP_EXTRA_BOMB_FLAG | SHOP_EXTRA_LIFE_FLAG | SHOP_POWER_PROPELANT_FLAG | SHOP_MEGA_BOMB_FLAG,
        SHOP_EXTRA_BOMB_FLAG | SHOP_EXTRA_LIFE_FLAG | SHOP_POWER_PROPELANT_FLAG | SHOP_MEGA_BOMB_FLAG | SHOP_UFO_FLAG,
    };

    shop_current_selection = 0;

    shop_available_items_mask = available_items[level];

    shop_input_throttle = 0;

    if (level == 0)
    {
        shop_current_text = shop_str_greetings;
    }
    else
    {
        shop_current_text = shop_str_welcomeback;
    }

    shop_has_moved = 0;

    // Set items
    u8 i, j;
    for (j=0; j<32; j++)
    {
        for (i=0; i<32; i++)
        {
            shop_bg0_map[j][i] = 0;
        }
    }

    shop_selected_items_mask = 0;
    if (get_player_max_bombs(0) > 1)
    {
        shop_selected_items_mask |= SHOP_EXTRA_BOMB_FLAG;
        set_selected(0);
    }
    if (get_player_max_lives(0) > 2)
    {
        shop_selected_items_mask |= SHOP_EXTRA_LIFE_FLAG;
        set_selected(1);
    }
    if (get_player_propellant(0) != 0xFF)
    {
        shop_selected_items_mask |= SHOP_POWER_PROPELANT_FLAG;
        set_selected(2);
    }
    if (find_player_mega_bomb(0) > 0)
    {
        shop_selected_items_mask |= SHOP_MEGA_BOMB_FLAG;
        set_selected(3);
    }
    if (get_player_ufo(0) > 0)
    {
        shop_selected_items_mask |= SHOP_UFO_FLAG;
        set_selected(4);
    }

    for (i=0; i<5; i++)
    {
        u8 flag = 1<<i;
        if ((shop_available_items_mask & flag) == 0)
        {
            shop_bg0_map[16][(i*6)+2] = SHOP_GFX_INTERROGATION_MARK_4X4+0;
            shop_bg0_map[16][(i*6)+3] = SHOP_GFX_INTERROGATION_MARK_4X4+1;
            shop_bg0_map[16][(i*6)+4] = SHOP_GFX_INTERROGATION_MARK_4X4+2;
            shop_bg0_map[16][(i*6)+5] = SHOP_GFX_INTERROGATION_MARK_4X4+3;
            shop_bg0_map[17][(i*6)+2] = SHOP_GFX_INTERROGATION_MARK_4X4+4;
            shop_bg0_map[17][(i*6)+3] = SHOP_GFX_INTERROGATION_MARK_4X4+5;
            shop_bg0_map[17][(i*6)+4] = SHOP_GFX_INTERROGATION_MARK_4X4+6;
            shop_bg0_map[17][(i*6)+5] = SHOP_GFX_INTERROGATION_MARK_4X4+7;
            shop_bg0_map[18][(i*6)+2] = SHOP_GFX_INTERROGATION_MARK_4X4+8;
            shop_bg0_map[18][(i*6)+3] = SHOP_GFX_INTERROGATION_MARK_4X4+9;
            shop_bg0_map[18][(i*6)+4] = SHOP_GFX_INTERROGATION_MARK_4X4+10;
            shop_bg0_map[18][(i*6)+5] = SHOP_GFX_INTERROGATION_MARK_4X4+11;
            shop_bg0_map[19][(i*6)+2] = SHOP_GFX_INTERROGATION_MARK_4X4+12;
            shop_bg0_map[19][(i*6)+3] = SHOP_GFX_INTERROGATION_MARK_4X4+13;
            shop_bg0_map[19][(i*6)+4] = SHOP_GFX_INTERROGATION_MARK_4X4+14;
            shop_bg0_map[19][(i*6)+5] = SHOP_GFX_INTERROGATION_MARK_4X4+15;
        }
    }

    shop_clear_cursor();
    shop_set_cursor();
    // Cannot force the text to appear on fade in because
    // of reset_text after calling this init_state method.
    //print_text(shop_current_text);

    bgInitMapSet(
        0,
        (u8*)shop_bg0_map,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );

    release_player_bomb(0, BOMB_0 | BOMB_1);
    release_player_bomb(1, BOMB_0 | BOMB_1);
}

u8 shop_selection()
{
    u16 pad0 = padsCurrent(0);
    if (pad0)
    {
        shop_has_moved = 1;
    }

    if (shop_input_throttle == 0)
    {
        if (pad0 & KEY_LEFT)
        {
            if (shop_current_selection > 0 && shop_current_selection != 0xFF)
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

        if (pad0 & KEY_A)
        {
            if (shop_current_selection == 0xFF)
            {
                return 1;
            }
            else
            {
                u8 flag = 1 << shop_current_selection;
                flag = flag & shop_available_items_mask;
                flag = flag & ~shop_selected_items_mask;
                if (flag && score_transaction(0, -shop_prices[shop_current_selection]))
                {
                    shop_selected_items_mask |= flag;
                    set_selected(shop_current_selection);

                    if (flag & SHOP_EXTRA_LIFE_FLAG)
                    {
                        max_out_player_lives(0);
                    }

                    if (flag & SHOP_POWER_PROPELANT_FLAG)
                    {
                        max_out_player_propellant(0);
                    }

                    if (flag & SHOP_EXTRA_BOMB_FLAG)
                    {
                        max_out_player_bombs(0);
                    }

                    if (flag & SHOP_MEGA_BOMB_FLAG)
                    {
                        max_out_player_mega_bombs(0);
                    }

                    if (flag & SHOP_UFO_FLAG)
                    {
                        set_player_ufo(0);
                    }

                    compute_ui_elements();
                    display_ui_elements(0);
                    display_ui_elements(1);
                }
            }
        }
        else if (shop_has_moved == 1)
        {
            if (shop_current_selection == 0xFF)
            {
                shop_current_text = shop_str_exit;
            }
            else
            {
                u8 flag = 1 << shop_current_selection;
                if ((flag & shop_available_items_mask) == 0)
                {
                    shop_current_text = shop_str_not_yet_available;
                }
                else if ((flag & shop_selected_items_mask))
                {
                    shop_current_text = shop_str_already_bought;
                }
                else
                {
                    switch (flag)
                    {
                    case SHOP_EXTRA_LIFE_FLAG:
                        shop_current_text = shop_str_life;
                        break;

                    case SHOP_EXTRA_BOMB_FLAG:
                        shop_current_text = shop_str_bomb;
                        break;

                    case SHOP_POWER_PROPELANT_FLAG:
                        shop_current_text = shop_str_propellant;
                        break;

                    case SHOP_MEGA_BOMB_FLAG:
                        shop_current_text = shop_str_megabomb;
                        break;

                    case SHOP_UFO_FLAG:
                        shop_current_text = shop_str_ufo;
                        break;

                    default:
                        break;
                    }
                }
            }
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

        print_text(shop_current_text);

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
