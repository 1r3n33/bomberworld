#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <snes.h>

struct explosion_t
{
    u8 x;
    u8 y;
    u8 frame;
    u8 pad;
};

void init_explosion(u8 id, u8 x, u8 y);

void update_explosion(u8 id);

void disable_explosion(u8 id);

#endif // EXPLOSION_H
