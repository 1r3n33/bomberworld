#ifndef GAME_H
#define GAME_H

#include <snes.h>

#define GAME_MODE_FLAG_1P           0x00
#define GAME_MODE_FLAG_2P           0x01
#define GAME_MODE_FLAG_EDITOR_MAP   0x02

u8 run_game(u8 mode);

#endif // GAME_H
