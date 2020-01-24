#include <snes.h>

#include "bomb.h"
#include "building.h"
#include "collision.h"
#include "editor.h"
#include "graphics.h"
#include "pilot.h"
#include "titlescreen.h"

// Game mode: 1 player (0) or 2 players (1)
u8 game_mode = 0;

void update_bomb(struct bomb_t * bomb, u8 id, u16 pad, struct pilot_t * pilot)
{
	if (bomb->dropped)
	{
		move_bomb(id);
		bomb_buildings_collision(bomb, id);
	}
	else
	{
		// Drop bomb from the pilot position.
		// Constraint the bomb drop position to be entirely on screen
		if (pad == KEY_A && is_pilot_entirely_on_screen(id, BOMB_COLLISION_OFFSET_LEFT, BOMB_COLLISION_OFFSET_RIGHT))
		{
			// Take into account the pilot x pos is actually shifted by 512 to handle screen borders.
			drop_bomb(id, (pilot->x>>4)-512, pilot->y+8);
		}		
	}
}

// Game loop returns 1 if exited normaly, 0 for reset
u8 game_loop(u8 speed)
{
	u16 pad0, pad1;

	while(1) {
		pad0 = padsCurrent(0);
		pad1 = padsCurrent(1);

		if (pad0 & KEY_START)
		{
			game_mode = 0;
			return 0;
		}
		if (pad1 & KEY_START)
		{
			game_mode = 1;
			return 0;
		}

		move_pilot(0, speed);
		move_pilot(1, (game_mode == 0) ? 0 : speed);

		update_bomb(get_bomb(0), 0, pad0, get_pilot(0));
		update_bomb(get_bomb(1), 1, pad1, get_pilot(1));

		bomb_pilot_collision(get_bomb(0), get_pilot(1));

		pilot_buildings_collision(get_pilot(0), 0);
		pilot_buildings_collision(get_pilot(1), 1);

		update_graphics();
		WaitForVBlank();

		if (is_building_destroyed())
		{
			return 1;
		}
	}
}

// Run the game. mode 1P (0) or 2P (1)
u8 run_game(u8 mode)
{
	u8 res;
	u8 speed = 8;
	u8 min_building_size = 2;
	u8 max_building_size = 8;

	game_mode = mode;
	
	init_graphics();
	
	while (1)
	{
		init_pilot(0);
		init_pilot(1);
		init_bomb(0);
		init_bomb(1);

		init_building(min_building_size, max_building_size);	

		setFadeEffect(FADE_IN);
		res = game_loop(speed);
		setFadeEffect(FADE_OUT);
		
		if (res == 1)
		{
			speed += 4;
			min_building_size += 1;
			max_building_size += 2;
			if (min_building_size > 16)
			{
				min_building_size = 16;
			}
			if (max_building_size > 16)
			{
				max_building_size = 16;
			}
		}
		else
		{
			speed = 8;
			min_building_size = 2;
			max_building_size = 8;
		}
	}

	return 0;
}
