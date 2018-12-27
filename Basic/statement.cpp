/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include <sstream>
#include "parser.h"
#include "statement.h"

#include "../StanfordCPPLib/tokenScanner.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/error.h"
// using namespace std;

/* Implementation of the Statement class */

Statement::Statement() { }

Statement::Statement (const std::string &expression): text(expression) { }

Statement::~Statement() { }

std::string Statement::to_string() {
	return text;
}


SequentialStatement::SequentialStatement(): Statement() { }

SequentialStatement::SequentialStatement (const std::string &line): Statement(line) { }

SequentialStatement::~SequentialStatement() { }


ControlStatement::ControlStatement(): Statement() { }

ControlStatement::ControlStatement (const std::string &line): Statement(line) { }

ControlStatement::~ControlStatement() { }


Reminder::Reminder() { }

Reminder::Reminder (const std::string &line): SequentialStatement(line) { }

Reminder::~Reminder() { }

type_of_statement Reminder::get_type() {
	return REMINDER;
}

void Reminder::execute (EvalState &state) {
	state.next_line();
}


Assignment::Assignment() { }

Assignment::Assignment (const std::string &line): SequentialStatement(line) {
	exp = to_expression(line.substr(line.find("LET") + 4));
	// std::cout << "Assignment: " << line.substr(line.find("LET") + 4) << std::endl;
	if (exp->getType() != COMPOUND)
		error("expression type error");
}

Assignment::~Assignment() {
	delete exp;
}

type_of_statement Assignment::get_type() {
	return ASSIGNMENT;
}

void Assignment::execute (EvalState &state) {
	exp->eval(state);
	state.next_line();
}


Output::Output() { }

Output::Output (const std::string &line): SequentialStatement(line) {
	exp = to_expression(line.substr(line.find("PRINT") + 6));
}

Output::~Output() {
	delete exp;
}

type_of_statement Output::get_type() {
	return OUTPUT;
}

void Output::execute (EvalState &state) {
	std::cout << exp->eval(state) << std::endl;
	state.next_line();
}


Input::Input() { }

Input::Input (const std::string &line): SequentialStatement(line) {
	name = line.substr(line.find("INPUT") + 6);
}

Input::~Input() { }

type_of_statement Input::get_type() {
	return INPUT;
}

void Input::execute (EvalState &state) {
	state.setValue(name, getInteger(" ? "));
	state.next_line();
}


Halt::Halt() { }

Halt::Halt (const std::string &line): ControlStatement(line) { }

Halt::~Halt() { }

type_of_statement Halt::get_type() {
	return HALT;
}

void Halt::execute (EvalState &state) {
	state.halt();
}


Jump::Jump() { }

Jump::Jump (const std::string &line): ControlStatement(line) {
	auto linum_str = line.substr(line.find("GOTO") + 5);

	auto tmp = to_expression(linum_str);
	if (tmp->getType() != CONSTANT)
		error("LINE NUMBER ERROR");
	delete tmp;

	std::stringstream stream(linum_str);
	stream >> linum;
}

Jump::~Jump() { }

type_of_statement Jump::get_type() {
	return JUMP;
}

void Jump::execute (EvalState &state) {
	state.jump_to(linum);
}


Conditional::Conditional() { }

Conditional::Conditional (const std::string &line): ControlStatement(line) {
	auto expr = line.substr(line.find("IF") + 3);
	int pos = (int)expr.find("THEN");
	auto linum_str = expr.substr(pos + 5);
	expr = expr.substr(0, pos - 1);

	auto tmp = to_expression(linum_str);
	if (tmp->getType() != CONSTANT)
		error("LINE NUMBER ERROR");
	delete tmp;

	std::stringstream stream(linum_str);
	stream >> linum;

	int count = 0;
	for (auto oper : {"=", "<", ">"}) {
		if (expr.find(oper) != std::string::npos) {
			pos = expr.find(oper);
			op = oper;
			count++;
		}
	}
	if (!count)
		error("No comparison operator found");
	else if (count > 1)
		error("Multiple comparison operator found");

	left = to_expression(expr.substr(0, pos));
	right = to_expression(expr.substr(pos + 1));

	// std::cout << pos << "expr:" << expr << "     left:" << expr.substr(0, pos) << "    right:" << expr.substr(pos + 1) << std::endl;
}

Conditional::~Conditional() {
	delete left, right;
}

type_of_statement Conditional::get_type(){
	return CONDITIONAL;
}

void Conditional::execute (EvalState &state) {
	int left_value = left->eval(state), right_value = right->eval(state);

	bool result;
	if (op == "=")
		result = (left_value == right_value);
	else if (op == "<")
		result = (left_value < right_value);
	else if (op == ">")
		result = (left_value > right_value);
	else error("Operator \"" + op + "\" is not a valid comparison operator");

	// std::cout << "op:" << op << " left_value = " << left_value << " right_value = " << right_value << std::endl;
	// std::cout << "result = " << (int)result << std::endl;

	if (result) 
		state.jump_to(linum);
	else state.next_line();
}

Statement *to_statement(const std::string &line){
	if (line.find("REM") != std::string::npos)
		return new Reminder(line);

	else if (line.find("LET") != std::string::npos)
		return new Assignment(line);

	else if (line.find("PRINT") != std::string::npos)
		return new Output(line);

	else if (line.find("INPUT") != std::string::npos)
		return new Input(line);

	else if (line.find("END") != std::string::npos)
		return new Halt(line);

	else if (line.find("GOTO") != std::string::npos)
		return new Jump(line);

	else if (line.find("IF") != std::string::npos && line.find("THEN") != std::string::npos)
		return new Conditional(line);

	else error("SYNTAX ERROR");
	
	return nullptr;
}

