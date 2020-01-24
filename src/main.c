#include <snes.h>
#include "editor.h" 
#include "game.h"
#include "titlescreen.h"

int main()
{
    consoleInit();

    while(1)
    {
        u8 selection = run_titlescreen();
        
        if (selection == TITLESCREEN_SELECTION_1P)
        {
            run_game(0);
        }
        else if (selection == TITLESCREEN_SELECTION_2P)
        {
            run_game(1);
        }
        else if (selection == TITLESCREEN_SELECTION_EDIT)
        {
            run_editor();
        }
    }

    return 0;
}
