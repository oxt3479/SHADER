#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <unordered_map>

std::string get_file_contents(const char* filename);

class ShaderProgram {
public:
	GLuint ID;
	ShaderProgram(const char* vertexFile, const char* fragmentFile);

    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, int value);

	void Activate();
	void Delete();

private:
    std::unordered_map<std::string, GLint> uniformLocationCache; // Cache for uniform locations
    GLint getUniformLocation(const std::string& name);
	
	void checkCompileErrors(unsigned int shader, const char* type);
	void checkLinkingErrors(unsigned int program);
};