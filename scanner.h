#pragma once
#include "sym.h"
#include "symbol.h"
using namespace std;

void scanner(FILE *f);
void reading();
void advance();
struct symbolTag *nextToken();

int nextChar;
int cp = 0;
bool lastline = false;
int linelen = 0;
int linenum = 0;
char line[512];
FILE *sourcefile;

// Get the file descriptor from parser.h
void scanner(FILE *f)
{
	sourcefile = f;
}

// Check if reaches the EOF, else read next line
void reading() {

	if (feof(sourcefile))
	{
		linelen = 0;
		nextChar = -1;
	}
	else
	{
		if (fgets(line, 512, sourcefile) != NULL)
		{
			linelen = strlen(line);
			linenum++;
			cp = 0;
			nextChar = line[cp];
			printf("%4d %s", linenum, line);
		}
		else lastline = true;
	}
}

// Continue reading the current line
void advance()
{
	cp++;
	if (cp < linelen)
		nextChar = line[cp];
	else reading();
}

struct symbolTag *nextToken()
{
	static char s[128] = "";
	int n = 0;
	while (1)
	{
		// If the opening character is alphabet 
		if (nextChar >= 'a' && nextChar <= 'z' ||
			nextChar >= 'A' && nextChar <= 'Z')
		{
			// Keep reading to the end of the word
			do
			{
				s[n++] = nextChar;
				advance();
			} while (nextChar >= 'a' && nextChar <= 'z' ||
				nextChar >= 'A' && nextChar <= 'Z' ||
				nextChar >= '0' && nextChar <= '9');
			s[n] = '\0';

			// Check if the word is reserved by default, else store it as an identifier
			if (strcmp(s, "int") == 0)return newSymbol(symINT, linenum, cp, s);
			else if (strcmp(s, "char") == 0)return newSymbol(symCHAR, linenum, cp, s);
			else if (strcmp(s, "double") == 0)return newSymbol(symDOUBLE, linenum, cp, s);
			else if (strcmp(s, "return") == 0)return newSymbol(symRETURN, linenum, cp, s);
			else if (strcmp(s, "if") == 0)return newSymbol(symIF, linenum, cp, s);
			else if (strcmp(s, "while") == 0)return newSymbol(symWHILE, linenum, cp, s);
			else if (strcmp(s, "else") == 0)return newSymbol(symELSE, linenum, cp, s);
			else if (strcmp(s, "do") == 0)return newSymbol(symDO, linenum, cp, s);
			else if (strcmp(s, "break") == 0)return newSymbol(symBREAK, linenum, cp, s);
			else if (strcmp(s, "continue") == 0)return newSymbol(symCON, linenum, cp, s);
			else if (strcmp(s, "for") == 0)return newSymbol(symFOR, linenum, cp, s);
			else if (strcmp(s, "enum") == 0)return newSymbol(symENUM, linenum, cp, s);
			else if (strcmp(s, "const") == 0)return newSymbol(symCONST, linenum, cp, s);
			else if (strcmp(s, "static") == 0)return newSymbol(symSTATIC, linenum, cp, s);
			else if (strcmp(s, "include") == 0)return newSymbol(symINCLUDE, linenum, cp, s);
			else if (strcmp(s, "define") == 0)return newSymbol(symDEFINE, linenum, cp, s);
			else if (strcmp(s, "struct") == 0)return newSymbol(symSTRUCT, linenum, cp, s);
			else if (strcmp(s, "printf") == 0)return newSymbol(symPRINT, linenum, cp, s);
			else return newSymbol(symID, linenum, cp, s);
		}
		// if the opening character is digit
		else if (nextChar >= '0' && nextChar <= '9')
		{
			do
			{
				s[n++] = nextChar;
				advance();
			} while (nextChar >= '0' && nextChar <= '9');
			s[n] = '\0';
			return newSymbol(symNUM, linenum, cp, s);
		}
		// if other cases
		else
		{
			switch (nextChar) {
			case '.':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symPERIOD, linenum, cp, s);
			case '!':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symNEQUAL, linenum, cp, s);
				}
				return newSymbol(symNOT, linenum, cp, s);
			case '~':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symBNOT, linenum, cp, s);
			case '*':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symMPASS, linenum, cp, s);
				}
				return newSymbol(symMUL, linenum, cp, s);
			case '/':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symDPASS, linenum, cp, s);
				}
				return newSymbol(symDIV, linenum, cp, s);
			case '%':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symPPASS, linenum, cp, s);
				}
				return newSymbol(symMODE, linenum, cp, s);
			case '+':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symAPASS, linenum, cp, s);
				}
				else if (nextChar == '+') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symPP, linenum, cp, s);
				}
				return newSymbol(symADD, linenum, cp, s);
			case '-':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symSPASS, linenum, cp, s);
				}
				else if (nextChar == '-') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symSS, linenum, cp, s);
				}
				return newSymbol(symSUB, linenum, cp, s);
			case '=':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symEQUAL, linenum, cp, s);
				}
				return newSymbol(symPASS, linenum, cp, s);
			case '>':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symBIGE, linenum, cp, s);
				}
				else if (nextChar == '>') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					if (nextChar == '=') {
						s[n++] = nextChar;
						s[n] = '\0';
						advance();
						return newSymbol(symBRSPASS, linenum, cp, s);
					}
					return newSymbol(symBRS, linenum, cp, s);
				}
				return newSymbol(symBIG, linenum, cp, s);
			case '<':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symSMALLE, linenum, cp, s);
				}
				else if (nextChar == '<') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					if (nextChar == '=') {
						s[n++] = nextChar;
						s[n] = '\0';
						advance();
						return newSymbol(symBLSPASS, linenum, cp, s);
					}
					return newSymbol(symBLS, linenum, cp, s);
				}
				return newSymbol(symSMALL, linenum, cp, s);
			case '&':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '&') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symAND, linenum, cp, s);
				}
				else if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symBAPASS, linenum, cp, s);
				}
				return newSymbol(symBAND, linenum, cp, s);
			case '^':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symBXPASS, linenum, cp, s);
				}
				return newSymbol(symBXOR, linenum, cp, s);
			case '|':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				if (nextChar == '|') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symOR, linenum, cp, s);
				}
				else if (nextChar == '=') {
					s[n++] = nextChar;
					s[n] = '\0';
					advance();
					return newSymbol(symBOPASS, linenum, cp, s);
				}
				return newSymbol(symBOR, linenum, cp, s);
			case ',':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symCOMMA, linenum, cp, s);
			case '(':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symLBRAS, linenum, cp, s);
			case ')':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symRBRAS, linenum, cp, s);
			case '[':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symLBRAM, linenum, cp, s);
			case ']':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symRBRAM, linenum, cp, s);
			case '{':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symLBRAL, linenum, cp, s);
			case '}':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symRBRAL, linenum, cp, s);
			case ';':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symSEMI, linenum, cp, s);
			case '\"':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symSQM, linenum, cp, s);
			case '\'':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symSQQM, linenum, cp, s);
			case '#':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symHASH, linenum, cp, s);
			case '@':
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symALPHA, linenum, cp, s);
			case '\0':
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				advance();
				break;
			case -1:
				return newSymbol(symNULL, linenum, 0, s);
			default:
				s[n++] = nextChar;
				s[n] = '\0';
				advance();
				return newSymbol(symUNKNOWN, linenum, cp, s);
			}
		}
	}
}