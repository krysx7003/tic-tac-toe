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

void Gui_Item::SetBgColor(float r, float g, float b) { BgColor = glm::vec3(r, g, b); }

void Gui_Item::SetFgColor(float r, float g, float b) { FgColor = glm::vec3(r, g, b); }

float Gui_Item::GetHeight() { return height; }
float Gui_Item::GetWidth() { return width; }
