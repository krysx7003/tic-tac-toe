#pragma once

#include "core/primitives/rect.h"
#include "core/resource_manager.h"
#include "core/resources/sprite.h"

class Tile {
	int start_pos_x, start_pos_y;
	int width, height;
	Rect background;
	glm::vec4 color;
	glm::vec2 window;

	SpriteRenderer *Renderer;

	glm::vec4 hexToColor(std::string);
	void setUpBackground();

  public:
	class State {
	  public:
		static const char Empty = ' ';
		static const char TakenX = 'X';
		static const char TakenO = 'O';
	};
	char State;

	Tile(int start_pos_x, int start_pos_y, int width, int height)
		: start_pos_x(start_pos_x), start_pos_y(start_pos_y), width(width), height(height) {

		json config = ResourceManager::Config;
		window.x = config["window"]["width"].get<int>();
		window.y = config["window"]["height"].get<int>();

		setUpBackground();

		glm::mat4 projection = glm::ortho(0.0f, 600.0f, 640.0f, 0.0f, -1.0f, 1.0f);
		ResourceManager::GetShader("piece").Use().SetInteger("image", 0);
		ResourceManager::GetShader("piece").SetMatrix4("projection", projection);
		Renderer = new SpriteRenderer(ResourceManager::GetShader("piece"));
	};
	Tile() {};

	void Render();
	bool IsMouseOn(double mouseX, double mouseY);

	void SetColor(std::string);
	void SetState(char state);
};
