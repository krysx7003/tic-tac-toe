#include "resource_manager.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;
json ResourceManager::Config;

Shader ResourceManager::LoadShader(std::string vShaderFile, std::string fShaderFile,
								   std::string name) {
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name) { return Shaders[name]; }

Texture2D ResourceManager::LoadTexture(std::string file, bool alpha, std::string name) {
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name) { return Textures[name]; }

json ResourceManager::LoadConfig() {
	Config = loadConfigFromFile();
	return Config;
}

void ResourceManager::Clear() {
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);

	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(std::string vShaderFile, std::string fShaderFile) {
	std::string vShaderPath = ShadersDir + vShaderFile;
	std::string fShaderPath = ShadersDir + fShaderFile;

	std::string vertexCode;
	std::string fragmentCode;
	try {
		std::ifstream vertexShaderFile(vShaderPath);
		std::ifstream fragmentShaderFile(fShaderPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	} catch (std::exception e) {
		printf("ERROR::SHADER: Failed to read shader files\n");
	}
	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();

	Shader shader;
	shader.Compile(vShaderCode, fShaderCode);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(std::string file, bool alpha) {
	std::string TexturePath = TexturesDir + file;

	Texture2D texture;
	if (alpha) {
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}

	int width, height, nrChannels;
	unsigned char *data = stbi_load(TexturePath.c_str(), &width, &height, &nrChannels, 0);

	texture.Generate(width, height, data);

	stbi_image_free(data);
	return texture;
}

json ResourceManager::loadConfigFromFile() {
	std::ifstream file(ConfigPath);
	json config;
	if (file.is_open()) {
		file >> config;
		return config;
	} else {
		printf("ERROR::CONFIG: Failed to open cofig file\n");
		exit(-1);
	}
}
