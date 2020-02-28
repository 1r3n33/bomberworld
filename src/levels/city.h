#ifndef CITY_H
#define CITY_H

#include <snes.h>

// building tiles (bkg_buildings.bmp)
#define TIL_BUILDING_BLANK                      0
#define TIL_BUILDING_FLOOR_SIDE_DOOR            1
#define TIL_BUILDING_FLOOR_CENTER_DOOR          2
#define TIL_BUILDING_FLOOR_SIDE                 3
#define TIL_BUILDING_BODY_CENTER_WINDOW_0       4
#define TIL_BUILDING_BODY_CENTER_WINDOW_1       5
#define TIL_BUILDING_BODY_CENTER_WINDOW_2       6
#define TIL_BUILDING_BODY_CENTER_WINDOW_3       7
#define TIL_BUILDING_BODY_SIDE_WINDOW_0         8
#define TIL_BUILDING_BODY_SIDE_WINDOW_1         9
#define TIL_BUILDING_BODY_SIDE_WINDOW_2         10
#define TIL_BUILDING_BODY_SIDE_WINDOW_3         11
#define TIL_BUILDING_CEILING_CENTER_WINDOW_0    12
#define TIL_BUILDING_CEILING_CENTER_WINDOW_1    13
#define TIL_BUILDING_CEILING_CENTER_WINDOW_2    14
#define TIL_BUILDING_CEILING_CENTER_WINDOW_3    15
#define TIL_BUILDING_CEILING_SIDE_WINDOW_0      16
#define TIL_BUILDING_CEILING_SIDE_WINDOW_1      17
#define TIL_BUILDING_CEILING_SIDE_WINDOW_2      18
#define TIL_BUILDING_CEILING_SIDE_WINDOW_3      19
#define TIL_BUILDING_CEILING_TOWER_WINDOW_0     20
#define TIL_BUILDING_CEILING_TOWER_WINDOW_1     21
#define TIL_BUILDING_CEILING_TOWER_WINDOW_2     22
#define TIL_BUILDING_CEILING_TOWER_WINDOW_3     23
#define TIL_BUILDING_24                         24
#define TIL_BUILDING_25                         25
#define TIL_BUILDING_26                         26
#define TIL_BUILDING_27                         27
#define TIL_BUILDING_FLOOR_SIDE_BROKEN          28
#define TIL_BUILDING_BODY_SIDE_BROKEN           29
#define TIL_BUILDING_CEILING_SIDE_BROKEN        30
#define TIL_BUILDING_BODY_CENTER_BROKEN_0       31
#define TIL_BUILDING_BODY_CENTER_BROKEN_1       32
#define TIL_BUILDING_CEILING_TOWER_BROKEN       33

void init_city_level_state(u8 level);

void init_city_level_gfx();

void build_city_level_tilemap();

void update_city_level_gfx(u8 frame);

u8 check_city_level_bomb_collision(u8 top, u8 bottom, u8 left, u8 right);

u8 check_city_level_pilot_collision(u8 x, u8 y);

u8 check_city_level_done();

#endif // CITY_H
