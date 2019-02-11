#include <iostream>
#include <vector>
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
    BOMB,
    _1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8
};

constexpr auto red_bg = "\033[1;41m";
constexpr auto blue_bg = "\033[1;44m";
constexpr auto white_bg = "\033[1;47m";
constexpr auto reset = "\033[0m";
constexpr auto endl = "\n";

using KEY = char;
using GRID_INT = std::vector <std::vector <int>>;
using GRID_BOOL = std::vector <std::vector <bool>>;
using COLOUR = std::string;

// struct Cell{
//         Cell();
//         void toggle();  
//         bool hidden;
//         CELL_STATES state;
//         COLOUR col;    
// }

// Cell::Cell(){
//     hidden = true;
//     state = EMPTY;
//     col = white;
// }

// void Cell::toggle(){
//     hidden = false; 
// }

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
        void drawField();
        void getPos();
    private:
        int l;
        int b;
        int m;
        int x;
        int y;
        GRID_INT cells;
        GRID_BOOL hidden;

} field;

Field::Field(){
    std::cin >> l >> b >> m;
    x = l/2;
    y = b/2;
    for(int i = 0; i < l; ++i){
        std::vector<int> v(b, EMPTY);  
        cells.push_back(v);
        std::vector<bool> s(b, true);
        hidden.push_back(s);
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
            if(hidden[j][k]) std::cout << white_bg;
            if(j == y && k == x) std::cout << red_bg;
            std::cout << " " << reset << " │";
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

void Field::getPos(){
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
    }
}

int main(){
    while(true){
        field.drawField();
        field.getPos();
        system("clear");
    }
    std::cout << endl;
	return 0;
}