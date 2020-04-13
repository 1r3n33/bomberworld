#ifndef ENDING_H
#define ENDING_H

#include <snes.h>

void init_ending_level_state(u8 level);

void init_ending_level_gfx();

void build_ending_level_tilemap();

void update_ending_level_gfx(u8 frame);

u8 check_ending_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right);

u8 check_ending_level_pilot_collision(u8 x, u8 y);

u8 check_ending_level_done();

#endif // ENDING_H