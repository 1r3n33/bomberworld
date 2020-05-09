#include <snes.h>
#include "bomb.h"
#include "collision.h"
#include "graphics.h"
#include "pilot.h"

u8 bomb_tilemap_collision(u8 bomb_id, tilemap_box_collider_t collider)
{
    struct bomb_t * bomb = get_bomb(bomb_id);

    // Compute bomb bounding box
    u8 bomb_bottom = bomb->y + BOMB_COLLISION_OFFSET_BOTTOM;
    u8 bomb_left = bomb->x + BOMB_COLLISION_OFFSET_LEFT;
    u8 bomb_right = bomb->x + BOMB_COLLISION_OFFSET_RIGHT;

    u8 collision = collider(0, bomb_bottom, bomb_left, bomb_right);
    if (collision > 0)
    {
        switch (collision)
        {
        case 1:
            bomb->dropped--;
            break;

        case 2:
            bomb->dropped = 0;
            break;

        default:
            break;
        }

        // Hide the bomb
        if (bomb->dropped == 0)
        {
            init_bomb(bomb_id);
        }

        return 1;
    }

    return 0;
}

u8 pilot_tilemap_collision(u8 pilot_id, tilemap_point_collider_t collider)
{
    struct pilot_t * pilot = get_pilot(pilot_id);

    // Compute pilot collision point
    u16 pilot_x = ((pilot->x>>4) - 512) + ((pilot_id == 0) ? PILOT_COLLISION_OFFSET_RIGHT : PILOT_COLLISION_OFFSET_LEFT);
    u16 pilot_y = pilot->y + PILOT_BUILDING_COLLISION_OFFSET_BOTTOM;

    // Check y pos out of bound.
    if (pilot_y > 256)
    {
        return;
    }

    u8 collision = collider(pilot_x, pilot_y);
    if (collision > 0)
    {
        return 1;
    }

    return 0;
}

u8 bomb_pilot_collision(struct bomb_t * bomb, struct pilot_t * pilot)
{
    if (bomb->dropped == 0) return 0;

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

    if (bomb_top > pilot_bottom) return 0;
    if (bomb_bottom < pilot_top) return 0;
    if (bomb_left > pilot_right) return 0;
    if (bomb_right < pilot_left) return 0;

    return 1;
}
