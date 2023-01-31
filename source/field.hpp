#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include "sevsegdisp.hpp"
#include "buffer.hpp"
#include "display.hpp"
#include "colour.hpp"
#include "cell.hpp"

class Field
{
  public:
    Field();
    void mineTheField();
    void markAdjMineCells();
    void startSweep(int, int);
    void startSweep(int, int, POSOFCELL, DIR_X, DIR_Y);
    void drawField();
    void checkVictoryAndFlagMines();
    void getMove();

    bool checkValidityOfQuickClear();
  
  private:
    int l;
    int b;
    int m;
    int x;
    int y;
    int flags;
    int hiddenCells;
    
    bool firstSweep;
    
    std::vector<std::pair<int, int>> mines; //store the location of mines
    
    GRID cells;
};

Field::Field()
{
    switch (gameMode)
    {
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
        do
        {
            dispBanner();
            std::cout << endl;
            std::cout << white_fg << "    Minimum breadth of MineField is 9" << endl;
            std::cout << white_fg << "    Maximum size of MineField is 30 x 16" << endl;
            std::cout << blue_fg << "    Enter length of MineField : " << white_fg;
            std::cin >> l;

            std::cout << blue_fg << "    Enter breadth of MineField : " << white_fg;
            std::cin >> b;

            std::cout << blue_fg << "    Enter number of Mines : " << red_fg;
            std::cin >> m;
            system("clear");
        } while ((!l || !b || !m) || b < 9 || l > 30 || b > 16);
    }
    x = l / 2;
    y = b / 2;
    if (m > l * b - 9)
        m = l * b - 9;
    flags = m;
    flagDisp.set(flags);
    hiddenCells = l * b;
    firstSweep = true;
    for (int i = 0; i < l; ++i)
    {
        Cell c;
        std::vector<Cell> v(b, c);
        cells.push_back(v);
    }
}

void Field::mineTheField()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> x_uni(0, l - 1);
    std::uniform_int_distribution<int> y_uni(0, b - 1);
    auto m_copy = m;
    while (m_copy)
    {
        auto i = x_uni(rng);
        auto j = y_uni(rng);
        if ((i >= x - 1 && i <= x + 1) &&
            (j >= y - 1 && j <= y + 1))
            continue;
        if (cells[i][j].state != MINE)
        {
            cells[i][j].setMine();
            mines.push_back(std::make_pair(i, j));
            --m_copy;
        }
    }
}

void Field::markAdjMineCells()
{
    for (auto mine : mines)
    {
        int x_pos = mine.first, y_pos = mine.second;

        for (int i = x_pos - 1; i < x_pos + 2; ++i)
        {
            if (i < 0 || i > l - 1)
                continue;

            for (int j = y_pos - 1; j < y_pos + 2; ++j)
            {
                if (j < 0 || j > b - 1)
                    continue;
                if (cells[i][j].state == MINE)
                    continue;
                int mineCount = 0;

                for (int c = i - 1; c < i + 2; ++c)
                {
                    if (c < 0 || c > l - 1)
                        continue;
                    for (int d = j - 1; d < j + 2; ++d)
                    {
                        if (d < 0 || d > b - 1)
                            continue;
                        if (cells[c][d].state == MINE)
                            ++mineCount;
                    }
                }
                if (mineCount)
                    cells[i][j].markAdjMine(mineCount);
            }
        }
    }
}

