# Password Generator

Author: Joseph Tomada, Jeiko Santos
Approximate time to complete: 1 week
Created: April 2021

This was a school project that was designed to generate a unique password based off of words from an inputted text final. The program will also try to guess the generated password using two different methods. To do so, the program consists of three different classes for generating unique tokens, creating a randomly generated password, and guessing the outputted password.

**Note: This program is a finalized version and will no longer be worked on.**

# How to use

To use this file, compile PasswordGame.cpp and input a text file as a command line argument. For instance:

**g++ - o myprog PasswordGame.cpp**
**./myprog textfile.txt**

## TokenDetector
TokenDetector class parses unique tokens from the text file and puts them into a list of strings. It primarily does through the constructor:

**TokenDetector (fstream &file)** - The parametrized constructor takes in a file and begins to parse tokens at the start. All of the unique tokens are put into the class's global list<string>, tokensList.

## PasswordGenerator
PasswordGenerator class primarily accomplishes two tasks. The first task is that it can create a randomly generated password using the tokens created from the TokenDetector class. The second task is to iterate through all of the possible combinations that the PasswordGenerator can create. The class's most important methods are the following:

**getRandomPassword(int numWords)** - using srand, the program iterates to a random point in the unique tokensList and returns the token at that point. This is then repeated until **numWords** have been chosen to successfully generate a unique password

**next()** - Whenever this method is called, the program will iterate through one of the many possible combinations that **getRandomPassword(int numWords)** can generate. This is accomplished through an algorithm which utilizes the push/pop methods granted by **list**. 

**hasNext()** - If the next() has went through all the possible combinations of a generated password, the method will return **false**. Else, the following method will return **true**.

## PasswordGuesser
This class tries to guess the generated password with two different methods. The first is a brute force method where it will continue to iterate through **next()**, from the PasswordGenerator class, until a match is found. The second is a purely random method where **getRandomPassword(int numWords)** is continuously found until a match is found. The original plan is separate the two methods into two threads in order to accurately gauge which method found a match first. However, due to variance in computers, we have decided to put both methods in a while loop until a match is found or **hasNext()** returns true. This gives the user a pseudo head-to-head comparison.
