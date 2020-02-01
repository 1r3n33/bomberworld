#ifndef COLLISION_H
#define COLLISION_H

#include <snes.h>

typedef u8 (*tilemap_collider_t)(u8, u8);

void bomb_tilemap_collision(u8 bomb_id, tilemap_collider_t collider);

void pilot_tilemap_collision(u8 pilot_id, tilemap_collider_t collider);

void bomb_pilot_collision(struct bomb_t * bomb, struct pilot_t * pilot);

#endif // COLLISION_H
