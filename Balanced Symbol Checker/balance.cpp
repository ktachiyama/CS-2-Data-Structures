#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <stack>
#include <iomanip>
using namespace std;

class BalanceChar;
void printFile(istream &dictfile);
bool balanceFile(istream &dictfile);
bool isMatchingPair(const char b, const int lineNum, stack<BalanceChar> &s);
void pushStack(const char c, const int lineNum, stack<BalanceChar> &s);

//stores the character and what line it's on
class BalanceChar {
private:
	char m_char;
	int m_line;

public:
	BalanceChar() {
		m_line = -1;
		m_char = ' ';
	}
	void setC(char c) {
		m_char = c;
	}
	void setLine(int lineNum) {
		m_line = lineNum;
	}
	char getChar() {
		return m_char;
	}
	int getcLine() {
		return m_line;
	}
};

//prints the  file
void printFile(istream &dictfile) { 
	int lineNumber = 1;
	while (!dictfile.eof()) {
		string p;
		getline(dictfile, p);
		cout << setw(2) << right << lineNumber << " ";
		cout << p << endl;
		lineNumber++;
	}
	dictfile.clear();  //allows file to be read in again later in the program
	dictfile.seekg(0, dictfile.beg);
}

//returns false if the file is not balanced, and true otherwise
bool balanceFile(istream &dictfile) {
	int lineNum = 1;	//keeps track of what line is being read in
	stack <BalanceChar> braceStack;
	stack <char> commentStack;
	stack <char> quoteStack;

	char c;
	while (dictfile.get(c)) {

		if (c == '\n'){
			lineNum++;
		}
		
		//if c is a single quote, search for a matching single 
		//quote (\') on the same line.
		else if (c == '\'') {
			char sq;
			quoteStack.push('/0');
			//ignore all characters until a '\'' or newline is found.
			//if a \' is found, there is a matching quote
			//if a newline is found first, the quote is unbalanced => return false
			while (dictfile.get(sq)) {
				if (sq == '\'') {
					cout << "pair matching ' and '" << endl;

					while (!quoteStack.empty())
						quoteStack.pop();

					break;
				}
				else if (sq == '\n') {
					cout << "unbalanced quote ' on line " << lineNum << endl;
					return false;
				}
				else
					quoteStack.push(sq);
			}
			//if the end of the file is reached before finding a balanced quote,
			//the quote is unbalanced =>return false
			if (!quoteStack.empty()) {
				cout << "unbalanced quote ' on line " << lineNum << endl;
				return false;
			}
		}

		//if c is a double quote, search for a closing double quote.
		//if a newline or the end of the file is reached without finding a
		//double quote, it is unbalanced => return false
		else if (c == '"') {

			char dq;
			int quoteLine = lineNum;
			quoteStack.push('\0');

			while (dictfile.get(dq)) {
				if (dq == '\n') {
					cout << "unbalanced quote " << c << " on line " << quoteLine << endl;
					return false;
				}

				if (dq == '"') {
					cout << "pair matching " << dq << " and " << dq << endl;

					while (!quoteStack.empty())
						quoteStack.pop();

					break;
				}
			}
			if (!quoteStack.empty()) {
				cout << "unbalanced quote " << c << " on line " << quoteLine << endl;
				return false;
			}
		}

		//if c is '/'
		//	if the following char is '/', ignore the rest of the 
		//	characters up to a newline
		//	else if the following char is '*', search for the closing comment "*/"
		//	else, c is a division operator and can be ignored
		else if (c == '/') {
			
			char cmnt;
			int cmntLine = lineNum;

			if(!dictfile.get(cmnt))
				break;
			
			if (cmnt == '\n')
				lineNum++;

			else if (cmnt == '/' || cmnt == '*') {
				switch (cmnt) {
				//if there's a "//", ignore the rest of the line
				case '/':
					dictfile.ignore(1000, '\n');
					lineNum++;
					break;

				//if there's a "/*",  search for a matching "*/"
				//	if a matching */ is not found, /* is unbalanced
				case '*':

					commentStack.push('\0');

					while(dictfile.get(cmnt)) {
						if (cmnt == '\n')
							lineNum++;

						else if (cmnt == '/') {
							if (commentStack.top() == '*') {
								cout << "pair matching /* and */" << endl;

								while (!commentStack.empty())
									commentStack.pop();

								break;
							}						
						}
						else {
							commentStack.push(cmnt);
						}
					}
				}
				if (!commentStack.empty()) {
					cout << "unbalanced /* symbol on line " << cmntLine << endl;
					return false;
				}
			}
		}

		// if a "*/" is found, it's an unbalanced comment block
		else if (c == '*') {
			dictfile.get(c);
			if (c == '/') {
				cout << "unbalanced  */ symbol on line " << lineNum << endl;
				return false;
			}

		}

		//if c is an opening brace, push the stack
		else if (c == '(' || c == '{' || c == '[') {
			pushStack(c, lineNum, braceStack);
		}

		//if c is a closing bracket check to see if there's a matching pair
		else if (c == ')' || c == '}' || c == ']') {
			if (!isMatchingPair(c, lineNum, braceStack))
				return false;
		}
	}

	if (braceStack.empty()) {
		return true;
	}
	else {
		cout << "unbalanced " << braceStack.top().getChar() << " on line "
			<< braceStack.top().getcLine() << endl;
		return false;
	}
}

//takes in a closing brace, char, and a stack
//if the stack is empty => there's no matching pair. Return false.
//if the top of the stack is not the correct opening brace => no matching pair. Return false.
//if the top of the stack is matching, there is a matching pair. Pop stack and return true.
bool isMatchingPair(const char b, const int lineNum, stack<BalanceChar> &s) {
	if (s.empty()) {
		cout << "unbalanced " << b << " symbol on line " << lineNum << endl;
		return false;
	}

	switch (b) {
	case '}':
		if (s.top().getChar() != '{') {
			cout << "unbalanced { symbol on line " << s.top().getcLine() << endl;
			return false;
		}
		else {
			cout << "pair matching { and }" << endl;
			s.pop();
			return true;
		}
		break;
	case ']':
		if (s.top().getChar() != '[') {
			cout << "unbalanced ] symbol on line " << s.top().getcLine() << endl;
			return false;
		}
		else {
			cout << "pair matching [ and ]" << endl;
			s.pop();
			return true;
		}
		break;
	case ')':
		if (s.top().getChar() != '(') {
			cout << "unbalanced ( symbol on line " << s.top().getcLine() << endl;
			return false;
		}
		else {
			cout << "pair matching ( and )" << endl;
			s.pop();
			return true;
		}
		break;
	}
}

void pushStack(const char c, const int lineNum, stack<BalanceChar> &s) {
	BalanceChar b;
	b.setC(c);
	b.setLine(lineNum);
	s.push(b);
}

int main() {
	ifstream infile;
	string filename;

	cout << "Please enter filename for C++ code: ";
	cin >> filename;
	infile.open(filename.c_str());

	if (!infile) {
		cout << "File not found!" << endl;
		return (1);
	}

	printFile(infile);

	if (balanceFile(infile))
		cout << "balance ok" << endl;
	else
		cout << "balance not okay" << endl;
}