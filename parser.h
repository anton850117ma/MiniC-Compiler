#pragma once
#include <vector>
#include <stack>
#include <utility>
#include <queue>
#include <deque>
#include <map>
#include "scanner.h"
#include "err.h"
using namespace std;

void print();
void store(char *str, int type);
void Error(int kind);
void storeItem(int var, char* name);
void Arithmetic(int con);
void NChoice(int con);
void Ending(int temp);
bool Lcheck();
bool Echeck(int temp);
void Mainfun();
void Iffun();
void Whfun();
void Printfun();
void Processor();
void Statement(int temp);
void Parser();
void next();
void Scanner(char *input);

symbol *token, *t_token;	// current token and previous token
vector<string> errors;		// error messages
stack<level*> fun;		// current levels
map<int, int> domain;		// domain name, domain number
vector<record*> arra;		// identifiers
deque<func*> opera;		// program flow

int whether = 0;		// checking status
int errorCount = 0;		// number of errors
int bracket[6] = { 0 };		// 6 kinds of brackets' numbers: '(', ')', '[', ']', '{', '}'
int theend = 1;			// check if exist an expression outside 'main'

// Print all error messages
void print() {
	for (int i = 0; i < errors.size(); ++i)
		cout << errors[i];
}

// Expand an expression stored in a function
void store(char *str, int type) {
	
	string temp;
	
	if (type == symID) { 
		temp.assign(str);
		opera.back()->action += temp + ' '; 
	}
	else if (type == symNUM) {
		char ss[128];
		sprintf(ss, "%c%s", '#', str);	// specify as a number with '#'
		temp.assign(ss);
		opera.back()->action += temp + ' ';
	}
	else opera.back()->action += to_string(type) + ' ';

}

// Construct error messages and finding another semicolon or EOF
void Error(int kind) {


	++errorCount;
	char temp[100];
	
	if (kind == 0)		// identifier undeclared
		sprintf(temp, "%4d: line%d:\t '%s' %s\n", errorCount, token->left, token->value, errmsgs[kind]);
	else if (kind == 1)	// identifier redeclared
		sprintf(temp, "%4d: line%d:\t '%s' %s\n", errorCount, token->left, token->value, errmsgs[kind]);
	else if (kind == 4)	// expect semicolon
		sprintf(temp, "%4d: line%d:\t %s\n", errorCount, t_token->left, errmsgs[kind]);
	else			// other errors
		sprintf(temp, "%4d: line%d:\t %s\n", errorCount, token->left, errmsgs[kind]);

	string tempp(temp);
	errors.push_back(tempp);
	while (1) {
		if (token->sym == symSEMI) {	// if encounter semicolon, reset status and continue checking
			if (bracket[0] > bracket[1])bracket[0] = bracket[1];
			else bracket[1] = bracket[0];
			next(); Parser(); break;
		}
		else if (token->sym == symNULL) // if encounter EOF, leave
			break;
		next();
	}
} 

// Store identifiers only
void storeItem(int var, char* name) {
	arra.push_back(newItem(fun.top()->dname, fun.top()->dorder, var, name));
}

