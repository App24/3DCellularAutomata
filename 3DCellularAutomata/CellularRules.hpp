#pragma once

#include <vector>
#include "NeighbourhoodMode.h"

#include <string>

class CellularRules {
public:
	std::vector<int> birth;
	std::vector<int> survive;
	int states;
	NeighbourhoodMode neighbourhoodMode;
	std::string rule;
};



extern bool generate_rule(std::string rule, CellularRules* automataRules);