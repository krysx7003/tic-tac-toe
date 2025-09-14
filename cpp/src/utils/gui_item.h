#pragma once

#include <cstdio>
#include <glm/ext/vector_float3.hpp>

#include "../thirdparty/glad/glad.h"
#include "resource_manager.h"
#include <string>
#include <vector>

class Gui_Item {

	GLuint VAO_Bg, VBO_Bg;
	GLuint VAO_lines, VBO_lines;

  protected:
	glm::vec4 BgColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec3 FgColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 OutColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::bvec4 outlines = glm::vec4(false);

	float outline_width = 1.0f;

	int height = 0;
	int width = 0;
	int start_pos_x;
	int start_pos_y;
	float window_width;
	float window_height;
	int padding_top = 15;

	std::vector<glm::vec3> background_vert;
	std::vector<glm::vec3> outline_vert;

	void drawSquare(glm::vec4 color);
	void drawOutline();

	void setupBuffer();
	void setupVertices();
	glm::vec4 hexToColor(std::string color);

  public:
	int Menu_id;
	std::string item_text;
	bool Visible = true;
	enum class Type { BUTTON, TEXT_FIELD, MENU };

	Gui_Item(int width, int height, int start_pos_x, int start_pos_y)
		: width(width), height(height), start_pos_x(start_pos_x), start_pos_y(start_pos_y) {

		json config = ResourceManager::Config;
		window_width = config["window"]["width"].get<int>() / 2.0f;
		window_height = config["window"]["height"].get<int>() / 2.0f;

		setupVertices();
	}
	Gui_Item() {};

	void SetBgColor(std::string color);
	void SetBgColor(glm::vec4 color);
	void SetFgColor(std::string color);
	void SetFgColor(glm::vec3 color);
	void SetOutColor(std::string color);
	void SetOutColor(glm::vec4 color);

	void SetVisibility(bool visible);
	void SetPadding(int top);
	void SetOutline(glm::bvec4 outline);
	void SetOutlineWidth(float width);

	int GetHeight();
	int GetStartY();
	int GetWidth();
	int GetPadding();

	void SetHeight(int new_height);
	void SetId(int id);

	virtual void SetStartY(int start_y);
	virtual void Draw() = 0;
	virtual ~Gui_Item() = default;
};
