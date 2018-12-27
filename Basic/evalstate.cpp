/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */

#include <string>
#include "evalstate.h"

#include "../StanfordCPPLib/map.h"
// using namespace std;

/* Implementation of the EvalState class */

// EvalState::EvalState(): variables(), iter(variables.begin()) { }

EvalState::EvalState (const std::map <int, Statement*> &lines):
	variables(), lines(&lines), iter(lines.begin()) { }

EvalState::~EvalState() { }

void EvalState::setValue (string var, int value) {
	variables[var] = value;
}

int EvalState::getValue (std::string var) {
	if (!isDefined(var))
		error("VARIABLE NOT DEFINED");
	return variables[var];
}

bool EvalState::isDefined (std::string var) {
   return variables.count(var);
}

int EvalState::first_linum() {
	if (lines->empty())
		return -1;
	return lines->begin()->first;
}

int EvalState::next_linum() {
	if (iter == lines->end())
		return -1;
	auto it = iter;
	if (++it == lines->end())
		return -1;
	return it->first;
}

bool EvalState::finished() {
	return iter == lines->end();
}

void EvalState::clear() {
	variables.clear();
	iter = lines->begin();
}

void EvalState::start() {
	iter = lines->begin();
}

void EvalState::next_line() {
	if (iter != lines->end())
		iter++;
}

void EvalState::jump_to (int linum) {
	iter = lines->find(linum);
	if (iter == lines->end())
		error("LINE NUMBER ERROR");
}

void EvalState::halt() {
	iter = lines->end();
}
