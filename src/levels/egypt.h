#ifndef EGYPT_H
#define EGYPT_H

#include <snes.h>

void init_egypt_level_state(u8 level);

void init_egypt_level_gfx();

void build_egypt_level_tilemap(u16 tilemap[32][32]);

void update_egypt_level_gfx();

u8 check_egypt_level_bomb_collision(u8 x, u8 y);

u8 check_egypt_level_pilot_collision(u8 x, u8 y);

u8 check_egypt_level_done();

#endif // EGYPT_H