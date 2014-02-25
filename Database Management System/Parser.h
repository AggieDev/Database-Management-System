#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "Database.h"
#include "Table.h"

class Parser
{
private:
public:
	Parser();
	~Parser();

	// read a file containing DML commands
	void readFile(std::string fileName);
	Table readquery(std::vector<string> inputVec);
	// use this function with readInputLine's returned vector 
	// to call the correct database/table functions
	void evaluateInputVector(std::vector<std::string> inputVector);

	// split a line of input into a vector of strings
	std::vector<std::string> readInputLine(std::string intputLine);

	// single table manipulation functions
	Table selection(std::vector<std::string> input);
	Table projection(std::vector<std::string>input);
	Table rename(std::vector<std::string>input);

	
	// parse the given input and set the Table t appropriately
	Table interpretAtomicExpression(std::vector<std::string> input);
	Table parseExpression(std::vector<std::string>, string);


	// call command function based on user input, return true on success
	bool insertCmd(std::vector<std::string> inputLine);
	bool deleteCmd(std::vector<std::string> input);
	bool exitCmd(std::vector<std::string> inputLine);
	void ShowCmd(std::vector<std::string>input);
	Table createCmd(std::vector<std::string>input);
	void updateCmd(std::vector<std::string>input);



	// evaluates an expr and appropriately calls select, project, rename, atomic-expr,
	// or a dual-table manipulation function, delegated in parseExpression
	Table getTableFromExpression(std::vector<std::string> expr);

	// evaluate the union, difference, product, or natural-join
	//Table parseExpression(std::vector<std::string>, std::string);


	Table evaluateAtomicExpression(std::vector<std::string> input);
	Table evaluateCondition(std::vector<std::string> conditionVec, Table table);
	
	// read the entire vector of input, then set the attribute names and column types
	void evaluateTypeAttributeList(std::vector<std::string> input, std::vector<std::string>*attributes, std::vector<char>*types);	
	//read the entire vector of input, then sets the entries in the attribute name equal to literal
	void evaluateSetAttribute(std::vector<std::string> input);

	// read from input, starting at inputIndex, assign it to word,
	// and return the number of characters read
	int readType(std::string& word, std::string input, int inputIndex);
	int readOp(std::string& op, std::string input, unsigned int inputIndex);
	int readLiteral(std::string& word, std::string input, unsigned int inputIndex);
	int readIdentifier(std::string& word, std::string, int inputIndex);
	int readInteger(std::string& word, std::string input, int inputIndex);

	// any of <,>,=,!,+,-,*
	bool isOp(char c);
	// can be space, open/close parenthesis, semi-colon
	bool isDelimiter(char c);
	// INTEGER or VARCHAR(integer)
	bool isType(std::string s);
	// word(s) in quotes
	bool isLiteral(std::string s);

	// utility functions
	void removeParenthesis(std::vector<std::string>* vec);

};
