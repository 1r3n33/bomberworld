#include <snes.h>

#include "bomb.h"
#include "collision.h"
#include "editor.h"
#include "graphics.h"
#include "pilot.h"
#include "tilemap.h"
#include "titlescreen.h"

#include "levels/city.h"

// Game mode: 1 player (0) or 2 players (1)
u8 game_mode = 0;

void update_bomb(struct bomb_t * bomb, u8 id, u16 pad, struct pilot_t * pilot)
{
	if (bomb->dropped)
	{
		move_bomb(id);
		bomb_tilemap_collision(id, check_city_level_bomb_collision);
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

		pilot_tilemap_collision(0, check_city_level_pilot_collision);
		pilot_tilemap_collision(1, check_city_level_pilot_collision);

		update_city_level_gfx();
		WaitForVBlank();

		if (check_city_level_done())
		{
			return 1;
		}
	}
}

// Run the game. mode 1P (0) or 2P (1)
u8 run_game(u8 mode)
{
	u8 level = 0;
	u8 speed = 8;

	game_mode = mode;
	
	init_graphics();
	init_city_level_gfx();
	
	while (1)
	{
		init_pilot(0);
		init_pilot(1);
		init_bomb(0);
		init_bomb(1);

		init_city_level_state(level);
		init_tilemap(build_city_level_tilemap);

		setFadeEffect(FADE_IN);
		u8 res = game_loop(speed);
		setFadeEffect(FADE_OUT);
		
		if (res == 1)
		{
			level++;
			speed += 4;
		}
		else
		{
			level = 0;
			speed = 8;
		}
	}

	return 0;
}
