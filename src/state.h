#ifndef STATE_H
#define STATE_H

#include <snes.h>

struct state_t
{
    u8  game_mode;
    u16 player_scores[2];
    u8  player_lives[2];
    struct level_t * current_level;
};

void init_game_state(u8 mode);

struct level_t * get_current_level();

struct level_t * get_next_level();

u8 score_transaction(u8 id, s16 points);

u8 set_player_death(u8 id);

u8 is_player_enabled(u8 id);

u8 is_game_over();

void display_score();
void display_lives();

#endif // STATE_H
