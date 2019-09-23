#pragma once
#include <fstream>
#include "parser.h"
using namespace std;

int Calculate(string str);
void Postorder(string str);
void Initmap();
void Corecal();
int Conseq(int num1, int num2, int oppo);
int Implement(syntax *one);
int data(string str);
void change(func *ptr, bool ch);
void Begin(char* input);

vector<pair<int, int>> area, atemp;		// domain name, domain order
vector<syntax*> post, ans;				// postfix and temporary results
map<int, int> priority, ifrec;
record *inreturn;						
stack<func*>loop;						// temporary loop body
fstream fp;

// Convert infix to postfix
void Postorder(string str) {


	int pos = 2, bound = 1, operate, did = 0;
	string temp, pretext = " ", t_temp;
	stack<int> op;

	post.clear();

	while (1) {		

		pos = str.find(" ", pos);
		if (pos == -1)break;
		temp = str.substr(++bound, pos - bound);
		if (temp[0] == '#') {						// bound a number with + or -
			t_temp.assign(temp, 1, temp.size() - 1);
			if (pretext == "100" || pretext == "101") post.push_back(recItem(_num, t_temp, stoi(pretext), 0));
			else post.push_back(recItem(_num, t_temp, 0, 0));
		}
		else if (temp[0] >= 'a' && temp[0] <= 'z' ||
			temp[0] >= 'A' && temp[0] <= 'Z') {		// bound an identifier with prefix(++ or -- or + or -)
			if (pretext == "100" || pretext == "101" || pretext == "4" || pretext == "5")
				post.push_back(recItem(_id, temp, stoi(pretext), 0));
			else post.push_back(recItem(_id, temp, 0, 0));
		}
		else if (temp == "100" || temp == "101");
		else if (temp == "4" || temp == "5") {		// bound an identifier with suffix(++ or --)
			if (pretext[0] >= 'a' && pretext[0] <= 'z' ||
				pretext[0] >= 'A' && pretext[0] <= 'Z') {
				post.back()->suffix = stoi(temp);
			}
		}
		else {	// main conversion
			operate = stoi(temp);
			did = 0;
			if (operate == symLBRAS)op.push(operate);
			else if (operate == symRBRAS) {
				while (!op.empty()) {
					if (op.top() != symLBRAS) {
						post.push_back(recItem(_op, to_string(op.top()), 0, 0));
						op.pop();
					}
					else { op.pop(); break; }
				}
			}
			else {
				if (op.empty())op.push(operate);
				else {
					while (!op.empty()) {
						if (priority[operate] < priority[op.top()]) {
							op.push(operate); did = 1; break;
						}
						else { post.push_back(recItem(_op, to_string(op.top()), 0, 0)); op.pop(); }
					}
					if (!did)op.push(operate);
				}
			}
		}
		pretext = temp;
		bound = pos++;
	}
	while (!op.empty()) { post.push_back(recItem(_op, to_string(op.top()), 0, 0)); op.pop(); }


}

// Calculate the result of an expression
int Calculate(string str) {

	Postorder(str);
	ans.clear();
	int num1, num2, curop, temp;

	syntax *first, *second;
	for (int i = 0; i < post.size(); ++i) {
		if (post[i]->type == _num)ans.push_back(post[i]);
		else if (post[i]->type == _id)ans.push_back(post[i]);
		else {
			curop = stoi(post[i]->item);
			if (curop == symNOT) {
				first = ans.back();
				ans.pop_back();
				num1 = Implement(first);
				ans.push_back(recItem(_num, to_string(!num1), 0, 0));
			}
			else if (curop == symBNOT) {
				first = ans.back();
				ans.pop_back();
				num1 = Implement(first);
				ans.push_back(recItem(_num, to_string(~num1), 0, 0));
			}
			else {
				second = ans.back();	
				ans.pop_back();
				first = ans.back();		
				ans.pop_back();
				if (priority[curop] == 15) {	// assign statement
					num2 = Implement(second);
					num1 = Implement(first);		
				}
				else {
					num1 = Implement(first);
					num2 = Implement(second);		
				}
				temp = Conseq(num1, num2, curop);
				if (priority[curop] == 15)inreturn->value = temp;
				ans.push_back(recItem(_num, to_string(temp), 0, 0));
			}
		}
	}
	if (ans.back()->type == _num)return stoi(ans[0]->item);
	else return Implement(ans.back());
}

