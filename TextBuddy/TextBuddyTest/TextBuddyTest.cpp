#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTest
{		
	TEST_CLASS(TextBuddyTest)
	{
	public:
		
		TEST_METHOD(TestSearch)
		{
			// TODO: Your test code here
			string fileName = "unitTestSearch";
			string Keyword = "what";
			string test1 = "what is this";
			string test2 = "what are those?";
			string test3 = "They are nothing";
			string result = test1 + "\n" + test2 + "\n" + "Search Completed.\n";
			
			TextBuddy testTextBuddy(fileName);

			testTextBuddy.addTask(test1);
			testTextBuddy.addTask(test2);
			testTextBuddy.addTask(test3);
			string searchResults = testTextBuddy.searchKeyword(Keyword);

			Assert::AreSame(result, searchResults); 
			testTextBuddy.clearAllTasks();
			
		}

	};
}