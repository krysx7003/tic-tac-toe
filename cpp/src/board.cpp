#include "board.h"

#include "player_manager.h"
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
	prettyPrint = config["pretty_print"];

	Tiles = new Tile[tiles_num];

	if (boardGui) {
		for (int row = 0; row < width; row++) {
			for (int col = 0; col < width; col++) {
				int id = col + row * 3;
				int x = col * tile_width;
				int y = row * tile_width + top_menu_height + 5;
				Tiles[id] = Tile(x, y, tile_width, tile_width);
				// The +5 pulled directly out of my ass
				// (I have no idea why does it work, but it does)

				Tiles[id].State = Tile::State::Empty;
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
	} else {
		for (int i = 0; i < tiles_num; i++) {
			Tiles[i] = Tile();
			Tiles[i].State = Tile::State::Empty;
		}
	}
}

void Board::Render() {
	for (int i = 0; i < tiles_num; i++) {
		Tiles[i].Render();
	}
	lines.Draw(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Board::Print(bool tooltip) {
	if (!prettyPrint) {
		printf("%s\n", GetState().c_str());
		return;
	}

	printf("\n-------------\n");
	printf("| %c | %c | %c | ", Tiles[0].State, Tiles[1].State, Tiles[2].State);
	if (tooltip)
		printf("| 0 | 1 | 2 |");
	printf("\n----------\n");

	printf("| %c | %c | %c | ", Tiles[3].State, Tiles[4].State, Tiles[5].State);
	if (tooltip)
		printf("| 3 | 4 | 5 |");
	printf("\n----------\n");

	printf("| %c | %c | %c | ", Tiles[6].State, Tiles[7].State, Tiles[8].State);
	if (tooltip)
		printf("| 6 | 7 | 8 |");
	printf("\n----------\n");
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

int Board::TileUnderMouse(double x, double y) {
	int i = 0;
	for (; i < tiles_num; i++) {
		if (Tiles[i].IsMouseOn(x, y))
			break;
	}

	return i;
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
