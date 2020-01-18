#include <snes.h>
#include "editor.h" 
#include "game.h"
#include "titlescreen.h"

int main()
{
    consoleInit();

    while(1)
    {
        run_titlescreen();
        u8 editor = run_editor();
        run_game(editor);
    }

    return 0;
}
