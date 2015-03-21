#include "Piece.h"
#include "Style.h"
#include "GameBoard.h"

//TODO:make the consts an enum
const int Piece::I = 0;
const int Piece::J = 1;
const int Piece::L = 2;
const int Piece::O = 3;
const int Piece::S = 4;
const int Piece::T = 5;
const int Piece::Z = 6;
const int Piece::WIDTH = 4;
const int Piece::HEIGHT = 4;

Piece::Piece(int t) {
	row = -4;
	col = GameBoard::WIDTH/2-2;
	rotated = false;

	switch(t) {
	case 0:
		copy(PIECE_I);	
	break;
	case 1:
		copy(PIECE_J);	
	break;
	case 2:
		copy(PIECE_L);	
	break;
	case 3:
		row = -3;
		copy(PIECE_O);	
	break;
	case 4:
		row = -3;
		copy(PIECE_S);	
	break;
	case 5:
		row = -3;
		copy(PIECE_T);	
	break;
	case 6:
		row = -3;
		copy(PIECE_Z);	
	break;
	}
}

void Piece::copy(const int pattern[4][4]) {
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			tiles[i][j] = pattern[i][j];
}

void Piece::draw(WINDOW* win, bool ignore_rc) {
	for(int r = 0; r < HEIGHT; ++r)
		for(int c = 0; c < WIDTH; ++c) {
			if(tiles[r][c] == 0)
				continue;

			int cell_row = r;
			int cell_col = c;
			if(!ignore_rc) {
				cell_row+=row;
				cell_col+=col;
			}

			if(cell_row < 0 || cell_col < 0)
				continue;

			setStyle(win, tiles[r][c]);
			mvwaddch(win, cell_row+1, 2*(cell_col)+1, ' ');
			mvwaddch(win, cell_row+1, 2*(cell_col)+2, ' ');
		}
}

void Piece::rotate() {
	int temp[WIDTH][HEIGHT];
	for(int r = 0; r < HEIGHT; ++r)
		for(int c = 0; c < WIDTH; ++c)
			temp[r][c] = tiles[r][c];
	for(int r = 0; r < HEIGHT; ++r)
		for(int c = 0; c < WIDTH; ++c) {
			tiles[r][c] = temp[c][HEIGHT-1-r];
		}
	rotated = !rotated;
}

