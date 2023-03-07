
#include <iostream>     //including I/O support
#include <fstream>      //including file I/O support
#include <vector>		//including support of vector container
#include <string>		//including the new C++ string class support
#include <cstring>		//including old C character array support
#include <conio.h>	//including support to use _getch()
						//	to read one char at a time from user input

#include "interface.h"
#include "lexScanner.h"
#include "expEvaluator.h"
#include "Interpreter.h"



using namespace std;

ofstream outFile;
ifstream inFile;
char fileName[100];

// 1. Read in a program (lines of statements) from a file
void Interface::loadProgramFromFile()
{
	//char fileName[MaxLengthFileName];	
	// this char array stores the name of the file the program reads from.


	cout<<"Name of file to upload program from (ex. file.txt): ";
	cin >> fileName;
	while( strlen(fileName) == 0 )
	{
		cin >> fileName;
	}

	ifstream inFile;
	inFile.open( fileName );
	if ( inFile.fail() )
	{ 
		cout << endl << "Cannot open this file!" << endl;
		return;
	}

	lines.clear();  //Delete all the lines of statements
	string inputBuffer;
	while( !inFile.eof() )
	{
		getline(inFile, inputBuffer);
		while( !inFile.eof() && inputBuffer.size() == 0 )
		{
			getline(inFile, inputBuffer);
		}

		if (!inFile.eof())
		{
  			cout<< "Line["
				<< lines.size()
				<< "] read:"
				<< inputBuffer << endl;
			lines.push_back(inputBuffer);
		}

	}

	inFile.close();
}

// Save program to a file
void Interface::saveProgramIntoFile()
{

	char fileName1[100];

	cout << endl << endl << "Now give me a file name to store into a file:" << endl;
	cin >> fileName1;
	outFile.open(fileName1);

	for (const auto i : lines)
	{
		outFile << i << endl;
	}

	outFile.close();
}

// Display the lines of statements
void Interface::displayProgram()
{	
	int size = lines.size();
	cout << endl;
		for (int i = 0; i < size; i++)
		{
			cout << "["
				<< i
				<< "] read: ";
			cout << lines[i] << endl;
			
		}
}


// Append new lines of code to the end of the program
void Interface::appendLines()
{
	string appendedLine;
	cout << "Please enter line(s) of code to append:\n"
		<< " [Enter . when finished]\n";
	cin.ignore();
	
	while (appendedLine != ".")
	{	
		cout << ": ";
			//while loop to check if getline empty			
			    getline (cin, appendedLine); //takes in whole sentences instead of one word only, but shows 2 numbers, glitches
				
				if (appendedLine != ".")
				{
					lines.push_back(appendedLine);
				}			
	}
		displayProgram();
}


// Insert new lines after an existing line
void Interface::insertLines()
{
	string newLine;
	int lineNum;


	cout << "Before which line would you like to insert a new line?" << endl;
	cout << "From [0] to [" << lines.size()-1 << "]:\n";
	cin >> lineNum;

	cout << "What would you like to enter into this new line?: "
	<< " [Enter . when finished]\n";

	cin.ignore();
	while (newLine != ".")
	{
		cout << ": ";
		getline (cin, newLine);

		if (newLine != ".")
		{
			lines.insert(lines.begin() + lineNum, newLine);
			//can just increase the line number, change where your iterator is pointing too
		}
		
	}
	 displayProgram();
}


// Delete a range of existing line
void Interface::deleteLines()
{
	int BeginNumber, EndNumber;
	

	cout << "Which lines do you want to delete?\n"
		 << "From [0] to [" << lines.size() - 1 << "]:\n"
		<< "Begin line #: ";
	cin >> BeginNumber;
	cout << "End line #: ";
	cin >> EndNumber;

	vector <string>::iterator begNum = lines.begin() + BeginNumber;
	vector <string>::iterator endNum = lines.begin() + EndNumber;
 
	lines.erase(begNum, endNum);
	//lines.erase(lines.begin() + BeginNumber, lines.begin() + EndNumber + 1);


	cout << "[DISPLAY PROGRAM]:" << endl;
	displayProgram();

	 

	
}


// Replace the contents of an existing line
void Interface::replaceOneLine()
{
	
	string newLine, inputBuffer;
		int index;
	//vector <string>::iterator;
	
	cout << "Which line would you like to Modify?\n "// << lines.size() << " *\n";
		<< "From [0] to [" << lines.size() - 1 << "]:\n";
	
	cin >> index;

	vector <string>::iterator itIndex = lines.begin() + index;
	lines.erase(itIndex);
	 
	cin.ignore();
	cout << "What would you like to write into that line?\n";
	getline (cin, newLine);

	lines.insert(lines.begin() + index, newLine);

	displayProgram();

}						  


//An auxilliary message displaying function
void Interface::endOfService(const string service)
{
	cout<<endl << service << " done." << endl << "Press any key to continue.";
	_getch();
	cout << endl;
	cout << "****************************************************" <<endl<< endl;

}	

