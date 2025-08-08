#include "board.h"
#include "player.h"

#include <GLFW/glfw3.h>

Board::Board() {
	for (int row = 0; row < BOARD_WIDTH; row++) {
		for (int col = 0; col < BOARD_WIDTH; col++) {
			addTile(row, col);
			tiles_state[(row * 3) + col] = TileState::Empty;
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

	curr_player = Player::O;
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

void Board::swapPlayer() {
	if (curr_player == Player::O) {
		curr_player = Player::X;
	} else {
		curr_player = Player::O;
	}
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

bool Board::takeTile(int pos) {
	if (tiles_state[pos] != TileState::Empty) {
		return false;
	}
	if (curr_player == Player::O) {
		tiles_state[pos] = TileState::TakenO;
	} else {
		tiles_state[pos] = TileState::TakenX;
	}
	return true;
}

int Board::getTileCol(float pos) {
	if (pos <= -0.33f)
		return 0;
	if (pos < 0.33f && pos > -0.33f)
		return 1;
	if (pos >= 0.33f)
		return 2;

	return -1;
}

int Board::getTileRow(float pos) {
	if (pos >= 0.33f)
		return 0;
	if (pos < 0.33f && pos > -0.33f)
		return 1;
	if (pos <= -0.33f)
		return 2;

	return -1;
}

int Board::getSize() { return tiles.size(); }

short Board::getPlayer() { return curr_player; }

short *Board::getTilesState() { return tiles_state; }
