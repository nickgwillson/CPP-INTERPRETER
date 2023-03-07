#ifndef INTERFACE_H   
//The compiler checks whether INTERFACE_H is defined to see
//whether this header file is already included.

#define INTERFACE_H
//If INTERFACE_H not defined yet, define it and include
//what should be included in order to utilize the implementation
//in the interface.cpp


#include <vector>		//including support of vector container
#include <string>		//including the new C++ string class support
using namespace std;



	class Interface
	{
	 private:
		//Store a program as vector of strings, with each statement stored as a string.
		//Initially it is an empty vector.
		vector<string> lines;

	 public:

		//This constant static data member is used to s
		//		set the maximum length of filenames to 100 characters
		static const unsigned int MaxLengthFileName = 100;

		// Load a program (lines of statements) from a file
		void loadProgramFromFile();

		// Save lines of program code to a file
		void saveProgramIntoFile();

		// Display the lines of statements in the program
		void displayProgram();

		// Append new lines of code to the end of the program
		void appendLines();

		// Insert new lines after an existing line
		void insertLines();

		// Delete existing lines
		void deleteLines();

		// Replace the contents of an existing line
		void replaceOneLine();

		// does lexical analysis stuff :)
		void lexicalAnalysis();

		// This startInterface() function serves as the central place
		//	to access all the services provided by the interpreter
		//	including the functions listed above
		void startInterface();

		void Calculator();

		//This is a function to facilitate the display of messages to the user
		void endOfService(const string service);
	};


#endif

