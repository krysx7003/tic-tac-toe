#include "gui_item.h"
#include "shader.h"

void Gui_Item::drawSquare(glm::vec4 color) {
	Shader shader = ResourceManager::GetShader("argb").Use();

	glBindVertexArray(VAO_Bg);
	shader.SetVector4f("ourColor", color, false);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Gui_Item::drawOutline() {
	Shader shader = ResourceManager::GetShader("argb").Use();

	glBindVertexArray(VAO_lines);
	shader.SetVector4f("ourColor", OutColor, false);

	glLineWidth(outline_width);
	if (outlines[0]) {
		glDrawArrays(GL_LINES, 0, 2);
	}
	if (outlines[3]) {
		glDrawArrays(GL_LINES, 3, 2);
	}

	glLineWidth(outline_width);
	if (outlines[1]) {
		glDrawArrays(GL_LINES, 1, 2);
	}
	if (outlines[2]) {
		glDrawArrays(GL_LINES, 2, 2);
	}
}

void Gui_Item::setupBuffer() {
	// ============== Lines VAO ============
	glGenVertexArrays(1, &VAO_lines);
	glGenBuffers(1, &VBO_lines);

	glBindVertexArray(VAO_lines);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_lines);
	glBufferData(GL_ARRAY_BUFFER, outline_vert.size() * sizeof(glm::vec3), outline_vert.data(),
				 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// ============== Background VAO ============
	glGenVertexArrays(1, &VAO_Bg);
	glGenBuffers(1, &VBO_Bg);

	glBindVertexArray(VAO_Bg);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Bg);
	glBufferData(GL_ARRAY_BUFFER, background_vert.size() * sizeof(glm::vec3),
				 background_vert.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

void Gui_Item::setupVertices() {
	background_vert.erase(background_vert.begin(), background_vert.end());
	outline_vert.erase(outline_vert.begin(), outline_vert.end());

	float fWidth = (width / window_width);
	float fHeight = (height / window_height);
	float fStart_pos_x = (start_pos_x / window_width) - 1;
	float fStart_pos_y = (start_pos_y / window_height) - 1;

	glm::vec3 bottom_left = {fStart_pos_x, fStart_pos_y, 0.0f};
	glm::vec3 bottom_right = {fStart_pos_x + fWidth, fStart_pos_y, 0.0f};
	glm::vec3 top_left = {fStart_pos_x, fStart_pos_y + fHeight, 0.0f};
	glm::vec3 top_right = {fStart_pos_x + fWidth, fStart_pos_y + fHeight, 0.0f};

	background_vert.push_back(top_left);
	background_vert.push_back(top_right);
	background_vert.push_back(bottom_right);

	background_vert.push_back(top_left);
	background_vert.push_back(bottom_left);
	background_vert.push_back(bottom_right);

	outline_vert.push_back(top_left);
	outline_vert.push_back(top_right);
	outline_vert.push_back(bottom_right);
	outline_vert.push_back(bottom_left);
	outline_vert.push_back(top_left);

	setupBuffer();
}

glm::vec4 Gui_Item::hexToColor(std::string color) {
	if (color[0] != '#') {
		printf("ERROR::GUI_ITEM: Color is invalid\n");
		return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	float r = std::stoi(color.substr(1, 2), nullptr, 16) / 255.0f;
	float g = std::stoi(color.substr(3, 2), nullptr, 16) / 255.0f;
	float b = std::stoi(color.substr(5, 2), nullptr, 16) / 255.0f;
	float a = 1.0f;
	if (color.size() == 9) {
		a = std::stoi(color.substr(7, 2), nullptr, 16) / 255.0f;
	}

	return glm::vec4(r, g, b, a);
}

void Gui_Item::SetBgColor(std::string color) { BgColor = hexToColor(color); }
void Gui_Item::SetBgColor(glm::vec4 color) { this->BgColor = color; }

void Gui_Item::SetFgColor(std::string color) {
	glm::vec4 rgba = hexToColor(color);
	FgColor = glm::vec3(rgba.r, rgba.g, rgba.b);
}
void Gui_Item::SetFgColor(glm::vec3 color) { this->FgColor = color; }

void Gui_Item::SetOutColor(std::string color) { OutColor = hexToColor(color); }
void Gui_Item::SetOutColor(glm::vec4 color) { this->OutColor = color; }

void Gui_Item::SetOutline(glm::bvec4 outline) { this->outlines = outline; }
void Gui_Item::SetOutlineWidth(float width) { this->outline_width = width; }

int Gui_Item::GetHeight() {
	if (this->Visible) {
		return height + padding_top;
	} else {
		return 0;
	}
}

int Gui_Item::GetWidth() { return width; }
int Gui_Item::GetPadding() { return padding_top; }

void Gui_Item::SetVisibility(bool visible) { this->Visible = visible; }

void Gui_Item::SetHeight(int new_height) {
	this->height = new_height;
	setupVertices();
}

void Gui_Item::SetStartY(int start_y) {
	this->start_pos_y = start_y;
	setupVertices();
}

void Gui_Item::SetPadding(int top) { this->padding_top = top; }
void Gui_Item::SetId(int id) { this->Menu_id = id; }

int Gui_Item::GetStartY() { return start_pos_y; }
