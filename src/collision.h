#ifndef COLLISION_H
#define COLLISION_H

#include <snes.h>

// Returns 0 (no collision), 1 (normal collision: bomb-1), 2 (bomb disabling collision)
typedef u8 (*tilemap_box_collider_t)(u8 top, u8 bottom, u8 left, u8 right);

typedef u8 (*tilemap_point_collider_t)(u8 x, u8 y);

u8 bomb_tilemap_collision(u8 bomb_id, tilemap_box_collider_t collider);

u8 pilot_tilemap_collision(u8 pilot_id, tilemap_point_collider_t collider);

u8 bomb_pilot_collision(struct bomb_t * bomb, struct pilot_t * pilot);

#endif // COLLISION_H
