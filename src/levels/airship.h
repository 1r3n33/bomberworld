#ifndef AIRSHIP_H
#define AIRSHIP_H

#include <snes.h>

void init_airship_cutscene_gfx();

void init_airship_cutscene_state(u8 level);

u8 airship_cutscene_loop();

#endif // AIRSHIP_H
