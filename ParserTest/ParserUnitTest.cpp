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

	};
}