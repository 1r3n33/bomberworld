#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <snes.h>

#define PILOT_0_EXPLOSION_ID 4
#define PILOT_1_EXPLOSION_ID 5

struct explosion_t
{
    u16 x;
    u16 y;
    u8  frame;
    u8  pad;
};

void init_explosion(u8 id, u16 x, u16 y);

u8 update_explosion(u8 id);

void disable_explosion(u8 id);

#endif // EXPLOSION_H
