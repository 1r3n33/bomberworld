#include <snes.h>

#include "bomb.h"
#include "collision.h"
#include "editor.h"
#include "game.h"
#include "graphics.h"
#include "level_manager.h"
#include "pilot.h"
#include "text.h"
#include "titlescreen.h"

#define GAMEPLAY_SCORE_INCREMENT 5

u16 player_scores[2];

u8 player_lives[2];

#define BOMB_0 0x01
#define BOMB_1 0x02
u8 player_bombs[2]; // Bits represents available bombs.
u8 player_bomb_throttles[2]; // Prevent dropping next bomb too fast.

struct level_t * current_level;

u8 game_mode = 0;

void display_score(u8 id)
{
    if (id == 0)
    {
        set_text_number(OBJ_TEXT, player_scores[0], 0, 0);
    }
    else
    {
        set_text_number(OBJ_TEXT+32, player_scores[1], SCREEN_WIDTH-64, 0);
    }
}

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
            u8 collision = bomb_tilemap_collision(bomb_ids[0], current_level->bomb_collider);
            if (collision > 0)
            {
                player_scores[player_id] += GAMEPLAY_SCORE_INCREMENT;
                display_score(player_id);
            }
            // Give back the bomb if disabled. (collision on the tilemap or on the ground)
            if (!bomb->dropped)
            {
                player_bombs[player_id] |= BOMB_0;
            }
        }
    }

    {
        struct bomb_t * bomb = get_bomb(bomb_ids[1]);
        if (bomb->dropped)
        {
            move_bomb(bomb_ids[1]);
            u8 collision = bomb_tilemap_collision(bomb_ids[1], current_level->bomb_collider);
            if (collision > 0)
            {
                player_scores[player_id] += GAMEPLAY_SCORE_INCREMENT;
                display_score(player_id);
            }
            // Give back the bomb if disabled. (collision on the tilemap or on the ground)
            if (!bomb->dropped)
            {
                player_bombs[player_id] |= BOMB_1;
            }
        }
    }
}

#define EMPTY_HEART (96+32)
#define PLAIN_HEART (97+32)

void display_lives()
{
    u8 player_enabled[] = {
        (player_lives[0] != 0xFF),
        (player_lives[1] != 0xFF),
    };

    u8 hearts[] = { 0, 0, 0 };

    if (player_enabled[0])
    {
        hearts[0] = player_lives[0] > 0 ? PLAIN_HEART : EMPTY_HEART;
        hearts[1] = player_lives[0] > 1 ? PLAIN_HEART : EMPTY_HEART;
    }
    else
    {
        hearts[0] = ' ';
        hearts[1] = ' ';
    }
    set_text(OBJ_TEXT+64, hearts, 72, 0);

    if (player_enabled[1])
    {
        hearts[1] = player_lives[1] > 0 ? PLAIN_HEART : EMPTY_HEART;
        hearts[0] = player_lives[1] > 1 ? PLAIN_HEART : EMPTY_HEART;
    }
    else
    {
        hearts[0] = ' ';
        hearts[1] = ' ';
    }
    set_text(OBJ_TEXT+72, hearts, SCREEN_WIDTH-88, 0);
}

// Internal gameplay loop returns 0 for reset, 1 for completed level, 2 for game over
u8 loop()
{
    u8 frame = 0;
    u16 pad0, pad1;

    u8 player_enabled[] = {
        (player_lives[0] != 0xFF),
        (player_lives[1] != 0xFF),
    };

    u8 speed = current_level->speed;

    while(1)
    {
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

        move_pilot(0, player_enabled[0] ? speed : 0);
        move_pilot(1, player_enabled[1] ? speed : 0);

        update_bombs(0, pad0, get_pilot(0));
        update_bombs(1, pad1, get_pilot(1));

        u8 b0p1_collision = bomb_pilot_collision(get_bomb(0), get_pilot(1));
        u8 b1p1_collision = bomb_pilot_collision(get_bomb(1), get_pilot(1));

        u8 p0_collision = pilot_tilemap_collision(0, current_level->pilot_collider);
        u8 p1_collision = pilot_tilemap_collision(1, current_level->pilot_collider);

        if (current_level->state_end_level_checker())
        {
            return 1;
        }

        if (b0p1_collision == 1 || b1p1_collision == 1)
        {
            player_lives[1]--;
            return 2;
        }

        if (p0_collision == 1)
        {
            player_lives[0]--;
            return 2;
        }

        if (p1_collision == 1)
        {
            player_lives[1]--;
            return 2;
        }

        frame++;
    }
}

// Run the game.
u8 run_game(u8 mode)
{
    game_mode = mode;

    current_level = level_manager_init(game_mode);

    init_graphics();

    player_scores[0] = 0;
    player_scores[1] = 0;

    player_lives[0] = 1;
    player_lives[1] = (game_mode & GAME_MODE_FLAG_2P) ? 1 : 0xFF;

    u8 game_loop = 1;
    while (game_loop)
    {
        current_level->gfx_initializer();

        current_level->state_initializer(current_level->sub_level);

        if (game_mode & GAME_MODE_FLAG_EDITOR_MAP)
        {
            current_level->editor_tilemap_builder(get_editor_tilemap());
        }
        else
        {
            current_level->tilemap_builder();
        }

        reset_text();

        u8 level_loop = 1;
        while (level_loop)
        {
            init_pilot(0);
            init_pilot(1);

            init_bomb(0);
            init_bomb(1);
            init_bomb(2);
            init_bomb(3);

            player_bombs[0] = BOMB_0 | BOMB_1;
            player_bombs[1] = BOMB_0 | BOMB_1;

            player_bomb_throttles[0] = 0;
            player_bomb_throttles[1] = 0;

            display_score(0);
            display_score(1);
            display_lives();

            setFadeEffect(FADE_IN);
            u8 res = loop();
            setFadeEffect(FADE_OUT);

            switch (res)
            {
            case 0:
                level_loop = 0;
                game_loop = 0;
                break;

            case 1:
                current_level = next_level();
                level_loop = 0;

                if (current_level == 0)
                {
                    game_loop = 0;
                }
                break;

            case 2:
                if (player_lives[0] == 0xFF && player_lives[1] == 0xFF)
                {
                    level_loop = 0;
                    game_loop = 0;
                }
                break;
            }
        }
    }

    return 0;
}
