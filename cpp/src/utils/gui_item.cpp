#include "gui_item.h"
#include "shader.h"

void Gui_Item::drawSquare() {
	Shader shader = ResourceManager::GetShader("tile").Use();

	glBindVertexArray(VAO_Bg);
	shader.SetVector3f("ourColor", BgColor, false);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Gui_Item::setupBuffer() {
	glGenVertexArrays(1, &VAO_Bg);
	glGenBuffers(1, &VBO_Bg);

	glBindVertexArray(VAO_Bg);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Bg);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(),
				 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

void Gui_Item::setupVertices() {
	vertices.erase(vertices.begin(), vertices.end());

	float fWidth = (width / window_width);
	float fHeight = (height / window_height);
	float fStart_pos_x = (start_pos_x / window_width) - 1;
	float fStart_pos_y = (start_pos_y / window_height) - 1;

	vertices.push_back({fStart_pos_x, fStart_pos_y, 0.0f});
	vertices.push_back({fStart_pos_x + fWidth, fStart_pos_y, 0.0f});
	vertices.push_back({fStart_pos_x + fWidth, fStart_pos_y + fHeight, 0.0f});

	vertices.push_back({fStart_pos_x, fStart_pos_y, 0.0f});
	vertices.push_back({fStart_pos_x + fWidth, fStart_pos_y + fHeight, 0.0f});
	vertices.push_back({fStart_pos_x, fStart_pos_y + fHeight, 0.0f});

	setupBuffer();
}

void Gui_Item::SetBgColor(float r, float g, float b) { BgColor = glm::vec3(r, g, b); }

void Gui_Item::SetFgColor(float r, float g, float b) { FgColor = glm::vec3(r, g, b); }

int Gui_Item::GetHeight() { return height + padding_top; }
int Gui_Item::GetWidth() { return width; }
int Gui_Item::GetPadding() { return padding_top; }

void Gui_Item::SetStartY(int start_y) {
	this->start_pos_y = start_y;
	setupVertices();
}

void Gui_Item::SetPadding(int top) { this->padding_top = top; }

int Gui_Item::GetStartY() { return start_pos_y; }
