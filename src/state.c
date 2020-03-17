#include <snes.h>
#include "bomb.h"
#include "game.h"
#include "graphics.h"
#include "level_manager.h"
#include "state.h"

struct state_t state;

void init_game_state(u8 mode)
{
    state.game_mode = mode;
    state.current_level = level_manager_init(mode);

    state.player_scores[0] = 0;
    state.player_scores[1] = 0;

    state.player_lives[0] = 1;
    state.player_lives[1] = (mode & GAME_MODE_FLAG_2P) ? 1 : 0xFF;
}

struct level_t * get_current_level()
{
    return state.current_level;
}

struct level_t * get_next_level()
{
    state.current_level = next_level();
    return state.current_level;
}

void add_player_score(u8 id, s8 points)
{
    state.player_scores[id] += points;
}

u8 set_player_death(u8 id)
{
    state.player_lives[id]--;
}

u8 is_player_enabled(u8 id)
{
    return state.player_lives[id] != 0xFF;
}

u8 is_game_over()
{
    return (state.player_lives[0] == 0xFF && state.player_lives[1] == 0xFF);
}

void display_score(u8 id)
{
    if (id == 0)
    {
        set_text_number(OBJ_TEXT, state.player_scores[0], 0, 0);
    }
    else
    {
        set_text_number(OBJ_TEXT+32, state.player_scores[1], SCREEN_WIDTH-64, 0);
    }
}

#define EMPTY_HEART (96+32)
#define PLAIN_HEART (97+32)

void display_lives()
{
    u8 player_enabled[] = {
        (state.player_lives[0] != 0xFF),
        (state.player_lives[1] != 0xFF),
    };

    u8 hearts[] = { 0, 0, 0 };

    if (player_enabled[0])
    {
        hearts[0] = state.player_lives[0] > 0 ? PLAIN_HEART : EMPTY_HEART;
        hearts[1] = state.player_lives[0] > 1 ? PLAIN_HEART : EMPTY_HEART;
    }
    else
    {
        hearts[0] = ' ';
        hearts[1] = ' ';
    }
    set_text(OBJ_TEXT+64, hearts, 72, 0);

    if (player_enabled[1])
    {
        hearts[1] = state.player_lives[1] > 0 ? PLAIN_HEART : EMPTY_HEART;
        hearts[0] = state.player_lives[1] > 1 ? PLAIN_HEART : EMPTY_HEART;
    }
    else
    {
        hearts[0] = ' ';
        hearts[1] = ' ';
    }
    set_text(OBJ_TEXT+72, hearts, SCREEN_WIDTH-88, 0);
}
