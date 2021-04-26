#include <iostream>
#include <random>
#include <list>
#include <string>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <algorithm>
#include <ctime>

using namespace std;

class TokenDetector {
    public:
        TokenDetector() {
            cout << "PROGRAM ERROR: you must use the parametized constructor in order for prog to work" << endl;
        }
        TokenDetector (const char userFile[]) {
            fstream file;
            
            file.open(userFile, ios::in);
            if (file.is_open()) {
                string preToken;

                while (file >> preToken) {
                    preToken = parseTokens (preToken);
                    if (!tokenExists(preToken)) {
                        tokensList.push_back(preToken);
                    }
                }
            }
            else {
                cout << "ERROR: file " << userFile << " does not exist" << endl;
            }
            printList();
            file.close();
        }

        list<string> getUnique () {
            return tokensList;
        }

        void printList () {
            for (list<string>::iterator i = tokensList.begin(); i != tokensList.end(); ++i) {
                string junkString (*i);
                cout << junkString << endl;
            }
        }

    private:
        list<string> tokensList;

        //Correctly parses a token so that it removes any trailing punctuation and capitilization
        string parseTokens(string inString) {
            for_each(inString.begin(), inString.end(), [&](char & curr) {
                curr = tolower(curr);
            });

            for (int i = 0; i < inString.length(); ++i) {
                if(i == (inString.length() - 1) || inString.at(i + 1) == 32) { //if we are looking at the last character OR a character before a whitespace
                    if(ispunct(inString.at(i))) {
                        inString.erase(i, 1); //Removes the single character at i
                    }
                }
            }

            return inString;
        }

        //Checks to see if token already exists in the list
        bool tokenExists(string token) {
            //Fixes error where it returns null since nothing existed yet
            if (tokensList.size() <= 0) {
                return false;
            }
            
            //Will first check the front and end of the list in cases where method can run at constant time
            if (tokensList.front().compare(token) == 0 || tokensList.back().compare(token) == 0) {
                return true;
            }

            for (list<string>::iterator i = tokensList.begin(); i != tokensList.end(); ++i) {
                string junkString (*i);
                if (junkString.compare(token) == 0) {
                    return true;
                }
            }
            return false; //Reaching means the other conditions were not able to be reached
        }
};

class PasswordGenerator {
    public:
        PasswordGenerator () {
            cout << "PROGRAM ERROR: You must use parametized constructor" << endl;
        }
        PasswordGenerator (list<string> tokens) {
            tokensList = tokens;
            iterationLength = 0;
        }

        string getRandomPassword(int numWords) {
            list<string> *paramList = new list<string>(tokensList);

            string returnString (passwordHelper(paramList, numWords));


            delete paramList;
            return returnString;
        }
        
        bool setIterationLength(int numWords) {
            if (numWords > tokensList.size()) {
                cout << "Error: the number of words exceeds the number of tokens found in the file" << endl;
                return false;
            }
            else {
                iterationLength = numWords;
                return true;
            }
        }

        

    private:
        int iterationLength;
        list<string> tokensList;
        list<string>* nextHelper; //Only instanciated in the next method

        string passwordHelper (list<string> *&paramList, int numWords) {
            srand((unsigned) time(0)); //Makes it so that seeding is random every run
            int count = 0;
            int listPos = 0;
            string returnString ("");

            for (int i = 0; i < numWords; ++i) {
                listPos = rand() % (paramList->size());
                
                for (list<string>::iterator i = paramList->begin(); i != paramList->end(); ++i) {
                    if (count == listPos) {
                        returnString.append(*i +" ");
                        paramList->erase(i);
                        count = 0;
                        break;
                    }
                    ++count;
                }
            }
            return returnString;
        }
};

int main (int argc, char **argv) {
    if (argc != 2) {
        cout << "HELP: PasswordGame.cpp text.txt" << endl;
    }
    else {
        int numTokens;
        TokenDetector test("testFile.txt");
        delete argv;

        cout << "Enter number of tokens to use in password: ";
        cin >> numTokens;

        PasswordGenerator* generator = new PasswordGenerator(test.getUnique());
        cout << generator->getRandomPassword(numTokens) << endl;

        delete generator;
    }

    return 0;
}   