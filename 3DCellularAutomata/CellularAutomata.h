#pragma once

#include <glm/glm.hpp>

using namespace glm;

#include "CellularRules.hpp"
#include "NeighbourhoodMode.h"
#include <thread>

class CellularAutomata {
public:
	CellularAutomata(int width, int height, int depth, std::vector<float> colors, CellularRules rules, bool cube, int cubeSize);
	~CellularAutomata();

	void simulate();

	bool isCell(int x, int y, int z);
	int getCell(int x, int y, int z);

	int getWidth();
	int getHeight();
	int getDepth();
	std::vector<float> colors;
	CellularRules rules;
	bool wrap;

	static int threadCount;

	void updateThreads();
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