// Parse operators
void Arithmetic(int con) {

	switch (token->sym) {
	case symRBRAS: // ')'
		bracket[1]++;
		store(token->value, token->sym);
		next();
		if (con <= Arith) Arithmetic(con);	// if in an expression
		else {
			if (bracket[0] == bracket[1]) {	// end the expression and go to parse the function's body
				opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "push"));	// To a new domain
				if (token->sym == symLBRAL) {	// with '{'
					bracket[4]++;
					next(); Parser();
				}
				else { 
					opera.push_back(newFunc(Arith, 0, "@ "));	// without '{'
					fun.top()->left = false;
					Parser();
				}
			}
			else if (bracket[0] > bracket[1]) Arithmetic(con);	// continue parse in a expression
		}
		break;
	case symNOT:
		store(token->value, token->sym);
		next();	Statement(con);
	case symBNOT:
		store(token->value, token->sym);
		next();	Statement(con);
	case symMUL:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symDIV:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symMODE:
		store(token->value, token->sym);
		next();	Statement(con);
	case symADD:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symSUB:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symBLS:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symBRS:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symBIG:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symSMALL:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symBIGE:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symSMALLE:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symEQUAL:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symNEQUAL:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symBAND:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symBXOR:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symBOR:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symAND:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symOR:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	/*case symQUES:
		store(token->value, token->sym);
		next();	Statement(con);
		break;
	case symCOLON:
		store(token->value, token->sym);
		next();	Statement(con);
		break;*/
	case symPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symAPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symSPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symMPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symDPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symPPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symBAPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symBXPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symBOPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symBLSPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symBRSPASS:
		if (whether == 1)Error(17);
		else {
			store(token->value, token->sym);
			next();	Statement(con);
		}
		break;
	case symCOMMA:
		store(token->value, token->sym);
		next();	Statement(opera.back()->type);
		break;
	case symSEMI:
		if (Lcheck()) { next(); Ending(con); }
		else Error(11);
		break;
	case symNULL:
		break;
	default:
		Error(3);
	}
}

// Check which condition is upcoming
void NChoice(int con) {

	switch (token->sym) {
	case symCOMMA:	// do something again
		store(token->value, token->sym);
		next();
		Statement(opera.back()->type);
		break;
	case symPASS:	// parse statement
		store(token->value, token->sym);
		next();
		Statement(Arith);
		break;
	case symSEMI:	// end of a domain
		if (Lcheck()) { next(); Ending(con); }
		else Error(11);
		break;
	default:
		Error(4);
	}
}

// Check for if statement
void Ending(int temp) {

	whether = 0;
	if (temp >= Arith && !fun.top()->left) { 
		opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "pop"));
		fun.pop();
		if (opera.back()->type == _if && token->sym == symELSE) {	// if there is an else statement after if statement
			next();
			if (token->sym == symLBRAL) {		// followed by '{'
				fun.push(newLev(_else, opera.back()->number, true, fun.top()->pos));
				opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "push"));
				bracket[4]++; next(); Parser();
			}
			else if (token->sym == symSEMI) {	// followed by ';'
				fun.push(newLev(_else, opera.back()->number, false, fun.top()->pos));
				opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "push"));
				opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "pop"));
				fun.pop();
				next(); Parser();
			}
			else {								// followed by an expression
				fun.push(newLev(_else, opera.back()->number, false, fun.top()->pos));
				opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "push"));
				Parser();
			}
		}
		else Parser(); 
	}
	else Parser();
}

// Check that whether brackets are paired
bool Lcheck() {

	bool ok = true;

	// Does not check '[' and ']' because this compiler cannot deal with array
	for (int i = 0; i < 3; i += 2)
		if (bracket[i] != bracket[i + 1])ok = false;
	return ok;
}

// Check if an identifier was undeclared or redeclared
bool Echeck(int temp) {

	string temp2(token->value);
	if (temp < _return) {
		for (int i = 0; i < arra.size(); ++i) {
			if (fun.top()->dname == arra[i]->d_name) {
				if (temp2 == arra[i]->name)return false;	// redeclared
			}
		}
		return true;
	}
	else {
		for (int i = 0; i < arra.size(); ++i) {
			if (temp2 == arra[i]->name && arra[i]->used == true)return true;
		}
		return false;	// undeclared
	}
}

// Only 'main(){' format is acceptable in this compiler
void Mainfun() {
	next();
	if (token->sym != symLBRAS)Error(12);
	else {
		bracket[0]++;
		next();
		if (token->sym != symRBRAS)Error(8);
		else {
			bracket[1]++;
			next();
			if (token->sym != symLBRAL) Error(9);
			else {
				fun.push(newLev(_main, ++domain[_main], true, domain[_while]));
				opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "push"));
				bracket[4]++;
				theend--;
				next();
				Parser();
			}
		}
	}
}

// Get into if statement's domain
void Iffun() {

	if (token->sym != symLBRAS)Error(12);
	else {
		bracket[0]++;
		opera.push_back(newFunc( _if, ++domain[_if],"@ "));
		fun.push(newLev(_if, domain[_if], true, fun.top()->pos));
		store(token->value, token->sym);
		next();
		Statement(_if);
	}
}

