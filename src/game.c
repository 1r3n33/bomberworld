#include <snes.h>

#include "bomb.h"
#include "game.h"
#include "graphics.h"
#include "level_manager.h"

u16 player_scores[2];

u8 player_lives[2];

u8 player_bombs[2]; // Bits represents available bombs.
u8 player_bomb_throttles[2]; // Prevent dropping next bomb too fast.

struct level_t * current_level;

u8 game_mode = 0;

extern char soundbrr,soundbrrend;
brrsamples sfx_exp_short_soft1;

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

// Run the game.
u8 run_game(u8 mode)
{
    game_mode = mode;

    current_level = level_manager_init(game_mode);

    init_graphics();

    spcSetSoundEntry(15, 8, 4, &soundbrrend-&soundbrr, &soundbrr, &sfx_exp_short_soft1);

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

            disable_explosion(0);
            disable_explosion(1);
            disable_explosion(2);
            disable_explosion(3);

            display_score(0);
            display_score(1);
            display_lives();

            setFadeEffect(FADE_IN);
            u8 res = gameplay_loop();

            terminate_explosion_animation_loop();

            u8 gameover = (player_lives[0] == 0xFF && player_lives[1] == 0xFF);
            if (gameover)
            {
                gameover_loop();
            }

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
                if (gameover)
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
