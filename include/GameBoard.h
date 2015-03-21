#ifndef __GAME_BOARD_H
#define __GAME_BOARD_H

#include <vector>
#include "Piece.h"

class GameBoard {
public:
	static const int WIDTH;
	static const int HEIGHT;

	static const int COLLISION;

	GameBoard();
	void keyPress(char ch);
	void clearBoard() {clearBoard(0);}

	int update(Piece& p); //returns collision type
	int removeLines();
	void add(Piece& p);

	//TODO: push draw method out (seperation of concerns)
	void draw(WINDOW* win = stdscr);

private:
	int board[20][10]; //TODO: what is the correct syntax for a multidimensional array in a class?
	void clearBoard(int val);
	void removeLine(int row);
};

#endif
