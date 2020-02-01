#include <snes.h>
#include "editor.h"
#include "graphics.h"
#include "tilemap.h"

#include "levels/city.h"

extern char spr_editor_til_begin, spr_editor_til_end;
extern char spr_editor_pal_begin, spr_editor_pal_end;

u16 editor_tilemap[32][32];

struct cursor_t
{
    u8  spr;
    u8  x;
    u8  y;
    u8  state;
};

struct cursor_t cursors[2];

void do_nothing()
{
}

u16 * get_editor_tilemap()
{
    return (u16 *)editor_tilemap;
}

void init_cursor(u8 id)
{
    cursors[id].spr   = id * 4;
    cursors[id].x     = (id == 0) ? 8 : (256-16);
    cursors[id].y     = 25*8;
    cursors[id].state = 1;

    oamSet(cursors[id].spr, 0xFF, 0xFF, 3, 0, 0, 0, 0);
    oamSetEx(cursors[id].spr, OBJ_SMALL, OBJ_SHOW);
}

void move_cursor(u8 id, u16 pad)
{
    if (pad & KEY_UP)
    {
        cursors[id].y -= 8;
        if (cursors[id].y < (25-16)*8)
        {
            cursors[id].y = (25-16)*8;
        }
    }    

    if (pad & KEY_DOWN)
    {
        cursors[id].y += 8;
        if (cursors[id].y > (25*8))
        {
            cursors[id].y = (25*8);
        }
    }    

    if (pad & KEY_LEFT)
    {
        cursors[id].x -= 8;
        if (cursors[id].x < 8)
        {
            cursors[id].x = 8;
        }
    }    

    if (pad & KEY_RIGHT)
    {
        cursors[id].x += 8;
        if (cursors[id].x > (256-16))
        {
            cursors[id].x = (256-16);
        }
    }

    oamSetXY(cursors[id].spr, cursors[id].x, cursors[id].y);
}

void add_block(u8 id)
{
    u8 x = cursors[id].x/8;
    u8 y = cursors[id].y/8;

    editor_tilemap[y][x] = TIL_BUILDING_BODY_CENTER_WINDOW_0;

    bgInitMapSet(
        0,
        (u8*)editor_tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );
} 

void remove_block(u8 id)
{
    u8 x = cursors[id].x/8;
    u8 y = cursors[id].y/8;

    editor_tilemap[y][x] = 0;

    bgInitMapSet(
        0,
        (u8*)editor_tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );
} 

u8 run_editor()
{
    init_graphics();

	oamInitGfxSet(
		&spr_editor_til_begin,
		(&spr_editor_til_end-&spr_editor_til_begin),
		&spr_editor_pal_begin,
		(&spr_editor_pal_end-&spr_editor_pal_begin),
		0,
		VRAM_ADDR_SPR,
		OBJ_SIZE8
	);

    init_tilemap(do_nothing);

    init_cursor(0);
    init_cursor(1);

    setScreenOn();

    u8 frame_count = 0;
	while (1)
	{
        if ((frame_count & 3)==0) // input rate limiter. a superior version can be found in the titlescreen.
        {
            u16 pad0 = padsCurrent(0);
            u16 pad1 = padsCurrent(1);

            move_cursor(0, pad0);
            move_cursor(1, pad1);

            if (pad0 & KEY_A)
            {
                add_block(0);
            }
            if (pad0 & KEY_B)
            {
                remove_block(0);
            }

            if (pad1 & KEY_A)
            {
                add_block(1);
            }
            if (pad1 & KEY_B)
            {
                remove_block(1);
            }

            if (pad0 & KEY_START)
            {
                break;
            }
            if (pad1 & KEY_START)
            {
                break;
            }
        }

        WaitForVBlank();
        frame_count++;
    }

    setFadeEffect(FADE_OUT);
    return 1;
}
