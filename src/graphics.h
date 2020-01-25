#ifndef GRAPHICS_H
#define GRAPHICS_H

// Init game graphics
void init_graphics();

// Update graphics each frame
void update_graphics();

#define VRAM_ADDR_BG0_GFX    0x0000
#define VRAM_ADDR_BG0_MAP    0x1000
#define VRAM_ADDR_BG1_GFX    0x2000
#define VRAM_ADDR_BG1_MAP    0x3000
#define VRAM_ADDR_BG2_GFX    0x4000
#define VRAM_ADDR_BG2_MAP    0x5000
#define VRAM_ADDR_SPR        0x6000

// OAM OBJ id
#define OBJ_PILOT_0 0
#define OBJ_PILOT_1 4
#define OBJ_PILOT_2 8
#define OBJ_PILOT_3 12

#define OBJ_BOMB_0  16
#define OBJ_BOMB_1  20
#define OBJ_BOMB_2  24
#define OBJ_BOMB_3  28

#define OBJ_TEXT    128

#define OBJ_NEXT_AVAILABLE_ID  256 

// 16x16 sprite id (spr_game.bmp)
#define SPR_PILOT_0_FRAME_0     0
#define SPR_PILOT_0_FRAME_1     2
#define SPR_PILOT_1_FRAME_0     4
#define SPR_PILOT_1_FRAME_1     6
#define SPR_BOMB                8
#define SPR_EXPLOSION_FRAME_0   10
#define SPR_EXPLOSION_FRAME_1   12
#define SPR_EXPLOSION_FRAME_2   14

#define SPR_ASCIITABLE          256

// building tiles (bkg_buildings.bmp)
#define TIL_BUILDING_BLANK                      0
#define TIL_BUILDING_FLOOR_SIDE_DOOR            1
#define TIL_BUILDING_FLOOR_CENTER_DOOR          2
#define TIL_BUILDING_FLOOR_SIDE                 3
#define TIL_BUILDING_BODY_CENTER_WINDOW_0       4
#define TIL_BUILDING_BODY_CENTER_WINDOW_1       5
#define TIL_BUILDING_BODY_CENTER_WINDOW_2       6
#define TIL_BUILDING_BODY_CENTER_WINDOW_3       7
#define TIL_BUILDING_BODY_SIDE_WINDOW_0         8
#define TIL_BUILDING_BODY_SIDE_WINDOW_1         9
#define TIL_BUILDING_BODY_SIDE_WINDOW_2         10
#define TIL_BUILDING_BODY_SIDE_WINDOW_3         11
#define TIL_BUILDING_CEILING_CENTER_WINDOW_0    12
#define TIL_BUILDING_CEILING_CENTER_WINDOW_1    13
#define TIL_BUILDING_CEILING_CENTER_WINDOW_2    14
#define TIL_BUILDING_CEILING_CENTER_WINDOW_3    15
#define TIL_BUILDING_CEILING_SIDE_WINDOW_0      16
#define TIL_BUILDING_CEILING_SIDE_WINDOW_1      17
#define TIL_BUILDING_CEILING_SIDE_WINDOW_2      18
#define TIL_BUILDING_CEILING_SIDE_WINDOW_3      19
#define TIL_BUILDING_CEILING_TOWER_WINDOW_0     20
#define TIL_BUILDING_CEILING_TOWER_WINDOW_1     21
#define TIL_BUILDING_CEILING_TOWER_WINDOW_2     22
#define TIL_BUILDING_CEILING_TOWER_WINDOW_3     23
#define TIL_BUILDING_24                         24
#define TIL_BUILDING_25                         25
#define TIL_BUILDING_26                         26
#define TIL_BUILDING_27                         27
#define TIL_BUILDING_FLOOR_SIDE_BROKEN          28
#define TIL_BUILDING_BODY_SIDE_BROKEN           29
#define TIL_BUILDING_CEILING_SIDE_BROKEN        30
#define TIL_BUILDING_BODY_CENTER_BROKEN_0       31
#define TIL_BUILDING_BODY_CENTER_BROKEN_1       32
#define TIL_BUILDING_CEILING_TOWER_BROKEN       33

#endif // GRAPHICS_H
