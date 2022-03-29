#include "CellularRules.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>

inline std::vector<std::string> tokenize(std::string s, std::string del = " ")
{
	int start = 0;
	int end = s.find(del);
	std::vector<std::string> strings;
	while (end != -1) {
		strings.push_back(s.substr(start, end-start));
		start = end + del.size();
		end = s.find(del, start);
	}
	strings.push_back(s.substr(start, end - start));
	return strings;
}

inline char asciitolower(char in) {
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

bool generate_rule(std::string rule, CellularRules* automataRules)
{
	std::string originalRule = rule;
	std::for_each(rule.begin(), rule.end(), [](char& c) {
		c = std::tolower(c);
		});

	std::vector<std::string> test = tokenize(rule, "/");
	if (test.size() < 4) {
		return false;
	}

	std::string surviveString = test[0];
	std::string birthString = test[1];
	std::string statesString = test[2];
	std::string neighbourhoodMode = test[3];

	automataRules->rule = originalRule;

	std::vector<std::string> survives = tokenize(surviveString, ",");

	for (size_t i = 0; i < survives.size(); i++)
	{
		std::string survive = survives[i];
		try {
			automataRules->survive = { std::stoi(survive) };
		}
		catch (...) {
			std::vector<std::string> bounds = tokenize(survive, "-");
			int begin = 0;
			try {
				begin = std::stoi(bounds[0]);
			}
			catch (...) {
				return false;
			}
			int end = 0;
			try {
				end = std::stoi(bounds[1]);
			}
			catch (...) {
				return false;
			}
			std::vector<int> surviveVector;
			for (size_t i = begin; i <= end; i++)
			{
				surviveVector.push_back(i);
			}
			automataRules->survive = surviveVector;
		}
	}

	std::vector<std::string> births = tokenize(birthString, ",");

	for (size_t i = 0; i < births.size(); i++)
	{
		std::string birth = births[i];
		try {
			automataRules->birth = { std::stoi(birth) };
		}
		catch (...) {
			std::vector<std::string> bounds = tokenize(birth, "-");
			int begin = 0;
			try {
				begin = std::stoi(bounds[0]);
			}
			catch (...) {
				return false;
			}
			int end = 0;
			try {
				end = std::stoi(bounds[1]);
			}
			catch (...) {
				return false;
			}
			std::vector<int> birthVector;
			for (size_t i = begin; i <= end; i++)
			{
				birthVector.push_back(i);
			}
			automataRules->birth = birthVector;
		}
	}

	try {
		automataRules->states = std::stoi(statesString) - 1;
	}
	catch(...) {
		return false;
	}

	if (neighbourhoodMode == "m") {
		automataRules->neighbourhoodMode = NeighbourhoodMode::Moore;
	}
	else {
		automataRules->neighbourhoodMode = NeighbourhoodMode::VonNeurmann;
	}

	return true;
}