void Field::drawField()
{
    writeBuf << reset;
    for (int s = 0; s <= l * 4; ++s)
        writeBuf << " ";
    writeBuf << endl;

    writeBuf << reset;
    writeBuf << "    ";
    if (cells[0][0].hidden)
        writeBuf << "┏";
    else
        writeBuf << reset << "┌";

    for (int i = 0; i < l - 1; ++i)
    {
        if (cells[i][0].hidden)
        {
            writeBuf << "━━━";
            if (cells[i + 1][0].hidden)
                writeBuf << "┳";
            else
                writeBuf << "┱";
        }
        else
        {
            writeBuf << "───";
            if (cells[i + 1][0].hidden)
                writeBuf << "┲";
            else
                writeBuf << "┬";
        }
    }

    if (cells[l - 1][0].hidden)
        writeBuf << "━━━┓";
    else
        writeBuf << "───┐";
    writeBuf << endl;

    writeBuf << reset;
    for (int j = 0; j < b; ++j)
    {
        if (cells[0][j].hidden)
            writeBuf << "    ┃";
        else
            writeBuf << reset << "    │";

        for (int i = 0; i < l; ++i)
        {
            if (cells[i][j].state != MINE || cells[i][j].hidden)
                writeBuf << " ";
            else
                writeBuf << " "; //implement different whitespace char here

            if (i == x && j == y)
            {
                if ((cells[i][j].hidden || cells[i][j].state == EMPTY) &&
                    (!cells[i][j].flagged))
                    writeBuf << blue_bg << " ";

                else
                    writeBuf << blue_bg << cells[i][j].sym;
            }
            else
                writeBuf << cells[i][j].sym;

            if (cells[i][j].hidden || (i != l - 1 && cells[i + 1][j].hidden))
                writeBuf << reset << " ┃";
            else
                writeBuf << reset << " │";
        }

        if (j != b - 1)
        {
            writeBuf << endl;
            writeBuf << reset;
            writeBuf << "    ";

            if (cells[0][j].hidden)
            {
                if (cells[0][j + 1].hidden)
                    writeBuf << "┣";
                else
                    writeBuf << "┡";
            }

            else
            {
                if (cells[0][j + 1].hidden)
                    writeBuf << "┢";
                else
                    writeBuf << "├";
            }

            for (int k = 0; k < l - 1; ++k)
            {
                if (cells[k][j].hidden)
                {
                    writeBuf << "━━━";

                    if (cells[k + 1][j].hidden && !cells[k][j + 1].hidden && !cells[k + 1][j + 1].hidden)
                    {
                        writeBuf << "╇";
                    }

                    else if (!cells[k + 1][j].hidden && cells[k][j + 1].hidden && !cells[k + 1][j + 1].hidden)
                    {
                        writeBuf << "╉";
                    }

                    else if (!cells[k + 1][j].hidden && !cells[k][j + 1].hidden && !cells[k + 1][j + 1].hidden)
                    {
                        writeBuf << "╃";
                    }

                    else
                    {
                        writeBuf << "╋";
                    }
                }
                else
                {
                    if (cells[k][j + 1].hidden)
                        writeBuf << "━━━";
                    else
                        writeBuf << "───";

                    if (cells[k + 1][j].hidden && !cells[k][j + 1].hidden && !cells[k + 1][j + 1].hidden)
                    {
                        writeBuf << "╄";
                    }

                    else if (!cells[k + 1][j].hidden && cells[k][j + 1].hidden && !cells[k + 1][j + 1].hidden)
                    {
                        writeBuf << "╅";
                    }

                    else if (!cells[k + 1][j].hidden && !cells[k][j + 1].hidden && cells[k + 1][j + 1].hidden)
                    {
                        writeBuf << "╆";
                    }

                    else if (cells[k + 1][j].hidden && !cells[k][j + 1].hidden && cells[k + 1][j + 1].hidden)
                    {
                        writeBuf << "╊";
                    }

                    else if (!cells[k + 1][j].hidden && cells[k][j + 1].hidden && cells[k + 1][j + 1].hidden)
                    {
                        writeBuf << "╈";
                    }

                    else if (!cells[k + 1][j].hidden && !cells[k][j + 1].hidden && !cells[k + 1][j + 1].hidden)
                    {
                        writeBuf << "┼";
                    }

                    else
                    {
                        writeBuf << "╋";
                    }
                }
            }

            if (cells[l - 1][j].hidden)
            {
                writeBuf << "━━━";
                if (cells[l - 1][j + 1].hidden)
                    writeBuf << "┫";
                else
                    writeBuf << "┩";
            }

            else
            {
                if (cells[l - 1][j + 1].hidden)
                    writeBuf << "━━━┪";
                else
                    writeBuf << "───┤";
            }
        }

        writeBuf << endl;
        writeBuf << reset;
    }

    writeBuf << "    ";
    if (cells[0][b - 1].hidden)
        writeBuf << "┗";
    else
        writeBuf << "└";

    for (int i = 0; i < l - 1; ++i)
    {
        if (cells[i][b - 1].hidden)
        {
            writeBuf << "━━━";
            if (cells[i + 1][b - 1].hidden)
                writeBuf << "┻";
            else
                writeBuf << "┹";
        }
        else
        {
            writeBuf << "───";
            if (cells[i + 1][b - 1].hidden)
                writeBuf << "┺";
            else
                writeBuf << "┴";
        }
    }

    if (cells[l - 1][b - 1].hidden)
        writeBuf << "━━━┛";
    else
        writeBuf << "───┘";

    writeBuf << endl;

    writeBuf.goToLine(0);
}

void Field::getMove()
{
    KEY k = getKey();
    switch (k)
    {
    case K_K:
    case K_k:
    case K_UP:
        if (y)
            --y;
        return;

    case K_J:
    case K_j:
    case K_DOWN:
        if (y != b - 1)
            ++y;
        return;

    case K_H:
    case K_h:
    case K_LEFT:
        if (x)
            --x;
        return;

    case K_L:
    case K_l:
    case K_RIGHT:
        if (x != l - 1)
            ++x;
        return;

    case K_SPACE:
    case K_F:
        if (cells[x][y].flagged)
        {
            ++flags;
            ++flagDisp;
            cells[x][y].toggleflag();
        }
        else if (flags && cells[x][y].hidden)
        {
            --flags;
            --flagDisp;
            cells[x][y].toggleflag();
        }
        return;

    case K_ENTER:
    case K_S:
        if (firstSweep)
        {
            mineTheField();
            markAdjMineCells();
            firstSweep = false;
        }
        startSweep(x, y);
        return;
    }
}

void Field::checkVictoryAndFlagMines()
{
    if (hiddenCells == m && gameState != DEFEAT)
    {
        gameState = VICTORY;
        //flag all mines that weren't flagged
        for (auto mine : mines)
        {
            if (!cells[mine.first][mine.second].flagged)
            {
                cells[mine.first][mine.second].toggleflag();
            }
        }
    }
}

