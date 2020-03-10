#ifndef LOOP_H
#define LOOP_H

// Internal gameplay loop returns 0 for reset, 1 for completed level, 2 for game over
u8 gameplay_loop();

void terminate_explosion_animation_loop();

void gameover_loop();

#endif // LOOP_H