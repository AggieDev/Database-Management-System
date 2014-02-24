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

			string inputNumAtEnd = "Some text 9876";
			inputIndex = 10;
			count = p.readInteger(word, inputNumAtEnd, inputIndex);
			Assert::AreEqual(string("9876"), word);
		}
		TEST_METHOD(TestReadInputLine)
		{ // An input line with multiple integers should be read into a vector where each integer has a spot
			Parser p = Parser();
			string sampleInput1 = "CREATE TABLE baseball_players (fname VARCHAR(20), lname VARCHAR(30), team VARCHAR(20), homeruns INTEGER, salary INTEGER) PRIMARY KEY (fname, lname);";
			string sampleInput2 = "INSERT INTO animals VALUES FROM (\"Joe bob\", \"bird\", 2);";
			string sampleInput3 = "product_test <- shapes * colors;";
			string sampleInput4 = "SHOW no_such_relation;";
			
			vector<string> inputVector1 = p.readInputLine(sampleInput1);
			vector<string> inputVector2 = p.readInputLine(sampleInput2);
			vector<string> inputVector3 = p.readInputLine(sampleInput3);
			vector<string> inputVector4 = p.readInputLine(sampleInput4);
			
			Assert::AreEqual(30, (int)inputVector1.size());
			Assert::AreEqual(10, (int)inputVector2.size());
			Assert::AreEqual(5, (int)inputVector3.size());
			Assert::AreEqual(2, (int)inputVector4.size());
		}
		TEST_METHOD(TestEvaluateAtomicExpression_RelationName)
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
			Table interpretedTable = p.evaluateAtomicExpression(AtomicExpression_RelationName);

			// the table retrieved should be the one we added to the database
			Assert::AreEqual(string("dbTableName"), interpretedTable.getName());
		}
		TEST_METHOD(TestEvaluateAtomicExpression_Union)
		{ // test atomic expression if it is of the simple form:
			//		atomic-expr ::= relation-name

			// first make a database that has the table we are looking for
			Parser p = Parser();
			vector<string> expr;
			Table t1 = Table("table1");
			Table t2 = Table("table2");
			Database::addTable(t1);
			Database::addTable(t2);

			vector<string> columnNames;
			columnNames.push_back("Names");
			columnNames.push_back("Ages");
			Table first = Table("DiffTest1", columnNames, 2);
			Table second = Table("DiffTest2", columnNames, 2);
			vector<string> firstCols;
			vector<string> secondCols;
			firstCols.push_back("debra");
			firstCols.push_back("40");
			secondCols.push_back("tom");
			secondCols.push_back("30");
			first.addEntry(firstCols);
			second.addEntry(secondCols);
			Table unionTable = Database::setunion(first, second);
			Database::addTable(unionTable);

			// make an atomic expression with only one value; a relation-name
			vector<string> AtomicExpression_UnionName;
			AtomicExpression_UnionName.push_back("table1");
			AtomicExpression_UnionName.push_back("+");
			AtomicExpression_UnionName.push_back("table2");

			// get a table using the parser's interpret atomic expression function
			Table interpretedTable = p.evaluateAtomicExpression(AtomicExpression_UnionName);
			cout << "manual union table then interpreted table:\n=================\n\n";
			unionTable.printTable();
			interpretedTable.printTable();

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
			vector<char> colTypes;
			colTypes.push_back('s');
			colTypes.push_back('s');

			Table t = Table("baseball_players", columns, colTypes);

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
			string expressionString = "select (team == Dinosaurs) baseball_players;";
			vector<string> selectionExprVector = p.readInputLine(expressionString);
			Table resultTable = p.selection(selectionExprVector);

			Assert::AreEqual(1, (int)resultTable.getEntries().size());
			Assert::AreEqual(string("2"), resultTable.getEntries().at(0).fields.at(0));

		}
		TEST_METHOD(TestReadLiteralAndIdentifier)
		{
			Parser p = Parser();
			string word;
			string myLiteral = "0123456789\"literal with spaces\" and the rest of my line";
			string litAtEnd = "0123 \"Literal\"";
			
			// should read all characters in first literal; 21
			int startIndex = 10;
			int charsRead = p.readLiteral(word, myLiteral, startIndex);

			Assert::AreEqual(21, charsRead);
			Assert::AreEqual(word, string("\"literal with spaces\""));

			// try with second literal
			startIndex = 5;
			charsRead = p.readLiteral(word, litAtEnd, startIndex);
			Assert::AreEqual(9, charsRead);
			Assert::AreEqual(string("\"Literal\""), word);

			// Test readIdentifier here also (very similar)
			string someID = "0123 iden_345_ier hello";
			startIndex = 5;
			charsRead = p.readIdentifier(word, someID, startIndex);
			Assert::AreEqual(12, charsRead);
			Assert::AreEqual(string("iden_345_ier"), word);

			someID = "012345 ident_12";
			startIndex = 7;
			charsRead = p.readIdentifier(word, someID, startIndex);
			Assert::AreEqual(8, charsRead);
			Assert::AreEqual(string("ident_12"), word);




		}
		TEST_METHOD(TestReadOp)
		{
			Parser p = Parser();
			string oneCharOp = "012345>";
			string twoCharOp = "012345 <= 98765";
			string opAtEnd = "01234>=";
			string op1, op2, op3;

			// readOp should set op's value to the string "<=", and count 2 characters
			int oneCharRead = p.readOp(op1, oneCharOp, 6);
			int twoCharsRead = p.readOp(op2, twoCharOp, 7);
			int twoCharsAtEnd = p.readOp(op3, opAtEnd, 5);
			

			Assert::AreEqual(1, oneCharRead);
			Assert::AreEqual(string(">"), op1);
			Assert::AreEqual(2, twoCharsRead);
			Assert::AreEqual(string("<="), op2);
			Assert::AreEqual(2, twoCharsAtEnd);
			Assert::AreEqual(string(">="), op3);
		}
		
		TEST_METHOD(TestDatabaseSelect)
		{	// Test the database select function

			Parser p = Parser();

			// Build a Sample Table
			vector<string> attributes;
			vector<char> attTypes;
			attributes.push_back("FirstName");
			attTypes.push_back('s');
			attributes.push_back("LastName");
			attTypes.push_back('s');
			attributes.push_back("Age");
			attTypes.push_back('i');
			attributes.push_back("Price");
			attTypes.push_back('f');

			Table table("Testing", attributes, attTypes);

			vector<string> entry1;
			entry1.push_back("Eliutt");
			entry1.push_back("Rivera");
			entry1.push_back("20");
			entry1.push_back("205");

			// only this entry should be in results table
			vector<string> entry2;
			entry2.push_back("Bob");
			entry2.push_back("Sagget");
			entry2.push_back("40");
			entry2.push_back("5025");

			table.addEntry(entry1);
			table.addEntry(entry2);
			
			Database::addTable(table);

			// Condition
			vector<string> condition;
			condition.push_back("Price"); //left
			condition.push_back("!="); //center
			condition.push_back("205"); //right

			Table results = Database::select(condition, table);

			Table tabletest("Testing", attributes, attTypes);
			tabletest.addEntry(entry2);
			
			Assert::AreEqual(0, results.hasEntry(entry2));
			Assert::AreEqual(1, (int)results.getEntries().size());
		}
		TEST_METHOD(TestDatabaseProject)
		{
			// Trying to test the database project function
			Parser p = Parser();
			vector<string> attributes;
			vector<char> attTypes;
			attributes.push_back("FirstName");
			attTypes.push_back('s');
			attributes.push_back("LastName");
			attTypes.push_back('s');
			attributes.push_back("Age");
			attTypes.push_back('i');
			attributes.push_back("Price");
			attTypes.push_back('f');
			Table table("Test", attributes, attTypes);

			vector<string> entry1;
			entry1.push_back("Eli");
			entry1.push_back("Riv");
			entry1.push_back("1");
			entry1.push_back("45");

			vector<string> entry2;
			entry2.push_back("pat");
			entry2.push_back("green");
			entry2.push_back("32");
			entry2.push_back("50.25");

			table.addEntry(entry1);
			table.addEntry(entry2);
			Database::addTable(table);
			
			vector<string> projectAttr;
			projectAttr.push_back("FirstName");
			projectAttr.push_back("LastName");
			projectAttr.push_back("Price");

			Table results = Database::Project(projectAttr, table);

			Assert::AreEqual(3, results.getNumCols());
			Assert::AreEqual(string("FirstName"), results.getColNames().at(0));
			Assert::AreEqual(string("LastName"), results.getColNames().at(1));
			Assert::AreEqual(string("Price"), results.getColNames().at(2));
		}
	

		TEST_METHOD(TestProjectCall)
		{ // test atomic expression if it is of the more complicated expression
			//		atomic-expr ::= ( expr )
			Parser p = Parser();

			makeSampleTables();


			// this should get a table of a singleton entry (2, Dinosaurs)
			string expressionString = "project (team, name) baseball_players;";
			vector<string> projectionExprVector = p.readInputLine(expressionString);
			Table resultTable = p.projection(projectionExprVector);

			// check resultTable's size and column names
			Assert::AreEqual(5,(int)resultTable.getEntries().size());
			Assert::AreEqual(2, resultTable.getNumCols());
			Assert::AreEqual(resultTable.getColNames().at(0), string("team"));
			Assert::AreEqual(resultTable.getColNames().at(1), string("name"));
			
			// check a few entry fields
			Assert::AreEqual(string("Elephants"), resultTable.getEntries().at(0).fields.at(0));
			Assert::AreEqual(string("SanFran"), resultTable.getEntries().at(3).fields.at(0));

			Assert::AreEqual(string("\"hernandez\""), resultTable.getEntries().at(1).fields.at(1));
			Assert::AreEqual(string("\"Slim Shady\""), resultTable.getEntries().at(4).fields.at(1));

		}
		TEST_METHOD(TestDatabaseRename)
		{
			Parser p = Parser();
			
			vector<string> attributes;
			vector<char> attTypes;
			attributes.push_back("FirstName");
			attTypes.push_back('s');
			attributes.push_back("LastName");
			attTypes.push_back('s');
			attributes.push_back("Age");
			attTypes.push_back('s');
			attributes.push_back("Price");
			attTypes.push_back('s');
			Table table("Test", attributes, attTypes);

			vector<string> entry1;
			entry1.push_back("Eli");
			entry1.push_back("Riv");
			entry1.push_back("1");
			entry1.push_back("45");

			vector<string> entry2;
			entry2.push_back("pat");
			entry2.push_back("green");
			entry2.push_back("32");
			entry2.push_back("50.25");

			table.addEntry(entry1);
			table.addEntry(entry2);
			Database::addTable(table);


			vector<string> renamedAttr;
			renamedAttr.push_back("First");
			renamedAttr.push_back("Last");
			renamedAttr.push_back("birth");
			renamedAttr.push_back("money");
			

			Table results = Database::rename_table(&table,renamedAttr);

			Table tabletest("Testing", attributes, attTypes);
			tabletest.addEntry(entry1);
			tabletest.addEntry(entry2);


			Assert::AreEqual(results.getColNames().at(0), string("First"));
			Assert::AreEqual(results.getColNames().at(1), string("Last"));
			Assert::AreEqual(results.getColNames().at(2), string("birth"));
			Assert::AreEqual(results.getColNames().at(3), string("money"));

		}
		TEST_METHOD(TestRenameCall)
		{ // test atomic expression if it is of the more complicated expression
			//		atomic-expr ::= ( expr )
			Parser p = Parser();

			vector<string> columns;
			columns.push_back("index");
			columns.push_back("team");
			columns.push_back("name");
			vector<char> colTypes;
			colTypes.push_back('s');
			colTypes.push_back('s');
			colTypes.push_back('s');

			Table t = Table("baseball_players", columns, colTypes);

			vector<string> entryFields1;
			entryFields1.push_back("1");
			entryFields1.push_back("Elephants");
			entryFields1.push_back("jose");
			vector<string> entryFields2;
			entryFields2.push_back("2");
			entryFields2.push_back("Dinosaurs");
			entryFields2.push_back("hernandez");
			vector<string> entryFields3;
			entryFields3.push_back("3");
			entryFields3.push_back("Giraffes");
			entryFields3.push_back("palermo");

			t.addEntry(entryFields1);
			t.addEntry(entryFields2);
			t.addEntry(entryFields3);
			Database::addTable(t);

			
			string expressionString = "rename (id,teamname,playername) baseball_players;";
			vector<string> renameExprVector = p.readInputLine(expressionString);
			Table resultTable = p.rename(renameExprVector);
			
			Assert::AreEqual(resultTable.getColNames().at(0), string("id"));
			Assert::AreEqual(resultTable.getColNames().at(1), string("teamname"));
			Assert::AreEqual(resultTable.getColNames().at(2), string("playername"));
			

		}
		TEST_METHOD(TestRemoveParenthesis)
		{
			vector<string> noParen;
			noParen.push_back("a");
			noParen.push_back("short");
			noParen.push_back("sentence");
			vector<string> withParen;
			withParen.push_back("(");
			withParen.push_back("I");
			withParen.push_back("have");
			withParen.push_back("parenthesis");
			withParen.push_back(")");
			
			Parser p = Parser();
			p.removeParenthesis(&noParen);
			p.removeParenthesis(&withParen);

			Assert::AreEqual(string("a"), noParen.at(0));
			Assert::AreEqual(string("short"), noParen.at(1));
			Assert::AreEqual(string("sentence"), noParen.at(2));

			Assert::AreEqual(string("I"), withParen.at(0));
			Assert::AreEqual(string("have"), withParen.at(1));
			Assert::AreEqual(string("parenthesis"), withParen.at(2));
		}
		TEST_METHOD(TestEvaluateTypeAttributeList)
		{ // a typed attribute list describes how a table will be organized (needed for create-cmd)
			Parser p = Parser();

			string attrListString = "(name VARCHAR(20), kind VARCHAR(8), years INTEGER)";
			vector<string> attrListVec = p.readInputLine(attrListString);

			vector<string> attributes;
			vector<char> colTypes;

			p.evaluateTypeAttributeList(attrListVec, &attributes, &colTypes);

			// attributes and colTypes vectors should have been updated according to the attr-list
			Assert::AreEqual(string("name"), attributes.at(0));
			Assert::AreEqual(string("kind"), attributes.at(1));
			Assert::AreEqual(string("years"), attributes.at(2));

			Assert::AreEqual('s', colTypes.at(0));
			Assert::AreEqual('s', colTypes.at(1));
			Assert::AreEqual('i', colTypes.at(2));
			
		}
		TEST_METHOD(TestCreateCommand)
		{ // a typed attribute list describes how a table will be organized (needed for create-cmd)
			Parser p = Parser();
			

			string attrListString = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY(name, kind)";
			vector<string> attrListVec = p.readInputLine(attrListString);
			
			Table test = p.createCmd(attrListVec);
			
			Assert::AreEqual(string("name"), test.getColNames().at(0));
			Assert::AreEqual(string("kind"), test.getColNames().at(1));
			Assert::AreEqual(string("years"), test.getColNames().at(2));

		}

		TEST_METHOD(TestFindCondition)
		{	// check Table::findCondition(vector<string> whereOps)
			// whereOps.size() == 3 and of form operand1 op operand2
			
			Parser p = Parser();

			// create a table
			vector<string> columns;
			columns.push_back("index");
			columns.push_back("team");
			columns.push_back("name");
			vector<char> colTypes;
			colTypes.push_back('s');
			colTypes.push_back('s');
			colTypes.push_back('s');

			Table t = Table("baseball_players", columns, colTypes);

			vector<string> entryFields1;
			entryFields1.push_back("1");
			entryFields1.push_back("Elephants");
			entryFields1.push_back("\"jose\"");
			t.addEntry(entryFields1);

			vector<string> entryFields2;
			entryFields2.push_back("2");
			entryFields2.push_back("Dinosaurs");
			entryFields2.push_back("\"hernandez\"");
			t.addEntry(entryFields2); 
			
			vector<string> entryFields3;
			entryFields3.push_back("3");
			entryFields3.push_back("Dinosaurs");
			entryFields3.push_back("\"palermo\"");
			t.addEntry(entryFields3);

			// build condition strings
			string condString1 = "team == Dinosaurs";
			string condString2 = "index < 3";
			string condString3 = "name == \"palermo\"";
			string condString4 = "team != Dinosaurs";

			// parse them into condition vectors
			vector<string> condVec1 = p.readInputLine(condString1);
			vector<string> condVec2 = p.readInputLine(condString2);
			vector<string> condVec3 = p.readInputLine(condString3);
			vector<string> condVec4 = p.readInputLine(condString4);

			// pass them to Table::findCondition()
			// this function returns vector<int> => indices of the valid entries
			vector<int> validEntries1 = t.findCondition(condVec1);
			vector<int> validEntries2 = t.findCondition(condVec2);
			vector<int> validEntries3 = t.findCondition(condVec3);
			vector<int> validEntries4 = t.findCondition(condVec4);

			// ensure the correct number of entries were retrieved
			Assert::AreEqual(2, (int)validEntries1.size());
			Assert::AreEqual(2, (int)validEntries2.size());
			Assert::AreEqual(1, (int)validEntries3.size());
			Assert::AreEqual(1, (int)validEntries4.size());

			// ensure the correct indices (from original) were collected
			Assert::AreEqual(1, validEntries1.at(0)); // "team == Dinosaurs"
			Assert::AreEqual(2, validEntries1.at(1));

			Assert::AreEqual(0, validEntries2.at(0)); // "index < 3"
			Assert::AreEqual(1, validEntries2.at(1));

			Assert::AreEqual(2, validEntries3.at(0)); // "name == \"palermo\""

			Assert::AreEqual(0, validEntries4.at(0)); // "team != Dinosaurs"
		}
		TEST_METHOD(TestTableStringOperatorCompare)
		{	// creating an operator from a string representation
			Table t = Table();
			bool shouldBeTrue, shouldBeFalse;

			shouldBeTrue = t.stringOperatorCompare("TeSt1", "==", "TeSt1");
			shouldBeFalse = t.stringOperatorCompare("TeSt1", "!=", "TeSt1");
			Assert::AreEqual(true, shouldBeTrue);
			Assert::AreEqual(false, shouldBeFalse);

			shouldBeTrue = t.stringOperatorCompare("45", "<", "412345");
			shouldBeFalse = t.stringOperatorCompare("124", ">", "124");
			Assert::AreEqual(true, shouldBeTrue);
			Assert::AreEqual(false, shouldBeFalse);

			shouldBeTrue = t.stringOperatorCompare("45", "<=", "45");
			shouldBeFalse = t.stringOperatorCompare("123", ">=", "124");
			Assert::AreEqual(true, shouldBeTrue);
			Assert::AreEqual(false, shouldBeFalse);
		}
		TEST_METHOD(TestInsertCmd_Option1)
		{	// insert-cmd ::= INSERT INTO relation-name VALUES FROM RELATION expr
			
			makeSampleTables(); // see utility function below
			Parser p = Parser();
			string myInsertCmd = "INSERT INTO empty_table VALUES FROM RELATION (select (index >= 3) baseball_players);";
			vector<string> inputVec = p.readInputLine(myInsertCmd);

			Table alteredTable = Database::getTable("empty_table");
			Assert::AreEqual(0, (int)alteredTable.getEntries().size());

			bool insertSuccess = p.insertCmd(inputVec);
			Assert::AreEqual(true, insertSuccess);

			alteredTable = Database::getTable("empty_table");
			Assert::AreEqual(3, (int)alteredTable.getEntries().size());
			
			string firstIndex = alteredTable.getEntries().at(0).fields.at(0);
			Assert::AreEqual(string("3"), firstIndex);

			string secondTeam = alteredTable.getEntries().at(1).fields.at(1);
			Assert::AreEqual(string("SanFran"), secondTeam);

			string thirdName = alteredTable.getEntries().at(2).fields.at(2);
			Assert::AreEqual(string("\"Slim Shady\""), thirdName);

		}
		TEST_METHOD(TestInsertCmd_Option2)
		{	// insert-cmd ::= INSERT INTO relation-name VALUES FROM(literal {, literal })
			
			makeSampleTables(); // see utility function below
			Parser p = Parser();
			string myInsertCmd = "INSERT INTO baseball_players VALUES FROM (6, Aggies, \"Timothy S\");";
			vector<string> inputVec = p.readInputLine(myInsertCmd);
			// check size of original
			Table originalTable = Database::getTable("baseball_players");
			Assert::AreEqual(5, (int)originalTable.getEntries().size());
			// insert the new entry
			bool insertSuccess = p.insertCmd(inputVec);
			Assert::AreEqual(true, insertSuccess);
			// check that size increased
			Table alteredTable = Database::getTable("baseball_players");
			Assert::AreEqual(6, (int)alteredTable.getEntries().size());
			// check the proper entry was added at the end
			Entry entry = alteredTable.getEntries().at(5);
			Assert::AreEqual(string("6"), entry.fields.at(0));
			Assert::AreEqual(string("Aggies"), entry.fields.at(1));
			Assert::AreEqual(string("\"Timothy S\""), entry.fields.at(2));
		}
		TEST_METHOD(TestDeleteCmd)
		{	// delete-cmd ::= DELETE FROM relation-name WHERE condition

			makeSampleTables();
			Parser p = Parser();
			string myDeleteCmd = "DELETE FROM baseball_players WHERE (index <= 3);";
			vector<string> inputVec = p.readInputLine(myDeleteCmd);
			// check size of original
			Table originalTable = Database::getTable("baseball_players");
			Assert::AreEqual(5, (int)originalTable.getEntries().size());
			
			// delete an entry
			p.deleteCmd(inputVec);
			// check that size decreased
			Table alteredTable = Database::getTable("baseball_players");
			Assert::AreEqual(2, (int)alteredTable.getEntries().size());

			// check the proper entries are still present
			Entry entry1 = alteredTable.getEntries().at(0);
			Entry entry2 = alteredTable.getEntries().at(1);

			Assert::AreEqual(string("4"), entry1.fields.at(0));
			Assert::AreEqual(string("SanFran"), entry1.fields.at(1));
			Assert::AreEqual(string("\"Bob Saget\""), entry1.fields.at(2));

			Assert::AreEqual(string("5"), entry2.fields.at(0));
			Assert::AreEqual(string("America"), entry2.fields.at(1));
			Assert::AreEqual(string("\"Slim Shady\""), entry2.fields.at(2));
		}
		void makeSampleTables()
		{	// create a sample table
			vector<string> columns;
			columns.push_back("index");
			columns.push_back("team");
			columns.push_back("name");

			vector<char> colTypes;
			colTypes.push_back('s');
			colTypes.push_back('s');
			colTypes.push_back('s');

			Table t1 = Table("baseball_players", columns, colTypes);
			Table t2 = Table("empty_table", columns, colTypes);

			vector<string> entryFields1;
			entryFields1.push_back("1");
			entryFields1.push_back("Elephants");
			entryFields1.push_back("\"jose\"");
			t1.addEntry(entryFields1);

			vector<string> entryFields2;
			entryFields2.push_back("2");
			entryFields2.push_back("Dinosaurs");
			entryFields2.push_back("\"hernandez\"");
			t1.addEntry(entryFields2);

			vector<string> entryFields3;
			entryFields3.push_back("3");
			entryFields3.push_back("Dinosaurs");
			entryFields3.push_back("\"palermo\"");
			t1.addEntry(entryFields3);

			vector<string> entryFields4;
			entryFields4.push_back("4");
			entryFields4.push_back("SanFran");
			entryFields4.push_back("\"Bob Saget\"");
			t1.addEntry(entryFields4);

			vector<string> entryFields5;
			entryFields5.push_back("5");
			entryFields5.push_back("America");
			entryFields5.push_back("\"Slim Shady\"");
			t1.addEntry(entryFields5);

			Database::addTable(t1);
			Database::addTable(t2);
		}
	};
}