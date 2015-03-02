//This TextBuddy class decides the various cases based on the commands
//and computes them accordingly.
//The main method called will be executeCommand which reads in the user input
//Feedback is given to the user via showToUser method

#include "TextBuddy.h"

//messages contain the word MESSAGE for easy searching and debugging
const string TextBuddy::WELCOME_MESSAGE = "Welcome to TextBuddy. %s is ready for use.\n";
const string TextBuddy::MESSAGE_SEARCH_COMPLETED = "Search Completed. ";
const string TextBuddy::MESSAGE_NO_CONTENT = "File is empty.\n";
const string TextBuddy::MESSAGE_CONTENT_CLEARED = "all content deleted.\n";
const string TextBuddy::MESSAGE_FILE_ERROR = "Unable to open file.";
const string TextBuddy::MESSAGE_INVALID_FORMAT = "Invalid format.\n";
const string TextBuddy::MESSAGE_EMPTY_STRING = "Empty command.\n";
const string TextBuddy::MESSAGE_FILE_NAME_LENGTH_EXCEEDED = "File name too long.\n";
const string TextBuddy::EMPTY_STRING = "";

const int TextBuddy::BUFFER_LENGTH = 255;
const int TextBuddy::NOT_FOUND = -1;

TextBuddy::TextBuddy(string inputFileName) {
        string fileName = appendFileExtensionType(inputFileName);
        setTextFileName(fileName);
        createTextFile();
}

TextBuddy::~TextBuddy() {}

void TextBuddy::showWelcomeMessage() {
        cout << parseTextFileName();
}

void TextBuddy::showToUser(string feedback) {
        cout << feedback;
}

//main command executed in the main
string TextBuddy::executeCommand(string userInput) {

        if(userInput == EMPTY_STRING) {
                return MESSAGE_INVALID_FORMAT;
        }

        string validatedUserInput = removeLeadingTrailingWhitespaces(userInput);

        if(validatedUserInput == EMPTY_STRING) {
                return MESSAGE_INVALID_FORMAT;
        } else {
                string command = getAction(validatedUserInput);
                string text = getRemainingWords(validatedUserInput);

                CommandType commandType = determineCommandType(command);
                Capacity capacity = isEmptyTasksList();

                if(capacity == EMPTY) {
                        if(commandType != ADD_TASK && commandType != GET_TOTAL_TASKS && commandType != EXIT) {
                                return MESSAGE_NO_CONTENT;
                        }
                }

                switch(commandType) {
                case ADD_TASK:
                        return addTask(text);
                case DELETE_TASK:
                        return deleteSingleTask(text);
                case DISPLAY_TASKS:
                        return displayTasks();
                case CLEAR_ALL_TASKS:
                        return clearAllTasks();
                case EXIT:
                        exit(1);
                case INVALID:
                        return MESSAGE_INVALID_FORMAT;
                }
        }
}

void TextBuddy::setTextFileName(string inputFileName) {
        textFileName = inputFileName;
}

//ensure that the filename is shorter than BUFFER_LENGTH
string TextBuddy::parseTextFileName() {
        try {
                char buffer[BUFFER_LENGTH];
                sprintf_s(buffer, WELCOME_MESSAGE.c_str(), textFileName.c_str());
                if(WELCOME_MESSAGE.length() + textFileName.length() > BUFFER_LENGTH) {
                        throw MESSAGE_FILE_NAME_LENGTH_EXCEEDED;
                }
                return buffer;
        } catch(string error) {
                return "Invalid Input: " + error;
        }
}

void TextBuddy::createTextFile() {
        fstream fileStream;
        fileStream.open(textFileName, fstream::in | fstream::app);
        fileStream.close();
}

//ensures that the file extension type is of .txt
string TextBuddy::appendFileExtensionType(string inputFileName) {
        if(inputFileName.find_last_of(".txt") == NOT_FOUND) {
                return inputFileName += ".txt";
        }

        return inputFileName;
}

string TextBuddy::removeLeadingTrailingWhitespaces(string userInput) {
        int leadingSpaceIndex = userInput.find_first_not_of(" ");
        int trailingSpaceIndex = userInput.find_last_not_of(" ");

        if(leadingSpaceIndex == NOT_FOUND && trailingSpaceIndex == NOT_FOUND) {
                return MESSAGE_INVALID_FORMAT;
        }

        return userInput.substr(leadingSpaceIndex, trailingSpaceIndex + 1);
}

string TextBuddy::getAction(string userInput) {
        stringstream strStream;
        strStream.clear();
        strStream.str(userInput);

        string command;
        strStream >> command;

        if(command == EMPTY_STRING) {
                return MESSAGE_INVALID_FORMAT;
        }

        return command;
}

