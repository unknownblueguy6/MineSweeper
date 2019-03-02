#pragma once

#include <iostream>
#include <iomanip>
#include <cctype>
#include "setup.hpp"
#include "sevsegdisp.hpp"
#include "colour.hpp"

void dispBanner()
{ //will be moved to separate header later
    std::cout << blue_fg;
    std ::cout << R"(
    ##     ##  ##  ##    ## ########  ######  ##      ## ######## ######## ########  ######## ########
    ###   ###  ##  ###   ## ##       ##    ## ##  ##  ## ##       ##       ##     ## ##       ##     ##
    #### ####  ##  ####  ## ##       ##       ##  ##  ## ##       ##       ##     ## ##       ##     ##
    ## ### ##  ##  ## ## ## ######    ######  ##  ##  ## ######   ######   ########  ######   ########
    ##     ##  ##  ##  #### ##             ## ##  ##  ## ##       ##       ##        ##       ##   ##
    ##     ##  ##  ##   ### ##       ##    ## ##  ##  ## ##       ##       ##        ##       ##    ##
    ##     ##  ##  ##    ## ########  ######   ###  ###  ######## ######## ##        ######## ##     ##)";
    std::cout << reset << endl;
}

void dispFlagCounter()
{
    writeBuf << white_fg << R"(                (_)               )" << endl;
    writeBuf << white_fg << R"(   _ __ ___  _ _ __   ___ ___ )" << endl;
    writeBuf << white_fg << R"(  | '_ ` _ \| | '_ \ / _ / __|)" << endl;
    writeBuf << white_fg << R"(  | | | | | | | | | |  __\__ \)" << endl;
    writeBuf << white_fg << R"(  |_| |_| |_|_|_| |_|\___|___/)" << endl;
    writeBuf << reset;

    flagDisp.update("      ");
}

void dispControls()
{
    writeBuf << endl;
    writeBuf << white_fg << "  ↑, ←, ↓, → : " << endl;
    writeBuf << white_fg << "       OR      Movement" << endl;
    writeBuf << white_fg << "  H, J, K, L : " << endl;
    writeBuf << white_fg << "  S or ENTER : Sweep" << endl;
    writeBuf << white_fg << "  F or SPACEBAR : Toggle Flag" << endl;
}

void dispVictoryOrDefeat()
{
    writeBuf << endl;
    COLOUR col = gameState == VICTORY ? green_fg : red_fg;

    writeBuf << col << R"(  __   _____  _   _ )" << endl;
    writeBuf << col << R"(  \ \ / / _ \| | | |)" << endl;
    writeBuf << col << R"(   \ V / (_) | |_| |)" << endl;
    writeBuf << col << R"(    |_| \___/ \___/ )" << endl;

    writeBuf << endl;

    if (gameState == VICTORY)
    {

        writeBuf << col << R"(  __      _____ _  _ _ )" << endl;
        writeBuf << col << R"(  \ \    / /_ _| \| | |)" << endl;
        writeBuf << col << R"(   \ \/\/ / | || .` |_|)" << endl;
        writeBuf << col << R"(    \_/\_/ |___|_|\_(_))" << endl;
    }

    else
    {
        writeBuf << col << R"(   _    ___  ___ ___ _ )" << endl;
        writeBuf << col << R"(  | |  / _ \/ __| __| |)" << endl;
        writeBuf << col << R"(  | |_| (_) \__ \ _||_|)" << endl;
        writeBuf << col << R"(  |____\___/|___/___(_))" << endl;
    }
}

void setGameMode()
{
    int ch;
    do
    {
        dispBanner();
        std::cout << std::setw(30);
        std::cout << yellow_fg << "Maximise Terminal or press F11 for the optimal experience" << reset;
        std::cout << endl
                  << endl;
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

        if (ch < 1 || ch > 4)
        {
            std::cout << red_fg << "Invalid Choice. Try again\n";
            auto c = getch();
        }
        else
            gameMode = (GAME_MODE)ch;
        system("clear");

    } while (ch < 1 || ch > 4);
}

void getQuickClearSettings()
{
    char ch;
    int choice;
    do
    {
        dispBanner();
        std::cout << endl;
        std::cout << yellow_fg << 
        R"(            Quick Clear : If a cell with a number on it is already revealed,
                         and it's neighbours have been flagged with the same number,
                         then all tiles adjacent to this cell are also sweeped.
            
            Enable Quick Clear? )";
        
        std::cout << white_fg << "[" << green_fg <<"Y" << white_fg << " / " << red_fg << "N" << white_fg << "] : " << reset;

        std::cin >> ch;

        ch = std::tolower(ch);

        if (!(ch == 'y' || ch == 'n'))
        {
            std::cout << red_fg << "Invalid Choice. Try again\n";
            auto c = getch();
        }
        else
            QUICKCLEAR = std::tolower(ch) == 'y' ? true : false;
        
        system("clear");


    } while (!(ch == 'y' || ch == 'n'));
    
}
