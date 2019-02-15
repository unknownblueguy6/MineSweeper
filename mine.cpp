#include "field.hpp"

int main(){
    while(game == RUNNING){
        field.drawField();
        field.getMove();
        system("clear");
    }
    
    field.drawField();
    if(game == VICTORY) std::cout << green_fg << endl << "You won!" << reset;
    else std::cout << red_fg << endl << "You lost!" << reset;
    
    std::cout << endl;
	
    return 0;
}