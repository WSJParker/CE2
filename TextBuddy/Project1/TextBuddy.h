#ifndef TEXTBUDDY_H
#define TEXTBUDDY_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
using namespace std;

class TextBuddy {

public:
        string textFileName;

        //feedback messages
        static const string WELCOME_MESSAGE;
        static const string MESSAGE_SEARCH_COMPLETED;
		static const string MESSAGE_SORT_COMPLETED;
        static const string MESSAGE_NO_CONTENT;
        static const string MESSAGE_CONTENT_CLEARED;
        static const string MESSAGE_FILE_ERROR;
        static const string MESSAGE_INVALID_FORMAT;
        static const string MESSAGE_EMPTY_STRING;
        static const string MESSAGE_FILE_NAME_LENGTH_EXCEEDED;

        static const string EMPTY_STRING;

        static const int BUFFER_LENGTH;
        static const int NOT_FOUND;

        enum CommandType {
                ADD_TASK, DELETE_TASK,
                CLEAR_ALL_TASKS, DISPLAY_TASKS,
                SEARCH_KEYWORD,SORT_TASK, GET_TOTAL_TASKS, EXIT,
                INVALID
        };

        enum DeleteType {
                SINGLE_TASK,  INVALID_DELETE_TYPE
        };

        enum Capacity {
                EMPTY, NOT_EMPTY
        };

        //file related methods
        void setTextFileName(string inputFileName);
        string parseTextFileName();
        void createTextFile();
        string appendFileExtensionType(string textFileName);

        //user input parsing methods
        string removeLeadingTrailingWhitespaces(string userInput);
        string getAction(string userInput);
        string getRemainingWords(string userInput);

        //helper methods
        Capacity isEmptyTasksList();
        CommandType determineCommandType(string userCommand);

        //deletion methods
        void extractNumbersFromUserInput(string text, vector<string> &numbers);
        void getDeleteSingleTaskIndex(string text, int &taskIndexOne);
        string writeTasksFromTextFileToVector(vector<string>& taskList, int taskIndexOne);
        void writeTasksFromVectorToTextFile(vector<string>& taskList);

        //execution methods
        
        string deleteSingleTask(string text);
        string displayTasks();
        string clearAllTasks();
		string searchKeyword(string searchTerm);
		string sortTasks();
		string addTask(string validatedUserInput);

        TextBuddy(string inputFileName);
        ~TextBuddy();
        
        void showWelcomeMessage();
        void showToUser(string feedback);

        string executeCommand(string userInput);
};

#endif