string TextBuddy::getRemainingWords(string userInput) {
        stringstream strStream;
        strStream.clear();
        strStream.str(userInput);

        string command;
        string text;
        strStream >> command;
        strStream >> ws;
        strStream.clear();
        getline(strStream, text);

        return text;
}

TextBuddy::CommandType TextBuddy::determineCommandType(string userCommand) {
        if(userCommand == "add" || userCommand == "-a") {
                return ADD_TASK;
        } else if(userCommand == "delete" || userCommand == "-d") {
                return DELETE_TASK;
        } else if(userCommand == "display" || userCommand == "-dp") {
                return DISPLAY_TASKS;
        } else if(userCommand == "clear" || userCommand == "-c") {
                return CLEAR_ALL_TASKS;
        } else if(userCommand == "search" || userCommand == "-sc") {
                return SEARCH_KEYWORD;
        } else if(userCommand == "total" || userCommand == "-tot") {
                return GET_TOTAL_TASKS;
        } else if(userCommand == "exit" || userCommand == "-e") {
                return EXIT;
        } else {
                return INVALID;
        }
}

TextBuddy::Capacity TextBuddy::isEmptyTasksList() {
        fstream fileStream;
        fileStream.open(textFileName, fstream::in);

        string read;
        fileStream >> read;
        fileStream.close();

        if(read == EMPTY_STRING) {
                return EMPTY;
        }

        return NOT_EMPTY;
}

void TextBuddy::extractNumbersFromUserInput(string text, vector<string> &numbers) {
        stringstream strBuffer(text);
        string extractedWord;
        while(strBuffer) {
                strBuffer >> extractedWord;
                numbers.push_back(extractedWord);
        }
}

void TextBuddy::getDeleteSingleTaskIndex(string text, int &taskIndexOne) {
        vector<string> numbers;
        extractNumbersFromUserInput(text, numbers);

        taskIndexOne = atoi(numbers[0].c_str());
}

string TextBuddy::writeTasksFromTextFileToVector(vector<string>& taskList, int taskIndexOne) {
        fstream fileStream;
        fileStream.open(textFileName, fstream::in);

        if(!fileStream.is_open()) {
                throw(MESSAGE_FILE_ERROR);
        }

        string task;
        stringstream deletedLine;
        //store the other tasks except the task to be deleted
        for(int i=1; !getline(fileStream, task).fail(); i++) {
                if(i == taskIndexOne) {
                        deletedLine << "Deleted from " << textFileName << ": " << "\"" << task << "\n";
                        continue;
                }
                taskList.push_back(task);
        }
        fileStream.close();

        return deletedLine.str();
}

void TextBuddy::writeTasksFromVectorToTextFile(vector<string>& taskList) {
        fstream fileStream;
        fileStream.open(textFileName, fstream::out | fstream::trunc);

        if (!fileStream.is_open()) {
                throw(MESSAGE_FILE_ERROR);
        }

        for(unsigned int i=1; i<taskList.size()+1; i++) {
                fileStream << taskList[i-1] << endl;
        }
        fileStream.close();
}

string TextBuddy::addTask(string validatedUserInput) {
        fstream fileStream;
        fileStream.open(textFileName, fstream::in | fstream::app);

        if(!fileStream.is_open()) {
                throw(MESSAGE_FILE_ERROR);
        }

        if(validatedUserInput == EMPTY_STRING) {
                return MESSAGE_EMPTY_STRING;
        }

        fileStream << validatedUserInput << endl;

        fileStream.close();

        stringstream buffer;
        buffer << "Added to " << textFileName << ": \"" << validatedUserInput << "\"" << endl;

        return buffer.str();
}

string TextBuddy::deleteSingleTask(string text) {
        vector<string> taskList;
        int taskIndex;
        string feedback;

        getDeleteSingleTaskIndex(text, taskIndex);
        feedback = writeTasksFromTextFileToVector(taskList, taskIndex);
        writeTasksFromVectorToTextFile(taskList);

        return feedback;
}

string TextBuddy::displayTasks() {
        fstream fileStream;
        fileStream.open(textFileName, fstream::in);

        if(!fileStream.is_open()) {
                throw(MESSAGE_FILE_ERROR);
        }

        string output;
        stringstream displayOutput;
        for(int i=1; !getline(fileStream, output).fail(); i++) {
                displayOutput << i << ". " << output << endl;
        }
        fileStream.close();

        return displayOutput.str();
}

//this method clears all tasks by truncating the file content
string TextBuddy::clearAllTasks() {
        fstream fileStream;
        fileStream.open(textFileName, fstream::out | fstream::trunc);

        if(!fileStream.is_open()) {
                throw(MESSAGE_FILE_ERROR);
        }

        fileStream.close();

        return MESSAGE_CONTENT_CLEARED;
}