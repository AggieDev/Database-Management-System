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
	Database* _db;
public:
	Parser();
	Parser(Database* db);
	~Parser();

	// read a file containing DML commands
	void readFile(string fileName);

	// use this function with readInputLine's returned vector 
	// to call the correct database/table functions
	void interpretInputVector(vector<string> inputVector);

	// split a line of input into a vector of strings
	std::vector<std::string> readInputLine(std::string intputLine);

	// call appropriate function based on user input, return true on success
	bool InsertCmd(std::vector<std::string> inputLine);
    /*-----Eli---*/
    bool ExitCmd(std::vector<std::string> inputLine);
    
	Table selection(std::vector<std::string> input);
    
    /*-----Eli---*/
    Table projection(std::vector<std::string>input);
    /*-----Eli---*/
    //Table rename(std::vector<std::string>input);
    /*-----Eli---*/
    bool ShowCmd(std::vector<std::string>input);
    /*-----Eli---*/
    bool CreateCmd(std::vector<std::string>input);
    /*-----Eli---*/
    bool UpdateCmd(std::vector<std::string>input);
	
    Table deletion(std::vector<std::string> input);

	// parse the given input and set the Table t appropriately
	Table interpretAtomicExpression(std::vector<std::string> input);

	// Below is not needed because of selection function in Database
	// return a table as a subset of t that satisfies the conditions
	//Table modifyTableForCondition(std::vector<std::string> conditions, Table t);
	// return true if the comparison is satisfied
	//bool satisfiesComparison(Table t, Entry entry, std::string columnName, std::string op, std::string operand2);
	// return true if entry satisfies the one or more conditions represented in tokensForCondition
	//bool checkConditions(std::vector<std::string> tokensForCondition, Table t, Entry entry);

	Table getTableFromExpression(std::vector<std::string> expr);
	// read an integer from input, starting at inputIndex, assign it to word, and return the number of characters read
	int readInteger(std::string& word, std::string input, int inputIndex);
	
	// read a type from input, starting at inputIndex, assign it to word, (assign the varchar parameter to varCharNum if applicable),
	// and return the number of characters read
    
    
	int readType(std::string& word, std::string input, int inputIndex);
    /*-----Eli---*/
    int readOp(std::string& word, std::string input, int inputIndex);
    /*-----Eli---*/
    int readLiteral(std::string& word, std::string input, int inputIndex);

	bool isOp(char c);
	bool isType(std::string s);
	bool isDelimiter(char c); // can be space, quote, parenthesis, semi-colon, ...
	bool is_identifier (char c);

	int readIdentifier(std::string& word, std::string , int inputIndex);
};

