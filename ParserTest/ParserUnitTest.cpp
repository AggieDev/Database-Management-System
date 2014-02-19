#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ParserTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestReadInteger)
		{
			// modify the word parameter (1st)
			// don't modify input parameter (2nd)
			// return number of characters read
			string word = "old value";
			string input = "Read only five digits, 12345 is the integer to be read";
			int inputIndex = 23;
			
			Parser p = Parser();
			int count = p.readInteger(word, input, inputIndex);

			Assert::AreEqual(string("12345"), word);
			Assert::AreNotEqual(string("old value"), word);
			Assert::AreEqual(5, count);
			Assert::AreEqual(23, inputIndex);
		}
		TEST_METHOD(TestReadInputLineWithIntegers)
		{ // An input line with multiple integers should be read into a vector where each integer has a spot
			Parser p = Parser();
			//string sampleInput = "CREATE TABLE baseball_players (fname VARCHAR(20), lname VARCHAR(30), team VARCHAR(20), homeruns INTEGER, salary INTEGER) PRIMARY KEY (fname, lname);";
			string sampleInput = "123, 789;";
			vector<string> inputVector = p.readInputLine(sampleInput);
			Assert::AreEqual(2, (int)inputVector.size()); // should have these contents: [ 123 , 789 ]
		}
		TEST_METHOD(TestInterpretAtomicExpression_RelationName)
		{ // test atomic expression if it is of the simple form:
			//		atomic-expr ::= relation-name
			
			// first make a database that has the table we are looking for
			Table t = Table("dbTableName");
			Database::addTable(t);
			Parser p = Parser();


			// make an atomic expression with only one value; a relation-name
			vector<string> AtomicExpression_RelationName;
			AtomicExpression_RelationName.push_back("dbTableName");

			// get a table using the parser's interpret atomic expression function
			Table interpretedTable = p.interpretAtomicExpression(AtomicExpression_RelationName);

			// the table retrieved should be the one we added to the database
			Assert::AreEqual(string("dbTableName"), interpretedTable.getName());
		}
		TEST_METHOD(TestInterpretAtomicExpression_Union)
		{ // test atomic expression if it is of the simple form:
			//		atomic-expr ::= relation-name

			// first make a database that has the table we are looking for
			vector<string> columnNames;
			columnNames.push_back("Names");
			columnNames.push_back("Ages");
			Table first = Table("DiffTest1", columnNames, 2);
			Table second = Table("DiffTest2", columnNames, 2);
			Table unionTable = Database::setunion(first, second);
			Database::addTable(unionTable);
			Parser p = Parser();

			// make an atomic expression with only one value; a relation-name
			vector<string> AtomicExpression_UnionName;
			AtomicExpression_UnionName.push_back("table1");
			AtomicExpression_UnionName.push_back("+");
			AtomicExpression_UnionName.push_back("table2");

			// get a table using the parser's interpret atomic expression function
			Table interpretedTable = p.interpretAtomicExpression(AtomicExpression_UnionName);

			// the table retrieved should be the one we added to the database
			//Assert::AreEqual(unionTable.getColNames().size(), interpretedTable.getColNames().size());
			//Assert::AreEqual(unionTable.getColTypes().size(), interpretedTable.getColTypes().size());
			//Assert::AreEqual(unionTable.getNumCols(), interpretedTable.getNumCols());
			//Assert::AreEqual(unionTable.getEntries().size(), interpretedTable.getEntries().size());
		}
		TEST_METHOD(TestSelectionCall)
		{ // test atomic expression if it is of the more complicated expression
			//		atomic-expr ::= ( expr )
			Parser p = Parser();

			vector<string> columns;
			columns.push_back("index");
			columns.push_back("team");
			Table t = Table("baseball_players", columns, 2);

			vector<string> entryFields1;
			entryFields1.push_back("1");
			entryFields1.push_back("Elephants");
			vector<string> entryFields2;
			entryFields2.push_back("2");
			entryFields2.push_back("Dinosaurs");
			vector<string> entryFields3;
			entryFields3.push_back("3");
			entryFields3.push_back("Giraffes");

			t.addEntry(entryFields1);
			t.addEntry(entryFields2);
			t.addEntry(entryFields3);
			Database::addTable(t);

			// this should get a table of a singleton entry (2, Dinosaurs)
			string expressionString = "select (team == \"Dinosaurs\") baseball_players;";
			vector<string> selectionExprVector = p.readInputLine(expressionString);
			Table resultTable = p.selection(selectionExprVector);

			Assert::AreEqual(1, (int)resultTable.getEntries().size());
			Assert::AreEqual(string("2"), resultTable.getEntries().at(0).fields.at(0));

		}
		TEST_METHOD(TestReadLiteral)
		{
			Parser p = Parser();
			string myLiteral = "\"literal with spaces\" and the rest of my line";
			string wordToBeSet;

			// should read all characters in the literal; 21
			int litStartIndex = 0;
			int charsRead = p.readLiteral(wordToBeSet, myLiteral, litStartIndex);

			Assert::AreEqual(21, charsRead);
			Assert::AreEqual(wordToBeSet, string("\"literal with spaces\""));
		}

	};
}