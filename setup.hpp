#pragma once

#include <unistd.h>
#include <termios.h>

using KEY = char;
using COLOUR = const std::string;

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

// #ifdef _WIN32
// char getInput()

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

enum CELL_STATES{
    EMPTY,
    MINE,
    ADJ_TO_MINE
};

COLOUR red_fg = "\033[1;31m";
COLOUR green_fg = "\033[1;32m";
COLOUR blue_bg = "\033[1;44m";
COLOUR white_fg = "\033[37m";
COLOUR white_bg = "\033[1;47m";
COLOUR reset = "\033[0m";
COLOUR endl = "\n";