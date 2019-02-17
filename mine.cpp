#include "field.hpp"
#include "buffer.hpp"
#include "setup.hpp"
#include "colour.hpp"
#include "display.hpp"

int main(){
    setGameMode();
    
    Field field;
    
    while(state == RUNNING){
        dispBanner();
        field.drawField();
        dispFlagCounter();
        writeBuf.disp();
        field.getMove();
        system("clear");
        writeBuf.clear();
    }
    
    dispBanner();
    field.drawField();
    dispFlagCounter();
    writeBuf.disp();
    writeBuf.clear();
    
    if(state == VICTORY) std::cout << green_fg << endl << "You won!" << reset;
    else std::cout << red_fg << endl << "You lost!" << reset;
    
    std::cout << endl;
	
    return 0;
}