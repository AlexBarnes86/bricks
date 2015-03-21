#include <ncurses.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <stdlib.h>
#include <time.h>
#include "GameBoard.h"
#include "Piece.h"
#include "Style.h"

void initwin();
void gameThread();
void keyPress(char ch);

void fastClear(WINDOW* win);
void draw();
void drawBoardWindow();
void drawNextWindow();
void drawStatsWindow();

GameBoard gb;
Piece curPiece(0);
Piece nextPiece(0);
boost::mutex gb_mutex;

WINDOW* bw = NULL; //board window
WINDOW* nw = NULL; //next window
WINDOW* sw = NULL; //stats window

int lines = 0;
int level = 1;
int score = 0;
int ticks = 1000;

int main(int argc, char** argv) {
	initwin();

	bw = newwin(22, 22, 1, 1);//newwin(getmaxx(stdscr)/2-10, getmaxy(stdscr)/2-10,20, 20);
	nw = newwin(6, 12, 1, 24);
	sw = newwin(16, 12, 7, 24);
	
	srand(time(NULL));
	curPiece = Piece(rand()%7);
	nextPiece = Piece(rand()%7);

	draw();

	boost::thread counter(&gameThread);

	char ch;
	while((ch = getch()) != 'q') {
		gb_mutex.lock();

		keyPress(ch);
//		gb.removeLines();
		draw();

		gb_mutex.unlock();
	}
	
	delwin(sw);
	delwin(nw);
	delwin(bw);
	endwin();
}

void initwin() {
	initscr();
	//cbreak();
	noecho();
	keypad(stdscr, true);
	curs_set(0);
	timeout(-1);
	if(!has_colors()) {
		printw("Your terminal does not support colors!");
		exit(1);
	}
	start_color();
	initStyles();
	refresh();
}

void keyPress(char ch) {
	int row = curPiece.row;
	int col = curPiece.col;

	switch(ch) {
	case 'a': 
		curPiece.col--;
	break;
	case 's':
		while(gb.update(curPiece) != GameBoard::COLLISION) {
			row = curPiece.row;
			curPiece.row++;
		}
//		curPiece.row++;
//		if(gb.update(curPiece) == GameBoard::COLLISION) {
//			curPiece.row--;
//			gb.add(curPiece);
//			curPiece = nextPiece;
//			nextPiece = Piece(rand()%7);
//		}
	break;
	case 'd':
		curPiece.col++;
	break;
	case 'w':
		curPiece.rotate();
		if(gb.update(curPiece) == GameBoard::COLLISION)
			curPiece.rotate();
	break;
//	case 'n':
//		gb.add(curPiece);
//		curPiece = nextPiece;
//		nextPiece = Piece(rand()%7);
//	case 'c':
//		gb.clearBoard();
//	break;
//	case 'r':
//		clear();
//		draw();
//	break;
	}

	if(gb.update(curPiece) == GameBoard::COLLISION) {
		curPiece.row = row;
		curPiece.col = col;
	}
}

void gameThread() {
	while(1) {
		if(ticks < 0) //just a precaution
			ticks = 0;
		boost::this_thread::sleep(boost::posix_time::milliseconds(ticks)); 
		gb_mutex.lock();

		curPiece.row++;
		if(gb.update(curPiece) == GameBoard::COLLISION) {
			curPiece.row--;
			gb.add(curPiece);
			curPiece = nextPiece;
			nextPiece = Piece(rand()%7);
		}

		int removed = gb.removeLines();
		//addScore(removed);
		switch(removed) {
		case 1:
			score+=10;
		break;
		case 2:
			score+=100;
		break;
		case 3:
			score+=250;
		break;
		case 4:
			score+=500;
		break;
		} ////end addScore
		lines += removed; 
		level = lines / 10 + 1;
		ticks = 1000 - 50*(level-1);

		draw();

		gb_mutex.unlock();
	}
}

void draw() {
	drawBoardWindow();
	drawNextWindow();
	drawStatsWindow();
}

void drawBoardWindow() {
	setStyle(bw, 8);
	fastClear(bw);
	box(bw, 0, 0);
	gb.draw(bw);
	curPiece.draw(bw);
	wrefresh(bw);
}

void drawNextWindow() {
	setStyle(nw, 8);
	fastClear(nw);
	box(nw, 0, 0);
	mvwprintw(nw, 0, 0, "Next");
	nextPiece.draw(nw, true);
	wrefresh(nw);
}

void drawStatsWindow() {
	setStyle(sw, 8);
	box(sw, 0, 0);
	mvwprintw(sw, 2, 2, "Level:");
	mvwprintw(sw, 3, 2, "%d", level);
	mvwprintw(sw, 5, 2, "Lines:");
	mvwprintw(sw, 6, 2, "%d", lines);
	mvwprintw(sw, 8, 2, "Score:");
	mvwprintw(sw, 9, 2, "%d", score);
	wrefresh(sw);
}

void fastClear(WINDOW* win) {
	int w = getmaxx(win);
	int h = getmaxy(win);

	for(int r = 0; r < h; ++r)
		for(int c = 0; c < w; ++c)
			mvwaddch(win, r, c, ' ');
}
