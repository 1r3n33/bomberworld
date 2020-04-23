#ifndef DEBRIS_H
#define DEBRIS_H

#include <snes.h>

struct debris_t
{
    u16 x, y;
};

void reset_debris();

void init_debris(u16 x, u16 y);

void update_debris(u8 frame);

#endif // DEBRIS_H
