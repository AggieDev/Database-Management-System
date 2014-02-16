//#include "stdafx.h"
#include "Parser.h"

using namespace std;
Parser::Parser(Database* db)
{
	_db = db;
}
Parser::Parser()
{ // default constructor if no database reference is provided, make an empty one
	_db = new Database();
}

Parser::~Parser()
{
}

void Parser::interpretInputVector(vector<string> inputVector)
{ // conditional statements to match the input line to specific function calls
	// NOTE: Individual members in this vector are all strings but will be one of -
	//		- identifier(basically an individual word, ie INSERT, pets-table-name, etc)
	//		  -> type; represented the same as identifier, but is either VARCHAR or INTEGER
	//		- number (as a string, like "22")
	//		- operator (!=, ==, <, <-, etc) so this includes the query command
	//		- relevant symbols (parenthesis, +, -, *)
	//		  -> parenthesis will tell you to keep collecting subsequent strings in the 
	//			 list for, say, CREATE TABLE with arbitrarily many columns possible
	//		  -> set manipulation (union, difference, product)

	if (inputVector.at(1) == "<-") // Query
	{ // the phrase structure:		query ::= relation-name <- expr ;
		// this is one where our work collaborates. The query function will 
		// use relation-name to find the table, but the expr will be passed to 
		// a parsing function that handles how an expression is evaluated
		//		(can be a selection, projection, union, etc). 
	}
	
	if (inputVector.at(0) == "INSERT" && inputVector.at(1) == "INTO")
	{
		InsertCmd(inputVector);
	}
}
vector<string> Parser::readInputLine(string inputLine)
{ // expect an identifier to be a word, integer, or operand
	
	// strings to represent: identifiers, types, operators, parenthesis, *, and <-
	vector<string> inputVector;
	
	string word = "";
	unsigned int i = 0;
	while(i < inputLine.length())
	{
		char c = inputLine[i];
		if (c == '(' || c == ')')
		{ // individual chars that get own spot in vector
			stringstream ss;
			string singleCharString;
			ss << c;
			ss >> singleCharString;
			inputVector.push_back(singleCharString);
			i++;
		}
		else if (isDelimiter(c))
		{ // if a space or comma, ignore and move onto next char
			// does same for a semi-colon, but when that comes, the loop will exit
			i++;
		}
		
		else // if alpha, digit, quotation, operator, or attribute
		{ // determine what the following token is going to be
			bool isInteger = isdigit(c);
			bool isLiteral = (c == '"');
			bool isIdentifier = isalpha(c);
			bool isOperator = isOp(c);
		
				// the appropriate function reads a certain number of characters
			int charactersRead = 0;
			if (isInteger) // Patrick
			{ 
				charactersRead = readInteger(word, inputLine, i);
				inputVector.push_back(word);
			}
			else if (isLiteral)			// Elliut
			{ 
			}
			else if (isIdentifier)		// Garrett
			{
			}
			else if (isOperator)		// Elliut
			{ // this will also include the '<-' needed for a query, and +,-,* for set manipulation
			}
			else { throw new exception("token type was not identified"); }

			// advance the inputLine index by that many characters
			i += charactersRead; // TODO: check that this went right
		}
	}

	return inputVector;
}
bool Parser::isOp(char c)
{ // return true if c is the start of an operator
	// a full operator can be one of: [+,-,*,==,!=,<,<=,>,>=,<-]
	string operators = "=!<>+-*";
	return (operators.find(c) != string::npos);
}
bool Parser::isDelimiter(char c)
{ // ignored characters in the input line
	return (c == ' ') || (c == ',') || (c == ';');
}
bool Parser::InsertCmd(vector<string> input)
{ // insert into a table from explicit values or one obtained from another table

	string relationName = input.at(2);	// name of Table in the Database
	if (input.at(5) == "RELATION")
	{ // insert-cmd ::= INSERT INTO relation-name VALUES FROM RELATION expr
		vector<string> expression;
		for (unsigned int i = 6; i < input.size(); i++)
		{ // fill expression vector with the values following the word, 'RELATION'
			expression.push_back(input.at(i));

// TODO: hand off the expression vector to the appropriate sub parser

		}
	}
	else
	{ // insert-cmd ::= INSERT INTO relation-name VALUES FROM(literal {, literal })

		bool properOpenParenthesis = input.at(5) == "(";
		bool properCloseParenthesis = input.at(input.size() - 1) == ")";
		if (properOpenParenthesis && properCloseParenthesis)
		{ // ensure parenthesis are appropriately placed
			vector<string> fields;
			for (unsigned int i = 6; i < input.size() - 1; i++)
			{ // fill expression vector with the values following the word, 'RELATION', should be one or more literals
				fields.push_back(input.at(i));
			}
			Table* t = _db->getTable(relationName);
			t->addEntry(fields);
			return true;
		}
	}
	return false;
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
int Parser::readType(std::string& word, std::string input, int inputIndex)
{ // read a type from input, starting at inputIndex, assign it to word
	int myIndex = inputIndex;
	string tempWord = input.substr(inputIndex, 7); // assumes types are VARCHAR or INTEGER, both 7 chars
	if (isType(tempWord))
	{
		myIndex += tempWord.length();
		word = tempWord;
	}
	return (myIndex - inputIndex); // return how many characters were read
}