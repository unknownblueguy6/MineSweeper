#pragma once

#include <iostream>
#include <random>
#include "cell.hpp"

class Field{
    public:
        Field();
        void mineTheField();
        void markAdjMineCells();
        void drawField();
        void getMove();
    private:
        int l;
        int b;
        int m;
        int x;
        int y;
        GRID cells;

} field;

Field::Field(){
    std::cin >> l >> b >> m;
    x = l/2;
    y = b/2;
    for(int i = 0; i < b; ++i){
        Cell c;
        std::vector<Cell> v(l, c);
        cells.push_back(v);
    }
    mineTheField();
    markAdjMineCells();
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
        if (cells[i][j].state != MINE){
            cells[i][j].mine();
            --m_copy;
        } 
    }
}

void Field::markAdjMineCells(){
    for(int j = 0; j < b; ++j){
        for(int i = 0; i < l; ++i){
            if(cells[i][j].state == MINE) continue;
            int mines = 0;
            for(int d = j - 1; d < j + 2; ++d){
                if (d < 0 || d > b-1) continue;
                for(int c = i - 1; c < i + 2; ++c){
                    if (c < 0 || c > l-1) continue;
                    
                    if(cells[c][d].state == MINE) ++mines;
                }
            }
            if(mines) cells[i][j].markAdjMine(mines);
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
                else std::cout << blue_bg << cells[i][j].sym;
            }
            else std::cout << cells[i][j].sym;
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
    std::cout << x << " " << y << endl;
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
            cells[x][y].toggleflag();
            return;
        case K_S:
            cells[x][y].sweep();
            return;  
    }
}