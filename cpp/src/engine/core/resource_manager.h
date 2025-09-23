#pragma once

#include <map>
#include <string>

#include "glad/glad.h"
#include "json.hpp"

#include "core/resources/shader.h"
#include "core/resources/texture.h"

using json = nlohmann::json;

class ResourceManager {
  public:
	static inline std::string ShadersDir = "resources/shaders/";
	static inline std::string TexturesDir = "resources/textures/";
	static inline std::string ConfigPath = "resources/config.json";

	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;
	static json Config;

	static Shader LoadShader(std::string vShaderFile, std::string fShaderFile, std::string name);
	static Shader GetShader(std::string name);

	static Texture2D LoadTexture(std::string file, bool alpha, std::string name);
	static Texture2D GetTexture(std::string name);

	static json LoadConfig();

	static void Clear();

  private:
	ResourceManager() {}

	static Shader loadShaderFromFile(std::string vShaderFile, std::string fShaderFile);
	static Texture2D loadTextureFromFile(std::string file, bool alpha);
	static json loadConfigFromFile();
};
