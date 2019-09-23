#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#define MAX_NAME 50
using namespace std;

// Enumeration for different domains, types, and units
enum Domain { global = 10, _main, _null, _if, _else, _while };
enum Type { _int, _char, _return = 6, Arith };
enum Unit { _id = 200, _num, _op , _break, _con, _print};

// Symbol structure
typedef struct symbolTag
{
	int sym;
	int left;			// line number
	int right;			// postion in the line
	char value[MAX_NAME];		// name

}symbol;

// Store symbols
symbol *newSymbol(int sym, int left, int right, char *value)
{
	symbol *p = new symbol;
	p->sym = sym;
	p->left = left;				
	p->right = right;			
	strcpy(p->value, value);	
	return p;
}

// Identifier structure
typedef struct Record {
	
	int d_name;			// domain name
	int d_order;			// domain order
	int type;			// type
	string name;			// name
	int value;
	bool used;

}record;

// Store Identifiers
record *newItem(int dname, int dorder, int var, char *inn )
{
	record *p = new record;
	p->d_name = dname;		
	p->d_order = dorder;	
	p->type = var;			
	p->name.assign(inn);	
	p->value = 117;			// default value
	p->used = true;
	return p;
};

// Function structure
typedef struct Function {
	
	int type;			// type of function
	int number;			// time of type
	string action;			// function body

}func;

// Store functions
func *newFunc(int var, int num, string action) {

	func *p = new func;			
	p->type = var;				
	p->number = num;			
	p->action.assign(action);	
	return p;
}

// Domain structure
typedef struct Level {

	int dname;
	int dorder;
	bool left;
	int pos;

}level;

// Store domains
level *newLev(int dname, int dorder, int left, int cond) {

	level *p = new level;
	p->dname = dname;
	p->dorder = dorder;
	p->left = left;
	p->pos = cond;
	return p;
}

// Store identifiers for execution
typedef struct Surround {

	int type;
	string item;
	int prefix;
	int suffix;

}syntax;

// Store an identifier with side effects
syntax *recItem(int type, string item, int pre, int suf) {

	syntax *p = new syntax;
	p->type = type;
	p->item = item;
	p->prefix = pre;
	p->suffix = suf;
	return p;
}
