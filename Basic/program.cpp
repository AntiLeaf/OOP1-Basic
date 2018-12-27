/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"

// using namespace std;

Program::Program(): state(lines) { }

Program::~Program() { }

void Program::clear() {
	for (auto &item : lines)
		delete item.second;
	lines.clear();
	state = EvalState(lines);
}

void Program::addSourceLine (int lineNumber, std::string line) {
	lines[lineNumber] = to_statement(line);
}

void Program::removeSourceLine (int lineNumber) {
	auto iter = lines.find(lineNumber);
	if (iter != lines.end()) {
		delete iter->second;
		lines.erase(iter);
	}
}

std::string Program::getSourceLine (int lineNumber) {
	auto iter = lines.find(lineNumber);
	if (iter == lines.end())
		error("Line #" + integerToString(lineNumber) + "does not exists");
	return iter->second->to_string();
}

/*
void Program::setParsedStatement (int lineNumber, Statement *stmt) {
	// Replace this stub with your own code
}

Statement *Program::getParsedStatement (int lineNumber) {
	return nullptr;  // Replace this stub with your own code
}
*/

int Program::getFirstLineNumber() {
	return state.first_linum();
}

int Program::getNextLineNumber(int lineNumber) {
	return state.next_linum();
}

void Program::execute() {
	state.start();
	while (!state.finished())
		state.iter->second->execute(state);
}

void Program::execute_directly(const std::string &line) {
	Statement *statement = to_statement(line);
	
	auto type = statement->get_type();
	if (type != INPUT && type != OUTPUT && type != ASSIGNMENT)
		error("SYNTAX ERROR");

	statement->execute(state);
	delete statement;
}

std::string Program::to_string() {
	std::string result;
	for (const auto &line : lines) {
		result += integerToString(line.first);
		result += " ";
		result += line.second->to_string();
		result += "\n";
	}
	return result;
}
