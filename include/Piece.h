#ifndef __PIECE_H
#define __PIECE_H

#include <ncurses.h>

const int PIECE_I[4][4] = {{0, 1, 0, 0},
						   {0, 1, 0, 0},
						   {0, 1, 0, 0},
						   {0, 1, 0, 0}};

const int PIECE_J[4][4] = {{0, 0, 0, 0},
						   {0, 0, 2, 0},
						   {0, 0, 2, 0},
						   {0, 2, 2, 0}};

const int PIECE_L[4][4] = {{0, 0, 0, 0},
						   {0, 3, 0, 0},
						   {0, 3, 0, 0},
						   {0, 3, 3, 0}};

const int PIECE_O[4][4] = {{0, 0, 0, 0},
						   {0, 4, 4, 0},
						   {0, 4, 4, 0},
						   {0, 0, 0, 0}};

const int PIECE_S[4][4] = {{0, 0, 0, 0},
						   {0, 5, 5, 0},
						   {5, 5, 0, 0},
						   {0, 0, 0, 0}};

const int PIECE_T[4][4] = {{0, 0, 0, 0},
						   {0, 6, 0, 0},
						   {6, 6, 6, 0},
						   {0, 0, 0, 0}};

const int PIECE_Z[4][4] = {{0, 0, 0, 0},
						   {0, 7, 7, 0},
						   {0, 0, 7, 7},
						   {0, 0, 0, 0}};

class Piece {
public:
	static const int I, J, L, O, S, T, Z;
	static const int WIDTH, HEIGHT;

	int row, col;
	int tiles[4][4];

	Piece(int t);
	void copy(const int pattern[4][4]);

	void draw(WINDOW* win, bool ignore_rc = false);
	void rotate();

private:
	bool rotated;
};

#endif
