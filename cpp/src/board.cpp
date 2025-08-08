#include "board.h"

#include <GLFW/glfw3.h>
#include <sstream>
#include <string>

Board::Board() {
	for (int row = 0; row < BOARD_WIDTH; row++) {
		for (int col = 0; col < BOARD_WIDTH; col++) {
			addTile(row, col);
			tiles_state[row + col] = TileState::Empty;
		}
	}

	grid.push_back({1.0f / 3, 1.0f, 0.0f});
	grid.push_back({1.0f / 3, -1.0f, 0.0f});
	grid.push_back({-1.0f / 3, 1.0f, 0.0f});
	grid.push_back({-1.0f / 3, -1.0f, 0.0f});

	grid.push_back({1.0f, 1.0f / 3, 0.0f});
	grid.push_back({-1.0f, 1.0f / 3, 0.0f});
	grid.push_back({1.0f, -1.0f / 3, 0.0f});
	grid.push_back({-1.0f, -1.0f / 3, 0.0f});
}

void Board::render(GLuint shaderProgram) {
	GLint colorLoc = glGetUniformLocation(shaderProgram, "ourColor");
	glUseProgram(shaderProgram);

	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
	glBindVertexArray(VAO_tiles);
	glDrawArrays(GL_TRIANGLES, 0, tiles.size());

	glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
	glLineWidth(3.0f);
	glBindVertexArray(VAO_lines);
	glDrawArrays(GL_LINES, 0, grid.size());
}

void Board::setupBuffers() {
	glGenVertexArrays(1, &VAO_lines);
	glGenBuffers(1, &VBO_lines);

	glBindVertexArray(VAO_lines);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_lines);
	glBufferData(GL_ARRAY_BUFFER, grid.size() * sizeof(glm::vec3), grid.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &VAO_tiles);
	glGenBuffers(1, &VBO_tiles);

	glBindVertexArray(VAO_tiles);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tiles);
	glBufferData(GL_ARRAY_BUFFER, tiles.size() * sizeof(glm::vec3), tiles.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Board::addTile(int row, int col) {
	GLfloat left = -1.0f + TILE_SIZE * col;
	GLfloat right = left + TILE_SIZE;
	GLfloat top = 1.0f - TILE_SIZE * row;
	GLfloat bottom = top - TILE_SIZE;

	tiles.push_back({left, top, 0.0f});
	tiles.push_back({right, top, 0.0f});
	tiles.push_back({right, bottom, 0.0f});

	tiles.push_back({left, top, 0.0f});
	tiles.push_back({right, bottom, 0.0f});
	tiles.push_back({left, bottom, 0.0f});
}

const char *Board::getSize() {
	static std::ostringstream oss;
	oss.str("");
	oss << "Tiles count: " << tiles.size();
	static std::string result = oss.str();
	return result.c_str();
}
