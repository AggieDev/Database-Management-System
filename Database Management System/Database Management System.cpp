// Database Management System.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "table.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Table t = Table("pets");
	Entry e1 = Entry();
	Entry e2 = Entry();
	Entry e3 = Entry();
	e1.fields.push_back("winston");
	e1.fields.push_back("patrick");

	e2.fields.push_back("harley");
	e2.fields.push_back("waylon");
	
	e3.fields.push_back("gander");
	e3.fields.push_back("rj");
	
	t.addEntry(e1);
	t.addEntry(e2);
	t.addEntry(e3);

	Entry e4 = t.getEntry(1);	//harley
	Entry e5 = Entry();
	e5 = t.getEntry(0);			// winston
	Entry e6 = Entry();
	e6 = t.getEntry(1);			//harley

	return 0;
}

