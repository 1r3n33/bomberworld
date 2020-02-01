#ifndef GRAPHICS_H
#define GRAPHICS_H

// Init game graphics
void init_graphics();

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

#endif // GRAPHICS_H
