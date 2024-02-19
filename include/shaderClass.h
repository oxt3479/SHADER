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
	std::string vertex_path, fragment_path;	
	ShaderProgram(	const std::string& vertexFile,\
					const std::string& fragmentFile, bool load);
	
	void Load();
	void Activate();
	void Delete();

private:
	void checkCompileErrors(unsigned int shader, const char* type);
	void checkLinkingErrors(unsigned int program);
};