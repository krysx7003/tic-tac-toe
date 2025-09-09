#pragma once

#include <map>

#include "../thirdparty/glad/glad.h"
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class TextRenderer {
	static TextRenderer *instance;
	unsigned int width;
	unsigned int height;

	TextRenderer() {};
	TextRenderer(unsigned int width, unsigned int height);

  public:
	std::map<char, Character> Characters;
	Shader TextShader;

	static TextRenderer *GetInstance();
	void Load(std::string font, unsigned int fontSize);
	void RenderText(std::string text, int x, int y, float scale, glm::vec3 color = glm::vec3(0.0f));

  private:
	unsigned int VAO, VBO;
};
