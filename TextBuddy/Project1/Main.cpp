#include <iostream>
#include <string>
#include "TextBuddy.h"
using namespace std;

int main(int argc, char* argv[]) {
        TextBuddy textBuddy(argv[1]);

        textBuddy.showWelcomeMessage();

        while(true) {
                cout << "Command: ";
                string userInput;
                getline(cin, userInput);
                string feedback = textBuddy.executeCommand(userInput);
                textBuddy.showToUser(feedback);
        }

        return 0;
}