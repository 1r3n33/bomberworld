#include <snes.h>
#include "bomb.h"
#include "game.h"
#include "graphics.h"
#include "level_manager.h"
#include "state.h"
#include "text.h"

struct state_t state;

u16 display_scores_oam_ids[2];
u8 display_scores_oam_pos[2];

u16 display_lives_oam_ids[2];
u8 display_lives_oam_pos[2];

u16 display_bombs_oam_ids[2];
u8 display_bombs_oam_pos[2];

u16 display_mega_bombs_oam_ids[2];
u8 display_mega_bombs_oam_pos[2];

void init_game_state(u8 mode)
{
    state.game_mode = mode;
    state.current_level = level_manager_init(mode);

#if 1
    state.player_scores[0] = 0;
    state.player_scores[1] = 0;
#else
    state.player_scores[0] = 5000;
    state.player_scores[1] = 5000;
#endif

    state.player_cur_lives[0] = 2;
    state.player_cur_lives[1] = (mode & GAME_MODE_FLAG_2P) ? 2 : 0;

    state.player_max_lives[0] = 2;
    state.player_max_lives[1] = (mode & GAME_MODE_FLAG_2P) ? 2 : 0;

    state.player_cur_bombs[0] = BOMB_0 | BOMB_1;
    state.player_cur_bombs[1] = (mode & GAME_MODE_FLAG_2P) ? BOMB_0 | BOMB_1 : 0;

    state.player_max_bombs[0] = 1;
    state.player_max_bombs[1] = (mode & GAME_MODE_FLAG_2P) ? 1 : 0;

    state.player_mega_bombs[0] = 0;
    state.player_mega_bombs[1] = (mode & GAME_MODE_FLAG_2P) ? 0 : 0;

    compute_ui_elements();
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

u8 score_transaction(u8 id, s16 points)
{
    s16 res = state.player_scores[id] + points;
    if (res < 0)
    {
        return 0;
    }
    else
    {
        state.player_scores[id] = res;
        return 1;
    }
}

u8 get_player_max_lives(u8 id)
{
    return state.player_max_lives[id];
}

void max_out_player_lives(u8 id)
{
    state.player_cur_lives[id] = 3;
    state.player_max_lives[id] = 3;
}

u8 set_player_death(u8 id)
{
    state.player_cur_lives[id]--;
}

u8 is_player_enabled(u8 id)
{
    return state.player_cur_lives[id] != 0;
}

u8 is_game_over()
{
    return (state.player_cur_lives[0] == 0 && state.player_cur_lives[1] == 0);
}

u8 get_player_max_bombs(u8 id)
{
    return state.player_max_bombs[id];
}

u8 find_player_bomb(u8 id)
{
    if (state.player_max_bombs[id] == 1)
    {
        return (state.player_cur_bombs[id] & BOMB_0) ? BOMB_0 : 0xFF;
    }
    else
    {
        return (state.player_cur_bombs[id] & BOMB_0) ? BOMB_0 : (state.player_cur_bombs[id] & BOMB_1) ? BOMB_1 : 0xFF;
    }
}

void use_player_bomb(u8 id, u8 bomb_flag)
{
    state.player_cur_bombs[id] &= ~bomb_flag;
}

void release_player_bomb(u8 id, u8 bomb_flag)
{
    state.player_cur_bombs[id] |= bomb_flag;
}

void max_out_player_bombs(u8 id)
{
    state.player_cur_bombs[id] = BOMB_0 | BOMB_1;
    state.player_max_bombs[id] = 2;
}

void max_out_player_mega_bombs(u8 id)
{
    state.player_mega_bombs[id] = 1;
}

#define EMPTY_HEART (96+32)
#define PLAIN_HEART (97+32)
#define EMPTY_BOMB (98+32)
#define PLAIN_BOMB (99+32)
#define MEGA_BOMB (100+32)

void compute_ui_elements()
{
    display_scores_oam_ids[0] = OBJ_TEXT;
    display_scores_oam_pos[0] = 0;

    display_lives_oam_ids[0] = display_scores_oam_ids[0] + (6*4);
    display_lives_oam_pos[0] = display_scores_oam_pos[0] + (6*8) + 1;

    display_bombs_oam_ids[0] = display_lives_oam_ids[0] + ((state.player_max_lives[0]-1)*4);
    display_bombs_oam_pos[0] = display_lives_oam_pos[0] + ((state.player_max_lives[0]-1)*8) - 1;

    display_mega_bombs_oam_ids[0] = display_bombs_oam_ids[0] + (state.player_max_bombs[0]*4);
    display_mega_bombs_oam_pos[0] = display_bombs_oam_pos[0] + (state.player_max_bombs[0]*8);

    display_scores_oam_ids[1] = OBJ_TEXT + 64;
    display_scores_oam_pos[1] = SCREEN_WIDTH - (6*8);

    display_lives_oam_ids[1] = display_scores_oam_ids[1] + (6*4);
    display_lives_oam_pos[1] = display_scores_oam_pos[1] - ((state.player_max_lives[1]-1)*8);

    display_bombs_oam_ids[1] = display_lives_oam_ids[1] + ((state.player_max_lives[1]-1)*4);
    display_bombs_oam_pos[1] = display_lives_oam_pos[1] - (state.player_max_bombs[1]*8);

    display_mega_bombs_oam_ids[1] = display_bombs_oam_ids[1] + (state.player_max_bombs[1]*4);
    display_mega_bombs_oam_pos[1] = display_bombs_oam_pos[1] - 8;
}

void display_ui_score(u8 id)
{
    set_text_number(display_scores_oam_ids[id], state.player_scores[id], display_scores_oam_pos[id], 0);
}

void display_ui_lives(u8 id)
{
    u8 hearts[] = { 0, 0, 0 };
    hearts[0] = (state.player_cur_lives[id] > 1) ? PLAIN_HEART : EMPTY_HEART;
    hearts[1] = (state.player_max_lives[id] > 2) ? (state.player_cur_lives[id] > 2) ? PLAIN_HEART : EMPTY_HEART : 0;
    set_text(display_lives_oam_ids[id], hearts, display_lives_oam_pos[id], 0);
}

void display_ui_bombs(u8 id)
{
    u8 bombs[] = { 0, 0, 0 };
    bombs[0] = (state.player_cur_bombs[id] & BOMB_0) ? PLAIN_BOMB : EMPTY_BOMB;
    bombs[1] = (state.player_max_bombs[id] > 1) ? (state.player_cur_bombs[id] & BOMB_1) ? PLAIN_BOMB : EMPTY_BOMB : 0;
    set_text(display_bombs_oam_ids[id], bombs, display_bombs_oam_pos[id], 0);
}

void display_ui_mega_bomb(u8 id)
{
    u8 mega_bomb[] = { 0, 0 };
    mega_bomb[0] = (state.player_mega_bombs[id] > 0) ? MEGA_BOMB : 0;
    set_text(display_mega_bombs_oam_ids[id], mega_bomb, display_mega_bombs_oam_pos[id], 0);
}

void display_ui_elements(u8 id)
{
    if (state.player_max_lives[id] > 0)
    {
        display_ui_score(id);

        if (state.player_cur_lives[id] > 0)
        {
            display_ui_lives(id);
            display_ui_bombs(id);
            display_ui_mega_bomb(id);
        }
        else
        {
            u16 i;
            for (i=display_lives_oam_ids[id]; i<=display_mega_bombs_oam_ids[id]; i+=4)
            {
                oamSetVisible(i, 1);
            }
        }
    }
}
