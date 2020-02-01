#include <snes.h>

#include "bomb.h"
#include "collision.h"
#include "editor.h"
#include "graphics.h"
#include "pilot.h"
#include "tilemap.h"
#include "titlescreen.h"

#include "levels/city.h"

typedef void	(*state_initializer_t)(u8);
typedef u8 		(*state_end_level_checker_t)(void);
typedef void 	(*graphics_initializer_t)(void);
typedef void 	(*graphics_updater_t)(void);

struct level_t
{
	u8							level;
	u8							sub_level;
	u8							speed;
	u8							pad_0;
	state_initializer_t			state_initializer;
	state_end_level_checker_t	state_end_level_checker;
	graphics_initializer_t 		gfx_initializer;
	graphics_updater_t			gfx_updater;
	tilemap_builder_t  			tilemap_builder;
	tilemap_collider_t 			bomb_collider;
	tilemap_collider_t 			pilot_collider;
};

struct level_manager_t
{
	struct level_t	levels[4];
	u8 				level_count;
	u8 				current;
	u8 				pad_0;
	u8 				pad_1;
};

struct level_manager_t level_manager;

struct level_t current_level;

void level_manager_init()
{
	level_manager.levels[0].level 					= 0;
	level_manager.levels[0].sub_level 				= 0;
	level_manager.levels[0].speed 					= 8;
	level_manager.levels[0].state_initializer 		= init_city_level_state;
	level_manager.levels[0].state_end_level_checker	= check_city_level_done;
	level_manager.levels[0].gfx_initializer 		= init_city_level_gfx;
	level_manager.levels[0].gfx_updater 			= update_city_level_gfx;
	level_manager.levels[0].tilemap_builder 		= build_city_level_tilemap;
	level_manager.levels[0].bomb_collider 			= check_city_level_bomb_collision;
	level_manager.levels[0].pilot_collider 			= check_city_level_pilot_collision;

	level_manager.levels[1].level 					= 0;
	level_manager.levels[1].sub_level 				= 1;
	level_manager.levels[1].speed 					= 12;
	level_manager.levels[1].state_initializer 		= init_city_level_state;
	level_manager.levels[1].state_end_level_checker = check_city_level_done;
	level_manager.levels[1].gfx_initializer 		= init_city_level_gfx;
	level_manager.levels[1].gfx_updater 			= update_city_level_gfx;
	level_manager.levels[1].tilemap_builder 		= build_city_level_tilemap;
	level_manager.levels[1].bomb_collider 			= check_city_level_bomb_collision;
	level_manager.levels[1].pilot_collider 			= check_city_level_pilot_collision;

	level_manager.levels[2].level 					= 0;
	level_manager.levels[2].sub_level 				= 2;
	level_manager.levels[2].speed 					= 16;
	level_manager.levels[2].state_initializer 		= init_city_level_state;
	level_manager.levels[2].state_end_level_checker = check_city_level_done;
	level_manager.levels[2].gfx_initializer 		= init_city_level_gfx;
	level_manager.levels[2].gfx_updater 			= update_city_level_gfx;
	level_manager.levels[2].tilemap_builder 		= build_city_level_tilemap;
	level_manager.levels[2].bomb_collider 			= check_city_level_bomb_collision;
	level_manager.levels[2].pilot_collider 			= check_city_level_pilot_collision;

	// MAX LEVEL COUNT: 4

	level_manager.level_count = 3;
	level_manager.current = 0;

	current_level = level_manager.levels[level_manager.current];
}

u8 level_manager_next()
{
	level_manager.current++;
	if (level_manager.current >= level_manager.level_count)
	{
		return 0;
	}

	current_level = level_manager.levels[level_manager.current];
	return 1;
}

// Game mode: 1 player (0) or 2 players (1)
u8 game_mode = 0;

void update_bomb(struct bomb_t * bomb, u8 id, u16 pad, struct pilot_t * pilot)
{
	if (bomb->dropped)
	{
		move_bomb(id);
		bomb_tilemap_collision(id, current_level.bomb_collider);
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
u8 game_loop()
{
	u16 pad0, pad1;

	u8 speed = current_level.speed;

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

		pilot_tilemap_collision(0, current_level.pilot_collider);
		pilot_tilemap_collision(1, current_level.pilot_collider);

		current_level.gfx_updater();
		WaitForVBlank();

		if (current_level.state_end_level_checker())
		{
			return 1;
		}
	}
}

// Run the game. mode 1P (0) or 2P (1)
u8 run_game(u8 mode)
{
	game_mode = mode;
	
	level_manager_init();

	init_graphics();
	
	while (1)
	{
		current_level.gfx_initializer();

		init_pilot(0);
		init_pilot(1);
		init_bomb(0);
		init_bomb(1);

		current_level.state_initializer(current_level.sub_level);
		init_tilemap(current_level.tilemap_builder);

		setFadeEffect(FADE_IN);
		u8 res = game_loop();
		setFadeEffect(FADE_OUT);
		
		if (res == 1)
		{
			u8 next = level_manager_next();
			if (next == 0)
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	return 0;
}
