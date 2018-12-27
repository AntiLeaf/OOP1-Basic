/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "parser.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

enum type_of_statement {REMINDER, ASSIGNMENT, OUTPUT, INPUT, HALT, JUMP, CONDITIONAL};

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */
	
	Statement();

	Statement (const std::string &line);

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

	virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

	virtual void execute (EvalState & state) = 0;

	virtual type_of_statement get_type() = 0;

	std::string to_string();

protected:

	std::string text;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class SequentialStatement : public Statement {

public:

	SequentialStatement();
	
	SequentialStatement (const std::string &line);
	
	virtual ~SequentialStatement();

	virtual type_of_statement get_type() = 0;
	
	virtual void execute (EvalState &state) = 0;

};

class ControlStatement : public Statement {

public:

	ControlStatement();

	ControlStatement (const std::string &line);
	
	virtual ~ControlStatement();

	virtual type_of_statement get_type() = 0;
	
	virtual void execute (EvalState &state) = 0;

};

class Reminder : public SequentialStatement {

public:

	Reminder();

	Reminder (const std::string &line);

	~Reminder();

	type_of_statement get_type();

	void execute (EvalState &state);

};

class Assignment : public SequentialStatement {

public:

	Assignment();

	Assignment (const std::string &line);

	~Assignment();

	type_of_statement get_type();

	void execute (EvalState &state);

private:
	
	Expression *exp;

};

class Output : public SequentialStatement {
	
public:

	Output();

	Output (const std::string &line);

	~Output();

	type_of_statement get_type();

	void execute (EvalState &state);

private:

	Expression *exp;

};

class Input : public SequentialStatement {

public:

	Input();
	
	Input (const std::string &line);

	~Input();

	type_of_statement get_type();

	void execute (EvalState &state);

private:

	std::string name;

};

class Halt : public ControlStatement {

public:

	Halt();

	Halt (const std::string &line);

	~Halt();

	type_of_statement get_type();

	void execute (EvalState &state);

};

class Jump : public ControlStatement {

public:

	Jump();

	Jump (const std::string &line);

	~Jump();

	type_of_statement get_type();

	void execute (EvalState &state);

private:

	int linum;

};

class Conditional : public ControlStatement {

public:

	Conditional();

	Conditional (const std::string &line);

	~Conditional();

	type_of_statement get_type();

	void execute (EvalState &state);

private:

	std::string op;

	Expression *left, *right;

	int linum;

};

Statement *to_statement (const std::string &line);

#endif
