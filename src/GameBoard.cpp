#include "GameBoard.h"
#include <ncurses.h>
#include <stdlib.h>
#include "Style.h"

const int GameBoard::WIDTH=10;
const int GameBoard::HEIGHT=20;
const int GameBoard::COLLISION=1;

using namespace std;

GameBoard::GameBoard() {
	clearBoard();
}

void GameBoard::draw(WINDOW* win) {
	for(int r = 0; r < HEIGHT; ++r) {
		for(int c = 0; c < WIDTH; ++c)  {
			if(board[r][c] == 0)
				continue;
			setStyle(win, board[r][c]);
			mvwaddch(win, r+1, c*2+1, ' ');
			mvwaddch(win, r+1, c*2+2, ' ');
		}//end for
	}//end for
}

int GameBoard::update(Piece& p) {
	for(int r = 0; r < p.HEIGHT; ++r)
		for(int c = 0; c < p.WIDTH; ++c) {
			if(p.tiles[r][c] == 0)
				continue;
			int cell_row = r+p.row;
			int cell_col = c+p.col;
			if(cell_col < 0 || cell_col >= WIDTH)
				return COLLISION;
			if(cell_row < 0)
				continue;
			//we let pieces fall from the top so it is ok not to check the top for collision
			if(cell_row >= HEIGHT)
				return COLLISION;
			if(board[cell_row][cell_col] != 0)
				return COLLISION;
		}
	return 0;
}

void GameBoard::removeLine(int row) {
	for(int r = row; r > 0; --r) {
		for(int c = 0; c < WIDTH; ++c)
			board[r][c] = board[r-1][c];
	}
	for(int c = 0; c < WIDTH; ++c)
		board[0][c] = 0;
}

int GameBoard::removeLines() {
	int ct = 0;
	for(int r = 0; r < HEIGHT; ++r) {
		int c = 0;
		for(; c < WIDTH; ++c) {
			if(board[r][c] == 0)
				break;
		}
		if(c == WIDTH) {
			ct++;
			removeLine(r);
		}
	}
	return ct;
}

void GameBoard::add(Piece& p) {
	for(int r = 0; r < p.HEIGHT; ++r)
		for(int c = 0; c < p.WIDTH; ++c) {
			if(p.tiles[r][c] == 0)
				continue;
			int cell_row = r + p.row;
			int cell_col = c + p.col;
			if(cell_row < 0 || cell_row >= HEIGHT || cell_col < 0 || cell_col > WIDTH)
				continue;
			board[cell_row][cell_col] = p.tiles[r][c];
		}
}

void GameBoard::keyPress(char ch) {
	//int val = (int) ch - '0';
	//if(val >= 0 && val <= 9)
	//	clearBoard(val);
}

void GameBoard::clearBoard(int val) {
	for(int r = 0; r < HEIGHT; ++r)
		for(int c = 0; c < WIDTH; ++c)
			board[r][c] = val;
}
