#include <snes.h>
#include "bomb.h"
#include "editor.h"
#include "explosion.h"
#include "game.h"
#include "graphics.h"
#include "level_manager.h"
#include "loop.h"
#include "pilot.h"
#include "state.h"
#include "text.h"

extern char soundbrr,soundbrrend;
brrsamples sfx_exp_short_soft1;

// Run the game.
u8 run_game(u8 mode)
{
    init_game_state(mode);
    struct level_t * current_level = get_current_level();

    init_graphics();

    spcSetSoundEntry(15, 8, 4, &soundbrrend-&soundbrr, &soundbrr, &sfx_exp_short_soft1);

    u8 game_loop = 1;
    while (game_loop)
    {
        current_level->gfx_initializer();

        current_level->state_initializer(current_level->sub_level);

        if (mode & GAME_MODE_FLAG_EDITOR_MAP)
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

            disable_explosion(0);
            disable_explosion(1);
            disable_explosion(2);
            disable_explosion(3);

            display_score(0);
            display_score(1);

            display_bombs(0);
            display_bombs(1);

            display_lives();

            setFadeEffect(FADE_IN);
            u8 res = current_level->loop();

            terminate_explosion_animation_loop();

            u8 gameover = is_game_over();
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
                current_level = get_next_level();

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
