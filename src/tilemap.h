#ifndef TILEMAP_H
#define TILEMAP_H

#include <snes.h>

typedef void (*tilemap_builder_t)();

void init_tilemap(tilemap_builder_t builder);

#endif // TILEMAP_H
