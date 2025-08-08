#pragma once

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
	void addTile(int row, int col);

  public:
	void setupBuffers();
	u_int tiles_state[BOARD_SIZE];
	Board();
	void render(GLuint shaderProgram);
	std::vector<glm ::vec3> getVertices();
	u_int *getTilesState();
	const char *getSize();

	~Board() {
		glDeleteVertexArrays(1, &VAO_lines);
		glDeleteBuffers(1, &VBO_lines);

		glDeleteVertexArrays(1, &VAO_tiles);
		glDeleteBuffers(1, &VBO_tiles);
	}
};
