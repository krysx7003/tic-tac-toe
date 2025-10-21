#pragma once

#include "core/primitives/line.h"
#include "core/resources/sprite.h"
#include "tile.h"

#include "glad/glad.h"
#include "json.hpp"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;

class Board {
	json config;

	float tile_width;

	SpriteRenderer *Renderer;
	Line lines;

  public:
	Tile *Tiles;
	int tiles_num;
	int width;

	Board() { Tiles = new Tile[tiles_num]; };
	void Init();

	void Render();
	void RenderWin(Texture2D win_texture, glm::vec2 texture_pos, glm::vec2 texture_size);
	bool TakeTile(int pos);
	void RestetTiles();

	std::vector<char> GetTilesState();

	int TileUnderMouse(double x, double y);

	std::string GetState();
};
