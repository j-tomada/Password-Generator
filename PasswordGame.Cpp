#include <iostream>
#include <random>
#include <list>
#include <string>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <ctime>
#include <algorithm>

using namespace std;

/**
 * TokenDetector class takes in a file and pushes each unique word
 * into a list of strings
 * User is able to acquire tokens via getUnique()
 */
class TokenDetector {
    public:
        /**
         * Default Constructor
         * Should not be used
         */
        TokenDetector() {
            cout << "PROGRAM ERROR: you must use the parametized constructor in order for prog to work" << endl;
        }
        
        /**
         * @param fstream &file
         * Takes in a file and parses its tokens
         * Will only add to the list if it has not been added to the list rn
         */
        TokenDetector (fstream &file) {
            string preToken;
                while (file >> preToken) {
                    preToken = parseTokens (preToken);
                    if (!tokenExists(preToken)) {
                        tokensList.push_back(preToken);
                    }
                }
                printList();
        }

        /**
         * @return list<string> which has all of the unique tokens
         */
        list<string> getUnique () {
            return tokensList;
        }

        /**
         * @return int
         * Returns int of the list's size
         * Used to ensure iterationLength does not go above the size
         */
        int getSize () {
            return tokensList.size();
        }

        /**
         * Iterates through the list to print each one on
         * a separate line
         * Mainly used for testing functions from other classes
         */
        void printList () {
            for (list<string>::iterator i = tokensList.begin(); i != tokensList.end(); ++i) {
                string junkString (*i);
                cout << junkString << endl;
            }
        }

    private:
        list<string> tokensList;

        /**
         * @param string inString
         * @return string
         * iterates through the param to ensure are letters are lowercase
         * any trailing punctuations are removed from the string
         * this edited version of inString is what is returned
         */ 
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

        /**
         * @param string token
         * @boolean
         * takes in one of the tokens as the parameter and checks to see
         * if it exists
         * true if it exists
         * false if it does not exist
         */
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

/**
 * This class utilizes the list from TokenDetector
 * to create random passwords stemming from the list
 * Can also offer all possible combinations of the password
 * using next()
 */
class PasswordGenerator {
    public:
        /**
         * Default Constructor
         * Should not be used
         */
        PasswordGenerator () {
            cout << "PROGRAM ERROR: You must use parametized constructor" << endl;
        }
        /**
         * Parametized Constructor
         * @param list<string> tokens
         * Sets all of the private fields
         * Sets tokensList to tokens
         */
        PasswordGenerator (list<string> tokens) {
            tokensList = tokens;
            iterationLength = 0;
            passwordPointer = new list<string>();
            isFinished = false;
            listCopy = new list<string>(tokensList);
        }
        /**
         * @param PasswordGenerator
         * Copy Constructor
         * Copies fields from one object to another
         */
        PasswordGenerator (PasswordGenerator &copy) {
            tokensList = copy.tokensList;
            iterationLength = copy.iterationLength;
            passwordPointer = copy.passwordPointer;
            isFinished = copy.isFinished;
            listCopy = copy.listCopy;
        }
        /**
         * Destructor
         * Destroys passwordPointer and listCopy
         */
        ~PasswordGenerator () {
            delete passwordPointer;
            delete listCopy;
        }

        /**
         * @param int numWords
         * @return string of words from the tokensList
         * creates a password with up to numWords from the list
         * see passwordHelper() for more on the algorithm
         */
        string getRandomPassword(int numWords) {
            list<string> *paramList = new list<string>(tokensList);
            string returnString (passwordHelper(paramList, numWords));

            delete paramList;
            return returnString;
        }
        
        /**
         * @param int numWords
         * Setter which sets iterationLength to numWords
         */
        void setIterationLength(int numWords) {
            iterationLength = numWords;
            initializePassword();
        }

        /**
         * @return string
         * returns one of the many possible combinations based off of the 
         * class' field
         * Uses checkingPassword() as a helper
         */
        string next () {
            string returnString("");

            if (globalIterator == listCopy->end()) {
                checkingPassword();
            }
            else {
                for (list<string>::iterator i = currPassword.begin(); i != currPassword.end(); ++i) {
                    returnString.append(*i +" ");
                }

                returnString.append(*globalIterator +" ");
                ++globalIterator;
                return returnString;
            }
            return next();
        }

