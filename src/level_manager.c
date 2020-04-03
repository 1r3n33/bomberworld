#include <snes.h>
#include "game.h"
#include "level_manager.h"
#include "loop.h"

#include "levels/airship.h"
#include "levels/city.h"
#include "levels/egypt.h"
#include "levels/moon.h"
#include "levels/sea.h"
#include "levels/shop.h"

void do_nothing_state_initializer(u8 _) {}
u8   do_nothing_end_level_checker() {}
void do_nothing_graphics_initializer() {}
void do_nothing_graphics_updater(u8 _) {}
void do_nothing_tilemap_builder() {}
void do_nothing_editor_tilemap_builder(u16 * _) {}
u8   do_nothing_bomb_collider(u8 top, u8 bottom, u8 left, u8 right) { return 0; }
u8   do_nothing_pilot_collider(u8 x, u8 y) { return 0; }

struct level_manager_t level_manager;

struct level_t * level_manager_init(u8 game_mode)
{
    u8 i = 0;

    if (game_mode & GAME_MODE_FLAG_EDITOR_MAP)
    {
        level_manager.levels[i].level                   = 0;
        level_manager.levels[i].speed                   = 8;
        level_manager.levels[i].state_initializer       = init_city_level_state;
        level_manager.levels[i].state_end_level_checker = check_city_level_done;
        level_manager.levels[i].gfx_initializer         = init_city_level_gfx;
        level_manager.levels[i].gfx_updater             = update_city_level_gfx;
        level_manager.levels[i].editor_tilemap_builder  = build_city_level_editor_tilemap;
        level_manager.levels[i].bomb_collider           = check_city_level_bomb_collision;
        level_manager.levels[i].pilot_collider          = check_city_level_pilot_collision;
        level_manager.levels[i].loop                    = gameplay_loop;
        i++;
    }
    else
    {
        level_manager.levels[i].level                   = 0;
        level_manager.levels[i].speed                   = 8;
        level_manager.levels[i].state_initializer       = init_city_level_state;
        level_manager.levels[i].state_end_level_checker = check_city_level_done;
        level_manager.levels[i].gfx_initializer         = init_city_level_gfx;
        level_manager.levels[i].gfx_updater             = update_city_level_gfx;
        level_manager.levels[i].tilemap_builder         = build_city_level_tilemap;
        level_manager.levels[i].bomb_collider           = check_city_level_bomb_collision;
        level_manager.levels[i].pilot_collider          = check_city_level_pilot_collision;
        level_manager.levels[i].loop                    = gameplay_loop;
        i++;

        level_manager.levels[i].gfx_initializer         = init_airship_city_gfx;
        level_manager.levels[i].state_initializer       = init_airship_state;
        level_manager.levels[i].tilemap_builder         = do_nothing_tilemap_builder;
        level_manager.levels[i].gfx_updater             = update_city_level_gfx;
        level_manager.levels[i].loop                    = airship_loop;
        i++;

        level_manager.levels[i].level                   = 0;
        level_manager.levels[i].gfx_initializer         = init_shop_gfx;
        level_manager.levels[i].state_initializer       = init_shop_state;
        level_manager.levels[i].tilemap_builder         = do_nothing_tilemap_builder;
        level_manager.levels[i].loop                    = shop_loop;
        i++;

        level_manager.levels[i].level                   = 1;
        level_manager.levels[i].speed                   = 8;
        level_manager.levels[i].state_initializer       = init_egypt_level_state;
        level_manager.levels[i].state_end_level_checker = check_egypt_level_done;
        level_manager.levels[i].gfx_initializer         = init_egypt_level_gfx;
        level_manager.levels[i].gfx_updater             = update_egypt_level_gfx;
        level_manager.levels[i].tilemap_builder         = build_egypt_level_tilemap;
        level_manager.levels[i].bomb_collider           = check_egypt_level_bomb_collision;
        level_manager.levels[i].pilot_collider          = check_egypt_level_pilot_collision;
        level_manager.levels[i].loop                    = gameplay_loop;
        i++;

        level_manager.levels[i].gfx_initializer         = init_airship_egypt_gfx;
        level_manager.levels[i].state_initializer       = init_airship_state;
        level_manager.levels[i].tilemap_builder         = do_nothing_tilemap_builder;
        level_manager.levels[i].gfx_updater             = update_egypt_level_gfx;
        level_manager.levels[i].loop                    = airship_loop;
        i++;

        level_manager.levels[i].level                   = 1;
        level_manager.levels[i].gfx_initializer         = init_shop_gfx;
        level_manager.levels[i].state_initializer       = init_shop_state;
        level_manager.levels[i].tilemap_builder         = do_nothing_tilemap_builder;
        level_manager.levels[i].loop                    = shop_loop;
        i++;

        level_manager.levels[i].level                   = 2;
        level_manager.levels[i].speed                   = 8;
        level_manager.levels[i].state_initializer       = init_sea_level_state;
        level_manager.levels[i].state_end_level_checker = check_sea_level_done;
        level_manager.levels[i].gfx_initializer         = init_sea_level_gfx;
        level_manager.levels[i].gfx_updater             = update_sea_level_gfx;
        level_manager.levels[i].tilemap_builder         = build_sea_level_tilemap;
        level_manager.levels[i].bomb_collider           = check_sea_level_bomb_collision;
        level_manager.levels[i].pilot_collider          = check_sea_level_pilot_collision;
        level_manager.levels[i].loop                    = gameplay_loop;
        i++;

        level_manager.levels[i].gfx_initializer         = init_airship_sea_gfx;
        level_manager.levels[i].state_initializer       = init_airship_state;
        level_manager.levels[i].tilemap_builder         = do_nothing_tilemap_builder;
        level_manager.levels[i].gfx_updater             = update_airship_sea_level_gfx;
        level_manager.levels[i].loop                    = airship_loop;
        i++;

        level_manager.levels[i].level                   = 2;
        level_manager.levels[i].gfx_initializer         = init_shop_gfx;
        level_manager.levels[i].state_initializer       = init_shop_state;
        level_manager.levels[i].tilemap_builder         = do_nothing_tilemap_builder;
        level_manager.levels[i].loop                    = shop_loop;
        i++;

        level_manager.levels[i].level                   = 3;
        level_manager.levels[i].speed                   = 16;
        level_manager.levels[i].state_initializer       = init_moon_level_state;
        level_manager.levels[i].state_end_level_checker = check_moon_level_done;
        level_manager.levels[i].gfx_initializer         = init_moon_level_gfx;
        level_manager.levels[i].gfx_updater             = update_moon_level_gfx;
        level_manager.levels[i].tilemap_builder         = build_moon_level_tilemap;
        level_manager.levels[i].bomb_collider           = check_moon_level_bomb_collision;
        level_manager.levels[i].pilot_collider          = check_moon_level_pilot_collision;
        level_manager.levels[i].loop                    = gameplay_loop;
        i++;
    }

    level_manager.level_count = i;

    level_manager.current = 0;

    return level_manager.levels;
}

struct level_t * next_level()
{
    level_manager.current++;
    if (level_manager.current >= level_manager.level_count)
    {
        return 0;
    }

    return level_manager.levels + level_manager.current;
}
