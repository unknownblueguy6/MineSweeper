#pragma once

#include <vector>
#include <string>
#include "colour.hpp"
#include "setup.hpp"

enum CELL_STATES
{
    EMPTY,
    MINE,
    ADJ_TO_MINE
};

COLOUR ADJMINECOLOURS[] = {
    reset,
    cyan_fg,    //1
    green_fg,   //2
    red_fg,     //3
    magenta_fg, //4
    yellow_fg,  //5
    white_fg,   //6
    magenta_fg, //7
    red_fg      //8
};

struct Cell
{
    Cell();
    void toggleflag();
    void reveal();
    void setMine();
    void markAdjMine(int);
    bool flagged;
    bool hidden;
    CELL_STATES state;
    int noOfAdjMines;
    std::string sym;
};

Cell::Cell()
{
    hidden = true;
    flagged = false;
    state = EMPTY;
    sym = white_bg + " ";
}

void Cell::toggleflag()
{
    if (hidden)
    {
        flagged = !flagged;
        if (flagged)
            sym = green_fg + "▶";
        
        else
            sym = white_bg + " ";
    }
}

void Cell::reveal()
{
    if (!flagged && hidden)
    {
        hidden = false;
        switch (state)
        {
        case EMPTY:
            sym = reset + " ";
            return;
        case MINE:
            sym = red_fg + "✸";
            return;
        case ADJ_TO_MINE:
            sym = ADJMINECOLOURS[noOfAdjMines] + std::to_string(noOfAdjMines);
            return;
        }
    }
}

void Cell::markAdjMine(int mines)
{
    state = ADJ_TO_MINE;
    noOfAdjMines = mines;
}

void Cell::setMine()
{
    state = MINE;
}

using GRID = std::vector<std::vector<Cell>>;