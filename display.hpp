#pragma once

#include <iostream>
#include <iomanip>
#include "setup.hpp"
#include "SevSegDisp.hpp"
#include "colour.hpp"

void dispBanner(){ //will be moved to separate header later
    std::cout << blue_fg;
    std::cout << "##     ##  ##  ##    ## ########  ######  ##      ## ######## ######## ########  ######## ######## \n"; 
    std::cout << "###   ###  ##  ###   ## ##       ##    ## ##  ##  ## ##       ##       ##     ## ##       ##     ##\n"; 
    std::cout << "#### ####  ##  ####  ## ##       ##       ##  ##  ## ##       ##       ##     ## ##       ##     ##\n"; 
    std::cout << "## ### ##  ##  ## ## ## ######    ######  ##  ##  ## ######   ######   ########  ######   ######## \n"; 
    std::cout << "##     ##  ##  ##  #### ##             ## ##  ##  ## ##       ##       ##        ##       ##   ##  \n"; 
    std::cout << "##     ##  ##  ##   ### ##       ##    ## ##  ##  ## ##       ##       ##        ##       ##    ## \n"; 
    std::cout << "##     ##  ##  ##    ## ########  ######   ###  ###  ######## ######## ##        ######## ##     ##\n";
    std::cout << endl << reset;
}

void dispFlagCounter(){
    writeBuf << white_fg;
    writeBuf << white_fg <<"          (_)               " << endl;
    writeBuf << white_fg <<" _ __ ___  _ _ __   ___ ___ " << endl;
    writeBuf << white_fg <<"| '_ ` _ \\| | '_ \\ / _ / __|" << endl;
    writeBuf << white_fg <<"| | | | | | | | | |  __\\__ \\" << endl;
    writeBuf << white_fg <<"|_| |_| |_|_|_| |_|\\___|___/" << endl;    
    writeBuf << reset;

    flagDisp.update();

    writeBuf.goToLine(0);

}

void setGameMode(){
    int ch;
    //std::cout << cyan_fg << "5. Quit\n" << reset;
    do{
        dispBanner();
        std::cout << std::setw(30);
        std::cout << white_fg << "Choose Game Mode:\n";
        std::cout << std::setw(30);
        std::cout << green_fg << "1. Beginner\n";
        std::cout << std::setw(30);
        std::cout << yellow_fg << "2. Intermediate\n";
        std::cout << std::setw(30);
        std::cout << red_fg << "3. Expert\n";
        std::cout << std::setw(30);
        std::cout << blue_fg << "4. Custom\n\n";
        std::cout << "                       ";
        std::cin >> ch;
        
        if(ch < 1 || ch > 4) std::cout << red_fg <<"Invalid Choice. Try again\n";
        else gameMode = (GAME_MODE)ch;
        system("clear");
    
    } while(ch < 1 || ch > 4);
}