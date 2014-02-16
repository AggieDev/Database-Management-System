//#include "stdafx.h"
#include "Parser.h"

using namespace std;
Parser::Parser()
{
}


Parser::~Parser()
{
}


//vector<string> Parser::readInputLine(string inputLine)
//{ // expect an identifier to be a word, integer, or operand
//	vector<string> inputVector; // strings to represent: identifiers, types, and integers
//	string word = "";
//	bool isInteger = false;
//	bool isIdentifier = false;
//	bool isLiteral = false;
//	bool isOperator = false;
//	bool isAttributeType = false;
//	unsigned int i = 0;
//	while(i < inputLine.length())
//	{
//		char c = inputLine[i];
//		if (c == ';')
//		{ // end of input line
//			break;
//		}
//		else if (word == "")
//		{ // determine what the following token is going to be
//			isInteger = isDigit(c);
//			isLiteral = (c == '"');
//			isIdentifier = isAlpha(c);
//			isOperator = isOp(c);
//			isAttributeType = isType(inputLine.substr(i, 7)); // VARCHAR or INTEGER
//		}
//		else
//		{
//			// the appropriate function reads a certain number of characters
//			int charactersRead = 0;
//			if (isInteger) // Patrick
//			{ 
//				charactersRead = readInteger(word, inputLine, i);
//			}
//			else if (isLiteral)			// Elliut
//			{
//			}
//			else if (isIdentifier)		// Garrett
//			{
//			}
//			else if (isOperator)		// Elliut
//			{
//			}
//			else if (isAttributeType)	// Patrick
//			{
//			}
//			else { throw new exception("token type was not identified"); }
//
//			// advance the inputLine index by that many characters
//			i += charactersRead; // TODO: check that this went right
//		}
//
//
//
//		//if (isDelimiter(c))
//		//{ // reached end of identifier if at space, quote, parenthesis, semi-colon, ...
//		//	inputVector.push_back(word);
//		//	word = "";
//		//}
//		//else if (isAlpha(c))
//		//{ // append character to current word
//		//	word += c;
//		//}
//		//else if (isDigit(c))
//		//{
//		//	if (word.length() >= 1 && isAlpha(word.at(0)))
//		//	{ // a digit cannot be the first character of an identifier
//		//		word += c;
//		//	}
//		//	else
//		//	{
//		//		word += c;
//		//	}
//		//}
//		
//		
//	}
//
//	return inputVector;
//
//	//if (line.find("INSERT INTO") == 0)
//	//{ 
//	//	InsertCmd(line);
//	//}
//}

void InsertCmd(string input)
{ /* Phrase structure grammar for insert command - 
	 insert-cmd ::= INSERT INTO relation-name VALUES FROM ( literal { , literal } )
  					| INSERT INTO relation-name VALUES FROM RELATION expr 
	
	ex: INSERT INTO animals VALUES FROM ("Joe", "cat", 4);

	*/

	string relationName;	// name of Table in the Database
	vector<string> fields;	// values of vields for new entry


}

bool Parser::isType(string s)
{ // return true if the given string is a type, defined as:
	// type ::= VARCHAR ( integer ) | INTEGER

	return (s.compare("VARCHAR") == 0) || (s.compare("INTEGER") == 0);
}

int Parser::readInteger(string& word, string input, int inputIndex)
{ // read an integer from input, starting at inputIndex, assign it to word
	
	string myWord = "";
	int myIndex = inputIndex;
	char nextDigit = input.at(myIndex);
	while (isdigit(nextDigit))
	{
		myWord += nextDigit;
		nextDigit = input.at(++myIndex);
	}
	word = myWord;
	return (myIndex - inputIndex); // return how many characters were read
}