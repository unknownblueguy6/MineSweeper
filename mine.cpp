#include "field.hpp"
#include "buffer.hpp"
#include "setup.hpp"
#include "colour.hpp"
#include "display.hpp"

int main(){
    system("clear");
    setGameMode();
    
    Field field;
    
    while(gameState == RUNNING){
        system("clear");
        dispBanner();
        field.drawField();
        dispFlagCounter();
        dispControls();
        writeBuf.disp();
        field.getMove();
        writeBuf.clear();
    }
    system("clear");
    dispBanner();
    field.drawField();
    dispFlagCounter();
    writeBuf.disp();
    writeBuf.clear();
    
    if(gameState == VICTORY) std::cout << green_fg << endl << "You won!" << reset;
    else std::cout << red_fg << endl << "You lost!" << reset;
    
    std::cout << endl;
	
    return 0;
}