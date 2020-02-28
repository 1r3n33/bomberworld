#ifndef SEA_H
#define SEA_H

#include <snes.h>

void init_sea_level_state(u8 level);

void init_sea_level_gfx();

void build_sea_level_tilemap();

void update_sea_level_gfx(u8 frame);

u8 check_sea_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right);

u8 check_sea_level_pilot_collision(u8 x, u8 y);

u8 check_sea_level_done();

#endif // SEA_H
