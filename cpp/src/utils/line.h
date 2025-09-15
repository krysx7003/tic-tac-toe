#pragma once

#include "primitives.h"

class Line : public Primitives {
	float width = 2.0f;

	void setupBuffer() override;
	void setupVertices() override;

  public:
	void Init(std::vector<glm::vec3> lines);
	void Draw(glm::vec4 color);
	void Draw(glm::vec4 color, glm::bvec4 outlines);

	void SetWidth(float width);
};
