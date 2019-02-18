#pragma once

#include <iostream>
#include <utility>
#include <queue>
#include <random>
#include "SevSegDisp.hpp"
#include "buffer.hpp"
#include "display.hpp"
#include "colour.hpp"
#include "cell.hpp"

class Field{
    public:
        Field();
        void mineTheField();
        void markAdjMineCells();
        void startSweep();
        void drawField();
        void checkVictoryAndFlagMines();
        void getMove();
    private:
        int l;
        int b;
        int m;
        int x;
        int y;
        int flags;
        int hiddenCells;
        bool firstSweep;
        std::vector <std::pair<int, int>> mines; //store the location of mines
        GRID cells;

};

Field::Field(){
    switch(gameMode){
        case BEGINNER:
            l = 9;
            b = 9;
            m = 10;
            break;
        case INTERMEDIATE:
            l = 16;
            b = 16;
            m = 40;
            break;
        case EXPERT:
            l = 30;
            b = 16;
            m = 99;
            break;
        case CUSTOM:
            dispBanner();
            std::cout << blue_fg << "Enter length of MineField : " << white_fg;
            std::cin >> l;
            std::cout << blue_fg << "Enter breadth of MineField : " << white_fg;
            std::cin >> b;
            std::cout << blue_fg << "Enter number of Mines : " << red_fg;
            std::cin >> m;
            system("clear");
    }
    x = l/2;
    y = b/2;
    flags = m;
    flagDisp.set(flags);
    hiddenCells = l*b;
    firstSweep = true;
    for(int i = 0; i < l; ++i){
        Cell c;
        std::vector<Cell> v(b, c);
        cells.push_back(v);
    }

}

void Field::mineTheField(){
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> x_uni(0,l-1);
    std::uniform_int_distribution<int> y_uni(0,b-1);
    auto m_copy = m;
    while(m_copy){
        auto i = x_uni(rng);
        auto j = y_uni(rng);
        if ((i >= x - 1  && i <= x + 1) &&
            (j >= y - 1 && j <= y + 1)) continue;
        if (cells[i][j].state != MINE){
            cells[i][j].setMine();
            mines.push_back(std::make_pair(i, j));
            --m_copy;
        } 
    }
}

void Field::markAdjMineCells(){
    for(auto mine: mines){
        int x_pos = mine.first, y_pos = mine.second;
        for(int i = x_pos -1; i < x_pos + 2; ++i){
            if(i < 0 || i > l - 1) continue;
            for(int j = y_pos - 1; j < y_pos + 2; ++j){
                if(j < 0 || j > b - 1) continue;
                if(cells[i][j].state == MINE)continue;
                int mineCount = 0;
                for(int c = i - 1; c < i + 2; ++c){
                    if (c < 0 || c > l-1) continue;
                    for(int d = j - 1; d < j + 2; ++d){
                        if (d < 0 || d > b-1) continue;
                        if(cells[c][d].state == MINE) ++mineCount;
                    }
                }
                if(mineCount) cells[i][j].markAdjMine(mineCount);
            }
        }
    }
}

void Field::drawField(){
    for(int s = 0; s <= l*4; ++s) writeBuf << reset << " ";
    writeBuf << reset << endl;
	writeBuf << reset << "┌" ;
	for(int i = 0; i < l - 1; ++i) writeBuf << reset << "───┬";
	writeBuf << reset << "───┐";
	writeBuf << reset << endl;
	for(int j = 0; j < b; ++j){
        writeBuf << reset << "│";
		for(int i = 0; i < l; ++i){
            if(cells[i][j].state != MINE || cells[i][j].hidden) writeBuf << reset << " ";
            else writeBuf << reset << " "; //implement different whitespace char here
            if(i == x && j == y){ 
                if((cells[i][j].hidden || cells[i][j].state == EMPTY) &&
                   (!cells[i][j].flagged) ) writeBuf << blue_bg << " " << reset << " │";
                else writeBuf << blue_bg << cells[i][j].sym << reset << " │";
            }
            else writeBuf << cells[i][j].sym << reset << " │";
		}
        if(j != b-1) {
            writeBuf << endl << reset <<"├";
            for(int k = 0; k < l-1; ++k) writeBuf << reset << "───┼";
            writeBuf << reset << "───┤";
        }
        writeBuf << reset << endl;
	}
    writeBuf << reset << "└";
    for(int i = 0; i < l - 1; ++i) writeBuf << reset << "───┴";
    writeBuf << reset << "───┘";
	writeBuf << reset << endl;
    writeBuf.goToLine(0);
}

void Field::getMove(){
    KEY k = getKey();
    switch(k){
        case K_UP : 
            if(y) --y;
            return;
        case K_DOWN : 
            if(y != b-1) ++y;
            return;
        case K_LEFT:
            if(x) --x;
            return;
        case K_RIGHT:
            if(x != l-1) ++x;
            return;
        case K_F:
            if(cells[x][y].flagged){
                ++flags;
                ++flagDisp;
                cells[x][y].toggleflag();
            }
            else if(flags){
                --flags;
                --flagDisp;
                cells[x][y].toggleflag();
            }
            return;
        case K_S:
            if(firstSweep){
                mineTheField();
                markAdjMineCells();
                firstSweep = false;
            }
            startSweep();
            return;  
    }
}

void Field::checkVictoryAndFlagMines(){
     if(hiddenCells == m){
        gameState = VICTORY;
        //flag all mines that weren't flagged 
        for(auto mine : mines){ 
            if(!cells[mine.first][mine.second].flagged){ 
            cells[mine.first][mine.second].toggleflag();
            }
        }
    }
}

void Field::startSweep(){
    if(gameState != RUNNING) return;
    if(cells[x][y].flagged) return;

    if(cells[x][y].state == ADJ_TO_MINE){
        if(cells[x][y].hidden)--hiddenCells;
        cells[x][y].reveal();
        checkVictoryAndFlagMines();
        return;
    }

    if(cells[x][y].state == MINE){
        gameState = DEFEAT;
        for(auto mine : mines) cells[mine.first][mine.second].reveal();
        return;
    }

    auto pos = std::make_pair(x, y);
    
    std::queue<std::pair<int, int>> cellsToCheck;
    cellsToCheck.push(pos);
    bool checked[l][b];

    while(!cellsToCheck.empty()){
        auto p = cellsToCheck.front();
        int x_pos = p.first, y_pos = p.second; 
        for(int i = x_pos - 1; i < x_pos + 2; ++i){
            if (i < 0 || i > l-1) continue;
            
            for(int j = y_pos - 1; j < y_pos + 2; ++j){
                if (j < 0 || j > b-1) continue;

                switch(cells[i][j].state){
                    case EMPTY:
                        if(!checked[i][j]) cellsToCheck.push(std::make_pair(i, j));
                    case ADJ_TO_MINE:
                        if(cells[i][j].hidden) --hiddenCells;
                        cells[i][j].reveal();
                        break;
                }
            }
        }
        cellsToCheck.pop();
        checked[x_pos][y_pos] = true;
    }

    checkVictoryAndFlagMines();
}