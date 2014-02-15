//#include "stdafx.h"
#include "Parser.h"

using namespace std;
Parser::Parser()
{
}


Parser::~Parser()
{
}

string getNext(char c)
{ // return type (of grammar)
	switch (c){
	case 0..9: return "digit";
		break;

	}
}

vector<string> Parser::readInputLine(string line)
{
	// split a line of input into a vector of strings
	return vector<string>();
}

void Parser::callFunction(std::vector<std::string> input)
{
<<<<<<< HEAD
	// input is a full line
	// ex: INSERT INTO animals VALUES FROM ("Joe", "bird", 2);
	if (input.at(0) == string("INSERT") && input.at(1) == string("INTO"))
	{
	}
=======
>>>>>>> cc7aba8f880a86e599099ed18df19261ec2e7fc2
}