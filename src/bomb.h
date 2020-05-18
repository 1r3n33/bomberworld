#ifndef BOMB_H
#define BOMB_H

#include <snes.h>

struct bomb_t
{
    u16 spr;	// OAM obj id
    u16 x;
    u16 y;
    u16 dropped;
};

// Bomb collision box (based on 16x16 sprite dimension)
#define BOMB_COLLISION_OFFSET_TOP       0	//
#define BOMB_COLLISION_OFFSET_BOTTOM    7  	// 1 pixel penetration looks better
#define BOMB_COLLISION_OFFSET_LEFT      5	//
#define BOMB_COLLISION_OFFSET_RIGHT     10	//

// Bomb flags
#define BOMB_0 0x01
#define BOMB_1 0x02

struct bomb_t * get_bomb(u8 id);

void init_bomb(u8 id);

void drop_bomb(u8 id, u16 x, u16 y);

void move_bomb(u8 id, u8 ground_y_pos);

u8 get_mega_bomb_mask(u8 i, u8 j);

#endif // BOMB_H
