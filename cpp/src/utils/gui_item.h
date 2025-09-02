#pragma once

#include <glm/ext/vector_float3.hpp>

#include "../thirdparty/glad/glad.h"
#include <string>
#include <vector>

class Gui_Item {
	glm::vec3 BgColor = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 FgColor = glm::vec3(1.0f, 1.0f, 1.0f);

	GLuint VAO_Bg, VBO_Bg;

  protected:
	float height = 0;
	float width = 0;
	float start_pos_x;
	float start_pos_y;

	std::vector<glm::vec3> vertices;

	void drawSquare();
	void setupBuffer();

  public:
	enum class Type { BUTTON, TEXT_FIELD, MENU };

	Gui_Item(float width, float height, float start_pos_x, float start_pos_y)
		: width(width), height(height), start_pos_x(start_pos_x), start_pos_y(start_pos_y) {

		vertices.push_back({start_pos_x, start_pos_y, 0.0f});
		vertices.push_back({start_pos_x + width, start_pos_y, 0.0f});
		vertices.push_back({start_pos_x + width, start_pos_y - height, 0.0f});

		vertices.push_back({start_pos_x, start_pos_y, 0.0f});
		vertices.push_back({start_pos_x + width, start_pos_y - height, 0.0f});
		vertices.push_back({start_pos_x, start_pos_y - height, 0.0f});
		setupBuffer();
	}
	Gui_Item() {};

	void SetBgColor(float r, float g, float b);
	void SetFgColor(float r, float g, float b);

	float GetHeight();
	float GetWidth();

	virtual void Draw() = 0;
	virtual ~Gui_Item() = default;
};
