#include "bomb.h"
#include "building.h"
#include "collision.h"
#include "graphics.h"
#include "pilot.h"

void bomb_buildings_collision(struct bomb_t * bomb, u8 id)
{
	u16 * map = get_building_tilemap();

	// Compute bomb bounding box
	u8 bomb_bottom = bomb->y + BOMB_COLLISION_OFFSET_BOTTOM;
	u8 bomb_left = bomb->x + BOMB_COLLISION_OFFSET_LEFT;
	u8 bomb_right = bomb->x + BOMB_COLLISION_OFFSET_RIGHT;

	// Compute candidate tiles to check collision with
	u8 map_min_x = bomb_left / 8;
	u8 map_max_x = (bomb_right-1) / 8;
	u8 map_max_y = (bomb_bottom-1) / 8;

	u8 i = map_min_x;
	u8 j = map_max_y;
	for (; i<=map_max_x; i++)
	{
		u8 tile = map[(j*32)+i]&0xFF;
		// Check collision against solid building block
		if (tile != 0 && tile < TIL_BUILDING_24)
		{
			// Trigger the bomb to building collision event
			hit_building();

			// Hide the bomb
			bomb->dropped--;
			if (bomb->dropped == 0)
			{
				init_bomb(id);
			}

			// Change the look of the surrounding tiles
			u16 tile_left  = map[(j*32)+(i-1)];
			u16 tile_right = map[(j*32)+(i+1)];
			u16 tile_up    = map[((j-1)*32)+i];

			u8 tile_left_solid = (tile_left != 0 && (tile_left&0xFF) < TIL_BUILDING_24) ? 1 : 0;
			u8 tile_right_solid = (tile_right != 0 && (tile_right&0xFF) < TIL_BUILDING_24) ? 1 : 0;

			if (tile_left_solid == 0 && tile_right_solid == 0)
			{
				map[(j*32)+i] = TIL_BUILDING_CEILING_TOWER_BROKEN;
			}
			else if (tile_left_solid == 1 && tile_right_solid == 1)
			{
				map[(j*32)+i] = TIL_BUILDING_BODY_CENTER_BROKEN_1;
			}
			else if (tile_left_solid == 0 && tile_right_solid == 1)
			{
				map[(j*32)+i] = TIL_BUILDING_CEILING_SIDE_BROKEN;
			}
			else if (tile_left_solid == 1 && tile_right_solid == 0)
			{
				map[(j*32)+i] = TIL_BUILDING_CEILING_SIDE_BROKEN | 0x4000;
			}
			
			u8 tile_left_broken = ((tile_left&0xFF) > TIL_BUILDING_27) ? 1 : 0;
			u8 tile_right_broken  = ((tile_right&0xFF) > TIL_BUILDING_27) ? 1 : 0;

			if (tile_left_broken)
			{
				if (tile_left == TIL_BUILDING_CEILING_SIDE_BROKEN)
				{
					map[(j*32)+(i-1)] = TIL_BUILDING_CEILING_TOWER_BROKEN;
				} 
				else if (tile_left == TIL_BUILDING_BODY_SIDE_BROKEN)
				{
					map[(j*32)+(i-1)] = 0;
				} 
				else if (tile_left == TIL_BUILDING_BODY_CENTER_BROKEN_1)
				{
					map[(j*32)+(i-1)] = TIL_BUILDING_CEILING_SIDE_BROKEN | 0x4000;
				}
			}

			if (tile_right_broken)
			{
				if (tile_right == (TIL_BUILDING_CEILING_SIDE_BROKEN | 0x4000))
				{
					map[(j*32)+(i+1)] = TIL_BUILDING_CEILING_TOWER_BROKEN;
				} 
				else if (tile_right == (TIL_BUILDING_BODY_SIDE_BROKEN | 0x4000))
				{
					map[(j*32)+(i+1)] = 0;
				} 
				else if (tile_right == TIL_BUILDING_BODY_CENTER_BROKEN_1)
				{
					map[(j*32)+(i+1)] = TIL_BUILDING_CEILING_SIDE_BROKEN;
				}
			}

			u8 tile_up_broken = ((tile_up&0xFF) > TIL_BUILDING_27) ? 1 : 0;
			if (tile_up_broken)
			{
				if (tile_up == TIL_BUILDING_CEILING_SIDE_BROKEN)
				{
					map[((j-1)*32)+i] = TIL_BUILDING_BODY_SIDE_BROKEN;
				} 
				else if (tile_up == (TIL_BUILDING_CEILING_SIDE_BROKEN | 0x4000))
				{
					map[((j-1)*32)+i] = TIL_BUILDING_BODY_SIDE_BROKEN | 0x4000;
				} 
				else if (tile_up == TIL_BUILDING_BODY_CENTER_BROKEN_1)
				{
					map[((j-1)*32)+i] = TIL_BUILDING_BODY_CENTER_BROKEN_0;
				} 
				else if (tile_up == TIL_BUILDING_CEILING_TOWER_BROKEN)
				{
					map[((j-1)*32)+i] = 0;
				} 
			}

			// Refresh BG
			bgInitMapSet(
				0,
				(u8*)map,
				32*32*2,
				SC_32x32,
				VRAM_BG0_MAP
			);

			return;
		}
	}
}

void bomb_pilot_collision(struct bomb_t * bomb, struct pilot_t * pilot)
{
	if (bomb->dropped == 0) return;

	// Compute bomb bounding box
	u16 bomb_top = bomb->y + BOMB_COLLISION_OFFSET_TOP;
	u16 bomb_bottom = bomb->y + BOMB_COLLISION_OFFSET_BOTTOM;
	u16 bomb_left = bomb->x + BOMB_COLLISION_OFFSET_LEFT;
	u16 bomb_right = bomb->x + BOMB_COLLISION_OFFSET_RIGHT;

	// Compute pilot bounding box
	u16 pilot_top = pilot->y + PILOT_BOMB_COLLISION_OFFSET_TOP;
	u16 pilot_bottom = pilot->y + PILOT_BOMB_COLLISION_OFFSET_BOTTOM;
	u16 pilot_left = ((pilot->x>>4) - 512) + PILOT_COLLISION_OFFSET_LEFT;
	u16 pilot_right = ((pilot->x>>4) - 512) + PILOT_COLLISION_OFFSET_RIGHT;

	if (bomb_top > pilot_bottom) return;
	if (bomb_bottom < pilot_top) return;
	if (bomb_left > pilot_right) return;
	if (bomb_right < pilot_left) return;

	while (1);
}

void pilot_buildings_collision(struct pilot_t * pilot, u8 id)
{
	u8 map_x, map_y;

	u16 tile;

	u16 * map = get_building_tilemap();

	// Compute pilot bounding box
	u16 pilot_top = pilot->y + PILOT_BUILDING_COLLISION_OFFSET_BOTTOM;
	u16 pilot_left = ((pilot->x>>4) - 512) + ((id == 0) ? PILOT_COLLISION_OFFSET_RIGHT : PILOT_COLLISION_OFFSET_LEFT);

	// Check x pos out of bound.
	if (pilot_left > 256)
	{
		return;
	}

	// Compute candidate tiles to check collision with
	map_x = pilot_left / 8;
	map_y = pilot_top / 8;

	tile = map[(map_y*32)+map_x];
	// Check collision against solid building block
	if (tile != 0 && (tile&0x0FFF) < TIL_BUILDING_24)
	{
		while(1);
	}
}
