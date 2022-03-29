#include "VoxelRenderer.h"
#include <iostream>
#include <GL/glew.h>

#define VERTEX_SIZE (3 + 3 + 1)

#define VERTEX(INDEX, X,Y,Z, LIGHT) buffer[INDEX+0] = (X);\
								  buffer[INDEX+1] = (Y);\
								  buffer[INDEX+2] = (Z);\
								  buffer[INDEX+3] = automata->colors[cellIndex+0];\
								  buffer[INDEX+4] = automata->colors[cellIndex+1];\
								  buffer[INDEX+5] = automata->colors[cellIndex+2];\
								  buffer[INDEX+6] = (LIGHT);\
								  INDEX += VERTEX_SIZE;

int automata_attrs[] = { 3, 3, 1, 0 };

VoxelRenderer::VoxelRenderer(size_t capacity) : capacity(capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
}

VoxelRenderer::~VoxelRenderer() {
	delete[] buffer;
}

inline void _renderBlock(float* buffer, int x, int y, int z, CellularAutomata* automata, size_t& index) {
	int cell = automata->getCell(x, y, z);

	if (!cell)
		return;

	int cellIndex = (cell-1) * 3;

	if (!automata->isCell(x, y + 1, z)) {
		VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, 0.8f);
		VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, 0.8f);
		VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, 0.8f);

		VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, 0.8f);
		VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, 0.8f);
		VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, 0.8f);
	}
	if (!automata->isCell(x, y - 1, z)) {
		VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, 0.1f);
		VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, 0.1f);
		VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, 0.1f);

		VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, 0.1f);
		VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, 0.1f);
		VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, 0.1f);
	}

	if (!automata->isCell(x + 1, y, z)) {

		VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, 0.7f);
		VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, 0.7f);
		VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, 0.7f);

		VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, 0.7f);
		VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, 0.7f);
		VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, 0.7f);
	}
	if (!automata->isCell(x - 1, y, z)) {

		VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, 0.3f);
		VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, 0.3f);
		VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, 0.3f);

		VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, 0.3f);
		VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, 0.3f);
		VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, 0.3f);
	}

	if (!automata->isCell(x, y, z + 1)) {

		VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, 0.9f);
		VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, 0.9f);
		VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, 0.9f);

		VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, 0.9f);
		VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, 0.9f);
		VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, 0.9f);
	}
	if (!automata->isCell(x, y, z - 1)) {
		VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, 0.2f);
		VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, 0.2f);
		VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, 0.2f);

		VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, 0.2f);
		VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, 0.2f);
		VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, 0.2f);
	}
}

Mesh* VoxelRenderer::render(CellularAutomata* automata) {
	size_t index = 0;
	for (int y = 0; y < automata->getHeight(); y++) {
		for (int z = 0; z < automata->getDepth(); z++) {
			for (int x = 0; x < automata->getWidth(); x++) {
				_renderBlock(buffer, x, y, z, automata, index);
			}
		}
	}
	return new Mesh(buffer, index / VERTEX_SIZE, automata_attrs);
}