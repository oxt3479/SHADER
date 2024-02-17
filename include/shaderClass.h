#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class ShaderProgram {
public:
	GLuint ID;
	ShaderProgram(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();

private:
	void checkCompileErrors(unsigned int shader, const char* type);
	void checkLinkingErrors(unsigned int program);
};