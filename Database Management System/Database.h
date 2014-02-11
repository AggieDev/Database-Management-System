#pragma once

#include "Table.h"

class Database
{
private:
	vector<Table> _tables;
public:
	Database();
	~Database();

	void addTable(Table t){ _tables.push_back(t); }
	Table differenceTable(Table t1, Table t2);
};

