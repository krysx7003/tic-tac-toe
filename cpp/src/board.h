#pragma once

#include "player.h"
#include "tile.h"
#include "utils/line.h"
#include "utils/sprite.h"

#include "thirdparty/glad/glad.h"
#include "thirdparty/json.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <istream>
#include <vector>

using json = nlohmann::json;
using namespace std;

class Board {
	json config;

	float tile_size;
	float tile_width;
	glm::vec2 tile_size_px;

	bool boardGui = false;
	bool prettyPrint = true;

	SpriteRenderer *Renderer;
	Line lines;

  public:
	Tile *Tiles;
	int tiles_num;
	int width;

	Board() { Tiles = new Tile[tiles_num]; };
	void Init();

	void Render();
	void Print(bool tooltip);
	void RenderWin(Texture2D win_texture, glm::vec2 texture_pos, glm::vec2 texture_size);
	bool TakeTile(int pos, char player);
	void RestetTiles();

	std::vector<char> GetTilesState();

	int TileUnderMouse(double x, double y);
};
