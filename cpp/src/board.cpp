#include "board.h"

#include "utils/resource_manager.h"
#include "utils/shader.h"
#include "utils/texture.h"

#include <GLFW/glfw3.h>
#include <cstdio>

void Board::Init() {
	config = ResourceManager::Config;

	boardGui = config["gui"];

	float top_menu_height = config["top_menu"]["height"];
	float offset = top_menu_height / 320.0f;

	tiles_num = config["board"]["tiles_num"];
	width = config["board"]["width"];
	tile_size = 2.0f / width;
	tile_width = config["board"]["tile_width"];
	float line_top = tile_width - top_menu_height;
	float line_bot = tile_width + top_menu_height;
	tile_size_px = glm::vec2(tile_width, tile_width);

	tiles_state = new char[tiles_num];

	SetTilesState();

	if (boardGui) {
		for (int row = 0; row < width; row++) {
			for (int col = 0; col < width; col++) {
				addTile(row, col);
				tiles_pos.push_back({col * 200, row * 200 + top_menu_height + 5});
				// The +5 pulled directly out of my ass
				// (I have no idea why does it work, but it does)
			}
		}

		grid.push_back({1.0f / 3, 1.0f - offset, 0.0f});
		grid.push_back({1.0f / 3, -1.0f, 0.0f});
		grid.push_back({-1.0f / 3, 1.0f - offset, 0.0f});
		grid.push_back({-1.0f / 3, -1.0f, 0.0f});

		grid.push_back({1.0f, line_top / 640.0f, 0.0f});
		grid.push_back({-1.0f, line_top / 640.0f, 0.0f});
		grid.push_back({1.0f, -line_bot / 640.0f, 0.0f});
		grid.push_back({-1.0f, -line_bot / 640.0f, 0.0f});

		glm::mat4 projection = glm::ortho(0.0f, 600.0f, 640.0f, 0.0f, -1.0f, 1.0f);
		ResourceManager::GetShader("piece").Use().SetInteger("image", 0);
		ResourceManager::GetShader("piece").SetMatrix4("projection", projection);
		Renderer = new SpriteRenderer(ResourceManager::GetShader("piece"));
	}
}

Board::Board() { SetTilesState(); }

void Board::SetTilesState() {
	for (int row = 0; row < width; row++) {
		for (int col = 0; col < width; col++) {
			tiles_state[(row * 3) + col] = TileState::Empty;
		}
	}
}

void Board::Render() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < tiles_num; i++) {
		if (tiles_state[i] == TileState::Empty) {
			continue;
		}
		Texture2D texture;
		if (tiles_state[i] == TileState::TakenO) {
			texture = ResourceManager::GetTexture("O");

		} else {
			texture = ResourceManager::GetTexture("X");
		}
		Renderer->DrawSprite(texture, tiles_pos[i], tile_size_px, 0.0f,
							 glm::vec3(0.0f, 0.0f, 0.0f));
	}

	// Shader shader = ResourceManager::GetShader("argb").Use();
	//
	// glBindVertexArray(VAO_tiles);
	// for (int i = 0; i < BOARD_SIZE; i++) {
	// 	shader.SetVector3f("ourColor", 1.0f, 1.0f, 1.0f, false);
	// 	glDrawArrays(GL_TRIANGLES, i * 6, 6);
	// }

	Shader line_shader = ResourceManager::GetShader("argb").Use();

	line_shader.SetVector4f("ourColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), false);
	glBindVertexArray(VAO_lines);
	glLineWidth(10.0f);
	glDrawArrays(GL_LINES, 0, grid.size());
}

void Board::RenderWin(Texture2D win_texture, glm::vec2 texture_pos, glm::vec2 texture_size) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Renderer->DrawSprite(win_texture, texture_pos, texture_size, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Board::SetupBuffers() {
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
	GLfloat left = -1.0f + tile_size * col;
	GLfloat right = left + tile_size;
	GLfloat top = 1.0f - tile_size * row;
	GLfloat bottom = top - tile_size;

	tiles.push_back({left, top, 0.0f});
	tiles.push_back({right, top, 0.0f});
	tiles.push_back({right, bottom, 0.0f});

	tiles.push_back({left, top, 0.0f});
	tiles.push_back({right, bottom, 0.0f});
	tiles.push_back({left, bottom, 0.0f});
}

bool Board::TakeTile(int pos, char player) {
	if (tiles_state[pos] != TileState::Empty) {
		return false;
	}
	if (player == Player::O) {
		tiles_state[pos] = TileState::TakenO;
	} else {
		tiles_state[pos] = TileState::TakenX;
	}
	return true;
}

int Board::GetTileCol(float pos) {
	if (pos <= -0.33f)
		return 0;
	if (pos < 0.33f && pos > -0.33f)
		return 1;
	if (pos >= 0.33f)
		return 2;

	return -1;
}

int Board::GetTileRow(float pos) {
	if (pos >= 0.33f)
		return 0;
	if (pos < 0.33f && pos > -0.33f)
		return 1;
	if (pos <= -0.33f)
		return 2;

	return -1;
}

int Board::GetSize() { return tiles.size(); }

char *Board::GetTilesState() { return tiles_state; }
