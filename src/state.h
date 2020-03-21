#ifndef STATE_H
#define STATE_H

#include <snes.h>

struct state_t
{
    u8  game_mode;
    u16 player_scores[2];
    u8  player_cur_lives[2];
    u8  player_max_lives[2];
    u8  player_cur_bombs[2];
    u8  player_max_bombs[2];
    struct level_t * current_level;
};

void init_game_state(u8 mode);

struct level_t * get_current_level();

struct level_t * get_next_level();

u8 score_transaction(u8 id, s16 points);

u8 get_player_max_lives(u8 id);

void max_out_player_lives(u8 id);

u8 set_player_death(u8 id);

u8 is_player_enabled(u8 id);

u8 is_game_over();

u8 get_player_max_bombs(u8 id);

u8 find_player_bomb(u8 id);

void use_player_bomb(u8 id, u8 bomb_flag);

void release_player_bomb(u8 id, u8 bomb_flag);

void max_out_player_bombs(u8 id);

void display_score(u8 id);

void display_lives();

void display_bombs(u8 id);

#endif // STATE_H
