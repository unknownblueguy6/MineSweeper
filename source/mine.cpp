#include "field.hpp"
#include "buffer.hpp"
#include "setup.hpp"
#include "colour.hpp"
#include "display.hpp"
#include <stdio.h>

void clrscr()
{
    int n;
    for (n = 0; n < 10; n++)
        printf( "\n\n\n\n\n\n\n\n\n\n" );
}

int main()
{
    clrscr();
    setGameMode();
    getQuickClearSettings();

    Field field;

    while (true)
    {
        clrscr();

        dispBanner();
        field.drawField();
        dispFlagCounter();
        
        if (gameState != RUNNING)
            dispVictoryOrDefeat();
        else
            dispControls();

        writeBuf.disp();
        writeBuf.clear();

        if (gameState == RUNNING)
            field.getMove();
        else
            break;
    }

    std::cout << endl
              << reset;

    return 0;
}