bool Field::checkValidityOfQuickClear()
{
    int flagsCopy = 0;
    for (int i = x - 1; i < x + 2; ++i)
    {
        for (int j = y - 1; j < y + 2; ++j)
        {
            if (i < 0 || i > l - 1 || j < 0 || j > b - 1)
                continue;

            if (i == x && j == y)
                continue;

            if (cells[i][j].flagged)
                ++flagsCopy;
        }
    }

    bool valid = flagsCopy == cells[x][y].noOfAdjMines; 
    if (valid)
    {
        for (int i = x - 1; i < x + 2; ++i)
        {
            for (int j = y - 1; j < y + 2; ++j)
            {
                if (i < 0 || i > l - 1 || j < 0 || j > b - 1)
                    continue;

                if (i == x && j == y)
                    continue;

                switch (cells[i][j].state)
                {
                case EMPTY:
                case ADJ_TO_MINE:
                    break;

                case MINE:
                    if(cells[i][j].flagged)
                        break;

                    gameState = DEFEAT;
                    for (auto mine : mines)
                        cells[mine.first][mine.second].reveal();
                }
            }
        }
    }
    return valid;
}

void Field::startSweep(int x, int y)
{
    if (gameState != RUNNING)
        return;
    if (cells[x][y].flagged)
        return;
    if (!cells[x][y].hidden && !QUICKCLEAR)
        return;

    switch (cells[x][y].state)
    {
    case EMPTY:
        --hiddenCells;
        cells[x][y].reveal();
        checkVictoryAndFlagMines();
        break;

    case ADJ_TO_MINE:
        if (cells[x][y].hidden)
        {
            --hiddenCells;
            cells[x][y].reveal();
            checkVictoryAndFlagMines();
            return;
        }

        else if (QUICKCLEAR)
        {
            bool isValid = checkValidityOfQuickClear();
            checkVictoryAndFlagMines();
            if(isValid)
                break;
            else 
                return;
        }

    case MINE:
        gameState = DEFEAT;
        for (auto mine : mines)
            cells[mine.first][mine.second].reveal();
        return;
    }

    startSweep(x - 1, y - 1, CORNER, LEFT      , UP        );
    startSweep(x    , y - 1, EDGE  , NULL_DIR_X, UP        );
    startSweep(x + 1, y - 1, CORNER, RIGHT     , UP        );
    startSweep(x - 1, y    , EDGE  , LEFT      , NULL_DIR_Y);
    startSweep(x + 1, y    , EDGE  , RIGHT     , NULL_DIR_Y);
    startSweep(x - 1, y + 1, CORNER, LEFT      , DOWN      );
    startSweep(x    , y + 1, EDGE  , NULL_DIR_X, DOWN      );
    startSweep(x + 1, y + 1, CORNER, RIGHT     , DOWN      );
    checkVictoryAndFlagMines();
}

void Field::startSweep(int x, int y, POSOFCELL pos, DIR_X x_dir, DIR_Y y_dir)
{
    checkVictoryAndFlagMines();
    if (x < 0 || x > l - 1 || y < 0 || y > b - 1)
        return;
    if (gameState != RUNNING)
        return;
    if (cells[x][y].flagged)
        return;
    if (!cells[x][y].hidden)
        return;

    switch (cells[x][y].state)
    {
    case EMPTY:
        cells[x][y].reveal();
        --hiddenCells;
        if (pos == CORNER)
        {
            startSweep(x + x_dir, y + y_dir, CORNER, x_dir          , y_dir          );
            startSweep(x + x_dir, y        , EDGE  , x_dir          , NULL_DIR_Y     );
            startSweep(x        , y + y_dir, EDGE  , NULL_DIR_X     , y_dir          );
            startSweep(x + x_dir, y - y_dir, CORNER, x_dir          , (DIR_Y)(-y_dir));
            startSweep(x - x_dir, y + y_dir, CORNER, (DIR_X)(-x_dir), y_dir          );
        }

        else
        {
            startSweep(x + x_dir, y + y_dir, EDGE , x_dir, y_dir);
            
            if (y_dir == NULL_DIR_Y)
            {
                startSweep(x + x_dir, y - 1, CORNER, x_dir     , UP  );
                startSweep(x        , y - 1, EDGE  , NULL_DIR_X, UP  );
                startSweep(x        , y + 1, EDGE  , NULL_DIR_X, DOWN);
                startSweep(x + x_dir, y + 1, CORNER, x_dir     , DOWN);
            }
            else
            {
                startSweep(x - 1, y + y_dir, CORNER, LEFT , y_dir     );
                startSweep(x - 1, y        , EDGE  , LEFT , NULL_DIR_Y);
                startSweep(x + 1, y        , EDGE  , RIGHT, NULL_DIR_Y);
                startSweep(x + 1, y + y_dir, CORNER, RIGHT, y_dir     );
            }
        }

        break;

    case ADJ_TO_MINE:
        cells[x][y].reveal();
        --hiddenCells;
        break;
    default:
        break;
    }
}