// Get into while statement's domain
void Whfun() {
	if (token->sym != symLBRAS)Error(12);
	else {
		bracket[0]++;
		opera.push_back(newFunc(_while, ++domain[_while], "@ "));
		fun.push(newLev(_while, domain[_while], true, domain[_while]));
		store(token->value, token->sym);
		next();
		Statement(_while);
	}
}

// Check that whether printf's format is accepable
void Printfun() {

	int getyou = 1; char changing, detect = 1;
	if (token->sym != symLBRAS)Error(12);
	else {
		bracket[0]++;
		next();
		string ss;
		if (token->sym != symSQM)Error(32); 
		else {
			ss += '\"';
			while (1) {
				if (cp == linelen-1 && nextChar != '\"') { getyou = 0; break; }
				if (nextChar != '\"') { 
					if (nextChar == '\\') {
						advance();
						if (nextChar == '\"') { changing = '\"'; ss += changing; }
						else if (nextChar == '\'') { changing = '\''; ss += changing; }
						else if (nextChar == 'n') { changing = '\n'; ss += changing; }
						else if (nextChar == 't') { changing = '\t'; ss += changing; }
						else if (nextChar == 'r') { changing = '\r'; ss += changing; }
						else if (nextChar == 'v') { changing = '\v'; ss += changing; }
						else if (nextChar == 'b') { changing = '\b'; ss += changing; }
						else if (nextChar == 'f') { changing = '\f'; ss += changing; }
						else if (nextChar == 'a') { changing = '\a'; ss += changing; }
						else if (nextChar == '0') { changing = '\0'; ss += changing; }
						else if (nextChar == '\\') { changing = '\\'; ss += changing; }
						else {
							changing = nextChar;
							ss += changing;
						}
					}
					else {
						changing = nextChar;
						ss += changing;
					}
				}
				else { break; }
				advance();
			}
			ss += '\"';
			if (!getyou)Error(15);
			else {
				next();	next();
				if (token->sym == symCOMMA) { 
					next();
					if (token->sym != symID)Error(16);
					else {
						if (!Echeck(_print))Error(0);
						else {
							string tt(token->value);
							ss += ' ' + tt;	next();
							if (token->sym != symRBRAS) Error(8);
							else {
								bracket[1]++; next();
								if (token->sym != symSEMI)Error(4);
								else {
									opera.push_back(newFunc(_print, 1, ss));
									NChoice(_print);
								}
							}
						}
					}
				}
				else if (token->sym == symRBRAS){
					bracket[1]++; next();
					if (token->sym != symSEMI)Error(4);
					else {
						opera.push_back(newFunc(_print, 0, ss));
						NChoice(_print);
					}
				}
				else Error(8);
			}
		}
	}
}

// Deal with possible character input and store it in number
void Processor() {

	string ss;
	char changing;
	int getyou = 1, detect = 1;
	while (1) {
		if (cp == linelen - 1 && nextChar != '\'') { getyou = 0; break; }
		if (nextChar != '\'') {
			if (nextChar == '\\') {
				advance();
				if (nextChar == '\'') { changing = '\''; ss += changing; }
				else if (nextChar == '\"') { changing = '\"'; ss += changing; }
				else if (nextChar == 'n') { changing = '\n'; ss += changing; }
				else if (nextChar == 't') { changing = '\t'; ss += changing; }
				else if (nextChar == 'r') { changing = '\r'; ss += changing; }
				else if (nextChar == 'v') { changing = '\v'; ss += changing; }
				else if (nextChar == 'b') { changing = '\b'; ss += changing; }
				else if (nextChar == 'f') { changing = '\f'; ss += changing; }
				else if (nextChar == 'a') { changing = '\a'; ss += changing; }
				else if (nextChar == '0') { changing = '\0'; ss += changing; }
				else if (nextChar == '\\') { changing = '\\'; ss += changing; }
				else {
					changing = nextChar;
					ss += changing;
				}
			}
			else {
				changing = nextChar;
				ss += changing;
			}
		}
		else { break; }
		advance();
	}
	if (!getyou )Error(33);
	else {
		if (ss.length() < 1)Error(7);
		else {
			int buffer = ss[ss.length() - 1];
			char integer[5];
			sprintf(integer, "%d", buffer);
			store(integer, symNUM);
			next();
		}
	}
}