// Calculate consequences
int Conseq(int num1, int num2, int oppo) {

	switch (oppo) {
	case symMUL:
		return num1 * num2;
	case symDIV:
		return num1 / num2;
	case symMODE:
		return num1 % num2;
	case symADD:
		return num1 + num2;
	case symSUB:
		return num1 - num2;
	case symBLS:
		return num1 << num2;
	case symBRS:
		return num1 >> num2;
	case symBIG:
		return num1 > num2;
	case symSMALL:
		return num1 < num2;
	case symBIGE:
		return num1 >= num2;
	case symSMALLE:
		return num1 <= num2;
	case symEQUAL:
		return num1 == num2;
	case symNEQUAL:
		return num1 != num2;
	case symBAND:
		return num1 & num2;
	case symBXOR:
		return num1 ^ num2;
	case symBOR:
		return num1 | num2;
	case symAND:
		return num1 && num2;
	case symOR:
		return num1 || num2;
	case symPASS:
		return num1 = num2;
	case symAPASS:
		return num1 += num2;
	case symSPASS:
		return num1 -= num2;
	case symMPASS:
		return num1 *= num2;
	case symDPASS:
		return num1 /= num2;
	case symPPASS:
		return num1 %= num2;
	case symBAPASS:
		return num1 &= num2;
	case symBXPASS:
		return num1 ^= num2;
	case symBOPASS:
		return num1 |= num2;
	case symBLSPASS:
		return num1 <<= num2;
	case symBRSPASS:
		return num1 >>= num2;
	case symCOMMA:
		return num2;
	default:
		return 0;
	}
}

// Get the value applied with its prefix and suffix
int Implement(syntax *one) {

	atemp = area;
	int fore;
	record *item = newItem(0, 0, 0, " ");

	if (one->type == _num) {
		if (one->prefix == symNEG) fore = stoi(one->item) * -1;
		else fore = stoi(one->item);
	}
	else {
		int success = 0;
		while (1) {
			for (int i = 0; i < arra.size(); ++i)
				if (arra[i]->name == one->item &&
					arra[i]->d_name == atemp.back().first &&
					arra[i]->d_order == atemp.back().second &&
					arra[i]->used == true) {
					item = arra[i]; success = 1; break;
				}
			if (success)break;
			else atemp.pop_back();
		}
		if (one->prefix == symPP)fore = ++item->value;
		else if (one->prefix == symSS)fore = --item->value;
		else if (one->suffix == symPP)fore = item->value++;
		else if (one->suffix == symSS)fore = item->value--;
		else fore = item->value;

		if (one->prefix == symNEG)fore *= -1;
		inreturn = item;
	}
	return fore;
}

// List priorities
void Initmap() {

	priority[symPP] = 1;
	priority[symSS] = 1;
	priority[symPOS] = 2;
	priority[symNEG] = 2;
	priority[symNOT] = 3;		
	priority[symBNOT] = 3;		
	priority[symMUL] = 4;
	priority[symDIV] = 4;
	priority[symMODE] = 4;
	priority[symADD] = 5;
	priority[symSUB] = 5;
	priority[symBLS] = 6;
	priority[symBRS] = 6;
	priority[symBIG] = 7;
	priority[symSMALL] = 7;
	priority[symBIGE] = 7;
	priority[symSMALLE] = 7;
	priority[symEQUAL] = 8;
	priority[symNEQUAL] = 8;
	priority[symBAND] = 9;
	priority[symBXOR] = 10;
	priority[symBOR] = 11;
	priority[symAND] = 12;
	priority[symOR] = 13;
	priority[symCOLON] = 14;
	priority[symPASS] = 15;
	priority[symAPASS] = 15;
	priority[symSPASS] = 15;
	priority[symMPASS] = 15;
	priority[symDPASS] = 15;
	priority[symPPASS] = 15;
	priority[symBAPASS] = 15;
	priority[symBXPASS] = 15;
	priority[symBOPASS] = 15;
	priority[symBLSPASS] = 15;
	priority[symBRSPASS] = 15;
	priority[symCOMMA] = 16;
	priority[symLBRAS] = 17;

}

