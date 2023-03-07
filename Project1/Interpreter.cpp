#include <iostream>
#include <vector>
#include <string> 
#include <algorithm>
using namespace std;

#include "interface.h"
#include "lexScanner.h"
#include "expEvaluator.h"
#include "Interpreter.h"

bool Interpreter::executeProgram(vector<string> & sourceProgram) 
{

	//check if syntactically it is correct, else display error messages

	//define data structures to store tokens and category vectors
	//as well as symbol table for variables

	vectOfCategoryVects tempCatVec;
	vectOfTokenVects tempTokVec;


	floatVarValueTable varTable;
	perLineCategoryVector categoryVec;
	perLineTokenVector tokenVec;

	float answer = 0;
	int size = sourceProgram.size();

	//get lex info
	LexicalScanner::getLexicalInfo(sourceProgram, tempTokVec, tempCatVec);

	//Use lexical scanner modules to extract tokens and identify categorys for SourceProgram
	for (size_t i = 0; i < size; i++) {

		LexicalScanner::getPerLineTokenVectFromOneStringObject(sourceProgram[i], tokenVec);
		LexicalScanner::getCategoryVectorFromTokenVector(tokenVec, categoryVec);

		string var;	//used to hold <var>
		float key;//used to hold variables value

		switch (categoryVec[0])
		{
		case KEYWORD:

			if (tokenVec[0] == "read")			// read <var>
			{ // Prompt the user for a numerical value and store it in the Symbol 
				//Table using <var> (i.e. the variable name) as the key

				var = tokenVec[1];
				cout << "What is the variables numerical value?\n";
				cin >> key;
				varTable.insert({ var, key });


			}
			else if (tokenVec[0] == "display")
			{

				if (categoryVec[1] == ID_NAME)  // display <exp>  // ie display v
				{
					cout << tokenVec[1] << " is " << varTable[tokenVec[1]] << endl;
				}
				else if (categoryVec[1] == STRING_LITERAL)	  // display <string>
				{	//Display the contents of the string literal, without the double quotes

					string str;
					str = tokenVec[1];
					str.erase(remove(str.begin(), str.end(), '\"'), str.end());	//removes quotes
					cout << str;

				}
				else
				{
					//else it is an expression
				   //Use the Expression Evaluator module to evaluate the <expr>
				   //and display the result on the screen
				   //check that it can evaluate

					float expression = 0;
					perLineTokenVector tempVector;

					for (int i = 1; i < tokenVec.size() - 1; i++)		//putting expression into new vector
					{
						tempVector.push_back(tokenVec[i]);
			
					}

					
					//testing the expression

					while (!ExpressionEvaluator::infixEvaluator(tempVector, varTable, expression))
					{
						cout << "Expression cannot be evaluated!" << endl;
						return 0;
					}
					string exp;
					LexicalScanner::getPerLineTokenVectFromOneStringObject(exp, tempVector);
					ExpressionEvaluator::infixEvaluator(tempVector, varTable, expression); //the vartable should hold all variables and their number

						cout << "The Expression equals: " << expression << endl;



				}


			}
			break;
		case ID_NAME:
			if (categoryVec[1] == ASSIGNMENT_OP)	 //<var> = ....
			{

				//Use the Expression Evaluator module to evaluate the <expr> on the right of the
				//assignment operator. Then store the evaluation result in the Symbol Table 
				//using <var> (i.e.the variable name) as the key, check that it can eval. (!execute)
				//if there is a variable in the expression that isnt in symbol table display message

				float expression = 0;
				perLineTokenVector tempVector;
				string	exp;

				for (int i = 2; i < tokenVec.size() - 1; i++)		 //putting exp into vector
				{
					tempVector.push_back(tokenVec[i]);
				}

				//testing the expression
				while (!ExpressionEvaluator::infixEvaluator(tempVector, varTable, expression))
				{
					cout << "Expression cannot be evaluated!" << endl;
					return 0;
				}

				LexicalScanner::getPerLineTokenVectFromOneStringObject(exp, tempVector);
				ExpressionEvaluator::infixEvaluator(tempVector, varTable, expression); //the vartable should hold all variables and their nu
				varTable.insert({ tokenVec[0], expression });//insert new var definition into table

				cout << "The Expression has '" << tokenVec[0] << "' equaling " << expression << endl;

			}
			else
			{
				cout << "there must have been an error\n";
			}
			break;

		default:
			cout << "SYNTAX ERROR\n";
			return 0;
			break;
		}


	}

}