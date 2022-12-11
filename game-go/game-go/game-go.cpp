#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

/* Comment: in the final program declare appropriate constants, e.g.,
   to eliminate from your program numerical values by replacing them
   with well defined identifiers */
const char* FIRST_NAME = "FIRST_NAME";
const char* LAST_NAME = "LAST_NAME";
const char* STUDENT_NUMBER = "STUDENT_NUMBER";

const int LOCATION_X_OF_LEGEND = 1;
const int LOCATION_X_OF_BOARD = 60;

int SIZE_OF_BOARD = 9;
const int MAX_SIZE_OF_BOARD = 19;

void drawBoard() {
	for (int i = LOCATION_X_OF_BOARD; i < LOCATION_X_OF_BOARD + SIZE_OF_BOARD + 2; i++) {
		cputs("-");
		gotoxy(LOCATION_X_OF_BOARD, i - LOCATION_X_OF_BOARD + 1);
		cputs("|");
		gotoxy(i, 2 + SIZE_OF_BOARD);
		cputs("-");
		gotoxy(LOCATION_X_OF_BOARD + SIZE_OF_BOARD + 1, i - LOCATION_X_OF_BOARD + 1);
		cputs("|");
		gotoxy(i, 1);
	}
}

char* getTitleLine() {
    char* result = new char[strlen(FIRST_NAME) + strlen(LAST_NAME) + strlen(STUDENT_NUMBER) + 2];
	strcpy(result, FIRST_NAME);
	strcat(result, " ");
	strcat(result, LAST_NAME);
	strcat(result, " ");
	strcat(result, STUDENT_NUMBER);
	return result;
}

char* getLineLegend(const char* firstPart, const char* secondPart) {
	char* result = (char*)malloc(strlen(firstPart) + strlen(secondPart));
	strcpy(result, firstPart);
	strcat(result, secondPart);
	return result;
}

char* intToCharArray(const int& value) {
	int size = value == 0 ? 1 : int(log10(value) + 1);
	char* number = (char*)malloc(size);
	_itoa(value, number, 10);
	return number;
}

char* getLineOfCoordinates(const int& x, const int& y) {
	char* result = (char*)malloc(27 + strlen(intToCharArray(x)) + strlen(intToCharArray(y)));
	strcpy(result, "Coordinates of cursor: (");
	strcat(result, intToCharArray(x));
	strcat(result, "; ");
	strcat(result, intToCharArray(y));
	strcat(result, ")");
	return result;
}

bool isCoordinateInInterval(const int& begin, const int& end, const int& coordinate) {
	return (coordinate > begin && coordinate < end);
}

const int DEFAULT_ZN = 0;
const int DEFAULT_X = 1;
const int DEFAULT_Y = 2;
const int DEFAULT_ATTR = 1;
const int DEFAULT_BACK = 0;
const int DEFAULT_ZERO = 0;

struct Stone {
	int x = 0;
	int y = 0;
	int color = 0; // 0 - empty // 1 - black(blue) // 2 - white(red)
};

void drawStones(Stone stones[MAX_SIZE_OF_BOARD][MAX_SIZE_OF_BOARD]) {
	for (int i = 0; i < SIZE_OF_BOARD; i++) {
		for (int j = 0; j < SIZE_OF_BOARD; j++) {
			if (stones[i][j].color != 0) {
				gotoxy(stones[i][j].x, stones[i][j].y);
				textcolor(stones[i][j].color);
				putch('o');
			}
		}
	}
}

int getIndexFromCoordinatesX(const int& x) {
	return x - LOCATION_X_OF_BOARD - DEFAULT_X;
}

int getIndexFromCoordinatesY(const int& y) {
	return y - DEFAULT_Y;
}

