
// simpleScanner.cpp gives some some hints on the implementation of your lexical scanner
// function. For example, it shows how you can break a statementinto basic 
// components (tokens) and determine their categories. 


#include <iostream>  // for basic iostream operations
#include <cctype>    // to check C character type using function like isdigit()

#include "lexScanner.h" 
// You may include interface information of simpleScanner.cpp in "lexScanner.h", such as
// the coding of component categories into integers, and the function prototype of 
// lexScanner. 
// The caller of the lexical scanner module will aso include "lexScanner.h"

using namespace std;

const char DOUBLE_QUOTE = '\"';

//***************************************************************************
//*  Section 1. two functions: 
//*  getPerLineTokenVectFromOneCharArray and 
//*	 getPerLineTokenVectFromOneStringObject
//*
//***************************************************************************
//*  They analyze one C-style character array or a string object and 
//*  collect the tokens there as a vetor of token strings.
//***************************************************************************
void LexicalScanner::getPerLineTokenVectFromOneCharArray
						(
							const char* ptrOriginalStatement, 
							perLineTokenVector & tokenVector
						)
// 1. One line of code is stored in a C-style character array: statement, 
//    with a terminating '\0' NULL character to end the string stored there.
// 2. In the end, we'll store all the tokens, each as an object of the string class,
//    in tokenVector, which is a vector of strings.
{
 string statement;
 while ( (*ptrOriginalStatement) != NULL)
 {
	if ( isspace(*ptrOriginalStatement) )
		//Convert every white space characters into a space ' '
		//	and append it to the end of statement;
		statement = statement + ' ';
	else
		//Append a character as it is to the end of statement;
		statement = statement + (*ptrOriginalStatement);
	++ptrOriginalStatement;
 }


 string tokenBuffer;
 tokenBuffer.reserve(200); 
 //reserve an initial storage of 200 characters

 size_t iBuffIndex = 0; // index into the statement string

 //A while loop that normalizes the statement string
 //into an equaivalent string with one space between two adjacent tokens.
 while (iBuffIndex< statement.size()) 
 //While it is not the end of thecharacter string stored in the statement array
 {
	char c, nextChar;
	c = statement[iBuffIndex];
	if (iBuffIndex+1 < statement.size())
		nextChar = statement[iBuffIndex+1];
	else
		nextChar = ' ';

	if (c == DOUBLE_QUOTE)
	 //if a double quote is encountered, search for the matching double quote 
	 //and skip the entire string literal like "Hello" enclosed in 
	 //the pair of double quotes.
	{	tokenBuffer += DOUBLE_QUOTE;  
		//Copy the starting double quote into token buffer

		iBuffIndex++;
		c = statement[iBuffIndex];
		while (c != NULL && c != DOUBLE_QUOTE)
		//Search for a matching double quote or a terminating NULL character.
		//Append all achracters in between into tokenBuffer
		 {	tokenBuffer += c; 

			iBuffIndex++;
			c = statement[iBuffIndex];
		 }
		tokenBuffer += DOUBLE_QUOTE; 
		//Copy the endinging double quote into token buffer
	}
	else //The character examined is not a double quote.
    // In the following id statement,
	// we try to detect the end (last character) of a token in the statement array.
	// We insert a space appropriately into the components string to make sure 
	// in the components string there is at least one space between two tokens. 
		if	(
				( (  isalnum(c) || c=='.' ) && 
					 ! (isalnum(nextChar) || nextChar=='.' )
			    )
				// If the current character c is alphnumerical character or a dot
				// (i.e. a letter ot a digit), but the next character is not.
				||
				( (c == '=') && ( nextChar != '=') ) 
				// equal sign, but not followed by another equal sign 
				|| 
				c==',' ||c==':' || c==';' 
				// comma, colon, semicolon 
				|| 
				c=='+' || c=='-' || c=='*' || c=='%' 
				// numerical operators: +,-, *, %
				|| 
				( (c == '/') && ( nextChar != '/') ) 
				// numerical operator: / but not followed by /
				|| 
				( (c == '>') && ( nextChar != '=') ) 
				// relational operator: > but not followed by =
				||
				( (c == '<') && ( nextChar != '=') ) 
				// relational operator: < but not followed by =
				||
				( (c == '&') && ( nextChar != '&') ) 
				// end of logical operator: && 
				|| 
				( (c == '|') && ( nextChar != '|') ) 
				// end of logical operator: ||
				|| 
				( (c == '!') ) 
				// end of logical operator: !
				|| 
				(	c== '(' || c==')'	)
				// parenthesis
				|| 
				(	c== '{' || c=='}'	)
				// curly braces
			) 
			//We detect c as the end of a token.
			{
				// Copy the character c from statement array i
				// and append it into the end of of the token components string.
				tokenBuffer += c;		
				
				// If the next character is not a space,
				// append a space to the end of components to make sure
				// in components there is at least one space between two tokens. 
				if ( nextChar != ' ')
					tokenBuffer += ' ';  
			}
		else 
			// c is not the end of a token.
		{	if (c != ' ')	 
					// c is not a space, but a part of a token while not the end of the token.
					// So just copy the character c from statement array i
					// and append it into the end of of the token components string.
					tokenBuffer += c;
			else
			{	if (c == ' ' && nextChar != ' ')
					// c is a space, but the next character is not a space.
					// append a space to the end of components to make sure
					// in components there is at least one space between two tokens. 
					tokenBuffer += ' ';		
			}
		}

	if (c != NULL)
	//If the character we just finish examining is not the terminating NULL,
	//prepare to examine the next character on the next iteration.
			iBuffIndex++;
	//else we have reached a NULL character signaling the end of the statement.

	//Prepare to check the next character in the statement array.
 }
 //end of while loop that normalizes the statement in the character array
 //into an equaivalent string with one space between two adjacent tokens.


 size_t startIndex; // The index of the beginning of a token
 size_t endIndex;	// The index of the end of a token

 bool keepChecking;
 //Detect the first NON-SPACE character in the string.
 //Set startIndex as the index this character or
 //set it as the length of the string if the entire string is composed of spaces.
 for (	startIndex=0, keepChecking = true; 	
		startIndex < tokenBuffer.size() && keepChecking ; 
	 )
	 if  ( tokenBuffer[startIndex]!= ' ')
		keepChecking = false;
	 else
		startIndex++;	

 tokenVector.clear();
 //cout << "tokenBuffer=[" <<tokenBuffer<<"]"<<endl;


 while ( startIndex < tokenBuffer.size() )
 {
	if (tokenBuffer[startIndex]== DOUBLE_QUOTE)
	//If the token is a string LITERAL like "YES" started with a double quote,
	//search for the matching double quote after the index startIndex.
	//Set endIndex as the index after that of the matching double quote.
	{
		for (	endIndex = startIndex+1, keepChecking = true; 	
				endIndex < tokenBuffer.size() && keepChecking ; 
				endIndex++
			)
			if  ( tokenBuffer[endIndex]== DOUBLE_QUOTE)
				keepChecking = false;
	}
	else
	//If the token is not started with a double quote,
	//Detect the first SPACE character after the index startIndex.
	//Set endIndex as the index this space character or
	//set it as the length of the string 
	//if none such space exists after the index startIndex.
	{
		for (	endIndex = startIndex+1, keepChecking = true; 	
				endIndex < tokenBuffer.size() && keepChecking ; 
			)
			if  ( tokenBuffer[endIndex]== ' ')
				keepChecking = false;
			else
				endIndex++;
	}

	//tokenBuffer.substr(startIndex, endIndex - startIndex) is a token.
	//Append such a token in the token vector to tokenVector
	tokenVector.push_back(tokenBuffer.substr(startIndex, endIndex - startIndex));
	//cout << "Token found:" << tokenVector.back() << endl;

	//Detect the next non-space character with an index after current endIndex.
	//Set startIndex as the index this character or
	//set it as the length of the string if the reaminder of the string 
	//is entirely composed of spaces.
	for (	startIndex=endIndex, keepChecking = true; 	
			startIndex < tokenBuffer.size() && keepChecking ; 
		)
		if  ( tokenBuffer[startIndex]!= ' ')
			keepChecking = false;
		else
			startIndex++;

 }

}


