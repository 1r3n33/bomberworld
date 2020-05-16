#include <snes.h>
#include "bomb.h"
#include "debris.h"
#include "explosion.h"
#include "graphics.h"
#include "level_manager.h"
#include "loop.h"
#include "pilot.h"
#include "state.h"
#include "text.h"

#define GAMEPLAY_SCORE_INCREMENT 5

u8  player_bomb_throttles[2];   // Prevent dropping next bomb too fast.

void update_bombs(u8 player_id, u16 pad, struct pilot_t * pilot)
{
    struct level_t * current_level = get_current_level();

    // Bomb ids of the current player.
    u8 bomb_ids[] = { (player_id*3), (player_id*3)+1 };

    u16 do_drop = (pad & KEY_A);
    u8 can_drop = (player_bomb_throttles[player_id] == 0) && is_pilot_entirely_on_screen(player_id, BOMB_COLLISION_OFFSET_LEFT, BOMB_COLLISION_OFFSET_RIGHT);
    u8 bomb = find_player_bomb(player_id);

    // Drop bomb from the pilot position.
    if (do_drop && can_drop && (bomb != 0xFF))
    {
        use_player_bomb(player_id, bomb);
        display_ui_bombs(player_id);

        // Take into account the pilot x pos is actually shifted by 512 to handle screen borders.
        drop_bomb(bomb_ids[bomb-1], (pilot->x>>4)-512, pilot->y+8);
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
            move_bomb(bomb_ids[0], current_level->ground);
            if (bomb->dropped)
            {
                u8 collision = bomb_tilemap_collision(bomb_ids[0], current_level->bomb_collider);
                if (collision > 0)
                {
                    score_transaction(player_id, GAMEPLAY_SCORE_INCREMENT);
                    display_ui_score(player_id);

                    release_player_bomb(player_id, BOMB_0);
                    display_ui_bombs(player_id);

                    init_explosion(bomb_ids[0], bomb->x, bomb->y+4);
                    spcPlaySound(0);

                    init_debris(bomb->x, bomb->y+4);
                }
            }
            else // bomb hit the ground or past screen height
            {
                release_player_bomb(player_id, BOMB_0);
                display_ui_bombs(player_id);

                if (bomb->y < SCREEN_HEIGHT)
                {
                    init_explosion(bomb_ids[0], bomb->x, bomb->y);
                    spcPlaySound(0);
                }
            }
        }
    }

    {
        struct bomb_t * bomb = get_bomb(bomb_ids[1]);
        if (bomb->dropped)
        {
            move_bomb(bomb_ids[1], current_level->ground);
            if (bomb->dropped)
            {
                u8 collision = bomb_tilemap_collision(bomb_ids[1], current_level->bomb_collider);
                if (collision > 0)
                {
                    score_transaction(player_id, GAMEPLAY_SCORE_INCREMENT);
                    display_ui_score(player_id);

                    release_player_bomb(player_id, BOMB_1);
                    display_ui_bombs(player_id);

                    init_explosion(bomb_ids[1], bomb->x, bomb->y+4);
                    spcPlaySound(0);

                    init_debris(bomb->x, bomb->y+4);
                }
            }
            else // bomb hit the ground or past screen height
            {
                release_player_bomb(player_id, BOMB_1);
                display_ui_bombs(player_id);

                if (bomb->y < SCREEN_HEIGHT)
                {
                    init_explosion(bomb_ids[1], bomb->x, bomb->y);
                    spcPlaySound(0);
                }
            }
        }
    }
}

void update_mega_bombs(u8 player_id, u16 pad, struct pilot_t * pilot)
{
    struct level_t * current_level = get_current_level();

    // Bomb ids of the current player.
    u8 bomb_id = (player_id*3)+2;

    u16 do_drop = (pad & KEY_B);
    u8 can_drop = find_player_mega_bomb(player_id) && is_pilot_entirely_on_screen(player_id, BOMB_COLLISION_OFFSET_LEFT, BOMB_COLLISION_OFFSET_RIGHT);

    // Drop bomb from the pilot position.
    if (do_drop && can_drop)
    {
        use_player_mega_bomb(player_id);
        display_ui_mega_bomb(player_id);

        // Take into account the pilot x pos is actually shifted by 512 to handle screen borders.
        drop_bomb(bomb_id, (pilot->x>>4)-512, pilot->y+8);
    }

    struct bomb_t * bomb = get_bomb(bomb_id);
    if (bomb->dropped)
    {
        move_bomb(bomb_id, current_level->ground);
        if (bomb->dropped)
        {
            u8 collision = bomb_tilemap_collision(bomb_id, current_level->bomb_collider);
            if (collision > 0)
            {
                init_explosion(bomb_id, bomb->x, bomb->y);
                spcPlaySound(0);
            }
        }

        if (!bomb->dropped)
        {
            init_explosion(bomb_id, bomb->x, bomb->y);
            spcPlaySound(0);

            mega_bomb_tilemap_collision(bomb_id, current_level->megabomb_collider);
        }
    }
}

