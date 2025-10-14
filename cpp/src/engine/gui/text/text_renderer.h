#pragma once

#include <map>

#include "glad/glad.h"
#include <glm/glm.hpp>

#include "core/resources/shader.h"

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

	int window_height;

	std::map<std::string, int> sizes;

	TextRenderer() {};
	TextRenderer(unsigned int width, unsigned int height);
	int textHeigth(std::string text, std::string size);

  public:
	std::map<std::string, std::map<char, Character>> Characters;
	Shader TextShader;

	static TextRenderer *GetInstance();
	int TextWidth(std::string text, std::string size);
	void Load(std::string font, std::string name, unsigned int fontSize);
	void RenderText(std::string text, std::string size, int x, int y, float scale,
					glm::vec3 color = glm::vec3(0.0f));

  private:
	unsigned int VAO, VBO;
};
