#pragma once
#include "board.h"

class Game {
	short curr_player = -1;
	short lastTile = -1;
	short winner = -1;

	bool verticalLine(int col);
	bool horizontalLine(int row);
	bool diagonalLine();
	bool diagonalLeftLine();
	bool diagonalRightLine();
	bool isDraw();

	void setWinner(char state);
	void swapPlayer();

  public:
	Board board;
	bool active = true;

	Game(bool gui);
	Game() {};

	void clickedTile(float col, float row);
	bool winCondition();
	short getLastTile();
	void restart();

	short getPlayer();
	short getWinner();
};
