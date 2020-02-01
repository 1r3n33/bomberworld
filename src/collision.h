#ifndef COLLISION_H
#define COLLISION_H

#include <snes.h>

typedef u8 (*tilemap_collider)(u8, u8);

void bomb_tilemap_collision(u8 bomb_id, tilemap_collider checker);

void pilot_tilemap_collision(u8 pilot_id, tilemap_collider checker);

void bomb_pilot_collision(struct bomb_t * bomb, struct pilot_t * pilot);

#endif // COLLISION_H
