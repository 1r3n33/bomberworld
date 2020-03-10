#include <snes.h>
#include "bomb.h"
#include "explosion.h"
#include "graphics.h"
#include "level_manager.h"
#include "loop.h"
#include "pilot.h"

#define GAMEPLAY_SCORE_INCREMENT 5

#define BOMB_0 0x01
#define BOMB_1 0x02

// Borrowed from game.c
// TODO: Clean. Access a gamestate object.
extern u16 player_scores[2];
extern u8 player_lives[2];
extern u8 player_bombs[2]; // Bits represents available bombs.
extern u8 player_bomb_throttles[2]; // Prevent dropping next bomb too fast.
extern struct level_t * current_level;

void update_bombs(u8 player_id, u16 pad, struct pilot_t * pilot)
{
    // Bomb ids of the current player.
    u8 bomb_ids[] = { (player_id*2), (player_id*2)+1 };

    // Drop bomb from the pilot position.
    // Constraint the bomb drop position to be entirely on screen
    if (player_bombs[player_id] != 0 &&
        player_bomb_throttles[player_id] == 0 &&
        pad == KEY_A &&
        is_pilot_entirely_on_screen(player_id, BOMB_COLLISION_OFFSET_LEFT, BOMB_COLLISION_OFFSET_RIGHT))
    {
        // Pick an available bomb.
        u8 b = (player_bombs[player_id] & BOMB_0) ? BOMB_0 : BOMB_1;
        player_bombs[player_id] &= ~b;
        // Take into account the pilot x pos is actually shifted by 512 to handle screen borders.
        drop_bomb(bomb_ids[b-1], (pilot->x>>4)-512, pilot->y+8);
        player_bomb_throttles[player_id] = 16;
    }
    else
    {
        // Clear throttle if we cannot drop.
        if (player_bomb_throttles[player_id] > 0)
        {
            player_bomb_throttles[player_id]--;
        }
    }

    {
        struct bomb_t * bomb = get_bomb(bomb_ids[0]);
        if (bomb->dropped)
        {
            move_bomb(bomb_ids[0]);
            if (bomb->dropped)
            {
                u8 collision = bomb_tilemap_collision(bomb_ids[0], current_level->bomb_collider);
                if (collision > 0)
                {
                    player_scores[player_id] += GAMEPLAY_SCORE_INCREMENT;
                    display_score(player_id);

                    player_bombs[player_id] |= BOMB_0;
                    init_explosion(bomb_ids[0], bomb->x, bomb->y+4);
                    spcPlaySound(0);
                }
            }
            else // bomb hit the ground
            {
                player_bombs[player_id] |= BOMB_0;
                init_explosion(bomb_ids[0], bomb->x, bomb->y);
                spcPlaySound(0);
            }
        }
    }

    {
        struct bomb_t * bomb = get_bomb(bomb_ids[1]);
        if (bomb->dropped)
        {
            move_bomb(bomb_ids[1]);
            if (bomb->dropped)
            {
                u8 collision = bomb_tilemap_collision(bomb_ids[1], current_level->bomb_collider);
                if (collision > 0)
                {
                    player_scores[player_id] += GAMEPLAY_SCORE_INCREMENT;
                    display_score(player_id);

                    player_bombs[player_id] |= BOMB_1;
                    init_explosion(bomb_ids[1], bomb->x, bomb->y+4);
                    spcPlaySound(0);
                }
            }
            else // bomb hit the ground
            {
                player_bombs[player_id] |= BOMB_1;
                init_explosion(bomb_ids[1], bomb->x, bomb->y);
                spcPlaySound(0);
            }
        }
    }
}

// Internal gameplay loop returns 0 for reset, 1 for completed level, 2 for game over
u8 gameplay_loop()
{
    u8 frame = 0;
    u16 pad0, pad1;

    u8 player_enabled[] = {
        (player_lives[0] != 0xFF),
        (player_lives[1] != 0xFF),
    };

    u8 speed = current_level->speed;

    struct pilot_t * p0 = get_pilot(0);
    struct pilot_t * p1 = get_pilot(1);

    while(1)
    {
        spcProcess();
        WaitForVBlank();

        current_level->gfx_updater(frame);

        pad0 = padsCurrent(0);
        pad1 = padsCurrent(1);

        if (pad0 & KEY_START)
        {
            return 0;
        }
        if (pad1 & KEY_START)
        {
            return 0;
        }

        move_pilot(0, player_enabled[0] ? speed : 0, player_enabled[1]);
        move_pilot(1, player_enabled[1] ? speed : 0, player_enabled[0]);

        update_explosion(0);
        update_explosion(1);
        update_explosion(2);
        update_explosion(3);

        update_bombs(0, pad0, p0);
        update_bombs(1, pad1, p1);

        u8 b2p0_collision = bomb_pilot_collision(get_bomb(2), p0);
        u8 b3p0_collision = bomb_pilot_collision(get_bomb(3), p0);

        u8 b0p1_collision = bomb_pilot_collision(get_bomb(0), p1);
        u8 b1p1_collision = bomb_pilot_collision(get_bomb(1), p1);

        u8 p0_collision = pilot_tilemap_collision(0, current_level->pilot_collider);
        u8 p1_collision = pilot_tilemap_collision(1, current_level->pilot_collider);

        if (current_level->state_end_level_checker())
        {
            return 1;
        }

        if (b2p0_collision | b3p0_collision | p0_collision)
        {
            player_lives[0]--;
            init_explosion(PILOT_0_EXPLOSION_ID, (p0->x>>4)-512, p0->y);
            return 2;
        }

        if (b0p1_collision | b1p1_collision | p1_collision)
        {
            player_lives[1]--;
            init_explosion(PILOT_1_EXPLOSION_ID, (p1->x>>4)-512, p1->y);
            return 2;
        }

        frame++;
    }
}

void terminate_explosion_animation_loop()
{
    u8 res = 0;
    do
    {
        WaitForVBlank();
        res = update_explosion(0);
        res |= update_explosion(1);
        res |= update_explosion(2);
        res |= update_explosion(3);
        res |= update_explosion(4);
        res |= update_explosion(5);
    }
    while (res);
}

void gameover_loop()
{
    // Color math Div2
    REG_CGWSEL = 0x00;
    REG_CGADSUB = 0x7f;

    set_text(OBJ_TEXT+80, "game over", (SCREEN_WIDTH/2)-36, (SCREEN_HEIGHT/2)-4);

    while (1)
    {
        WaitForVBlank();
        u16 pad = (padsCurrent(0) | padsCurrent(1));
        if (pad & KEY_START)
        {
            break;
        }
    }
}
