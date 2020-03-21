#ifndef AIRSHIP_H
#define AIRSHIP_H

#include <snes.h>

void init_airship_city_gfx();

void init_airship_egypt_gfx();

void init_airship_state(u8 level);

u8 airship_loop();

#endif // AIRSHIP_H