// Change domain and reset usage for loop
void change(func *ptr, bool ch) {

	if (ch == true) {
		area.push_back(make_pair(ptr->type, ptr->number));
		for (int i = 0; i < arra.size(); ++i)
			if (arra[i]->d_name == ptr->type && arra[i]->d_order == ptr->number)
				arra[i]->used = ch;
	}
	else {
		area.pop_back();
		for (int i = 0; i < arra.size(); ++i)
			if (arra[i]->d_name == ptr->type && arra[i]->d_order == ptr->number)
				arra[i]->used = ch;
	}
}

// Extract the value of an identifer
int data(string str) {

	atemp = area;
	int success = 0, value = NUMBER;
	while (1) {
		for (int i = 0; i < arra.size(); ++i)
			if (arra[i]->name == str &&
				arra[i]->d_name == atemp.back().first &&
				arra[i]->d_order == atemp.back().second &&
				arra[i]->used == true) {						
				value = arra[i]->value; success = 1; break;
			}
		if (success)break;
		else atemp.pop_back();
	}
	return value;
}

// Follow the program flow and execute codes
void Corecal() {

	int end = 0, carry, dtemp, otemp, whvalue = 0;
	int dd, cc, p1;
	func *ptr, *ptr2;

	while (!opera.empty()) {
		ptr = opera.front();
		opera.pop_front();
		loop.push(ptr);

		if (ptr->action == "@ ")continue;
		switch (ptr->type) {
		case global:
			change(ptr, true);
			break;
		case _main:
			if (ptr->action == "push")change(ptr, true);
			else change(ptr, false);
			break;
		case _null:
			if (ptr->action == "push")change(ptr, true);
			else change(ptr, false);
			break;
		case _while:
			if (ptr->action == "push") change(ptr, true);
			else if (ptr->action == "pop") {
				change(ptr, false);
				while (1) {		// put while's body back to flow
					if (loop.top()->type == ptr->type &&
						loop.top()->number == ptr->number &&
						loop.top()->action == "push")break;
					else {
						opera.push_front(loop.top());
						loop.pop();
					}
				}
				opera.push_front(loop.top());	// put 'push' back to flow			
				loop.pop();
				opera.push_front(loop.top());	// put while's condition expression back to flow		
				loop.pop();
			}
			else {
				if (!Calculate(ptr->action)) {
					dtemp = ptr->type;
					otemp = ptr->number;
					while (1) {
						ptr2 = opera.front();
						opera.pop_front();
						loop.push(ptr2);
						if (ptr2->type == dtemp &&
							ptr2->number == otemp &&
							ptr2->action == "pop")break;
					}
				}
			}
			break;
		case _if:
			if (ptr->action == "push")change(ptr, true);
			else if (ptr->action == "pop") change(ptr, false);
			else {
				if (!(ifrec[ptr->number] = Calculate(ptr->action))) {
					dtemp = ptr->type;
					otemp = ptr->number;
					while (1) {
						ptr2 = opera.front();
						opera.pop_front();
						loop.push(ptr2);
						if (ptr2->type == dtemp &&
							ptr2->number == otemp &&
							ptr2->action == "pop")break;
					}
				}
			}
			break;
		case _else:
			if (ptr->action == "push") {
				if (ifrec[ptr->number]) {
					otemp = ptr->number;
					while (1) {
						ptr2 = opera.front();
						opera.pop_front();
						loop.push(ptr2);
						if (ptr2->type == _else &&
							ptr2->number == otemp &&
							ptr2->action == "pop")break;
					}
				}
				else { change(ptr, true); }
			}
			else if (ptr->action == "pop")
				if (!ifrec[ptr->number])change(ptr, false);

			break;
		case _return:
			carry = Calculate(ptr->action);
			end = 1;
			break;
		case _break:	// break
			while (1) {
				ptr2 = opera.front();
				opera.pop_front();
				loop.push(ptr2);
				if (ptr2->type == _while &&
					ptr->number == ptr2->number &&
					ptr2->action == "pop") break;
			}
			while (1) {
				if (area.back().first == _while &&
					area.back().second == ptr->number)break;
				else {
					for (int i = 0; i < arra.size(); ++i)
						if (arra[i]->d_name == area.back().first &&
							arra[i]->d_order == area.back().second)
							arra[i]->used = false;
					area.pop_back();
				}
			}
			for (int i = 0; i < arra.size(); ++i)
				if (arra[i]->d_name == area.back().first &&
					arra[i]->d_order == area.back().second)
					arra[i]->used = false;
			area.pop_back();
			break;
		case _con:		// continue
			while (1) {
				if (loop.top()->type == _while &&
					loop.top()->number == ptr->number &&
					loop.top()->action == "push")break;
				else {
					opera.push_front(loop.top());
					loop.pop();
				}
			}
			opera.push_front(loop.top());				
			loop.pop();
			opera.push_front(loop.top());				
			loop.pop();
			while (1) {
				if (area.back().first == _while &&
					area.back().second == ptr->number)break;
				else {
					for (int i = 0; i < arra.size(); ++i)
						if (arra[i]->d_name == area.back().first &&
							arra[i]->d_order == area.back().second)
							arra[i]->used = false;
					area.pop_back();
				}
			}
			for (int i = 0; i < arra.size(); ++i)
				if (arra[i]->d_name == area.back().first &&
					arra[i]->d_order == area.back().second)
					arra[i]->used = false;
			area.pop_back();
			break;
		case _print:	//printf
			dd = ptr->action.find("%d", 1);
			cc = ptr->action.find("%c", 1);
			p1 = ptr->action.find("\"", 1);
			if (dd == -1 && cc == -1) {	// print the string only
				p1 = ptr->action.find("\"", 1);
				string str3 = ptr->action.substr(1, p1-1);
				fp << str3;
			}
			else if (cc == -1 || (dd < cc && dd != -1)) { 
				if (ptr->number) {
					string str1 = ptr->action.substr(p1 + 2);
					string str2 = to_string(data(str1));
					string str3 = ptr->action;
					str3.replace(dd, 2, str2);
					p1 = str3.find("\"", 1);
					fp << str3.substr(1, p1 - 1);
				}
				else {
					int number = NUMBER;
					string str2 = to_string(number);
					string str3 = ptr->action;
					str3.replace(dd, 2, str2);
					p1 = str3.find("\"", 1);
					fp << str3.substr(1, p1 - 1);
				}
			}
			else if(dd == -1 || (cc < dd && cc != -1)) {
				if (ptr->number) {
					string str1 = ptr->action.substr(p1 + 2);
					char str2 = data(str1);
					string str3 = ptr->action;
					str3.replace(cc, 2, 1, str2);
					p1 = str3.find("\"", 1);
					fp << str3.substr(1, p1 - 1);

				}
				else {
					char str2 = NUMBER;
					string str3 = ptr->action;
					str3.replace(cc, 2, 1,str2);
					p1 = str3.find("\"", 1);
					fp << str3.substr(1, p1 - 1);
				}
			}
			break;
		default:
			carry = Calculate(ptr->action);
		}
		if (end)break;
	}
}

// Execute the program
void Begin(char* input) {
	
	
	fp.open(input, ios::out); // open a file to write the result of printf

	Initmap();
	Corecal();
	
	fp.close();
}