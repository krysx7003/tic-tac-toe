#pragma once
#include "board.h"

class Game {
	char curr_player = -1;
	short lastTile = -1;
	short winner = -1;

	bool verticalLine(int col);
	bool horizontalLine(int row);
	bool diagonalLine();
	bool diagonalLeftLine();
	bool diagonalRightLine();

	void setWinner(char state);
	void swapPlayer();

  public:
	Board board;
	bool active = true;

	Game(bool gui);
	Game() {};

	void start();
	bool chosenTile(float col, float row);
	bool chosenTile(int tileId);
	bool winCondition();
	bool isDraw();
	short getLastTile();
	void restart();

	char getPlayer();
	short getWinner();
};
