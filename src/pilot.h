#ifndef PILOT_H
#define PILOT_H

#include <snes.h>

struct pilot_t
{
	u16 spr;
	u16 x;
	u16 y;
	u16 pad;
};

// Pilot collision box (based on 16x16 sprite dimension)
#define PILOT_BUILDING_COLLISION_OFFSET_TOP		0
#define PILOT_BOMB_COLLISION_OFFSET_TOP     	6
#define PILOT_BUILDING_COLLISION_OFFSET_BOTTOM	11
#define PILOT_BOMB_COLLISION_OFFSET_BOTTOM		13
#define PILOT_COLLISION_OFFSET_LEFT      		0
#define PILOT_COLLISION_OFFSET_RIGHT     		15

struct pilot_t * get_pilot(u8 id);

void init_pilot(u8 id);

void move_pilot(u8 id, u8 speed);

void set_pilot_pos(u8 id, u16 x, u16 y);

u8 is_pilot_entirely_on_screen(u8 id, u8 off_left, u8 off_right);

#endif // PILOT_H
