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
#define OBJ_BOMB_EXPLOSION_0 0   // Draw explosions in front of bombs and pilots
#define OBJ_BOMB_EXPLOSION_1 4   //
#define OBJ_BOMB_EXPLOSION_2 8   //
#define OBJ_BOMB_EXPLOSION_3 12  //

#define OBJ_PILOT_EXPLOSION_0   16 //
#define OBJ_PILOT_EXPLOSION_1   20 //

#define OBJ_PILOT_0      24  //
#define OBJ_P0_BOMB_0    28  // Draw bomb behind pilot_0
#define OBJ_P0_BOMB_1    32  // Draw bomb behind pilot_0
#define OBJ_P0_BOMB_MEGA 36  // Draw bomb behind pilot_0
#define OBJ_PILOT_1      40  // Draw pilot_1 behind pilot_0 bombs
#define OBJ_P1_BOMB_0    44  // Draw bomb behind pilot_1
#define OBJ_P1_BOMB_1    48  // Draw bomb behind pilot_1
#define OBJ_P1_BOMB_MEGA 52  // Draw bomb behind pilot_1
#define OBJ_DEBRIS_0     56
#define OBJ_DEBRIS_1     60
#define OBJ_DEBRIS_2     64
#define OBJ_DEBRIS_3     68
#define OBJ_DEBRIS_4     72
#define OBJ_DEBRIS_5     76
#define OBJ_DEBRIS_6     80
#define OBJ_DEBRIS_7     84
#define OBJ_DEBRIS_8     88
#define OBJ_DEBRIS_9     92
#define OBJ_DEBRIS_10    96
#define OBJ_DEBRIS_11    100
#define OBJ_DEBRIS_12    104
#define OBJ_DEBRIS_13    108
#define OBJ_DEBRIS_14    112
#define OBJ_DEBRIS_15    116
#define OBJ_BOSS_EYE_0   120
#define OBJ_BOSS_EYE_1   124
#define OBJ_BOSS_EYE_2   128
#define OBJ_BOSS_EYE_3   132
#define OBJ_BOSS_EYE_4   136

#define OBJ_TEXT         192

// 16x16 sprite id (spr_game.bmp)
#define SPR_PILOT_0_FRAME_0     0
#define SPR_PILOT_0_FRAME_1     2
#define SPR_PILOT_1_FRAME_0     4
#define SPR_PILOT_1_FRAME_1     6
#define SPR_BOMB                8
#define SPR_EXPLOSION_FRAME_0   10
#define SPR_EXPLOSION_FRAME_1   12
#define SPR_EXPLOSION_FRAME_2   14

#define SPR_EXPLOSION_FRAME_3   32
#define SPR_MEGA_BOMB           34
#define SPR_UFO_FRAME_0         36
#define SPR_UFO_FRAME_1         38
#define SPR_UFO_FRAME_2         40
#define SPR_DEBRIS_CITY         42
#define SPR_DEBRIS_EGYPT        44
#define SPR_DEBRIS_SHIP         46

#define SPR_DEBRIS_TENTACLES    64
#define SPR_BOSS_EYE            66

#define SPR_ASCIITABLE          256

#endif // GRAPHICS_H
