#include "board.h"

#include "utils/shader.h"
#include "utils/texture.h"

#include <GLFW/glfw3.h>
#include <cstdio>

void Board::Init() {
	config = ResourceManager::Config;

	boardGui = config["gui"];

	float top_menu_height = config["top_menu"]["height"];

	tiles_num = config["board"]["tiles_num"];
	width = config["board"]["width"];
	tile_size = 2.0f / width;
	tile_width = config["board"]["tile_width"];
	tile_size_px = glm::vec2(tile_width, tile_width);

	Tiles_n = new Tile[tiles_num];

	if (boardGui) {
		for (int row = 0; row < width; row++) {
			for (int col = 0; col < width; col++) {
				int id = col + row * 3;
				int x = col * tile_width;
				int y = row * tile_width + top_menu_height + 5;
				Tiles_n[id] = Tile(x, y, tile_width, tile_width);
				// The +5 pulled directly out of my ass
				// (I have no idea why does it work, but it does)
			}
		}

		float offset = top_menu_height / 320.0f;
		float line_top = tile_width - top_menu_height;
		float line_bot = tile_width + top_menu_height;

		std::vector<glm ::vec3> grid;
		grid.push_back({1.0f / 3, 1.0f - offset, 0.0f});
		grid.push_back({1.0f / 3, -1.0f, 0.0f});
		grid.push_back({-1.0f / 3, 1.0f - offset, 0.0f});
		grid.push_back({-1.0f / 3, -1.0f, 0.0f});

		grid.push_back({1.0f, line_top / 640.0f, 0.0f});
		grid.push_back({-1.0f, line_top / 640.0f, 0.0f});
		grid.push_back({1.0f, -line_bot / 640.0f, 0.0f});
		grid.push_back({-1.0f, -line_bot / 640.0f, 0.0f});

		lines.Init(grid);
		lines.SetWidth(10.0f);

		glm::mat4 projection = glm::ortho(0.0f, 600.0f, 640.0f, 0.0f, -1.0f, 1.0f);
		ResourceManager::GetShader("piece").Use().SetInteger("image", 0);
		ResourceManager::GetShader("piece").SetMatrix4("projection", projection);
		Renderer = new SpriteRenderer(ResourceManager::GetShader("piece"));
	}
}

void Board::Render() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < tiles_num; i++) {
		Tiles_n[i].Render();
	}
	lines.Draw(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Board::RenderWin(Texture2D win_texture, glm::vec2 texture_pos, glm::vec2 texture_size) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Renderer->DrawSprite(win_texture, texture_pos, texture_size, 0.0f,
						 glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
}

bool Board::TakeTile(int pos, char player) {
	if (Tiles_n[pos].State != Tile::State::Empty) {
		return false;
	}
	if (player == Player::O) {
		Tiles_n[pos].State = Tile::State::TakenO;
	} else {
		Tiles_n[pos].State = Tile::State::TakenX;
	}
	return true;
}

void Board::RestetTiles() {
	for (int i = 0; i < tiles_num; i++) {
		Tiles_n[i].State = Tile::State::Empty;
	}
}

int Board::TileUnderMouse(double x, double y) {
	int i = 0;
	for (; i < tiles_num; i++) {
		if (Tiles_n[i].IsMouseOn(x, y))
			break;
	}

	return i;
}

std::vector<char> Board::GetTilesState() {
	std::vector<char> tab(tiles_num);
	for (int i = 0; i < tiles_num; i++) {
		tab[i] = Tiles_n[i].State;
	}
	return tab;
}
