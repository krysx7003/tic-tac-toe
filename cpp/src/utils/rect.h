#pragma once

#include "primitives.h"

class Rect : public Primitives {

	glm::vec3 bottom_left;
	glm::vec3 bottom_right;
	glm::vec3 top_left;
	glm::vec3 top_right;

	void setupBuffer() override;
	void setupVertices() override;

  public:
	void Init(glm::vec3 bottom_left, glm::vec3 bottom_right, glm::vec3 top_left,
			  glm::vec3 top_right);
	void Draw(glm::vec4 color);
};
