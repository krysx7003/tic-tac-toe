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

	std::vector<glm ::vec3> tiles;
	std::vector<glm ::vec2> tiles_pos;
	GLuint VAO_tiles, VBO_tiles;
	bool boardGui = false;

	SpriteRenderer *Renderer;
	Line lines;

	void addTile(int row, int col);

  public:
	int tiles_num;
	int width;
	char *tiles_state;

	Board();
	void Init();

	void SetupBuffers();
	void Render();
	void RenderWin(Texture2D win_texture, glm::vec2 texture_pos, glm::vec2 texture_size);
	bool TakeTile(int pos, char player);
	void SetTilesState();

	char *GetTilesState();
	int GetSize();
	int GetTileCol(float pos);
	int GetTileRow(float pos);
};
