#pragma once
#include "Database.h"
#include "Table.h"
#include "Entry.h"
#include "Parser.h"

class Application
{	
	Parser parser;

	//menu functions
	string mainMenu();	//first prompt
	void printTableList();
	void createPrompt();
	void insertPrompt();
	void showPrompt();
	void unionPrompt();
	void differencePrompt();
	void productPrompt();
	void updatePrompt();
	void deletePrompt();
	void selectPrompt();
	void renamePrompt();
	void writePrompt();
	void openPrompt();
	void projectPrompt();
	

public:
	//**THESE FUNCTIONS ONLY MADE PUBLIC FOR UNIT TESTING PURPOSES**
	//keep application functions private, that way all events can happen through
	//the application class itself and not main 
	//each of these returns a query to be executed
	string createTable(string tableName, vector<string>, vector<string>);
	string insertIntoTable(string tableName, vector<string>);
	string showTable(string tableName);	//will change how this works if using GUI
	string unionTable(string, string, string);
	string differenceTable(string, string, string);
	string productTable(string, string, string);
	string updateTable(string, string, string, string);
	string deleteFromTable(string, string, string);
	string tableSelect(string, string, string, string);
	string renameTable(string, string, vector<string>);
	string writeTable(string tableName);
	string openTable(string tableName);
	string tableProjection(string, string, vector<string>);

	Application();
	~Application();

	void launch();
};

