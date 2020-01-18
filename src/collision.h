#ifndef COLLISION_H
#define COLLISION_H

// Check bomb to buildings collision
void bomb_buildings_collision(struct bomb_t * bomb, u8 id);

// Check bomb to pilot collision
void bomb_pilot_collision(struct bomb_t * bomb, struct pilot_t * pilot);

// Check pilot to buildings collision
void pilot_buildings_collision(struct pilot_t * pilot, u8 id);

#endif // COLLISION_H