// Parse the remaining part of an expression after Parser()
void Statement(int temp) {

	whether = 0;

	
	if (temp == _int) {			// Parse an expression after 'int'
		if (token->sym == symID) {
			if (strcmp(token->value, "main") == 0) {
				if (domain[_main] > 0)Error(2);
				else Mainfun();
			}
			else {
				if (Echeck(temp)) {
					store(token->value, token->sym);
					storeItem(temp, token->value);
					next(); NChoice(temp);
				}
				else Error(1);
			}
		}
		else if(token->sym == symSEMI)NChoice(temp);
		else Error(3);
	}
	else if (temp == _char) {	// Parse an expression after 'char'
		if (token->sym == symID) {
			if (Echeck(temp)) {
				store(token->value, token->sym);
				storeItem(temp, token->value);
				next(); NChoice(temp);
			}
			else Error(1);
		}
		else if (token->sym == symSEMI)NChoice(temp);
		else Error(3);
	}
	else if (temp >= Arith) {	// Parse an expression
		if (token->sym == symNUM) {											
			if (t_token->sym == symSEMI || t_token->sym == symRBRAL) 
				opera.push_back(newFunc( temp, 0,"@ "));
			store(token->value, token->sym); whether = 1;
			next(); Arithmetic(temp);
		}
		else if (token->sym == symID) {										
			if (Echeck(temp)) {
				if (t_token->sym == symSEMI || t_token->sym == symRBRAL) 
					opera.push_back(newFunc( temp, 0,"@ "));
				store(token->value, token->sym);
				next(); 
				if (token->sym == symPP || token->sym == symSS) {			
					store(token->value, token->sym);
					whether = 1; next();
					Arithmetic(temp);
				}
				else Arithmetic(temp);
			}
			else Error(0);
		}
		else if (token->sym == symSQQM) {							
			if (t_token->sym == symSEMI || t_token->sym == symRBRAL)
				opera.push_back(newFunc( temp, 0,"@ "));
			Processor();
			whether = 1;
			next(); Arithmetic(temp);
		}
		else if (token->sym == symLBRAS) {									
			bracket[0]++;
			if (t_token->sym == symSEMI || t_token->sym == symRBRAL) 
				opera.push_back(newFunc( temp, 0,"@ "));
			store(token->value, token->sym); next(); Statement(temp);
		}
		else if (token->sym == symADD || token->sym == symSUB) {			
			if (t_token->sym == symSEMI || t_token->sym == symRBRAL)
				opera.push_back(newFunc(temp, 0, "@ "));
			whether = 1;
			store(token->value, token->sym == symADD ? symPOS : symNEG); next();
			if (token->sym == symID) {
				if (Echeck(temp)) {
					store(token->value, token->sym); next();
					if (token->sym == symPP || token->sym == symSS) {
						store(token->value, token->sym); next();
						Arithmetic(temp);
					}
					else Arithmetic(temp);
				}
				else Error(0);
			}
			else if (token->sym == symNUM) {
				store(token->value, token->sym); next();
				Arithmetic(temp);
			}
			else if (token->sym == symSQQM) {
				Processor();
				whether = 1;
				next(); Arithmetic(temp);
			}
			else Error(20);
		}
		else if (token->sym == symPP || token->sym == symSS) {				
			if (t_token->sym == symSEMI || t_token->sym == symRBRAL)
				opera.push_back(newFunc(temp, 0, "@ "));
			whether = 1;
			store(token->value, token->sym); next();
			if (token->sym == symID) {
				if (Echeck(temp)) {
					store(token->value, token->sym); next();
					Arithmetic(temp);
				}
				else Error(0);
			}
			else Error(20);
		}
		else if(token->sym == symSEMI)NChoice(temp);
		else Error(3);
	}
}

