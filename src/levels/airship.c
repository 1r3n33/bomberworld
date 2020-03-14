#include <snes.h>
#include "airship.h"
#include "graphics.h"
#include "pilot.h"

extern char airship_bg0_til_begin, airship_bg0_til_end;
extern char airship_bg0_pal_begin, airship_bg0_pal_end;
extern char airship_bg0_map_begin, airship_bg0_map_end;

extern char airship_bg1_til_begin, airship_bg1_til_end;
extern char airship_bg1_pal_begin, airship_bg1_pal_end;
extern char airship_bg1_map_begin, airship_bg1_map_end;

extern char airship_bg2_til_begin, airship_bg2_til_end;
extern char airship_bg2_pal_begin, airship_bg2_pal_end;
extern char airship_bg2_map_begin, airship_bg2_map_end;

u16 airship_bg0_scroll_y;
u16 airship_bg2_scroll_x;

u16 airship_anim_frame;

void init_airship_cutscene_gfx()
{
    REG_HDMAEN = 0;

    // Init backgrounds
    bgInitTileSet(
        0,
        &airship_bg0_til_begin,
        &airship_bg0_pal_begin,
        1,
        (&airship_bg0_til_end - &airship_bg0_til_begin),
        (&airship_bg0_pal_end - &airship_bg0_pal_begin),
        BG_16COLORS,
        VRAM_ADDR_BG0_GFX
    );
    bgInitMapSet(
        0,
        &airship_bg0_map_begin,
        (&airship_bg0_map_end - &airship_bg0_map_begin),
        SC_32x64,
        VRAM_ADDR_BG0_MAP
    );

    bgInitTileSet(
        1,
        &airship_bg1_til_begin,
        &airship_bg1_pal_begin,
        2,
        (&airship_bg1_til_end - &airship_bg1_til_begin),
        (&airship_bg1_pal_end - &airship_bg1_pal_begin),
        BG_16COLORS,
        VRAM_ADDR_BG1_GFX
    );
    bgInitMapSet(
        1,
        &airship_bg1_map_begin,
        (&airship_bg1_map_end - &airship_bg1_map_begin),
        SC_32x32,
        VRAM_ADDR_BG1_MAP
    );

    bgInitTileSet(
        2,
        &airship_bg2_til_begin,
        &airship_bg2_pal_begin,
        0,
        (&airship_bg2_til_end - &airship_bg2_til_begin),
        (&airship_bg2_pal_end - &airship_bg2_pal_begin),
        BG_4COLORS,
        VRAM_ADDR_BG2_GFX
    );
    bgInitMapSet(
        2,
        &airship_bg2_map_begin,
        (&airship_bg2_map_end - &airship_bg2_map_begin),
        SC_32x32,
        VRAM_ADDR_BG2_MAP
    );

    setMode(BG_MODE1, 0);

    bgSetDisable(3);

    bgSetScroll(0, 0, 0xFF);
    bgSetScroll(1, 0, 0xFF);
    bgSetScroll(2, 0, 0xFF);
    bgSetScroll(3, 0, 0xFF);
}

void init_airship_cutscene_state(u8 level)
{
    airship_bg0_scroll_y = 4096;
    airship_bg2_scroll_x = 0;
    airship_anim_frame = 0;
}

void update_airship_cutscene_gfx(u16 frame)
{
    // The airship goes down...
    if (airship_anim_frame < 60*9)
    {
        airship_anim_frame++;
        airship_bg0_scroll_y -= 4;
        bgSetScroll(0, 0, airship_bg0_scroll_y >> 4);
    }
    // ... then floating
    else
    {
        s8 scroll_offset[2] = { 1, -1 };

        u8 y = (frame >> 6) & 1;
        airship_bg0_scroll_y += scroll_offset[y];

        bgSetScroll(0, 0, airship_bg0_scroll_y >> 4);
    }

    // Sky is scrolling
    airship_bg2_scroll_x += 4;
    bgSetScroll(2, airship_bg2_scroll_x >> 4, 0xFF);
}

u8 airship_cutscene_loop()
{
    u16 frame = 0;

    struct pilot_t * p0 = get_pilot(0);
    struct pilot_t * p1 = get_pilot(1);

    // Adjust pilot gfx
    oamSet(OBJ_PILOT_0, 0xFF, 0xFF, 2, 0, 0, SPR_PILOT_0_FRAME_0, 0);
    oamSet(OBJ_PILOT_1, 0xFF, 0xFF, 2, 0, 0, SPR_PILOT_1_FRAME_0, 0);

    p0->x = (512-32)<<4;
    p0->y = 94;
    p1->x = (512-64)<<4;
    p1->y = 94;

    while(1)
    {
        spcProcess();
        WaitForVBlank();

        update_airship_cutscene_gfx(frame);

        if (frame > 60*7)
        {
            p0->x += 12;
            p1->x += 12;

            oamSetXY(OBJ_PILOT_0, p0->x>>4, p0->y);
            oamSetXY(OBJ_PILOT_1, p1->x>>4, p1->y);

            animate_pilot(0);
            animate_pilot(1);
        }

        frame++;
        if (frame > (60*11)+(30))
        {
            return 1;
        }
    }
}
