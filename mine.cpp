#include <iostream>
#include <vector>
#include <random>
#include <unistd.h>
#include <termios.h>

enum KEYS{
	K_ESC = '\e',
	K_BRAC_OPEN = '[',
	K_UP = 'A',
	K_DOWN = 'B',
	K_LEFT = 'D',
	K_RIGHT = 'C',
    K_F = 'f', //flag a cell
    K_S = 's' //sweep a cell
}; 

enum CELL_STATES{
    EMPTY = -1,
    MINE,
    _1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8
};

const std::string red_fg = "\033[1;31m";
const std::string green_fg = "\033[1;32m";
const std::string blue_bg = "\033[1;44m";
const std::string white_bg = "\033[1;47m";
const std::string reset = "\033[0m";
const std::string endl = "\n";

struct Cell{
    Cell();
    void toggleflag();
    void sweep();
    void mine();
    bool flagged;  
    bool hidden;
    CELL_STATES state;
    std::string sym;
};

Cell::Cell(){
    hidden = true;
    flagged = false;
    state = EMPTY;
    sym = white_bg + " ";
}

void Cell::toggleflag(){
    if(hidden){
        flagged = !flagged;
        if(flagged){
            sym = green_fg + "▶";
        }
        else{
            sym = white_bg + " ";
        }
    }
}

void Cell::sweep(){
    if(!flagged){
        hidden = false;
        switch(state){
            case EMPTY:
                sym = reset + " ";
                return;
            case MINE:
                sym = red_fg + "●";
                return;
        }
    }
}

void Cell::mine(){
    state = MINE;
}

using KEY = char;
using GRID = std::vector <std::vector <Cell>>;

char getch() {
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("tcsetattr ~ICANON");
	return (buf);
}

KEY getKey(){
	char c = getch();
	c = (c == K_ESC) ? getch() : c;
	c = (c == K_BRAC_OPEN) ? getch() : c;
	return c;
}

class Field{
    public:
        Field();
        void mineTheField();
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

void Field::drawField(){
    std::cout << endl;
	std::cout << "┌" ;
	for(int i = 0; i < l - 1; ++i) std::cout << "───┬";
	std::cout << "───┐";
	std::cout << endl;
	for(int j = 0; j < b; ++j){
        std::cout << "│";
		for(int k = 0; k < l; ++k){
            std::cout << " ";
            if(j == y && k == x) std::cout << blue_bg << " " << reset << " │";
            else std::cout << cells[k][j].sym << reset << " │";
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

int main(){
    while(true){
        field.drawField();
        field.getMove();
        system("clear");
    }
    std::cout << endl;
	return 0;
}