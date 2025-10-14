#pragma once

#include <cstdio>
#include <glm/ext/vector_float3.hpp>

#include "core/primitives/line.h"
#include "core/primitives/rect.h"
#include "core/resource_manager.h"
#include "glad/glad.h"
#include <string>

class Gui_Item {

  protected:
	glm::vec3 FgColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 OutColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::bvec4 outlines = glm::vec4(false);

	Rect background;
	Line outline;

	int height = 0;
	int width = 0;
	int start_pos_x;
	int start_pos_y;
	float window_width;
	float window_height;
	int padding_top = 15;

	virtual void updatePos();
	glm::vec4 hexToColor(std::string color);

  public:
	glm::vec4 BgColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	int Menu_id;
	std::string item_text;
	bool Visible = true;
	enum class Type { BUTTON, TEXT_FIELD, MENU, DROPDOWN };
	Type ItemType;

	Gui_Item(int width, int height, int start_pos_x, int start_pos_y)
		: width(width), height(height), start_pos_x(start_pos_x), start_pos_y(start_pos_y) {

		json config = ResourceManager::Config;
		window_width = config["window"]["width"].get<int>();
		window_height = config["window"]["height"].get<int>();

		SetBgColor("#282A2E");
		SetFgColor("#C5C8C6");
		SetOutColor("#0a0a0b");

		updatePos();
	}
	Gui_Item() {};

	virtual void SetBgColor(std::string color);
	virtual void SetBgColor(glm::vec4 color);
	virtual void SetFgColor(std::string color);
	virtual void SetFgColor(glm::vec3 color);
	void SetOutColor(std::string color);
	void SetOutColor(glm::vec4 color);

	void SetType(Type type);
	void SetVisibility(bool visible);
	void SetVisibility();
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
