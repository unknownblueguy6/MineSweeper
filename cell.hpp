#pragma once

#include <vector>
#include <string>
#include "setup.hpp"


enum CELL_STATES{
    EMPTY,
    MINE,
    ADJ_TO_MINE
};

using COLOUR = const std::string;

COLOUR red_fg = "\033[1;31m";
COLOUR green_fg = "\033[1;32m";
COLOUR blue_bg = "\033[1;44m";
COLOUR white_fg = "\033[37m";
COLOUR white_bg = "\033[1;47m";
COLOUR reset = "\033[0m";
COLOUR endl = "\n";

struct Cell{
    Cell();
    void toggleflag();
    void reveal();
    void mine();
    void markAdjMine(int);
    bool flagged;  
    bool hidden;
    CELL_STATES state;
    int noOfAdjMines;
    std::string sym;
};

Cell::Cell(){
    hidden = true;
    flagged = false;
    state = EMPTY;
    sym = white_bg + " " + reset + " │";
}

void Cell::toggleflag(){
    if(hidden){
        flagged = !flagged;
        if(flagged){
            sym = green_fg + "▶" + reset + " │";
        }
        else{
            sym = white_bg + " " + reset + " │";
        }
    }
}

void Cell::reveal(){
    if(!flagged && hidden){
        hidden = false;
        switch(state){
            case EMPTY:
                sym = reset + " " + reset + " │";
                return;
            case MINE:
                sym = red_fg + "✸" + reset + " │";
                return;
            case ADJ_TO_MINE:
                sym = white_fg + std::to_string(noOfAdjMines) + reset + " │";
                return;
        }
    }
}

void Cell::markAdjMine(int mines){
    state = ADJ_TO_MINE;
    noOfAdjMines = mines;
}

void Cell::mine(){
    state = MINE;
}

using GRID = std::vector <std::vector <Cell>>;