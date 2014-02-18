#pragma once

#include <iostream>
#include <vector>

class Parser
{
public:
	Parser();
	~Parser();

	// split a line of input into a vector of strings
	std::vector<std::string> readInputLine(std::string line);

	// call appropriate function based on user input
	void callFunction(std::vector<std::string> input);


};

