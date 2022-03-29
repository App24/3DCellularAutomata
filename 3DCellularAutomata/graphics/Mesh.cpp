#include "Mesh.h"
#include <GL/glew.h>

Mesh::Mesh(const float* buffer, size_t vertices, const int* attrs) : Mesh(buffer, nullptr, vertices, attrs) {
	
}

Mesh::Mesh(const float* buffer, const unsigned int* indicesBuffer, size_t vertices, const int* attrs) : vertices(vertices), indices(0)
{
	vertexSize = 0;
	for (int i = 0; attrs[i]; i++) {
		vertexSize += attrs[i];
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (buffer) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, buffer, GL_STATIC_DRAW);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, 0, {}, GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	if (indicesBuffer) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertices, indicesBuffer, GL_STATIC_DRAW);
		indices = 1;
	}
	else {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, {}, GL_STATIC_DRAW);
	}


	// attributes
	int offset = 0;
	for (int i = 0; attrs[i]; i++) {
		int size = attrs[i];
		glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		offset += size;
	}

	glBindVertexArray(0);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Mesh::reload(const float* buffer, size_t vertices) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, buffer, GL_STATIC_DRAW);
	this->vertices = vertices;
	this->indices = 0;
}

void Mesh::reload(const float* buffer, const unsigned int* indices, size_t vertices)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	this->vertices = vertices;
	this->indices = 1;
}

void Mesh::draw(unsigned int primitive) {
	glBindVertexArray(vao);
	if (!indices)
		glDrawArrays(primitive, 0, vertices);
	else
		glDrawElements(primitive, vertices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}