#pragma once

#include <stdlib.h>
#include "../CellularAutomata.h"
#include "Mesh.h"

class VoxelRenderer {
	float* buffer;
	size_t capacity;
public:
	VoxelRenderer(size_t capacity);
	~VoxelRenderer();

	Mesh* render(CellularAutomata* automata);
};