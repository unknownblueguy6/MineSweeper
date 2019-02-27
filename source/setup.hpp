#pragma once

#include <unistd.h>
#include <termios.h>

enum GAME_MODE{
    BEGINNER = 1,
    INTERMEDIATE,
    EXPERT,
    CUSTOM
    
};

GAME_MODE gameMode;

enum GAME_STATE{
    VICTORY,
    DEFEAT,
    RUNNING
};

GAME_STATE gameState = RUNNING;

//only used with startSweep()
enum POSOFCELL{
    CENTRE,
    EDGE, 
    CORNER
};

enum DIR_X{
    NULL_DIR_X = 0,
    LEFT = -1, 
    RIGHT = 1
};

enum DIR_Y{
	NULL_DIR_Y = 0,
	UP = -1,
	DOWN = 1
};

const std::string endl = "\n";
using KEY = char;

enum KEYS{
	K_ESC = '\e',
	K_BRAC_OPEN = '[',
	K_UP = 'A',
	K_DOWN = 'B',
	K_LEFT = 'D',
	K_RIGHT = 'C',
	K_F = 'f', //flag a cell
	K_S = 's', //sweep an area
	K_ENTER = '\n',//sweep an area
	K_SPACE = ' ' //flag a cell
}; 

enum VI_KEYS{
	K_K = 75,
	K_J = 74,
	K_H = 72,
	K_L = 76,
	K_k = 107,
	K_j = 106,
	K_h = 104,
	K_l = 108
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