void checkStones(Stone stones[MAX_SIZE_OF_BOARD][MAX_SIZE_OF_BOARD], const int& colorCode, bool& posibilityToCancel) {
	for (int i = 0; i < SIZE_OF_BOARD; i++) {
		for (int j = 0; j < SIZE_OF_BOARD; j++) {
			

			if (stones[i][j].color == 0) {
				continue;
			}

			int hp = 4;
			if (i == 0 || i == SIZE_OF_BOARD - 1) {
				hp--;
			}

			if (j == 0 || j == SIZE_OF_BOARD - 1) {
				hp--;
			}

			if (i > 0 && stones[i - 1][j].color != stones[i][j].color && stones[i - 1][j].color != 0) {
				hp--;
			}

			if (i < SIZE_OF_BOARD - 1 && stones[i+1][j].color != stones[i][j].color && stones[i + 1][j].color != 0) {
				hp--;
			}

			if (j > 0 && stones[i][j - 1].color != stones[i][j].color && stones[i][j - 1].color != 0) {
				hp--;
			}

			if (j < SIZE_OF_BOARD - 1 && stones[i][j + 1].color != stones[i][j].color && stones[i][j + 1].color != 0) {
				hp--;
			}

			if (hp == 0 && colorCode != stones[i][j].color) {
				stones[i][j].color = 0;
				posibilityToCancel = false;
			}
		}
	}
}

char* getLineOfScore(const int& player, const int& score) {
	char* result = (char*)malloc(19 + strlen(intToCharArray(score)));
	strcpy(result, "Score of player ");
	strcat(result, intToCharArray(player));
	strcat(result, ": ");
	strcat(result, intToCharArray(score));
	return result;
}

void getScore(int& score1, int& score2, Stone stones[MAX_SIZE_OF_BOARD][MAX_SIZE_OF_BOARD]) {
	score1 = 0;
	score2 = 0;
	for (int i = 0; i < SIZE_OF_BOARD; i++) {
		for (int j = 0; j < SIZE_OF_BOARD; j++) {
			score1 = stones[i][j].color == 1 ? score1 + 1 : score1;
			score2 = stones[i][j].color == 4 ? score2 + 1 : score2;
		}
	}
}

