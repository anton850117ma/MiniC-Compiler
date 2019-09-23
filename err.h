#pragma once

// These are the error messages corresponding to the errors that can be detected in parser.h
char errmsgs[50][100] =
{
	"Undeclared.",											//0
	"Redeclared.",											//1
	"No designed start of the program.",					//2
	"Expected expression.",									//3
	"Expected ';'.",										//4
	"missing terminated ' character.",						//5
	"Expected number, identifier, or '('.",					//6
	"empty character constant.",							//7
	"Expected ')'.",										//8
	"Expected '{'.",										//9
	"Expected '}',",										//10
	"Domain error.",										//11
	"Expected '('.",										//12 
	"Declaration error."	,								//13
	"Expected declaration specifiers.",						//14
	"Missing terminating \" character.",					//15
	"Expected identifier",									//16
	"lvalue required as left operand of assignment.",		//17
	"Wrong domain.",										//18
	"Expected no ';'.",										//19
	"Wrong Expression.",									//20
	"Expected no expression.",								//21
	"Expected expression before 'int'.",					//22
	"Expected expression before 'char'.",					//23
	"Break statement not within loop.",						//24
	"Continue statement not within loop.",					//25
	"Expected identifier of '(' before 'return'.",			//26
	"Expected identifier of '(' before 'if'.",				//27
	"Expected identifier of '(' before 'while'.",			//28
	"Expected identifier of '(' before '{' token.",			//29
	"Expected identifier of '(' before '}' token.",			//30
	"Expected declaration or statement at end of input.",	//31
	"Missing starting \" character.",						//32
	"Missing terminating \' character."						//33
};