void Interface::lexicalAnalysis()
{
	vectOfCategoryVects CategoryVect;
	vectOfTokenVects TokenVect;
	LexicalScanner::getLexicalInfo(lines, TokenVect, CategoryVect);
	LexicalScanner::displayLexicalInfo(TokenVect, CategoryVect);

}

void Interface::Calculator()
{
floatVarValueTable floatTable; //map associative conatiner to store and update values for variables used in expression
string expression;
perLineTokenVector tempTokenVect;
expVector infixExp;	  		 //temp storage needed for infixEvaluator
float answer = 0;		//the stored value of the infix-postfix expression


cout << "Enter an Expression to calculate:\n";
getline(cin, expression);


//extract tokens from expression into TokenVector
LexicalScanner::getPerLineTokenVectFromOneStringObject(expression, tempTokenVect);	//give it the expression, returns string vector
 	
ExpressionEvaluator::infixEvaluator(tempTokenVect, floatTable, answer);			 //pass string vector

cout << "The infix expression equals - " << answer << "\n";
}


// This menu() function is the interface
void Interface::startInterface() {

	// "Then I suppose now is the time for me to say something profound....
	// Nothing comes to mind. Let's do it."

	bool inMenu = true;	// while the user is using the program this var is set to true and the menu keeps looping.
						// when the user wants to quit, it is set to false and the loop ends.
	char keyPressed;	// This var stores which menu selection the user picks.

	while ( inMenu )
	{
		cout<< endl << endl << endl;
		cout << "****************************************************" << endl;
		cout <<"**  MENU:(press a character to select an option)  **" << endl;
 		cout << "****************************************************" << endl;
		cout<<"Q. [QUIT]     Quit."<<endl;
		cout<<"L. [LOAD]     Read in a program (lines of statements) from a file"<<endl;
  		cout<<"S. [SAVE]     Save lines of statement as a program into a file"<<endl;
		cout << endl;
		cout << "E. [EXECUTE] - Biola Language Interpreter to Execute Program" << endl;
		cout <<"C. [CALCULATE]  Calculate Infix Expression" << endl;
		cout<<"D. [DISPLAY]  Display the source code as lines of statements"<<endl;
		cout << endl;
		cout << "N [ANALYZE]  Lexical Analysis of the code" << endl;
		cout<<"A. [APPEND]   Append new lines to the end of the program"<<endl;
		cout<<"I. [INSERT]   Insert new lines before an existent line"<<endl;
		cout<<"X. [DELETE]   Delete a range of existent lines"<<endl;
		cout<<"R. [REPLACE]  Replace the contents of an existent line"<<endl;
		cout << endl;

		cout << "****************************************************" <<endl<< endl;

		cout<<"Your choice is: ";
		keyPressed = _getch();
		cout<<keyPressed << endl;

		switch(keyPressed) {
		case 'Q': case 'q'://Quit
			cout << "[QUIT]:" << endl;
			inMenu = false;
			break;
		case 'C': case 'c'://Quit
			cout << "[Calculate]:" << endl;
			Calculator();;
			break;

		case 'E': case 'e'://Quit
			cout << "[EXECUTE]:" << endl;
			Interpreter::executeProgram(lines);
			break;

		case 'L': case 'l':// Load a program (lines of statements) from a file
 			cout << "[LOAD PROGRAM]:" << endl;
			loadProgramFromFile();
			endOfService("[LOAD PROGRAM]");
			break;

		case 'S': case 's':// Save program to a file
			cout << "[SAVE]:" << endl;
			saveProgramIntoFile();
			endOfService("[SAVE CODE INTO A FILE]");
			break;

		case 'N': case 'n':// Lexical Analysis of the code
			cout << "[ANALYZE]:" << endl;
			lexicalAnalysis();
			
			break;

		case 'D': case 'd':// Display the lines of statements in the program
			cout << "[DISPLAY PROGRAM]:" << endl;
			displayProgram();
			endOfService("[DISPLAY PROGRAM]");
			break;

		case 'A': case 'a':// Append new lines of code to the end
			cout << "[APPEND]:" << endl;
			appendLines();
			endOfService("[APPEND LINES]");
			break;

		case 'I': case 'i':// Insert a new line after an existing line
			cout << "[INSERT LINES]:" << endl;
			insertLines();
			endOfService("[INSERT LINES]");
			break;

		case 'X': case 'x':// Delete an existing line
			cout << "[DELETE LINES]:" << endl;
			deleteLines();
			endOfService("[DELETE LINES]");
			break;

		case 'R': case 'r':// Replace the contents of an existing line
			cout << "[REPLACE]:" << endl;
			replaceOneLine();
			endOfService("[REPLACE ONE LINE]");
			break;


		default:
			cout << "[?? UNKNOWN COMMAND]:" << endl;
			// Unrecognized key, do nothing
			break;
		}

	} // End of while loop.
}
