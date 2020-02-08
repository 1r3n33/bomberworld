#include <snes.h>

#include "bomb.h"
#include "collision.h"
#include "editor.h"
#include "graphics.h"
#include "pilot.h"
#include "text.h"
#include "tilemap.h"
#include "titlescreen.h"

#include "levels/city.h"
#include "levels/egypt.h"

#define GAMEPLAY_SCORE_INCREMENT 5

u16 players_score[2];

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
    tilemap_box_collider_t		bomb_collider;
    tilemap_point_collider_t 	pilot_collider;
};

struct level_manager_t
{
    struct level_t	levels[8];
    u8 				level_count;
    u8 				current;
    u8 				pad_0;
    u8 				pad_1;
};

struct level_manager_t level_manager;

struct level_t current_level;

void level_manager_init()
{
    u8 i = 0;

    level_manager.levels[i].level 					= 0;
    level_manager.levels[i].sub_level 				= 0;
    level_manager.levels[i].speed 					= 8;
    level_manager.levels[i].state_initializer 		= init_city_level_state;
    level_manager.levels[i].state_end_level_checker	= check_city_level_done;
    level_manager.levels[i].gfx_initializer 		= init_city_level_gfx;
    level_manager.levels[i].gfx_updater 			= update_city_level_gfx;
    level_manager.levels[i].tilemap_builder 		= build_city_level_tilemap;
    level_manager.levels[i].bomb_collider 			= check_city_level_bomb_collision;
    level_manager.levels[i].pilot_collider 			= check_city_level_pilot_collision;
    i++;

#if 0
    level_manager.levels[i].level 					= 0;
    level_manager.levels[i].sub_level 				= 1;
    level_manager.levels[i].speed 					= 12;
    level_manager.levels[i].state_initializer 		= init_city_level_state;
    level_manager.levels[i].state_end_level_checker = check_city_level_done;
    level_manager.levels[i].gfx_initializer 		= init_city_level_gfx;
    level_manager.levels[i].gfx_updater 			= update_city_level_gfx;
    level_manager.levels[i].tilemap_builder 		= build_city_level_tilemap;
    level_manager.levels[i].bomb_collider 			= check_city_level_bomb_collision;
    level_manager.levels[i].pilot_collider 			= check_city_level_pilot_collision;
    i++;

    level_manager.levels[i].level 					= 0;
    level_manager.levels[i].sub_level 				= 2;
    level_manager.levels[i].speed 					= 16;
    level_manager.levels[i].state_initializer 		= init_city_level_state;
    level_manager.levels[i].state_end_level_checker = check_city_level_done;
    level_manager.levels[i].gfx_initializer 		= init_city_level_gfx;
    level_manager.levels[i].gfx_updater 			= update_city_level_gfx;
    level_manager.levels[i].tilemap_builder 		= build_city_level_tilemap;
    level_manager.levels[i].bomb_collider 			= check_city_level_bomb_collision;
    level_manager.levels[i].pilot_collider 			= check_city_level_pilot_collision;
    i++;
#endif

    level_manager.levels[i].level 					= 1;
    level_manager.levels[i].sub_level 				= 0;
    level_manager.levels[i].speed 					= 8;
    level_manager.levels[i].state_initializer 		= init_egypt_level_state;
    level_manager.levels[i].state_end_level_checker = check_egypt_level_done;
    level_manager.levels[i].gfx_initializer 		= init_egypt_level_gfx;
    level_manager.levels[i].gfx_updater 			= update_egypt_level_gfx;
    level_manager.levels[i].tilemap_builder 		= build_egypt_level_tilemap;
    level_manager.levels[i].bomb_collider 			= check_egypt_level_bomb_collision;
    level_manager.levels[i].pilot_collider 			= check_egypt_level_pilot_collision;
    i++;

    level_manager.level_count = i;

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
        u8 collision = bomb_tilemap_collision(id, current_level.bomb_collider);
        if (collision > 0)
        {
            players_score[id] += GAMEPLAY_SCORE_INCREMENT;
        }
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

// Game loop returns 0 for reset, 1 for completed level, 2 for game over
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

        u8 bp_collision = bomb_pilot_collision(get_bomb(0), get_pilot(1));

        u8 p0_collision = pilot_tilemap_collision(0, current_level.pilot_collider);
        u8 p1_collision = pilot_tilemap_collision(1, current_level.pilot_collider);

        current_level.gfx_updater();

        set_text_number(OBJ_TEXT, players_score[0], 0, 0);
        set_text_number(OBJ_TEXT+32, players_score[1], SCREEN_WIDTH-64, 0);

        WaitForVBlank();

        if (current_level.state_end_level_checker())
        {
            return 1;
        }

        if (bp_collision == 1)
        {
            return 2;
        }

        if (p0_collision == 1)
        {
            return 2;
        }

        if (p1_collision == 1)
        {
            return 2;
        }
    }
}

// Run the game. mode 1P (0) or 2P (1)
u8 run_game(u8 mode)
{
    game_mode = mode;

    level_manager_init();

    init_graphics();

    players_score[0] = 0;
    players_score[1] = 0;

    while (1)
    {
        current_level.gfx_initializer();

        init_pilot(0);
        init_pilot(1);
        init_bomb(0);
        init_bomb(1);

        current_level.state_initializer(current_level.sub_level);
        init_tilemap(current_level.tilemap_builder);

        reset_text();

        setFadeEffect(FADE_IN);
        u8 res = game_loop();
        setFadeEffect(FADE_OUT);

        switch (res)
        {
        case 1:
            {
                u8 next = level_manager_next();
                if (next == 0)
                {
                    return 0;
                }
            }
            break;
        default:
            return 0;
        }
    }

    return 0;
}
