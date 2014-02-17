//#include "stdafx.h"
#include "Parser.h"

using namespace std;
Parser::Parser(Database* db)
{
	_db = db;
}
Parser::Parser()
{ // default constructor, if no database reference is provided, make an empty one
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
	else if (inputVector.at(0) == "select")
	{
		selection(inputVector);
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
                charactersRead = readOp(word,inputLine, i);
                inputVector.push_back(word);
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
Table Parser::selection(vector<string> input)
{ // select from a table according to a specific condition
	// selection ::= select ( condition ) atomic-expr

	bool selectKeyword = input.at(0) == "select";
	bool properOpenParenthesis = input.at(1) == "(";
	bool properCloseParenthesis = false;
	vector<string> valuesForCondition;
	vector<string> valuesForAtomicExpression;
	unsigned int i;
	for (i = 2; i < input.size(); i++)
	{		
		string temp = input.at(i);
		if (temp == ")")
		{ // done adding to condition phrase if parenthesis are closed
			properCloseParenthesis = true;
		}
		else if (!properCloseParenthesis)
		{ // continue appending to condition phrase
			valuesForCondition.push_back(input.at(i));
		}
		else
		{ // add to third part of selection phrase; the atomic-expr
			valuesForAtomicExpression.push_back(input.at(i));
		}
	}
	
	Table t = Table();
	// this should generate a table (existing one, or combination of two, etc)
	t = interpretAtomicExpression(valuesForAtomicExpression);

	// this should narrow down that^ table according to conditions
	t = modifyTableForCondition(valuesForCondition, t);

	if (selectKeyword && properOpenParenthesis && properCloseParenthesis)
	{
		return t;
	}
	throw new exception("An error occurred while parsing a selection phrase");
	return t;
}
Table Parser::interpretAtomicExpression(vector<string> input)
{ // parse the given input and set the Table t appropriately
	
	Table newTable = Table();
	if (input.size() == 1)
	{ // atomic-expr ::= relation-name
		string relationName = input.at(0);
		newTable = _db->getTable(relationName);
	}
	else if (input.size() >= 3)
	{ // atomic-expr ::= ( expr )
		input.erase(input.begin());
		input.erase(input.begin() + input.size() - 1);
		newTable = getTableFromExpression(input);
	}
	return newTable;
}
Table Parser::getTableFromExpression(vector<string> expr)
{ // evaluate an expression and return a pointer to a table
	// expr ::= atomic-expr | selection | projection | renaming 
	//						| union | difference | product | natural-join

	string first = expr.at(0);

	if (first == "select")
	{ // selection
		return selection(expr); // selection returns an appropriate table
	}
	else if (first == "project")
	{ // projection
// Elliut
	}
	else if (first == "rename")
	{ // renaming
// Elliut
	}
	else if (find(expr.begin(), expr.end(), "+") != expr.end())
	{ // union ::= atomic-expr + atomic-expr
// Waylon
	}
	else if (find(expr.begin(), expr.end(), "-") != expr.end())
	{ // difference ::= atomic-expr - atomic-expr
// Waylon
	}
	else if (find(expr.begin(), expr.end(), "*") != expr.end())
	{ // product ::= atomic-expr * atomic-expr
// Waylon
	}
	else if (find(expr.begin(), expr.end(), "JOIN") != expr.end())
	{ // natural-join ::= atomic-expr JOIN atomic-expr
// Waylon
	}
	return NULL;
}
Table Parser::modifyTableForCondition(vector<string> conditions, Table t)
{ // parse the given conditions and modify the Table t appropriately
	return Table();
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
			Table t = _db->getTable(relationName);
			t.addEntry(fields);
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
int Parser::readOp(std::string& word, std::string input, int inputIndex)
{
    string myWord = "";
    int myIndex = inputIndex;
    char nextDigit = input.at(myIndex);
    while(isOp(nextDigit))
    {
        myWord +=nextDigit;
        nextDigit = input.at(++myIndex);
    }
    word = myWord;
    return (myIndex - inputIndex); // return how many characters were read
    
}