void LexicalScanner::getPerLineTokenVectFromOneStringObject
									(	const string statement, 
										perLineTokenVector & tokenVector
									)
// 1. One line of code is stored in string object: statement. 
// 2. In the end, we'll store all the tokens, each as an object of the string class,
//    in tokenVector, which is a vector of strings.
{

	getPerLineTokenVectFromOneCharArray(statement.c_str(), tokenVector);

}




//******************************************************************
//* Section 2: 
//* The following is a colllection of functions to 
//* test the categories of tokens
//******************************************************************

//Check whether a token is a numerical token
bool LexicalScanner::isNUMERICAL_LITERAL(const string & token) 
{
	if (token == ".")
		return false;

	bool decimalPointSeenBefore = false; 
	//no decimal points seen yet

	for (	size_t i=0; //index into the string 
			i < token.size();
			i++	) 
		{
			char c;
			c = token[i];
			if ( c != '.' && !isdigit(c) ) 
				// not a digit, nor a demical point ==> Not Numerical 
				return false; 
			if ( c == '.' && decimalPointSeenBefore)
				// decimal point appears more than once ==> Not right 
				return false; //Not Numerical
			if ( c == '.' && !decimalPointSeenBefore)
				// Keep a record when decimal point is seen for the first time
				decimalPointSeenBefore = true;

		}

	return true;
}


