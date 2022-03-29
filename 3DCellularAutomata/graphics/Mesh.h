#pragma once

#include <stdlib.h>

class Mesh {
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	size_t vertices;
	size_t indices;
	size_t vertexSize;
public:
	Mesh(const float* buffer, size_t vertices, const int* attrs);
	Mesh(const float* buffer, const unsigned int* indicesBuffer, size_t vertices, const int* attrs);
	~Mesh();

	void reload(const float* buffer, size_t vertices);
	void reload(const float* buffer, const unsigned int* indices, size_t vertices);
	void draw(unsigned int primitive);
};