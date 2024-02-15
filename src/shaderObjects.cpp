#include <iostream>
#include "shaderObjects.h"

// Vertex Buffer Object
VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
void VBO::Bind() 	{ glBindBuffer(GL_ARRAY_BUFFER, ID); }
void VBO::Unbind()	{ glBindBuffer(GL_ARRAY_BUFFER, 0); }
void VBO::Delete() 	{ glDeleteBuffers(1, &ID); }

// Element(Index) Buffer Object
EBO::EBO(GLuint* indices, GLsizeiptr size) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
void EBO::Bind()	{ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
void EBO::Unbind()	{ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
void EBO::Delete()  { glDeleteBuffers(1, &ID); }

// Vertext Array Object
VAO::VAO() 		  	{ glGenVertexArrays(1, &ID); }
void VAO::LinkAttrib( GLuint attrIdx, GLuint numComponents, \
 						GLenum type, GLsizeiptr stride, void* offset) {
	/* Think of these parameters as descibing how to index
	 into the vertex(VBO) arrays*/
	glVertexAttribPointer(attrIdx, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(attrIdx);
}
void VAO::Bind() 	{ glBindVertexArray(ID);}
void VAO::Unbind() 	{ glBindVertexArray(0); }
void VAO::Delete() 	{ glDeleteVertexArrays(1, &ID); }