bool LexicalScanner::isKEYWORD(const string& token)
// Test whether it is a keyword of BIOLA, 
// like read, display, if, else, while
{
	//if (token == "read" || "if" || "else" || "while" || "display")
	if (token == "if")
		return true;
	else if (token == "read")
		return true;
	else if (token == "else")
		return true;
	else if (token == "while")
		return true;
	else if (token == "display")
		return true;
	else return false;
}									

bool LexicalScanner::isID_NAME(const string & token)
// Test whether it is an identifier, i.e. names of variables
{
	//if all other functions return false then it is an ID_Name
	// if any other functions return true then return false, it is not an ID name but something else 
	if (isNUMERICAL_LITERAL(token)|| isKEYWORD(token) || isASSIGNMENT_OP(token) || isLOGICAL_OP(token) || isNUMERICAL_OP(token) ||			//assuming that if its not the other members 
		isRELATIONAL_OP(token) || isSTRING_LITERAL(token) || isCOLON(token) || isSEMICOLON(token) || isCOMMA(token) ||												// then it is a ID name, could be problematic
		isLEFT_CURLYBRACE(token) || isRIGHT_CURLYBRACE(token) || isLEFT_PARENTHESIS(token) || isRIGHT_PARENTHESIS(token) || isCOMMENT(token))						//might need to seperate all function names into own if statments
		return false;
	else
		return true;

}


bool LexicalScanner::isASSIGNMENT_OP(const string & token)
// Test whether it is =, the assignment operator		
{
	if (token == "=")
		return true;
	else return false;
}

bool LexicalScanner::isLOGICAL_OP(const string & token)
{
	if (token == "&&")
		return true;
	else if (token == "||")
		return true;  
	else if (token == "!")
		return true;

	else
		return false;
}

bool LexicalScanner::isNUMERICAL_OP(const string & token)
// Test whether it is one of the following
// numerical operators	 (such as +, -, *, /, %)
{
	if (token == "+" )
		return true;
	else if (token == "-")
		return true;
	else if (token == "*")
		return true;
	else if (token == "/")
		return true;
	else if (token == "%")
		return true;
	else return false;
}

bool LexicalScanner::isRELATIONAL_OP(const string & token)
// Test whether it is one of the following
// numerical operators	 (such as >, ==, <, >=, <=)
{
	if (token == ">" )
		return true;
	else if (token == "==")
		return true;
	else if (token == "<")
		return true;
	else if (token == ">=")
		return true;
	else if (token == "<=")
		return true;
	else return false;
}


bool LexicalScanner::isSTRING_LITERAL(const string& token)
// Test whether it is a string literals 
// (like "Hi Hi", in a pair of double quotes)
{	
	int x = 0;
	for (size_t i = 0; i < token.size(); i++)											//token.size() - 1  ??
	{
		if (token[i] == DOUBLE_QUOTE)								
			x++;
											//not all paths return somehting
	}
	  if (x == 2)
			return true;
	 else
		return false;	
}

bool LexicalScanner::isCOLON(const string & token)
// Test whether it is a colon :
{
	if (token == ":")
		return true;
	else
		return false;
}	

