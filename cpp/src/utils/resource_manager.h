#include <map>
#include <string>

#include "../thirdparty/glad/glad.h"
#include "../thirdparty/json.hpp"

#include "shader.h"
#include "texture.h"

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
