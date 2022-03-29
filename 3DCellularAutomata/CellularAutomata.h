#pragma once

#include <glm/glm.hpp>

using namespace glm;

#include "CellularRules.hpp"
#include "NeighbourhoodMode.h"

class CellularAutomata {
public:
	CellularAutomata(int width, int height, int depth, vec3 color, CellularRules rules);
	~CellularAutomata();

	void simulate();

	bool isCell(int x, int y, int z);
	int getCell(int x, int y, int z);

	int getWidth();
	int getHeight();
	int getDepth();
	vec3 cellColor;
	CellularRules rules;
private:
	int* cells;
	int* calculateCells;

	int width, height, depth;

	void simulateCell(int x, int y, int z);

	int getNeighbourCount(int x, int y, int z);

};