int main() {
	int zn = DEFAULT_ZN, x = LOCATION_X_OF_BOARD + DEFAULT_X, y = DEFAULT_Y, attr = DEFAULT_ATTR, back = DEFAULT_BACK, zero = DEFAULT_ZERO;
	char txt[32];

	Stone stones[MAX_SIZE_OF_BOARD][MAX_SIZE_OF_BOARD];
	int lastI = 0, lastJ = 0;
	bool posibilyToCancel = false;
	int score1 = 0, score2 = 0;

	// if the program is compiled in pure C, then we need to initialize
	// the library ourselves; __cplusplus is defined only if a C++ compiler
	// is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	
	// settitle sets the window title
	settitle(getTitleLine());

	// hide the blinking cursor
	_setcursortype(_NOCURSOR);
	
	do {
		// we set black to be the background color
		// check conio2.h for available colors
		textbackground(BLACK);
		// clear the screen: we fill it out with spaces with
		// the specified background color
		clrscr();
		// we set the text color (7 == LIGHTGRAY)
		textcolor(7);
		
		drawStones(stones);
		textcolor(7);
		drawBoard();
		// we move the coursor to column 48 and row 1
		// rows and column are numbered starting with 1
		gotoxy(LOCATION_X_OF_LEGEND, 1);
		// we print out a text at a given cursor position
		// the cursor will move by the length of the text
		cputs(getLineLegend("First name: ", FIRST_NAME));
		gotoxy(LOCATION_X_OF_LEGEND, 2);
		cputs(getLineLegend("Last name: ", LAST_NAME));
		gotoxy(LOCATION_X_OF_LEGEND, 3);
		cputs(getLineLegend("Student number: ", STUDENT_NUMBER));
		gotoxy(LOCATION_X_OF_LEGEND, 4);
		cputs("arrows - moving the cursor over the board");
		gotoxy(LOCATION_X_OF_LEGEND, 5);
		cputs("q      - quit the program");
		gotoxy(LOCATION_X_OF_LEGEND, 6);
		cputs("n      - start new game");
		gotoxy(LOCATION_X_OF_LEGEND, 7);
		cputs("esc    - cancel current action");
		gotoxy(LOCATION_X_OF_LEGEND, 8);
		cputs("i      - place a stone on the board");
		gotoxy(LOCATION_X_OF_LEGEND, 9);
		cputs("s      - save the game state");
		gotoxy(LOCATION_X_OF_LEGEND, 10);
		cputs("l      - load the game state");
		gotoxy(LOCATION_X_OF_LEGEND, 11);
		cputs("f      - finish the game");
		gotoxy(LOCATION_X_OF_LEGEND, 12);
		cputs("+      - change board size");
		gotoxy(LOCATION_X_OF_LEGEND, 13);
		cputs(getLineOfCoordinates(x, y));
		gotoxy(LOCATION_X_OF_LEGEND, 14);
		cputs(getLineOfScore(1, score1));
		gotoxy(LOCATION_X_OF_LEGEND, 15);
		cputs(getLineOfScore(2, score2));
		gotoxy(LOCATION_X_OF_LEGEND, 16);
		// print out the code of the last key pressed
		if (zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
		else sprintf(txt, "key code: 0x%02x", zn);
		gotoxy(LOCATION_X_OF_LEGEND, 18);
		cputs(txt);

		// we draw a star
		gotoxy(x, y);
		textcolor(attr);
		textbackground(back);
		// putch prints one character and moves the cursor to the right
		putch('*');

		// we wait for key press and get its code
		// most key codes correspond to the characters, like
		// a is 'a', 2 is '2', + is '+', but some special keys
		// like cursors provide two characters, where the first
		// one is zero, e.g., "up arrow" is zero and 'H'
		zero = 0;
		zn = getch();
		if (zn == 0x6e)
			main();
		else if (zn == 0x3d) {
			switch (SIZE_OF_BOARD) {
				case 9: {
					SIZE_OF_BOARD = 13;
					main();
				}
					  break;
				case 13: {
					SIZE_OF_BOARD = 19;
					main();
				}
					   break;
				default:
				{
					SIZE_OF_BOARD = 9;
					main();
				}
			}
		}
		else if (zn == 0x2d) {
			switch (SIZE_OF_BOARD) {
			case 9: {
				SIZE_OF_BOARD = 19;
				main();
			}
				  break;
			case 13: {
				SIZE_OF_BOARD = 9;
				main();
			}
				   break;
			default:
			{
				SIZE_OF_BOARD = 13;
				main();
			}
			}
		}
		else if (zn == 0x69) {
			int i = x - DEFAULT_X - LOCATION_X_OF_BOARD, j = y - DEFAULT_Y;
			
			if (stones[i][j].color == 0) {
				putch('o');
				stones[i][j].x = x;
				stones[i][j].y = y;
				stones[i][j].color = attr;
				posibilyToCancel = true;
				checkStones(stones, attr, posibilyToCancel);
				attr = attr == 1 ? 4 : 1;
				lastI = i;
				lastJ = j;
			}

			textcolor(7);
		}
		else if (zn == 0x1b) {
			if (posibilyToCancel) {
				x = stones[lastI][lastJ].x;
				y = stones[lastI][lastJ].y;
				gotoxy(x, y);
				textcolor(stones[lastI][lastJ].color);
				attr = stones[lastI][lastJ].color;

				stones[lastI][lastJ].x = 0;
				stones[lastI][lastJ].y = 0;
				stones[lastI][lastJ].color = 0;
				posibilyToCancel = false;
			}
		};

		getScore(score1, score2, stones);
		
		// we do not want the key 'H' to play role of "up arrow"
		// so we check if the first code is zero
		if (zn == 0) {
			zero = 1;		// if this is the case then we read
			zn = getch();		// the next code knowing that this
			if (zn == 0x48 && isCoordinateInInterval(1, 2 + SIZE_OF_BOARD, y - 1)) y--;	// will be a special key
			else if (zn == 0x50 && isCoordinateInInterval(1, 2 + SIZE_OF_BOARD, y + 1)) y++;
			else if (zn == 0x4b && isCoordinateInInterval(LOCATION_X_OF_BOARD, LOCATION_X_OF_BOARD + SIZE_OF_BOARD + 1, x - 1)) x--;
			else if (zn == 0x4d && isCoordinateInInterval(LOCATION_X_OF_BOARD, LOCATION_X_OF_BOARD + SIZE_OF_BOARD + 1, x + 1)) x++;
		}
		else if (zn == ' ') attr = (attr + 1) % 16;
		else if (zn == 0x0d) back = (back + 1) % 16;	// enter key is 0x0d or '\r'
	} while (zn != 'q');

	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be
	// visible after the program ends
	return 0;
}