        /**
         * @return bool
         * Return false if there are no more possible
         * passwords returned by next()
         * return true if there is
         */
        bool hasNext () {
            if(isFinished) {
                return false;
            }
            else {
                return true;
            }

        }


    private:
        list<string>* passwordPointer; //This lists points to the previous password Iteration
        list<string> currPassword; //List of the currentPassword being returned by next()
        list<string> tokensList; //List of original tokens
        list<string>* listCopy; //Copy and pointer of tokensList
        list<string>::iterator globalIterator; //Iterates through listCopy
        int iterationLength;
        bool isFinished;    

        /**
         * Helper method for generatePassword()
         * @param list<string> *&paramList, int numWords
         * @return string
         * Algorithm to return a randomly generated password
         * based off of tokensList
         * Method ensures that each word is unique and has " " between
         */   
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

        /**
         * Initializes the first password out of the many potential combinations
         * Takes string from the beginning of listCopy and pushes it to the back of currPassword
         * Will be called again whenever something is popped out of the currPasswrod list
         */
        void initializePassword () {
            for (int i = currPassword.size(); i < (iterationLength - 1); ++i) {
                if (currPassword.size() == passwordPointer->size()) {
                    passwordPointer->push_back(listCopy->front());
                }
                currPassword.push_back(listCopy->front());
                listCopy->pop_front();
            }
            globalIterator = listCopy->begin();
        }

        /**
         * method checks to see if any of the passwordPointers have been reached
         * A pointer is reached when one part of the list matches the front of listCopy
         * When there is a match, this signifies that next() has looped through all of the possible
         * combinations on that part of currPassword
         */
        void checkingPassword () {
            listCopy->push_back(currPassword.back());
            currPassword.pop_back();

            if (listCopy->front().compare(passwordPointer->back()) == 0) {

                if(passwordPointer->size() == 1) {
                    isFinished = true;
                    return;
                }

                passwordPointer->pop_back();
                checkingPassword();
                return;
            }
            initializePassword();
        }
};

/**
 * Class runs a function as soon as an object of this type
 * is created
 * Runs both methods of guessing the password together in a while loop
 * when one of the methods has produced the given password, the method stops
 */
class PasswordGuesser {
    public:
        /**
         * Default Constructor
         * Should not be reached
         */
        PasswordGuesser() {
            cout << "ERROR: Should not be reached" << endl;
        }
        
        /**
         * Parametized Constructor
         * @param string password, int iterationLength, PasswordGenerator &generator
         * puts these parameter into performCalculation method
         */
        PasswordGuesser(string password, int iterationLength, PasswordGenerator &generator) {
            performCalculation(password, iterationLength, generator);
        }

        /**
         * @param string password, int iterationLength, PasswordGenerator &generator
         * Performs both methods of trying to guess the password at the same time
         * When one of them has reached, the method sends return
         */
        void performCalculation (string password, int iterationLength, PasswordGenerator &generator) {
            while (generator.hasNext()) {
                if (generator.next().compare(password) == 0) {
                    cout << "The brute force method solved it first" << endl;
                    return;
                }

                if (generator.getRandomPassword(iterationLength).compare(password) == 0) {
                    cout << "The requesting random passwords method solved it first" << endl;
                    return;
                }
            }
            cout << "Neither methods were able to find the password in time" << endl;
        }
};

int main (int argc, char **argv) {
    if (argc != 2) {
        cout << "HELP: PasswordGame.cpp text.txt" << endl;
    }
    else {
        fstream file;

        file.open(argv[1], ios::in);
        if (file.is_open()) {
            TokenDetector tokenExtractor(file);
            PasswordGenerator passwordGen(tokenExtractor.getUnique());
            int numWords;

            cout << "Enter in the number of words needed for the password: " << endl;
            cin >> numWords;

            if (numWords <= tokenExtractor.getSize()) { //Will check to see it the length of password exceeds the number of tokens available
                passwordGen.setIterationLength(numWords);
                string password (passwordGen.getRandomPassword(numWords));
                cout << password << endl;

                PasswordGuesser guesser (password, numWords, passwordGen);
            }
            else {
                cout << "ERROR: the number of words exceeds the number of tokens" << endl;
            }
    
        }
        else {
            cout << "ERROR: file " << argv[1] << " does not exist" << endl;
        }

        file.close();
    }

    return 0;
}   