#pragma once

#include "player.h"
#include "tile.h"

#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
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
	short curr_player;

	void addTile(int row, int col);

  public:
	short tiles_state[BOARD_SIZE];

	Board();

	void setupBuffers();
	void render(GLuint shaderProgram);
	void swapPlayer();
	bool takeTile(int pos);

	short *getTilesState();
	int getSize();
	int getTileCol(float pos);
	int getTileRow(float pos);
	short getPlayer();

	~Board() {
		glDeleteVertexArrays(1, &VAO_lines);
		glDeleteBuffers(1, &VBO_lines);

		glDeleteVertexArrays(1, &VAO_tiles);
		glDeleteBuffers(1, &VBO_tiles);
	}
};
