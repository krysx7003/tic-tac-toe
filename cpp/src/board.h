#pragma once

#include "player.h"
#include "tile.h"

#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <vector>

using namespace std;

const int BOARD_SIZE = 9;
const int BOARD_WIDTH = 3;
const float TILE_SIZE = 2.0f / BOARD_WIDTH;

class Board {
	std::vector<glm ::vec3> grid;
	std::vector<glm ::vec3> tiles;
	GLuint VAO_lines, VBO_lines;
	GLuint VAO_tiles, VBO_tiles;
	bool boardGui = false;

	void addTile(int row, int col);

  public:
	char tiles_state[BOARD_SIZE];

	Board(bool gui);
	Board();

	void setupBuffers();
	void render();
	bool takeTile(int pos, char player);
	void setTilesState();

	char *getTilesState();
	int getSize();
	int getTileCol(float pos);
	int getTileRow(float pos);
};
