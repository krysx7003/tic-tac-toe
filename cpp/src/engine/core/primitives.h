#pragma once

#include "glad/glad.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <vector>

class Primitives {
  protected:
	GLuint VAO, VBO;

	std::vector<glm::vec3> vertices;

	virtual void setupBuffer() = 0;
	virtual void setupVertices() = 0;

  public:
	Primitives() {};

	virtual ~Primitives() = default;
};
