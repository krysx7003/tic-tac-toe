#include <cstdio>
#include <iostream>

#include <ft2build.h>
#include <glm/gtc/matrix_transform.hpp>
#include FT_FREETYPE_H

#include "resource_manager.h"
#include "text_renderer.h"

TextRenderer *TextRenderer::instance = nullptr;

TextRenderer *TextRenderer::GetInstance() {
	if (TextRenderer::instance == nullptr) {
		json config = ResourceManager::Config;
		int window_width = config["window"]["width"].get<int>();
		int window_height = config["window"]["height"].get<int>();
		instance = new TextRenderer(window_width, window_height);
	}
	return instance;
}

int TextRenderer::TextWidth(std::string text, std::string size) {
	std::string::const_iterator c;
	int sum = 0;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Characters[size][*c];
		sum += (ch.Advance >> 6);
	}
	return sum;
}

TextRenderer::TextRenderer(unsigned int width, unsigned int height) {
	this->TextShader = ResourceManager::LoadShader("text_2d.vs", "text_2d.frag", "text");
	this->TextShader.SetMatrix4(
		"projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f),
		true);
	this->TextShader.SetInteger("text", 0);
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Load("artro.ttf", "small", 24);
	Load("artro.ttf", "medium", 36);
	Load("artro.ttf", "big", 44);
}

void TextRenderer::Load(std::string font, std::string name, unsigned int fontSize) {
	font = "resources/fonts/" + font;

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		printf("ERROR::FREETYPE: Could not init FreeType Library\n");
	}

	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
		printf("ERROR::FREETYPE: Failed to load font\n");
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	Characters[name] = std::map<char, Character>();
	auto &charMap = Characters[name];

	for (GLubyte c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
					 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {texture,
							   glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
							   glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
							   static_cast<unsigned int>(face->glyph->advance.x)};

		charMap.insert(std::pair<char, Character>(c, character));
	}
	sizes.insert(std::pair<std::string, int>(name, fontSize));

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, std::string size, int x, int y, float scale,
							  glm::vec3 color) {
	y = 640 - (y + sizes[size]);

	this->TextShader.Use();
	this->TextShader.SetVector3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Characters[size][*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y + (this->Characters[size]['H'].Bearing.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float vertices[6][4] = {{xpos, ypos + h, 0.0f, 1.0f}, {xpos + w, ypos, 1.0f, 0.0f},
								{xpos, ypos, 0.0f, 0.0f},

								{xpos, ypos + h, 0.0f, 1.0f}, {xpos + w, ypos + h, 1.0f, 1.0f},
								{xpos + w, ypos, 1.0f, 0.0f}};
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
