//#include "stdafx.h"

#include <exception>
#include <fstream>
#include "Table.h"
#include "Parser.h"

using namespace std;

Parser::Parser()
{
}

Parser::~Parser()
{
}

void Parser::readFile(string fileName)
{ // read a file written in DML. Send each line to readInputLine() which returns a vector of the tokens
	// send those tokens to evaluateInputVector()
	ifstream inFile(fileName);
	string line;
	if (inFile.is_open())
	{
		while (getline(inFile, line))
		{
			evaluateInputVector(readInputLine(line));
		}
		inFile.close();
	}
	else
	{
		cout << "Invalid input file name provided\n";
	}
}

void Parser::evaluateInputVector(vector<string> inputVector)
{ // conditional statements to match the input line to specific function calls
	// NOTE: Individual members in this vector are all strings but will be one of -
	//		- identifier (an individual word)
	//		- number (as a string, like "22")
	//		- operator (!=, ==, <, <-, etc)
	//		- relevant symbols (parenthesis, +, -, *)


	if (inputVector.at(1) == "<-") // Query
	{ // the phrase structure:		query ::= relation-name <- expr ;
		// this is one where our work collaborates. The query function will 
		// use relation-name to find the table, but the expr will be passed to 
		// a parsing function that handles how an expression is evaluated
		//		(can be a selection, projection, union, etc). 
		vector<string> expression;
		for (unsigned int i = 2; i < inputVector.size(); i++)
		{
			expression.push_back(inputVector.at(i));
		}
		getTableFromExpression(expression);
// TODO: query needs to assign the resulting table to the provided relation-name
	}
	
	if (inputVector.at(0) == "INSERT" && inputVector.at(1) == "INTO")
	{	// insert into a table
		insertCmd(inputVector);
	}
	else if (inputVector.at(0) == "DELETE" && inputVector.at(1) == "FROM")
	{	// delete from a table
		deletion(inputVector);
	}
	else if (inputVector.at(0) == "OPEN")
	{	// open file and then create a table based on the information in the file
		Database::getTable(inputVector.at(1)).open_file(inputVector.at(1));
	}
	else if (inputVector.at(0) == "CLOSE")
	{	// close the file
		Database::getTable(inputVector.at(1)).close_file(inputVector.at(1));
	}
	else if (inputVector.at(0) == "WRITE")
	{	// write to file
		Database::getTable(inputVector.at(1)).write_to_file(inputVector.at(1));
	}
	else if (inputVector.at(0) == "EXIT")
	{	// exit the database application
        exitCmd(inputVector);
	}
	else if (inputVector.at(0) == "SHOW")
	{	// show a table
        ShowCmd(inputVector);
	}
	else if (inputVector.at(0) == "CREATE" && inputVector.at(1) == "TABLE")
	{	// create a new table
       // createCmd(inputVector);
	}
	else if (inputVector.at(0) == "UPDATE")
	{	// update values in a table
       // updateCmd(inputVector);
	}
}
vector<string> Parser::readInputLine(string inputLine)
{ // expect an identifier to be a word, integer, or operand
	
	// strings to represent: identifiers, types, operators, parenthesis, *, and <-
	vector<string> inputVector;

	try
	{
		string word = "";
		unsigned int i = 0;
		while (i < inputLine.length())
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

			else // if alpha, digit, quotation, operator, or attribute;
			{	 // determine what the following token is going to be.
				// the appropriate function returns the number of characters read
				int charactersRead = 0;
				if (isdigit(c) != 0)
				{
					charactersRead = readInteger(word, inputLine, i);
					inputVector.push_back(word);
				}
				else if (c == '\"')
				{
					charactersRead = readLiteral(word, inputLine, i);
					inputVector.push_back(word);
				}
				else if (isalpha(c) || (c == '_'))
				{
					charactersRead = readIdentifier(word, inputLine, i);
					inputVector.push_back(word);
				}
				else if (isOp(c))
				{ // this will also include the '<-' needed for a query, and +,-,* for set manipulation
					charactersRead = readOp(word, inputLine, i);
					inputVector.push_back(word);
				}
				//else if ()
				else { throw new exception("token type was not identified"); }

				// advance the inputLine index by that many characters
				i += charactersRead; // TODO: check that this went right
			}
		}
	}
	catch (exception e)
	{
		cout << "exception caught in readInputLine\n";
	}
	return inputVector;
}
bool Parser::isOp(char c)
{ // return true if c is the start of an operator
	// a full operator can be one of: [+,-,*,==,!=,<,<=,>,>=,<-,||,&&]
	string operators = "=!<>+-*|&";
	return (operators.find(c) != string::npos);
}
bool Parser::isDelimiter(char c)
{ // ignored characters in the input line
	return (c == ' ') || (c == ',') || (c == ';');
}

