#include <glad/glad.h>

class VBO {
public:
	GLuint ID;
	VBO();
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

class EBO {
public:
	GLuint ID;
	EBO();
	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

class VAO {
public:
	GLuint ID;
	VBO vbo;
	EBO ebo;
	
	VAO(GLfloat* vertices, GLsizeiptr verticesSize, GLuint* indices, GLsizeiptr indicessSize);
	void LinkAttrib(GLuint attrIdx, GLuint numComponents, \
		GLenum type, GLsizeiptr stride, void* offset);

	void Bind();
	void Unbind();
	void Delete();
};