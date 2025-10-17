#include "board.h"

#include "core/resources/shader.h"
#include "core/resources/texture.h"
#include "player_manager.h"

#include <GLFW/glfw3.h>
#include <cstdio>

void Board::Init() {
	config = ResourceManager::Config;

	boardGui = config["gui"];

	float top_menu_height = config["top_menu"]["height"];

	float window_height = config["window"]["height"].get<int>();
	float window_width = config["window"]["width"].get<int>();
	tiles_num = config["board"]["tiles_num"];
	width = config["board"]["width"];
	tile_width = config["board"]["tile_width"];

	Tiles = new Tile[tiles_num];

	for (int row = 0; row < width; row++) {
		for (int col = 0; col < width; col++) {
			int id = col + row * 3;
			int x = col * tile_width;
			int y = row * tile_width;
			Tiles[id] = Tile(x, y, tile_width, tile_width);

			Tiles[id].SetId(id);
			Tiles[id].State = Tile::State::Empty;
		}
	}

	float offset = top_menu_height / (window_height / 2);
	float line_top = tile_width - top_menu_height;
	float line_bot = tile_width + top_menu_height;

	std::vector<glm ::vec3> grid;
	grid.push_back({1.0f / 3, 1.0f - offset, 0.0f});
	grid.push_back({1.0f / 3, -1.0f, 0.0f});
	grid.push_back({-1.0f / 3, 1.0f - offset, 0.0f});
	grid.push_back({-1.0f / 3, -1.0f, 0.0f});

	grid.push_back({1.0f, line_top / window_height, 0.0f});
	grid.push_back({-1.0f, line_top / window_height, 0.0f});
	grid.push_back({1.0f, -line_bot / window_height, 0.0f});
	grid.push_back({-1.0f, -line_bot / window_height, 0.0f});

	lines.Init(grid);
	lines.SetWidth(10.0f);

	glm::mat4 projection = glm::ortho(0.0f, window_width, 0.0f, window_height, -1.0f, 1.0f);
	ResourceManager::GetShader("piece").Use().SetInteger("image", 0);
	ResourceManager::GetShader("piece").SetMatrix4("projection", projection);
	Renderer = new SpriteRenderer(ResourceManager::GetShader("piece"));
}

void Board::Render() {
	for (int i = 0; i < tiles_num; i++) {
		Tiles[i].Render();
	}
	lines.Draw(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Board::RenderWin(Texture2D win_texture, glm::vec2 texture_pos, glm::vec2 texture_size) {
	Renderer->DrawSprite(win_texture, texture_pos, texture_size, 0.0f,
						 glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
}

bool Board::TakeTile(int pos) {
	if (Tiles[pos].State != Tile::State::Empty) {
		return false;
	}

	if (PlayerManager::Curr_player == Player::O) {
		Tiles[pos].State = Tile::State::TakenO;
	} else {
		Tiles[pos].State = Tile::State::TakenX;
	}
	return true;
}

void Board::RestetTiles() {
	for (int i = 0; i < tiles_num; i++) {
		Tiles[i].State = Tile::State::Empty;
	}
}

std::vector<char> Board::GetTilesState() {
	std::vector<char> tab(tiles_num);
	for (int i = 0; i < tiles_num; i++) {
		tab[i] = Tiles[i].State;
	}
	return tab;
}

std::string Board::GetState() {
	stringstream ss;
	for (int i = 0; i < tiles_num; i++) {
		ss << "\\" << Tiles[i].State;
	}

	ss << "\\";
	return ss.str();
}
