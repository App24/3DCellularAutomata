#pragma once

#include <glm/glm.hpp>

using namespace glm;

#include "CellularRules.hpp"
#include "NeighbourhoodMode.h"
#include <thread>

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

	static int threadCount;
private:
	int* cells;
	int* calculateCells;

	int width, height, depth;

	void simulateCell(int x, int y, int z);
	void simulateThread(int start, int end);

	int getNeighbourCount(int x, int y, int z);

	std::thread* threads;

	std::vector<int> linesDepth;

};

