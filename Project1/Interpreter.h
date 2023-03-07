#pragma once

#define INTERPRETER_H
//If INTERFACE_H not defined yet, define it and include
//what should be included in order to utilize the implementation
//in the interface.cpp
#include <vector>
#include <string>
using namespace std;

class Interpreter {

public:
	static bool executeProgram(vector<string> & sourceProgram);

};