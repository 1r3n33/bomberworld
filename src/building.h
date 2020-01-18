#ifndef BUILDING_H
#define BUILDING_H

#include <snes.h>

void reset_building();

// Create the building BG
// min cannot be < 2
void init_building(u8 min, u8 max);

void copy_building(u16 * editor_tilemap);

// Get the BG map
u16 * get_building_tilemap();

// On bomb to building collision
void hit_building();

// Check if the building is completely destroyed
u16 is_building_destroyed();

#endif // BUILDING_H
