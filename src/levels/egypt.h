#ifndef EGYPT_H
#define EGYPT_H

#include <snes.h>

#define TIL_PYRAMID_BLANK               0   // no palette
#define TIL_PYRAMID_SIDE_NORMAL         1   // palette 1
#define TIL_PYRAMID_BLOCK_NORMAL        2   // palette 1
#define TIL_PYRAMID_SIDE_GOLD           3   // palette 0
#define TIL_PYRAMID_BLOCK_GOLD          4   // palette 0
#define TIL_PYRAMID_SIDE_GOLD_CRACK     5   // palette 0
#define TIL_PYRAMID_BLOCK_GOLD_CRACK    6   // palette 0

#define TIL_PYRAMID_BROKEN_001          7   // palette 1
#define TIL_PYRAMID_BROKEN_011          8   // palette 1
#define TIL_PYRAMID_BROKEN_101          9   // palette 1
#define TIL_PYRAMID_BROKEN_111          10  // palette 1
#define TIL_PYRAMID_BROKEN_010          11  // palette 1

#define TIL_PYRAMID_BROKEN_FIRST_ID     TIL_PYRAMID_BROKEN_001

#define TIL_PYRAMID_ID_MASK             0x00FF // remove mirror_x flag & palette_id
#define TIL_PYRAMID_MIRROR_X_FLAG       0x4000
#define TIL_PYRAMID_PALETTE_1_FLAG      0x0400

void init_egypt_level_state(u8 level);

void init_egypt_level_gfx();

void build_egypt_level_tilemap();

void build_egypt_level_editor_tilemap(u16 * tilemap);

void update_egypt_level_gfx(u8 frame);

u8 check_egypt_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right);

u8 check_egypt_level_pilot_collision(u8 x, u8 y);

u8 check_egypt_level_done();

void mega_bomb_collision(u8 x, u8 y);

#endif // EGYPT_H
