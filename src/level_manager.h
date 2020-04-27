#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <snes.h>
#include "collision.h"

typedef void (*state_initializer_t)(u8);
typedef u8   (*state_end_level_checker_t)(void);
typedef void (*graphics_initializer_t)(void);
typedef void (*graphics_updater_t)(u8);
typedef void (*tilemap_builder_t)(void);
typedef void (*editor_tilemap_builder_t)(u16*);
typedef u8   (*loop_t)(void);

struct level_t
{
    u8                        level;
    u8                        speed;
    u8                        drop;
    u8                        ground;
    state_initializer_t       state_initializer;
    state_end_level_checker_t state_end_level_checker;
    graphics_initializer_t    gfx_initializer;
    graphics_updater_t        gfx_updater;
    tilemap_builder_t         tilemap_builder;
    editor_tilemap_builder_t  editor_tilemap_builder;
    tilemap_box_collider_t    bomb_collider;
    tilemap_point_collider_t  pilot_collider;
    loop_t                    loop;
};

struct level_manager_t
{
    struct level_t levels[16];
    u8             level_count;
    u8             current;
    u8             pad_0;
    u8             pad_1;
};

struct level_t * level_manager_init(u8 game_mode);

struct level_t * next_level();

#endif // LEVEL_MANAGER_H
