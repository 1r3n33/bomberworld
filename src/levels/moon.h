#ifndef MOON_H
#define MOON_H

#include <snes.h>

void init_moon_level_state(u8 level);

void init_moon_level_gfx();

void init_moon_boss_level_gfx();

void build_moon_level_tilemap();

void build_moon_boss_level_tilemap();

void build_moon_level_editor_tilemap(u16 * tilemap);

void update_moon_level_gfx(u8 frame);

void update_moon_boss_level_gfx(u8 frame);

u8 check_moon_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right);

u8 check_moon_boss_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right);

u8 check_moon_level_pilot_collision(u8 x, u8 y);

u8 check_moon_level_done();

#endif