// Parse the opening token of an expression
void Parser() {

	if (token->sym != symNULL) {
		if (theend < 0)Error(21);							
		else if (token->sym == symINT) {
			if (fun.top()->left == false)Error(22);				
			else {
				opera.push_back(newFunc(_int, 0, "@ "));	// '@ ' means doing nothing
				next(); Statement(_int);
			}
		}
		else if (token->sym == symCHAR) {
			if (fun.top()->left == false)Error(23);
			else {
				opera.push_back(newFunc( _char, 0,"@ "));
				next(); Statement(_char);
			}
		}
		else if (token->sym == symRETURN) {
			if (fun.top()->dname == global)Error(26);
			else {
				opera.push_back(newFunc( _return, 0,"@ "));
				next(); Statement(Arith);
			}
		}
		else if (token->sym == symIF) {
			if (fun.top()->dname == global)Error(27);
			else { next(); Iffun(); }
		}
		else if (token->sym == symWHILE) {
			if (fun.top()->dname == global)Error(28);
			else { next(); Whfun(); }
		}
		else if (token->sym == symPRINT) {
			if (fun.top()->dname == global)Error(14);
			else { next(); Printfun(); }
		}
		else if (token->sym == symBREAK) {
			if (fun.top()->pos == 0)Error(24);
			else {
				next();
				if(token->sym != symSEMI)Error(4);
				else {
					opera.push_back(newFunc(_break, fun.top()->pos, " "));
					next();
					Ending(_break);
				}
			}
		}
		else if (token->sym == symCON) {
			if (fun.top()->pos == 0)Error(25);
			else {
				next();
				if (token->sym != symSEMI)Error(4);
				else {
					opera.push_back(newFunc(_con, fun.top()->pos, " "));
					next();
					Ending(_con);
				}
			}
		}
		else if (token->sym == symSEMI) { next(); Parser(); }
		else if (token->sym == symLBRAL) {
			if (fun.top()->dname == global)Error(29);
			else {
				bracket[4]++;
				fun.push(newLev(_null, ++domain[_null], true, fun.top()->pos));
				opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "push"));	// 'push' means getting into the domain
				next(); Parser();
			}
		}
		else if (token->sym == symRBRAL) {
			if (fun.size() > 1) {
				bracket[5]++;
				for (int j = 0; j < arra.size(); ++j) {
					if (arra[j]->d_name == fun.top()->dname &&
						arra[j]->d_order == fun.top()->dorder)
						arra[j]->used = false;
				}
				opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "pop"));	// 'pop' means leaving the domain
				fun.pop();
				if (fun.top()->dname == global)theend--;
				next(); 
				if (opera.back()->type == _if && token->sym == symELSE) {
					next();
					if (token->sym == symLBRAL) {
						fun.push(newLev(_else, opera.back()->number, true, fun.top()->pos));
						opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "push"));
						bracket[4]++; next(); Parser();
					}
					else if (token->sym == symSEMI) {
						fun.push(newLev(_else, opera.back()->number, false, fun.top()->pos));
						opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "push"));
						opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "pop"));
						fun.pop();
						next(); Parser();
					}
					else {
						fun.push(newLev(_else, opera.back()->number, false, fun.top()->pos));
						opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "push"));
						Parser();
					}
				}
				else Parser();
			}
			else Error(30);
		}
		else { 
			opera.push_back(newFunc(Arith, 0, "@ "));
			Statement(Arith); 
		}
	}
	else {
		if (bracket[4] > bracket[5])Error(31);
		if (domain[_main] != 1)Error(2);
	}
}

// Get next token in the code
void next() {
	t_token = token;
	token = nextToken();
}

// Parse the input codes and record number of errors
void Scanner(char *input) {

	FILE *f = fopen(input, "r");
	scanner(f);

	// start with global domain
	fun.push(newLev(global, ++domain[global], true, domain[_while] = 0));
	opera.push_back(newFunc(fun.top()->dname, fun.top()->dorder, "push"));

	// begin to parse
	next();
	Parser();
	printf("\n compile completed!");
	printf("	\n error count: %d\n", errorCount);
	if (errorCount != 0)print();
	
	fclose(f);
}