Table Parser::deletion(vector<string> input)
{
	bool deleteKeyword = (input.at(0) == "DELETE") && (input.at(1) == "FROM");
	if (!deleteKeyword)
	{
		throw new exception("Invalid deletion call");
		return NULL;
	}
	string relationName = input.at(1);
	
	// badEntries are every entry in the table that meets the condition,
	// the resulting table will have its original minus the badEntries.
	// This is done using Database::differenceTable()
	Table badEntries = Table();
	Table* t = Database::getTableByReference(relationName);
	vector<Entry> entries = t->getEntries();

	// run selection() to get the table of entries that satisfy 
	// the condition and will be eliminated
	input.erase(input.begin());
	input.at(0) = "select";
	badEntries = selection(input);

	// update the appropriate table by reference
	Table temp = Database::differenceTable(badEntries, *t);
	t = &temp; 
	
	// and return the same one stored as a temp
	return temp;
}

Table Parser::selection(vector<string> input)
{ // select from a table according to a specific condition
	// selection ::= select ( condition ) atomic-expr

	bool selectKeyword = (input.at(0).compare("select") == 0);
	if (!selectKeyword || input.at(1) != "(")
	{ // selection call needs to start with "select" then "("
		throw new exception("Invalid selection call");
		return NULL;
	}
	bool parenthesisClosed = false;
	vector<string> conditionVec;	// a vector to represent a condition
	vector<string> atomicExprVec;	// a vector to represent an atomic expression
	unsigned int i;
	for (i = 1; i < input.size(); i++)
	{ // read each value in the vector after "select"
		string temp = input.at(i);
		if (!parenthesisClosed)
		{ // continue appending to condition phrase
			conditionVec.push_back(input.at(i));
			if (temp == ")")
			{ // done adding to condition phrase, if parenthesis are closed
				parenthesisClosed = true;
			}
		}
		else
		{ // add to the atomic-expr
			atomicExprVec.push_back(input.at(i));
		}
	}
	
	// this will generate a table (existing one, or combination of two, etc)
	Table fromTable = evaluateAtomicExpression(atomicExprVec);

	// this will return the proper selection of the 'fromTable'
	Table selectionTable = evaluateCondition(conditionVec, fromTable);
	
	return selectionTable;
}
Table Parser::evaluateCondition(vector<string> conditionVec, Table table)
{ // return a Table satisfying the conditions in conditionVec
	// formally:
	//			condition ::= conjunction { || conjunction }
	//			conjunction ::= comparison { && comparison }
	//			comparison ::= operand op operand | (condition)
	
	vector<string> operand1Vec;
	string op;
	bool opFound = false;
	vector<string> operand2Vec;

	for (unsigned int i = 0; i < conditionVec.size(); i++)
	{
		string val = conditionVec.at(i);
		if (isOp(val[0]))
		{ // reached an operand
			readOp(op, val, 0);
			opFound = true;
		}
		else if (val == "(" || val == ")")
		{ // don't include parenthesis in condition vector
			// this basically assumes it is a simple condition; (operand op operand)
			continue;
		}
		else if (!opFound)
		{ // still appending to first operand vector
			operand1Vec.push_back(val);
		}
		else 
		{ // if (opFound) keep appending to second operand vector
			operand2Vec.push_back(val);
		}
	}
	
	if (op == "||" || op == "&&")
	{
//TODO: handle more complicated, multi-condition phrase
	}
	else
	{ // otherwise, both operands should be of the simple form:
		// operand ::= literal | attribute-name
		if (operand1Vec.size() == 1 && operand2Vec.size() == 1)
		{	// simple case where operandVec is single operand
			//NOTE: Database::select() does not work yet
			// the following line will depend on this Database function working:
			//			Table Database::select(vector<string> condition, Table t)
			// then this following should work as well
			

			return Database::select(conditionVec, table);

		}
		else
		{
			throw new exception("Error while parsing a condition, expected form: 'operand op operand'");
		}
	}
	

	return Table();
}
Table Parser::evaluateAtomicExpression(vector<string> input)
{ // parse the given input and return the appropriate Table

	Table newTable = Table();
	if (input.size() == 1)
	{ // atomic-expr ::= relation-name
		string relationName = input.at(0);
		newTable = Database::getTable(relationName);
	}
	else if (input.size() > 1)
	{ // atomic-expr ::= ( expr )
		vector<string> inputCopy = input;

		// erase parenthesis if present
		removeParenthesis(&input);
		newTable = getTableFromExpression(inputCopy);
	}

	// the table will be empty if invalid expression was provided
	return newTable;
}
void Parser::evaluateTypeAttributeList(vector<string> input, vector<string>* attributes, vector<char>* types)
{ 	// parse the entire vector of input, set the attribute names and column types 
	removeParenthesis(&input);

	attributes->clear();
	types->clear();
	for (unsigned int i = 0; i < input.size(); i++)
	{ // populate attribute-names and column types using input vector

		string element = input.at(i);

		if (isType(element))
		{
			if (element.compare("INTEGER") == 0)
			{
				types->push_back('i');
			}
			else if (element.compare("VARCHAR") == 0)
			{
				types->push_back('s');
				i += 3; // assume ["(", "<integer>", ")"] are next three, ignore 'em for now
			}
		}
		else // if is attribute/column name
		{
			attributes->push_back(element);
		}
	}
	if (attributes->size() != types->size())
	{
		throw new exception("Error occurred in evaluateTypeAttributeList\n");
	}
}
void Parser::removeParenthesis(vector<string>* vec)
{ // if first and last elements are parenthesis, remove
	if (vec->at(0) == "(")
	{
		vec->erase(vec->begin());
	}
	if (vec->at(vec->size() - 1) == ")")
	{
		vec->erase(vec->begin() + vec->size() - 1);
	}
}
Table Parser::projection(vector<string> input)
{ // project from a table according
	// projection::= project ( attribute-list ) atomic-expr
    
	bool selectKeyword = (input.at(0) == "project");
	if (!selectKeyword)
	{
		throw new exception("Invalid projection call");
		return NULL;
	}
	bool properOpenParenthesis = input.at(1) == "(";
	bool properCloseParenthesis = false;
    
    
	vector<string> attributesList;
	vector<string> valuesForAtomicExpression;
	unsigned int i;
	for (i = 2; i < input.size(); i++)
	{
		string temp = input.at(i);
		if (temp == ")")
		{ // done adding to attributesList phrase if parenthesis are closed
			properCloseParenthesis = true;
		}
		else if (!properCloseParenthesis)
		{ // continue appending to attributesList phrase
			attributesList.push_back(input.at(i));
		}
		else
		{ // add to third part of selection phrase; the atomic-expr
			valuesForAtomicExpression.push_back(input.at(i));
		}
	}
	
	// this will generate a table (existing one, or combination of two, etc)
	Table fromTable = evaluateAtomicExpression(valuesForAtomicExpression);
    /*------Fix----*/
	Table projectionTable = Database::Project(attributesList, fromTable);
	return projectionTable;
}
Table Parser::rename(vector<string> input)
{
	// rename a table according
	// renaming::= rename ( attribute-list ) atomic-expr

	bool selectKeyword = (input.at(0) == "rename");
	if (!selectKeyword)
	{
		throw new exception("Invalid renaming call");
		return NULL;
	}
	bool properOpenParenthesis = input.at(1) == "(";
	bool properCloseParenthesis = false;


	vector<string> attributesList;
	vector<string> valuesForAtomicExpression;
	unsigned int i;
	for (i = 2; i < input.size(); i++)
	{
		string temp = input.at(i);
		if (temp == ")")
		{ // done adding to attributesList phrase if parenthesis are closed
			properCloseParenthesis = true;
		}
		else if (!properCloseParenthesis)
		{ // continue appending to attributesList phrase
			attributesList.push_back(input.at(i));
		}
		else
		{ // add to third part of rename; the atomic-expr
			valuesForAtomicExpression.push_back(input.at(i));
		}
	}


	Table fromTable = evaluateAtomicExpression(valuesForAtomicExpression);
	Table renameTable = Database::rename_table(&fromTable, attributesList);

	return renameTable;
}




