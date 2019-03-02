#include "field.hpp"
#include "buffer.hpp"
#include "setup.hpp"
#include "colour.hpp"
#include "display.hpp"

int main()
{
    system("clear");
    setGameMode();
    getQuickClearSettings();

    Field field;

    while (true)
    {
        system("clear");

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