#include "shaderClass.h"

std::string get_file_contents(const std::string& filename, const std::string& parentPath = "") {
    // This function recursively builds the shader file accounting for include statements for
    // other shader files.
    std::string filePath;
    if(parentPath.empty()) {
        filePath = filename;
    } else {
        filePath = parentPath + "/" + filename;
    }

    std::ifstream in(filePath, std::ios::binary);
    if (in) {
        std::string contents, line;
        while (getline(in, line)) {
            if (line.find("#include") != std::string::npos) {
                std::size_t start = line.find(" ") + 1;
                std::size_t end = line.find_last_of("\n") - start;
                std::string includeFile = line.substr(start, end);

                // Construct the path to the included file relative to the current file
                std::string includePath = filePath.substr(0, filePath.find_last_of("/\\") + 1);

                // Recursively process the included file
                contents += get_file_contents(includeFile.c_str(), includePath) + "\n";
            } else {
                contents += line + "\n";
            }
        }
        in.close();
        return contents;
    } else {
        std::cerr << "Error loading shader from: " << filePath << std::endl;
        throw(errno);
    }
}

ShaderProgram::ShaderProgram(   const std::string& vertexFile, 
                                const std::string& fragmentFile, bool load)
        : vertex_path(vertexFile), fragment_path(fragmentFile) {
    if ( load ) { Load(); };
}
void ShaderProgram::Load() {
	std::string vertexCode = get_file_contents(vertex_path, "");
	std::string fragmentCode = get_file_contents(fragment_path, "");

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
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkLinkingErrors(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
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