u8 propellant_grace_period[2] = { 0, 0 };

void update_pilot(u8 id, u16 pad)
{
    struct pilot_t * pilot = get_pilot(id);

    struct level_t * current_level = get_current_level();

    u8 speed  = current_level->speed;
    u8 drop   = current_level->drop;
    u8 ground = current_level->ground;

    u8 propellant = get_player_propellant(id);
    if (propellant == 0xFF)
    {
        propellant = 0;
    }

    if (propellant)
    {
        if ((pad & (KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT))==0)
        {
            propellant_grace_period[id] = 0;
        }

        if (pad & KEY_UP)
        {
            if (pilot->y > 0)
            {
                pilot->y -= 1;
                pilot->y_baseline -= 1;
            }
            if (propellant_grace_period[id] == 0)
            {
                use_player_propellant(id);
                propellant_grace_period[id] = 8;
            }
            else
            {
                propellant_grace_period[id]--;
            }
        }

        if (pad & KEY_DOWN)
        {
            if (pilot->y < ground)
            {
                pilot->y += 1;
                pilot->y_baseline += 1;
            }
            if (propellant_grace_period[id] == 0)
            {
                use_player_propellant(id);
                propellant_grace_period[id] = 8;
            }
            else
            {
                propellant_grace_period[id]--;
            }
        }

        if (pad & KEY_RIGHT)
        {
            if (id == 0)
            {
                speed = speed*4;
            }
            else
            {
                speed = speed/4;
            }
            if (propellant_grace_period[id] == 0)
            {
                use_player_propellant(id);
                propellant_grace_period[id] = 16;
            }
            else
            {
                propellant_grace_period[id]--;
            }
        }

        if (pad & KEY_LEFT)
        {
            if (id == 0)
            {
                speed = speed/4;
            }
            else
            {
                speed = speed*4;
            }
            if (propellant_grace_period[id] == 0)
            {
                use_player_propellant(id);
                propellant_grace_period[id] = 16;
            }
            else
            {
                propellant_grace_period[id]--;
            }
        }
    }
    else
    {
        propellant_grace_period[id] = 0;
    }

    move_pilot(id, is_player_enabled(id) ? speed : 0, is_player_enabled(1-id), drop);
}

// Internal gameplay loop returns 0 for reset, 1 for completed level, 2 for game over
u8 gameplay_loop()
{
    u8 frame = 0;

    release_player_bomb(0, BOMB_0 | BOMB_1);
    release_player_bomb(1, BOMB_0 | BOMB_1);

    player_bomb_throttles[0] = 0;
    player_bomb_throttles[1] = 0;

    struct level_t * current_level = get_current_level();

    struct pilot_t * p0 = get_pilot(0);
    struct pilot_t * p1 = get_pilot(1);

    while(1)
    {
        spcProcess();
        WaitForVBlank();

        current_level->gfx_updater(frame);

        u16 pad0 = padsCurrent(0);
        u16 pad1 = padsCurrent(1);

        if (pad0 & KEY_START)
        {
            return 0;
        }
        if (pad1 & KEY_START)
        {
            return 0;
        }
        if (pad0 & KEY_SELECT)
        {
            return 1;
        }

        update_pilot(0, pad0);
        update_pilot(1, pad1);

        update_explosion(0);
        update_explosion(1);
        update_explosion(2);
        update_explosion(3);

        update_debris(frame, current_level->ground);

        update_bombs(0, pad0, p0);
        update_mega_bombs(0, pad0, p0);
        update_bombs(1, pad1, p1);
        update_mega_bombs(1, pad1, p1);

        u8 b3p0_collision = bomb_pilot_collision(get_bomb(3), p0);
        u8 b4p0_collision = bomb_pilot_collision(get_bomb(4), p0);
        u8 b5p0_collision = bomb_pilot_collision(get_bomb(5), p0);

        u8 b0p1_collision = bomb_pilot_collision(get_bomb(0), p1);
        u8 b1p1_collision = bomb_pilot_collision(get_bomb(1), p1);
        u8 b2p1_collision = bomb_pilot_collision(get_bomb(2), p1);

        u8 p0_collision = pilot_tilemap_collision(0, current_level->pilot_collider);
        u8 p1_collision = pilot_tilemap_collision(1, current_level->pilot_collider);

        if (current_level->state_end_level_checker())
        {
            return 1;
        }

        if (b3p0_collision | b4p0_collision | b5p0_collision | p0_collision)
        {
            set_player_death(0);
            init_explosion(PILOT_0_EXPLOSION_ID, (p0->x>>4)-512, p0->y);
            return 2;
        }

        if (b0p1_collision | b1p1_collision | b2p1_collision | p1_collision)
        {
            set_player_death(1);
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
        spcProcess();
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
        spcProcess();
        WaitForVBlank();

        u16 pad = (padsCurrent(0) | padsCurrent(1));
        if (pad & KEY_START)
        {
            break;
        }
    }
}
