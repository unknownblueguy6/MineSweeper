#pragma once

#include <iostream>
#include <iomanip>
#include "setup.hpp"
#include "SevSegDisp.hpp"
#include "colour.hpp"

void dispBanner(){ //will be moved to separate header later
    std::cout << blue_fg;
    std :: cout <<  R"(
    ##     ##  ##  ##    ## ########  ######  ##      ## ######## ######## ########  ######## ########
    ###   ###  ##  ###   ## ##       ##    ## ##  ##  ## ##       ##       ##     ## ##       ##     ##
    #### ####  ##  ####  ## ##       ##       ##  ##  ## ##       ##       ##     ## ##       ##     ##
    ## ### ##  ##  ## ## ## ######    ######  ##  ##  ## ######   ######   ########  ######   ########
    ##     ##  ##  ##  #### ##             ## ##  ##  ## ##       ##       ##        ##       ##   ##
    ##     ##  ##  ##   ### ##       ##    ## ##  ##  ## ##       ##       ##        ##       ##    ##
    ##     ##  ##  ##    ## ########  ######   ###  ###  ######## ######## ##        ######## ##     ##
    )";
    std::cout << reset;
}

void dispFlagCounter(){
    writeBuf << white_fg <<R"(            (_)               )" << endl;
    writeBuf << white_fg <<R"(   _ __ ___  _ _ __   ___ ___ )" << endl;
    writeBuf << white_fg <<R"(  | '_ ` _ \| | '_ \ / _ / __|)" << endl;
    writeBuf << white_fg <<R"(  | | | | | | | | | |  __\__ \)" << endl;
    writeBuf << white_fg <<R"(  |_| |_| |_|_|_| |_|\___|___/)" << endl;    
    writeBuf << reset;

    flagDisp.update("      ");
}

void dispControls(){
    writeBuf << endl;
    writeBuf << white_fg <<"  ↑, ←, ↓, → : Movement" << endl;
    writeBuf << white_fg <<"  S : Sweep the area" << endl;
    writeBuf << white_fg <<"  F : Flag the current cell" << endl;
    
}

void dispVictoryOrDefeat(){
    writeBuf << endl;
    COLOUR col = gameState == VICTORY ? green_fg : red_fg;
    
    writeBuf << col << R"(  __   _____  _   _)" << endl;
    writeBuf << col << R"(  \ \ / / _ \| | | |)" << endl;
    writeBuf << col << R"(   \ V / (_) | |_| |)" << endl;
    writeBuf << col << R"(    |_| \___/ \___/ )" << endl;

    writeBuf << endl;
    if(gameState == VICTORY){
        writeBuf << col << R"(  __      _______  ____)" << endl;
        writeBuf << col << R"(  \ \    / /_ _| \| | |)" << endl;
        writeBuf << col << R"(   \ \/\/ / | || .` |_|)" << endl;
        writeBuf << col << R"(    \_/\_/ |___|_|\_(_))" << endl;
    }
    
    else{
        writeBuf << col << R"(  ___   ___  ____________)" << endl;
        writeBuf << col << R"(  | |  / _ \/ __|_   _| |)" << endl;
        writeBuf << col << R"(  | |_| (_) \__ \ | | |_|)" << endl;
        writeBuf << col << R"(  |____\___/|___/ |_| (_))" << endl;
    }
}

void setGameMode(){
    int ch;
    do{
        dispBanner();
        std::cout << std::setw(26);
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