//returns the union, difference, etc. table based on arthOperator
Table Parser::parseExpression(vector <string> expr, string arthOperator)
{
	//gets index of set manipulation operator: +,-,*, or JOIN
	int index = distance(expr.begin(), find(expr.begin(), expr.end(), arthOperator));
	cout << "Index that " << arthOperator << " is at in expression: " << index << "\n";

	vector<string>::const_iterator beginning = expr.begin();
	vector<string>::const_iterator end = expr.begin() + index;
	vector<string> beforeOperator(beginning, end);

	cout << "vector before the " << arthOperator << ": ";
	for (unsigned int i = 0; i < beforeOperator.size(); i++)
		cout << beforeOperator.at(i) << " ";
	cout << "\n";

	beginning = expr.begin() + index + 1;
	end = expr.end();
	vector<string> afterOperator(beginning, end);

	cout << "vector after the " << arthOperator << ": ";
	for (unsigned int i = 0; i < afterOperator.size(); i++)
		cout << afterOperator.at(i) << " ";
	cout << "\n";

	if (arthOperator == "+")
		return Database::setunion(evaluateAtomicExpression(beforeOperator), evaluateAtomicExpression(afterOperator));
	else if (arthOperator == "-")
		return Database::differenceTable(evaluateAtomicExpression(beforeOperator), evaluateAtomicExpression(afterOperator));
	else if (arthOperator == "*")
		//return Database::productTable(interpretAtomicExpression(beforeOperator), interpretAtomicExpression(afterOperator));
		return Database::productTable(evaluateAtomicExpression(beforeOperator), evaluateAtomicExpression(afterOperator));
	
	else if (arthOperator == "JOIN")
		return Database::naturalJoinTable(evaluateAtomicExpression(beforeOperator), evaluateAtomicExpression(afterOperator));
	
	throw new exception("Invalid call to parseExpression for union/diff/prod/join");
	return NULL;
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
        return projection(expr);// Elliut
	}
	else if (first == "rename")
	{ // renaming
        return rename(expr);// Elliut
	}
	else if (find(expr.begin(), expr.end(), "+") != expr.end())
	{ // union ::= atomic-expr + atomic-expr
		return parseExpression(expr, "+");
	}
	else if (find(expr.begin(), expr.end(), "-") != expr.end())
	{ // difference ::= atomic-expr - atomic-expr
		return parseExpression(expr, "-");
	}
	else if (find(expr.begin(), expr.end(), "*") != expr.end())
	{ // product ::= atomic-expr * atomic-expr
		return parseExpression(expr, "*");
	}
	else if (find(expr.begin(), expr.end(), "JOIN") != expr.end())
	{ // natural-join ::= atomic-expr JOIN atomic-expr
		return parseExpression(expr, "JOIN");
	}
	else if (expr.size() == 1)
	{ // atomic-expr, just the relation-name
		return evaluateAtomicExpression(expr);
	}

	return Table();
}

