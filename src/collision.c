#include <snes.h>
#include "bomb.h"
#include "collision.h"
#include "graphics.h"
#include "pilot.h"
#include "tilemap.h"

void bomb_tilemap_collision(u8 bomb_id, tilemap_collider_t collider)
{
	struct bomb_t * bomb = get_bomb(bomb_id);	

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
		u8 collision = collider(i, j);
		if (collision == 1)
		{
			// Hide the bomb
			bomb->dropped--;
			if (bomb->dropped == 0)
			{
				init_bomb(bomb_id);
			}
		}
	}
}

void pilot_tilemap_collision(u8 pilot_id, tilemap_collider_t collider)
{
	struct pilot_t * pilot = get_pilot(pilot_id);	

	// Compute pilot bounding box
	u16 pilot_top = pilot->y + PILOT_BUILDING_COLLISION_OFFSET_BOTTOM;
	u16 pilot_left = ((pilot->x>>4) - 512) + ((pilot_id == 0) ? PILOT_COLLISION_OFFSET_RIGHT : PILOT_COLLISION_OFFSET_LEFT);

	// Check x pos out of bound.
	if (pilot_left > 256)
	{
		return;
	}

	// Compute candidate tiles to check collision with
	u8 map_x = pilot_left / 8;
	u8 map_y = pilot_top / 8;

	u8 collision = collider(map_x, map_y);
	if (collision == 1)
	{
		while (1);
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

