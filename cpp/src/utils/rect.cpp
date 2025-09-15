#include "rect.h"

void Rect::setupBuffer() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(),
				 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

void Rect::setupVertices() {
	vertices.clear();

	vertices.push_back(top_left);
	vertices.push_back(top_right);
	vertices.push_back(bottom_right);

	vertices.push_back(top_left);
	vertices.push_back(bottom_left);
	vertices.push_back(bottom_right);

	setupBuffer();
}

void Rect::Draw(glm::vec4 color) {
	Shader shader = ResourceManager::GetShader("argb").Use();

	glBindVertexArray(VAO);
	shader.SetVector4f("ourColor", color, false);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Rect::Init(glm::vec3 bottom_left, glm::vec3 bottom_right, glm::vec3 top_left,
				glm::vec3 top_right) {

	this->bottom_left = bottom_left;
	this->bottom_right = bottom_right;
	this->top_left = top_left;
	this->top_right = top_right;

	setupVertices();
}
