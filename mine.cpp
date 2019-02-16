#include "field.hpp"
#include "display.hpp"

int main(){
    while(game == RUNNING){
        dispBanner();
        field.drawField();
        field.getMove();
        system("clear");
    }
    dispBanner();
    field.drawField();
    if(game == VICTORY) std::cout << green_fg << endl << "You won!" << reset;
    else std::cout << red_fg << endl << "You lost!" << reset;
    
    std::cout << endl;
	
    return 0;
}