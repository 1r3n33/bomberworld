#ifndef TILEMAP_H
#define TILEMAP_H

#include <snes.h>

typedef void (*tilemap_builder)(u16 [32][32]);

void init_tilemap(tilemap_builder builder);

u16 * get_tilemap();

#endif // TILEMAP_H
