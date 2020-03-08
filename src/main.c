#include <snes.h>
#include "editor.h"
#include "game.h"
#include "titlescreen.h"

int main()
{
    spcBoot();
    consoleInit();

    // Allocate 12K of sound ram (48 256-byte blocks).
    // Must be sized to hold the largest sound.
    spcAllocateSoundRegion(48);

    while(1)
    {
        u8 selection = run_titlescreen();

        if (selection == TITLESCREEN_SELECTION_1P)
        {
            run_game(GAME_MODE_FLAG_1P);
        }
        else if (selection == TITLESCREEN_SELECTION_2P)
        {
            run_game(GAME_MODE_FLAG_2P);
        }
        else if (selection == TITLESCREEN_SELECTION_EDIT)
        {
            u8 game_mode = run_editor();
            run_game(game_mode);
        }
    }

    return 0;
}
