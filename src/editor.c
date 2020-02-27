#include <snes.h>
#include "editor.h"
#include "graphics.h"
#include "tilemap.h"

#include "levels/city.h"

extern char spr_editor_til_begin, spr_editor_til_end;
extern char spr_editor_pal_begin, spr_editor_pal_end;

u16 editor_tilemap[32][32];

// Keep cursor inside the safe frame
#define SAFE_FRAME_TOP    (9*8)
#define SAFE_FRAME_BOTTOM (25*8)
#define SAFE_FRAME_LEFT   (1*8)
#define SAFE_FRAME_RIGHT  (30*8)

#define SPR_EDITOR_CURSOR_VALID     0
#define SPR_EDITOR_CURSOR_INVALID   2

struct cursor_t
{
    u8  spr;
    u8  x;
    u8  y;
    u8  state;
};

struct cursor_t cursors[2];

void editor_tilemap_builder(u16 tilemap[32][32])
{
    bgInitMapSet(
        0,
        (u8*)editor_tilemap,
        32*32*2,
        SC_32x32,
        VRAM_ADDR_BG0_MAP
    );
}

u16 * get_editor_tilemap()
{
    return (u16 *)editor_tilemap;
}

void init_cursor(u8 id)
{
    cursors[id].spr   = id * 4;
    cursors[id].x     = (id == 0) ? SAFE_FRAME_LEFT : SAFE_FRAME_RIGHT;
    cursors[id].y     = SAFE_FRAME_BOTTOM;
    cursors[id].state = SPR_EDITOR_CURSOR_VALID;

    oamSet(cursors[id].spr, 0xFF, 0xFF, 3, 0, 0, cursors[id].state, 0);
    oamSetEx(cursors[id].spr, OBJ_SMALL, OBJ_SHOW);
}

void update_cursor(u8 id, u16 pad)
{
    // Update position
    if (pad & KEY_UP)
    {
        cursors[id].y -= 8;
        if (cursors[id].y < SAFE_FRAME_TOP)
        {
            cursors[id].y = SAFE_FRAME_TOP;
        }
    }

    if (pad & KEY_DOWN)
    {
        cursors[id].y += 8;
        if (cursors[id].y > SAFE_FRAME_BOTTOM)
        {
            cursors[id].y = SAFE_FRAME_BOTTOM;
        }
    }

    if (pad & KEY_LEFT)
    {
        cursors[id].x -= 8;
        if (cursors[id].x < SAFE_FRAME_LEFT)
        {
            cursors[id].x = SAFE_FRAME_LEFT;
        }
    }

    if (pad & KEY_RIGHT)
    {
        cursors[id].x += 8;
        if (cursors[id].x > SAFE_FRAME_RIGHT)
        {
            cursors[id].x = SAFE_FRAME_RIGHT;
        }
    }

    // Check validity
    // If the cursor is on the bottom line, always OK as we start building up blocks from the ground.
    // If the cursor is above the bottom line, check if there is a block below or under the cursor.
    // This last rule makes edition a little less fun...
    if (cursors[id].y == SAFE_FRAME_BOTTOM)
    {
        cursors[id].state = SPR_EDITOR_CURSOR_VALID;
    }
    else
    {
        u8 x = cursors[id].x/8;
        u8 y = cursors[id].y/8;

        u16 under = editor_tilemap[y][x];
        u16 below = editor_tilemap[y+1][x];

        if (under | below)
        {
            cursors[id].state = SPR_EDITOR_CURSOR_VALID;
        }
        else
        {
            cursors[id].state = SPR_EDITOR_CURSOR_INVALID;
        }
    }

    // Update display
    oamSet(cursors[id].spr, cursors[id].x, cursors[id].y, 3, 0, 0, cursors[id].state, 0);
}

void add_block(u8 id)
{
    if (cursors[id].state == SPR_EDITOR_CURSOR_INVALID)
    {
        return;
    }

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
    if (cursors[id].state == SPR_EDITOR_CURSOR_INVALID)
    {
        return;
    }

    u8 x = cursors[id].x/8;
    u8 y = cursors[id].y/8;

    // Even if the cursor is valid, we may not be able to remove...
    u16 above = editor_tilemap[y-1][x];
    if (above)
    {
        // Keep the valid state but draw as invalid.
        oamSet(cursors[id].spr, cursors[id].x, cursors[id].y, 3, 0, 0, SPR_EDITOR_CURSOR_INVALID, 0);
        return;
    }

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
    init_city_level_gfx();

    oamInitGfxSet(
        &spr_editor_til_begin,
        (&spr_editor_til_end-&spr_editor_til_begin),
        &spr_editor_pal_begin,
        (&spr_editor_pal_end-&spr_editor_pal_begin),
        0,
        VRAM_ADDR_SPR,
        OBJ_SIZE8
    );

    init_tilemap(editor_tilemap_builder);

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

            update_cursor(0, pad0);
            update_cursor(1, pad1);

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
