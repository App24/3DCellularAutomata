#pragma once

#include <stdlib.h>

class Mesh;
class CellularAutomata;

class VoxelRenderer {
	float* buffer;
	size_t capacity;
public:
	VoxelRenderer(size_t capacity);
	~VoxelRenderer();

	Mesh* render(CellularAutomata* automata);
};