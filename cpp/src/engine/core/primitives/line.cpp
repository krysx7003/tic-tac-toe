#include "line.h"
#include "core/resource_manager.h"

void Line::setupBuffer() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(),
				 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

void Line::setupVertices() { setupBuffer(); }

void Line::Init(std::vector<glm::vec3> lines) {
	vertices.clear();
	vertices = lines;
	setupVertices();
}

void Line::Draw(glm::vec4 color, glm::bvec4 outlines) {
	Shader shader = ResourceManager::GetShader("argb").Use();

	glBindVertexArray(VAO);
	shader.SetVector4f("ourColor", color, false);

	glLineWidth(width);

	if (outlines[0]) {
		glDrawArrays(GL_LINES, 0, 2);
	}
	if (outlines[1]) {
		glDrawArrays(GL_LINES, 1, 2);
	}
	if (outlines[2]) {
		glDrawArrays(GL_LINES, 2, 2);
	}
	if (outlines[3]) {
		glDrawArrays(GL_LINES, 3, 2);
	}
}

void Line::Draw(glm::vec4 color) {
	Shader shader = ResourceManager::GetShader("argb").Use();

	glBindVertexArray(VAO);
	shader.SetVector4f("ourColor", color, false);

	glLineWidth(width);
	glDrawArrays(GL_LINES, 0, vertices.size());
}

void Line::SetWidth(float width) { this->width = width; }
