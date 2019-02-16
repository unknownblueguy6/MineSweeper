#pragma once

#include <iostream>
#include <utility>
#include <queue>
#include <random>
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

} field;

Field::Field(){
    std::cin >> l >> b >> m;
    x = l/2;
    y = b/2;
    flags = m;
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
    std::cout << endl;
	std::cout << "┌" ;
	for(int i = 0; i < l - 1; ++i) std::cout << "───┬";
	std::cout << "───┐";
	std::cout << endl;
	for(int j = 0; j < b; ++j){
        std::cout << "│";
		for(int i = 0; i < l; ++i){
            if(cells[i][j].state != MINE || cells[i][j].hidden) std::cout << " ";
            else std::cout << " "; //implement different whitespace char here
            if(i == x && j == y){ 
                if((cells[i][j].hidden || cells[i][j].state == EMPTY) &&
                   (!cells[i][j].flagged) ) std::cout << blue_bg << " " << reset << " │";
                else std::cout << blue_bg << cells[i][j].sym << reset << " │";
            }
            else std::cout << cells[i][j].sym << reset << " │";
		}
        if(j != b-1) {
            std::cout << endl << "├";
            for(int k = 0; k < l-1; ++k) std::cout << "───┼";
            std::cout << "───┤";
        }
        std::cout << endl;
	}
    std::cout << "└";
    for(int i = 0; i < l - 1; ++i) std::cout << "───┴";
    std::cout << "───┘";
	std::cout << endl;
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
                cells[x][y].toggleflag();
            }
            else if(flags){
                --flags;
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
        state = VICTORY;
        //flag all mines that weren't flagged 
        for(auto mine : mines){ 
            if(!cells[mine.first][mine.second].flagged){ 
            cells[mine.first][mine.second].toggleflag();
            }
        }
    }
}

void Field::startSweep(){
    if(state != RUNNING) return;
    if(cells[x][y].flagged) return;

    if(cells[x][y].state == ADJ_TO_MINE){
        if(cells[x][y].hidden)--hiddenCells;
        cells[x][y].reveal();
        checkVictoryAndFlagMines();
        return;
    }

    if(cells[x][y].state == MINE){
        state = DEFEAT;
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