bool LexicalScanner::isSEMICOLON(const string & token)
// Test whether it is a semicolon ;
{
	if (token == ";")
		return true;
	else
		return false;
}	

bool LexicalScanner::isCOMMA(const string & token)
// Test whether it is a comma ,
{
	if (token == ",")
		return true;
	else
		return false;

}	

bool LexicalScanner::isLEFT_CURLYBRACE(const string & token)
// Test whether it is a left CURLYBRACE {
{
	if (token == "{")
		return true;
	else
		return false;
}

bool LexicalScanner::isRIGHT_CURLYBRACE(const string & token)
// Test whether it is a right CURLYBRACE }
{
	if (token == "}")
		return true;
	else
		return false;
}

bool LexicalScanner::isLEFT_PARENTHESIS(const string & token)
// Test whether it is a left parenthesis (
{
	if (token == "(")
		return true;
	else
		return false;
}

bool LexicalScanner::isRIGHT_PARENTHESIS(const string & token)
// Test whether it is a right parenthesis )
{
	if (token == ")")
		return true;
	else
		return false;
}

bool LexicalScanner::isCOMMENT(const string & token)
// Test whether it is comment //
{
	if (token == "//")
		return true;
	else
		return false;
}




//**************************************************************************
//Section 3: one function: 
//getLexCategoryVectorOneLine and getLexCategoryVectorAllLines
//
//You should extend this function to categorize the tokens in the
//tokenVector and store the categories of the tokens in categoryVector
//by calling the category testing functions described in
//the section 2 above.
//**************************************************************************
void LexicalScanner::getCategoryVectorFromTokenVector(
	const perLineTokenVector & tokenVector,
	perLineCategoryVector & categoryVector		)
{

	
	//****************************************************
	//	This method will call the functions from Section 2 to fill the category
	//  vector based on what is stored in the token
	//****************************************************
	
	for (size_t i = 0; i < tokenVector.size(); i++ )
	{												
		 if (isNUMERICAL_LITERAL(tokenVector[i]))
			categoryVector.push_back(NUMERICAL_LITERAL);
		else if (isKEYWORD(tokenVector[i]))
			categoryVector.push_back(KEYWORD);
		else if (isASSIGNMENT_OP(tokenVector[i]))
			categoryVector.push_back(ASSIGNMENT_OP);
		else if (isLOGICAL_OP(tokenVector[i]))
			categoryVector.push_back(LOGICAL_OP);
		else if (isNUMERICAL_OP(tokenVector[i]))
			categoryVector.push_back(NUMERICAL_OP);
		else if (isRELATIONAL_OP(tokenVector[i]))
			categoryVector.push_back(RELATIONAL_OP);
		else if (isSTRING_LITERAL(tokenVector[i]))
			categoryVector.push_back(STRING_LITERAL);
		else if (isCOLON(tokenVector[i]))
			categoryVector.push_back(COLON);
		else if (isSEMICOLON(tokenVector[i]))
			categoryVector.push_back(SEMICOLON);
		else if (isCOMMA(tokenVector[i]))
			categoryVector.push_back(COMMA);
		else if (isLEFT_CURLYBRACE(tokenVector[i]))
			categoryVector.push_back(LEFT_CURLYBRACE);
		else if (isRIGHT_CURLYBRACE(tokenVector[i]))
			categoryVector.push_back(RIGHT_CURLYBRACE);
		else if (isRIGHT_PARENTHESIS(tokenVector[i]))
			categoryVector.push_back(RIGHT_PARENTHESIS);
		else if (isLEFT_PARENTHESIS(tokenVector[i]))
			categoryVector.push_back(LEFT_PARENTHESIS);
		else if (isID_NAME(tokenVector[i]))
			categoryVector.push_back(ID_NAME);
		else if (isCOMMENT(tokenVector[i]))
			categoryVector.push_back(COMMENT);
		//may need to do else if not if

	}

}


//**************************************************************************
// Section 4: a core function for the lexical scanner module
//
// getLexicalInfo is used to
// to get two kinds of lexical information from 
// all lines of a program, i.e. 
// (1) the vector of token vectors of all lines and
// (2) the vector of category vectors of all lines.
//
// Implementation:
// You should implement them by appropriately using the following functions 
//	getPerLineTokenVectFromOneStringObject (implemented in section 1 above), and
//	getCategoryVectorFromTokenVector (to be implemented in section 3 above)
//****************************************************************************


