/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include <sstream>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program &program);

/* Main program */

int main() {
	Program program;
	// cout << "Stub implementation of BASIC" << endl;
	while (true) {
		try {
			std::string line;
			std::getline(std::cin, line);
			processLine(line, program);
		} catch (ErrorException &ex) {
			std::cout << ex.getMessage() << std::endl;
		}
	}
	return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program &program) {
	if (line == "RUN")
		program.execute();

	else if (line == "LIST")
		std::cout << program.to_string();

	else if (line == "CLEAR")
		program.clear();

	else if (line == "QUIT")
		exit(0);

	else if (line == "HELP") {
		std::cout << std::endl;
		std::cout << "------------------------------------------------------" << std::endl;
		std::cout << std::endl;
		std::cout << "Simple Basic interpreter" << std::endl;
		std::cout << "By AntiLeaf (Shangfei Yang)   Student ID: 518030910434" << std::endl;
		std::cout << "Tool: Visual Studio 2017" << std::endl;
		std::cout << std::endl;
		std::cout << "I'm sorry I haven't written any help messages now." << std::endl;
		std::cout << "Maybe I'll complete it soon." << std::endl;
		std::cout << std::endl;
		std::cout << "Thanks for using!" << std::endl;
		std::cout << std::endl;
		std::cout << "------------------------------------------------------" << std::endl;
		std::cout << std::endl;
	}

	else {
		try {
			std::stringstream stream(line);
			int linum;
			stream >> linum;

			if (stream.fail()) // Run it derictly
				program.execute_directly(line);

			else {
				if (stream.eof())
					program.removeSourceLine(linum);

				else {
					std::getline(stream, line);
					int k = 0;
					while (k < (int)line.length() && isblank(line[k]))
						k++;
					line = line.substr(k);

					program.addSourceLine(linum, line);
				}
			}
		} catch(...) {
			throw;
		}
	}
}