bool Parser::insertCmd(vector<string> input)
{ // insert into a table from explicit values or one obtained from another table

	string relationName = input.at(2);	// name of Table in the Database
	Table* t = Database::getTableByReference(relationName);

	if (input.at(5) == "RELATION")
	{ // insert-cmd ::= INSERT INTO relation-name VALUES FROM RELATION expr
		vector<string> expression;
		for (unsigned int i = 6; i < input.size(); i++)
		{ // fill expression vector with the values following the word, 'RELATION'
			expression.push_back(input.at(i));
		}
		Table newValues = getTableFromExpression(expression);
		for (unsigned int i = 0; i < newValues.getEntries().size(); i++)
		{ // add every entry of the table of new values to the table referenced by relationName
			t->addEntry(newValues.getEntries().at(i));
		}
		return true;
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
			Table t = Database::getTable(relationName);
			t.addEntry(fields);
			return true;
		}
	}
	return false;
}
bool Parser::exitCmd(vector<string> input)
{
    return false;
}

void Parser::ShowCmd(vector<string> input)
{
    
    vector<string> atomicExpression(input.begin()+1, input.end());
	Table t(evaluateAtomicExpression(atomicExpression));
    t.printTable();

}
bool Parser::createCmd(vector<string> input)
{// create-cmd ::= CREATE TABLE relation-name(typed-attributed-list) PRIMARY KEY (attributed-list)
   
	string relationName = input.at(2);	// name of Table in the Database
	Table* t = Database::getTableByReference(relationName);
    bool properOpenParenthesis = input.at(3) == "(";
	bool properCloseParenthesis = input.at(input.size() - 1) == ")";
	bool complete = false;
	if (complete)
	{

		for (unsigned int i = 3; i < input.size(); i++)
		{

			if (properOpenParenthesis)
			{	
				for (unsigned int i = 4; i < input.size(); i++)
				{
					if (input.at(i) == ")" && input.at(i+1) =="PRIMARY")
					{
						break;
					}
					else
					{
						// fill attributeTypedList vector with the values following , '(',
						vector<string> attributeTypeList;
						attributeTypeList.push_back(input.at(i));
						//evaluateTypeAttributeList(attributeTypeList);
					}
				}
			}

			if (input.at(i) == "KEY" && properCloseParenthesis)
			{
				vector<string> attributeList;
				int referencePoint = i + 1;
				for (unsigned int i = referencePoint; i < input.size() - 1; i++)
				{ // fill attributeList vector with the values following the word, 'KEY', should be one or more attribute-names
					attributeList.push_back(input.at(i));
				}
				//Table t = Database::getTable(relationName);
				complete = true;
			}

		}
		return true;
	}

	return false;

}
bool Parser::updateCmd(vector<string> input)
{
    string relationName = input.at(1);	// name of Table in the Database
	Table* t = Database::getTableByReference(relationName);
	bool Set = input.at(2) == "SET";

	for (unsigned int i = 2; i < input.size(); i++)
	{

		if (Set)
		{
			vector<string> setAttribute;
			if (input.at(i+1) == "WHERE")
			{
				break;
			}
			else
			{
				for (unsigned int i = 3; i < input.size(); i++)
				{
					setAttribute.push_back(input.at(i));
				}
			}
		}


		if (input.at(i) == "WHERE")
		{
			vector<string>whereCondition;

			for (unsigned int i = 0; i < input.size() - 1;i++)
			{
				whereCondition.push_back(input.at(i));
			}

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
bool Parser::isLiteral(string s)
{ // return true if the string is enclose in quotation marks
	return (s[0] == '\"' && s[s.size()] == '\"');
}
int Parser::readInteger(string& word, string input, int inputIndex)
{ // read an integer from input, starting at inputIndex, assign it to word
	string myWord = "";
	unsigned int myIndex = inputIndex;
	char nextDigit = input.at(myIndex);
	while (isdigit(nextDigit))
	{
		myWord += nextDigit;
		myIndex++;
		if (myIndex >= input.length())
		{
			break;
		}
		nextDigit = input.at(myIndex);
	}
	word = myWord;
	return (myIndex - inputIndex); // return how many characters were read
}

int Parser::readType(string& word, string input, int inputIndex)
{ // read a type from input, starting at inputIndex, assign it to word
	int myIndex = inputIndex;
	string tempWord = input.substr(inputIndex, 7); // assumes types are VARCHAR or INTEGER, both 7 chars
	if (isType(tempWord))
	{
		myIndex += tempWord.length();
		word = tempWord;
	}
	return (myIndex - inputIndex + 1); // return how many characters were read
}
int Parser::readOp(string& op, string input, unsigned int inputIndex)
{ // will read a one or two char operand from input, starting at inputIndex
	// valid operands are: <,<=,>,>=,<-,==,!=

	char c1 = input.at(inputIndex);
	
	if (!isOp(c1))
	{
		throw new exception("Invalid call to readOp");
	}

	string myOp = "";
	myOp += c1;

	if (inputIndex + 1 < input.size())
	{ // look at next char if not past end of string
		char c2 = input.at(inputIndex + 1);
		if (isOp(c2))
		{
			myOp += c2;
		}
	}

	op = myOp;
    return myOp.size(); // return how many characters were read
}
int Parser::readLiteral(string& word, string input, unsigned int inputIndex)
{
    string myWord = "";
	unsigned int myIndex = inputIndex;
	if (input.at(myIndex) == '\"')
	{ // if theres an opening quote
		for (; myIndex < input.size(); myIndex++)
		{
			char c = input.at(myIndex);
			myWord += c;
			if (c == '\"' && myIndex > inputIndex)
			{ // if end of literal is reached; the closing quote
				break;
			}
		}
	}
    word = myWord;
    return (myIndex - inputIndex + 1); // return how many characters were read
}
int Parser::readIdentifier(string& word, string input, int inputIndex)
{
	unsigned int myIndex = inputIndex;
	string myWord = "";
	
	for (; myIndex < input.size(); myIndex++)
	{
		char c = input.at(myIndex);
		if (isalpha(c) || isdigit(c) || c == '_')
		{
			myWord += c;
		}
		else break;
	}
	word = myWord;
	return (myIndex - inputIndex);
}