void LexicalScanner::getLexicalInfo(
		const vector<string> & linesOfStatements, 
		vectOfTokenVects & tokenVectors, 
		vectOfCategoryVects & categoryVectors)	
{	


	perLineCategoryVector tempCategory;
	perLineTokenVector tempVect;
	tempCategory.clear();
	tempVect.clear();

	//Use a loop  to process each line statement by extracting lexical information for each line of code :
	for (size_t i = 0; i < linesOfStatements.size(); i++)
	{

		//Call getPerLineTokenVectFromOneStringObjectand store tokens in the vector of token vectors
		getPerLineTokenVectFromOneStringObject(linesOfStatements[i], tempVect);
		tokenVectors.push_back(tempVect);

		//Call getCategoryVectorFromTokenVectorand store categories in the vector of category vectors
		getCategoryVectorFromTokenVector(tempVect, tempCategory);
		categoryVectors.push_back(tempCategory);

		tempCategory.clear();
		tempVect.clear();
	}

}


//**************************************************************************
// Section 5: one function: displayLexicalInfo
//
// Function to display the lexical information stored in
// a vector of token vectors of all lines of statements, and 
// a vector of category vectors of all lines of statements.
//****************************************************************************
void LexicalScanner::displayLexicalInfo(
				vectOfTokenVects & tokenVectors, 
				vectOfCategoryVects & categoryVectors)
{
	if (tokenVectors.size() != categoryVectors.size() )
	{ cout << "# of token vectors inconsistent with # of category vectors." << endl;
	  return;
	}

	for (size_t i=0; i<tokenVectors.size(); i++)
	{	perLineTokenVector tokenVect;
		perLineCategoryVector categoryVect;

		tokenVect = tokenVectors[i];
		categoryVect = categoryVectors[i];

		if (tokenVect.size() != categoryVect.size() )
		{	cout	<< "Line " << i 
					<< ": # of tokens inconsistent with # of category labels." << endl;
			return;
		}

		cout	<< endl
				<< "***************************** "
				<< "Lexical Info of Line " << i
				<< "***************************** " 
				<< endl;

		for (size_t j=0; j<tokenVect.size(); j++)
		{	string categoryInfo;
			switch (categoryVect[j])
			{
				case KEYWORD:	
								categoryInfo="KEYWORD"; 
								break;
				case ASSIGNMENT_OP:	
								categoryInfo="ASSIGNMENT_OP"; 
								break;
				case COLON:	
								categoryInfo="COLON"; 
								break;
				case COMMA:	
								categoryInfo="COMMA"; 
								break;
				case COMMENT:	
								categoryInfo="COMMENT"; 
								break;
				case ID_NAME:	
								categoryInfo="ID_NAME"; 
								break;
				case LEFT_CURLYBRACE:	
								categoryInfo="LEFT_CURLYBRACE"; 
								break;
				case LEFT_PARENTHESIS:	
								categoryInfo="LEFT_PARENTHESIS"; 
								break;
				case LOGICAL_OP:	
								categoryInfo="LOGICAL_OP"; 
								break;
				case RIGHT_CURLYBRACE:
								categoryInfo="RIGHT_CURLYBRACE"; 
								break;
				case RIGHT_PARENTHESIS:	
								categoryInfo="RIGHT_PARENTHESIS"; 
								break;
				case NUMERICAL_LITERAL:	
								categoryInfo="NUMERICAL_LITERAL"; 
								break;
				case NUMERICAL_OP:	
								categoryInfo="NUMERICAL_OP"; 
								break;
				case RELATIONAL_OP:	
								categoryInfo="RELATIONAL_OP"; 
								break;
				case SEMICOLON:	
								categoryInfo="SEMICOLON"; 
								break;
				case STRING_LITERAL:	
								categoryInfo="STRING_LITERAL"; 
								break;
				default: 
								categoryInfo="UNKNOWN"; 
			}
	
			cout	<< "Token[" 
					<< j 
					<< "]:"
					<< tokenVect[j]
					<< "\t\t"
					<< categoryInfo
					<< endl;
		}//end for

	}

}
