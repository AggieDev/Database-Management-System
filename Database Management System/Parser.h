#pragma once

#include <iostream>
#include <vector>
#include <string>

class Parser
{
public:
	Parser();
	~Parser();

	// split a line of input into a vector of strings
	std::vector<std::string> readInputLine(std::string intputLine);

	// call appropriate function based on user input
	void InsertCmd(std::string input);


	// read an integer from input, starting at inputIndex, assign it to word, and return the number of characters read
	int readInteger(std::string& word, std::string input, int inputIndex);

// TODO: implement following functions
	bool isDelimiter(char c); // can be space, quote, parenthesis, semi-colon, ...
	
	// Elliut:
	bool isOp(char c);
	// Patrick:
	bool isType(std::string s);

	
};

