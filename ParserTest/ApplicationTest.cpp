#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "Application.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ParserTest
{
	TEST_CLASS(ApplicationTest)
	{
	public:
		
		TEST_METHOD(TestCreateTable)
		{	// test createTable, take string and vectors and output DML String
			Application a = Application();
			string tableName = "students";
			vector<string> columnNames;
			columnNames.push_back("Age");
			columnNames.push_back("Name");

			vector<string> types;
			types.push_back("VARCHAR(20)");
			types.push_back("VARCHAR(20)");

			string dmlCreateString = a.createTable(tableName, columnNames, types);
			string properString = "CREATE TABLE students (Age VARCHAR(20), Name VARCHAR(20)) PRIMARY KEY (Age, Name);";

			Assert::AreEqual(properString, dmlCreateString);
		}
		TEST_METHOD(TestInsertIntoTable)
		{	// test createTable, take string and vectors and output DML String
			Application a = Application();
			makeSampleTables();

			string tableName = "students";

			vector<string> entryValues;
			entryValues.push_back("10");
			entryValues.push_back("newTeam");
			entryValues.push_back("Patrick S");

			string properString = "INSERT INTO students VALUES FROM (\"10\", \"newTeam\", \"Patrick S\");";
			string actualString = a.insertIntoTable(tableName, entryValues);

			Assert::AreEqual(properString, actualString);
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