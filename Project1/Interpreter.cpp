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
		{
			if (tokenVec[0] == "read")			// read <var>
			{ // Prompt the user for a numerical value and store it in the Symbol 
				//Table using <var> (i.e. the variable name) as the key

				var = tokenVec[1];
				std::cout << "What is the variables numerical value?" << endl;
				cin >> key;
				varTable.emplace(var, key);				

			}

			else if (tokenVec[0] == "display")
			{
				if (LexicalScanner::isID_NAME(tokenVec[1]))  // display <exp>  // ie display v
				{
			
					std::cout << tokenVec[1] << " is " << varTable[tokenVec[1]] << endl;

			
				}
				else if (LexicalScanner::isSTRING_LITERAL(tokenVec[1]))	  // display <string>
				{	//Display the contents of the string literal, without the double quotes

					string str;
					str = tokenVec[1];
					str.erase(remove(str.begin(), str.end(), '\"'), str.end());	//removes quotes
					std::cout << str << endl;
				}
				else	//else it is an expression
				{
					
					float expression = 0;
					perLineTokenVector tempVector;

					for (int i = 1; i < tokenVec.size() - 1; i++)		//putting expression into new vector
					{
						tempVector.push_back(tokenVec[i]);
					}

					//testing the expression

					while (!ExpressionEvaluator::infixEvaluator(tempVector, varTable, expression))
					{
						std::cout << "Expression cannot be evaluated!" << endl;
						return 0;
					}
					string exp;
					LexicalScanner::getPerLineTokenVectFromOneStringObject(exp, tempVector);
					ExpressionEvaluator::infixEvaluator(tempVector, varTable, expression); //the vartable should hold all variables and their number

					std::cout << "The Expression equals: " << expression << endl;
				}


			}
			else if (tokenVec[0] == "while")
			{
				float expression = 0;
				perLineTokenVector tempExpVector;

				for (size_t i = 2; i < tokenVec.size()-1; i++)				 //extracting logical expression into tempExpvector
				{
					if (LexicalScanner::isLEFT_PARENTHESIS(tokenVec[1]))
					{
						if (tokenVec[i] == ")")	 //end of if ()
							i = 20;
						else					//hasn't hit other parethesis, push tokens into vector
							tempExpVector.push_back(tokenVec[i]);   //push expression into temp vector
						//std::cout << tokenVec[i] << endl;
					}
					else
						std::cout << "Error: there needs to be a parenthesis after 'while'" << endl;

				}
				
					int stack = i;
					vector<string> tempProgram;
					tempProgram.push_back(sourceProgram[stack]);
					ExpressionEvaluator::infixEvaluator(tempProgram, varTable, expression);

					while (expression == 0)
					{
						//execute the program contained between the curly brackets

						if (LexicalScanner::isLEFT_CURLYBRACE(sourceProgram[i + 1]))
						{
							for (size_t x = i + 2; x < size; x++)	//going through until next curlybrace
							{
								if (sourceProgram[x] == "}")
								{
									i = x + 1;
									x = size + 1;
								}
								else	//execute the lines of statemetn inbetween the curly brackets
								{
									vector<string> tempProgram;
									tempProgram.push_back(sourceProgram[x]);
									Interpreter::executeProgram(tempProgram);
									ExpressionEvaluator::infixEvaluator(tempProgram, varTable, expression);
									
									if (varTable[tokenVec[2]] == 0)	//checking if var in while exp has changed to 0
										expression = 1;
									
								}
							}
						}
						else
							std::cout << "Error: Need a curlybrace after while statement" << endl;
							
					}
					//	For while blocks (e.g. while (<logic-expr>)<nl>{<nl><lines><nl>}), the Interpreter should jump 
					 //	back to the line containing the while keyword until the <logic-expr> evaluates to false or 0. 
					 //	Thus, line number should be stored on the stack,
					 //	in order to be able to jump to it

			}
			else if (tokenVec[0] == "if")
			{
				float expression = 0;
				perLineTokenVector tempExpVector;

				for (size_t i = 2; i < tokenVec.size(); i++)				 //extracting logical expression
				{
					if (LexicalScanner::isLEFT_PARENTHESIS(tokenVec[1]))
					{
						if (tokenVec[i] == ")")			 //end of if ()
							i = tokenVec.size() +1;
						else							//hasn't hit other parethesis, push tokens into vector
							tempExpVector.push_back(tokenVec[i]);   //push expression into temp vector
					}
					else
						std::cout << "Error: there needs to be a parenthesis after 'if'" << endl;
				}
				string exp;
				ExpressionEvaluator::infixEvaluator(tempExpVector, varTable, expression);
				if (expression == 1)	//evaluate the expression, if expression workds
				{
					if (LexicalScanner::isLEFT_CURLYBRACE(sourceProgram[i + 1]))
					{
						for (size_t x = i + 2; x < size; x++)	//going through until next curlybrace
						{
							if (sourceProgram[x] == "}")
							{
								i = x + 1;
								x = size + 1;

								if (sourceProgram[i] == "else")	   //if there is an else statement after the if
								{
									for (size_t x = i; x < size; x++)	   //used to skip over the else statement 
									{
										if (sourceProgram[x] == "}")	  //if the if statement doesn't work then 
										{								// disregard all lines till next curly brace
											i = x;				//go to next line ie. "else"
											x = size + 1;		  //stopping for loop
										}
									}

								}
							}
							else	//execute the lines of statemetn inbetween the curly brackets
							{
									vector<string> tempProgram;
									tempProgram.push_back(sourceProgram[x]);
									Interpreter::executeProgram(tempProgram);
									
							}
						}
					}
					else
						std::cout << "Error: Need a curlybrace after if statement" << endl;

				}
				else	  //else expression returns false, check for else statemtn and disregard if {...}
				{

					for (size_t x = i + 2; x < size; x++)	//going through until next curlybrace
					{

						if (sourceProgram[x] == "}")	  //if the if statement doesn't work then 
						{								// disregard all lines till next curly brace
							i = x + 1;
							x = size + 1;
						}
					}

					if (sourceProgram[i] == "else")	 //if line after if statemtn is else
					{
						if (LexicalScanner::isLEFT_CURLYBRACE(sourceProgram[i + 1])) //checking for curly brace
						{
							for (size_t x = i + 2; x < size; x++)	//going through until next curlybrace
							{
								if (sourceProgram[x] == "}")
								{
									i = x;						//good
									x = size + 1;			
								}
								else	//execute the lines of statemetn inbetween the curly brackets
								{
									vector<string> tempProgram;
									tempProgram.push_back(sourceProgram[x]);	 //good
									executeProgram(tempProgram);
								}
							}
							
						}
						else
							std::cout << "Error: Need a curlybrace after else statement" << endl;
					}
				}
			}
			break;
		}					//end of case statement

		case ID_NAME:
		{		if (categoryVec[1] == ASSIGNMENT_OP)	 //<var> = ....
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
				std::cout << "Expression cannot be evaluated!" << endl;
				return 0;
			}

			LexicalScanner::getPerLineTokenVectFromOneStringObject(exp, tempVector);
			ExpressionEvaluator::infixEvaluator(tempVector, varTable, expression); //the vartable should hold all variables and their nu
			varTable.insert({ tokenVec[0], expression });//insert new var definition into table

			std::cout << "The Expression has '" << tokenVec[0] << "' equaling " << expression << endl;

		}
		else
		{
			std::cout << "Error: invalid syntax\n";
		}
		break;
		}
		default:
			std::cout << "SYNTAX ERROR - ";
			std::cout << sourceProgram[i] << endl;
	
			return 0;
			break;
		}

	}
}