#include "CellularAutomata.h"

int CellularAutomata::threadCount = 11;

CellularAutomata::CellularAutomata(int width, int height, int depth, vec3 color, CellularRules rules):width(width), height(height), depth(depth), cellColor(color), rules(rules)
{
	cells = new int[width * height * depth];
	calculateCells = new int[width * height * depth];

	memset(cells, 0, width * height * depth * sizeof(int));

	int midX = width / 2;
	int midY = height / 2;
	int midZ = height / 2;

	int radius = 10;

	for (int x = -radius; x <= radius; x++)
	{
		for (int y = -radius; y <= radius; y++)
		{
			for (int z = -radius; z <= radius; z++)
			{
				if (rand() % 2 == 0) {
					cells[((y + midY) * depth + (z + midZ)) * width + (midX + x)] = rules.states;
				}
			}
		}
	}

	/*for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			for (size_t z = 0; z < depth; z++)
			{
				if(rand()%2==0)
					cells[((y) * depth + (z)) * width + (x)] = rules.states;
			}
		}
	}*/
	memcpy(calculateCells, cells, width* height* depth * sizeof(int));
}

CellularAutomata::~CellularAutomata()
{
	delete[] cells;
	delete[] calculateCells;
	delete[] threads;
}

void CellularAutomata::simulate()
{
	linesDepth = std::vector<int>();
	for (size_t i = 0; i < depth; i++)
	{
		linesDepth.push_back(i);
	}

	threads = new std::thread[threadCount];
	for (size_t i = 0; i < threadCount; i++)
	{
		threads[i] = std::thread(&CellularAutomata::simulateThread, this, floor(height*(i/threadCount)), floor(height * ((i+1) / threadCount)));
	}

	for (size_t i = 0; i < threadCount; i++)
	{
		threads[i].join();
	}

	memcpy(cells, calculateCells, width * height * depth * sizeof(int));
}

bool CellularAutomata::isCell(int x, int y, int z)
{
	if (x < 0 || y < 0 || z < 0 || x >= width || y >= height || z >= depth)
		return false;
	return cells[(y * depth + z) * width + x];
}

int CellularAutomata::getCell(int x, int y, int z)
{
	while (x < 0)
	{
		x += width;
	}

	while (y < 0)
	{
		y += height;
	}

	while (z < 0)
	{
		z += depth;
	}

	while (x >= width) {
		x -= width;
	}

	while (y >= height) {
		y -= height;
	}

	while (z >= depth) {
		z -= depth;
	}
	return cells[(y * depth + z) * width + x];
}

int CellularAutomata::getWidth()
{
	return width;
}

int CellularAutomata::getHeight()
{
	return height;
}

int CellularAutomata::getDepth()
{
	return depth;
}

void CellularAutomata::simulateCell(int x, int y, int z)
{
	int& cell = calculateCells[(y * depth + z) * width + x];

	int neightbours = getNeighbourCount(x, y, z);

	if (cell == rules.states) {
		if (!std::count(rules.survive.begin(), rules.survive.end(), neightbours)) {
			cell--;
		}
	}
	else if(cell > 0) {
		cell--;
	}
	else {
		if (std::count(rules.birth.begin(), rules.birth.end(), neightbours)) {
			cell = rules.states;
		}
	}
}

void CellularAutomata::simulateThread(int start, int end)
{
	/*while (linesDepth.size() > 0) {
		if (linesDepth.size() <= 0)
			break;
		int line = linesDepth.back();
		if (linesDepth.size() <= 0)
			break;
		linesDepth.pop_back();*/

	for (size_t y = start; y < end; y++) {
		for (size_t x = 0; x < width; x++)
		{
			for (size_t z = 0; z < depth; z++)
			{
				simulateCell(x, y, z);
			}
		}
	}
	//}
}

int CellularAutomata::getNeighbourCount(int x, int y, int z)
{
	int count = 0;

	for (int _x = -1; _x <= 1; _x++)
	{
		for (int _y = -1; _y <= 1; _y++)
		{
			for (int _z = -1; _z <= 1; _z++)
			{
				if (_x == 0 && _y == 0 && _z == 0)
					continue;
				if (abs(_x) + abs(_y) + abs(_z) > 1 && rules.neighbourhoodMode == NeighbourhoodMode::VonNeurmann)
					continue;
				int cell = getCell(x + _x, y + _y, z + _z);
				if (cell == rules.states)
					count++;
			}
		}
	}

	return count;
}
