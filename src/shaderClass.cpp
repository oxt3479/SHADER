#include "shaderClass.h"

std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	std::cout << "Error loading shader from :" << filename << std::endl;
	throw(errno);
}

ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");

	ID = glCreateProgram();
	//glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkLinkingErrors(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/* Now in the program the original shaders are unneeded*/
}
void ShaderProgram::Activate() { glUseProgram(ID); }
void ShaderProgram::Delete() { glDeleteProgram(ID); }
void ShaderProgram::checkCompileErrors(unsigned int shader, const char* type) {
    GLint hasCompiled;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << infoLog << std::endl;
    }
}
void ShaderProgram::checkLinkingErrors(unsigned int program) {
    GLint hasLinked;
    char infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &hasLinked);
    if (hasLinked == GL_FALSE) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cout << "SHADER_LINKING_ERROR for: PROGRAM\n" << infoLog << std::endl;
    }
}
