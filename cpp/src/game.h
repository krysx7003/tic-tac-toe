#pragma once

#include "board.h"
#include <string>

class Game {
	json config;

	int top_menu_height;
	char curr_player = -1;
	short lastTile = -1;
	short winner = -1;
	glm::vec2 texture_pos = glm::vec2(0, 0);
	glm::vec2 texture_size = glm::vec2(0, 0);
	std::string texture;

	bool verticalLine(int col, std::vector<char> state);
	bool horizontalLine(int row, std::vector<char> state);
	bool diagonalLeftLine(std::vector<char> state);
	bool diagonalRightLine(std::vector<char> state);

	void setWinner(char state);
	void swapPlayer();

  public:
	Board board;
	bool active = true;

	void Init();
	Game() {};

	void Start();
	void Render();
	bool ChosenTile(double x, double y);
	bool ChosenTile(int tileId);
	bool WinCondition(std::vector<char> state);
	bool IsDraw(std::vector<char> state);
	short GetLastTile();
	void Restart();

	char GetPlayer();
	char GetWinner();
};
