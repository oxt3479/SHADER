#ifndef SHADERCLASS_H
#define SHADERCLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const std::string& filename, const std::string& parentPath);

class ShaderProgram {
public:
	GLuint ID;
	std::string vertex_path, fragment_path;	
	ShaderProgram(	const std::string& vertexFile,\
					const std::string& fragmentFile, bool load);
	
	void Load();
	void Activate();
	void Delete();
	void addRGBTexture( const char* samplerName, \
						const char* imageFile, int texture_idx);
	void addDepthTexture( const char* samplerName, \
						const char* imageFile, int texture_idx);

private:
	void checkCompileErrors(unsigned int shader, const char* type);
	void checkLinkingErrors(unsigned int program);
};

#endif