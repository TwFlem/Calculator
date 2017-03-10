//============================================================================
// Name        : Project1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<unordered_map>
#include<map>

using namespace std;									//used for small scale project

template <class T>
class Stack{											//Simple stack implemented with array
public:

	Stack(int size);									// constructor
	T Top();											// returns top of stack
	void push(T data);									// pushes data into stack
	void pop();											// pops data from stack
	bool isEmpty();										// checks if stack is empty

private:
	T* data_pointer;									// the stack of data type
	int top;											// length of stack
};

int op_precedence(char op);																				//Returns precedence of an operator

void evaluate(string input, string variable, bool let_expression, unordered_map<string, double> &m);	//Evaluates the parsed postfix expression

void infix_to_postfix(string input, unordered_map<string,double> &m);									//parses infix to postfix

bool isfunction(string token);																			// determines if token is log, sin, or cos

char function_type(string token);																		// returns special character pertaining to function

int main() {													//Main function

	string input = "";											//input
	unordered_map<string, double> m;							//hash map for variable storage (passed by reference twice)
	while(input!="quit"){										//grab input until user types quit
		if(input != "")											//ignores the empty char*
		infix_to_postfix(input, m);								//convert to postfix
		getline(cin, input);									//takes whole buffer as a string of input
	}

	return 0;													//end of program
}

template <class T>
Stack<T>::Stack(int size){										//stack constructor
	this->data_pointer = new T[size];							//array initialization of dersired data type
	this->top = -1;												//begin empty
}

template <class T>
void Stack<T>::push(T data){									//adds element to stack
	this->top++;
	this->data_pointer[top] =  data;
}

template <class T>
T Stack<T>::Top(){												//returns top of stack
	return data_pointer[top];
}

template <class T>
bool Stack<T>::isEmpty(){
	if(top == -1)												//returns true if empty
		return true;
	else
		return false;
}

template <class T>
void Stack<T>::pop(){											//decrements index of stack
	top--;
}

int op_precedence(char op){										//returns int based on operator preference

	if(op == '+' || op == '-')
		return 1;
	else if(op == '*' || op == '/')
		return 2;
	else if(op == '^')
		return 3;
	else
		return 0;
}

bool isoperator(char character){								//determines if a character is an operator
	if(character == '+' || character == '-' || character == '*'
			|| character == '/' || character == '^')
		return true;
	else
		return false;
}

bool isfunction(string token){									//determines if a token is a function
	if(token == "log" || token == "sin" || token == "cos")
		return true;
	else
		return false;
}

char function_type(string token){								//determines function type and returns character pertaining to function
	if(token == "sin")
		return '!';
	if(token == "cos")
		return '@';
	else
		return '#';
}

void evaluate(string input, string variable, bool let_expression, unordered_map<string,double> &m){	//evaluates the postfix expression

	double result = 0;																//result initialized to 0
	Stack<double> equation = Stack<double>(input.length());							//throws the equation onto stack
	string token = "";																//used to temporarily store each token
	bool zero_error = false;														//bool for throwing a divide by zero error
	unordered_map< string, double>::iterator mit;									//unordered map iterator


	for(auto it = input.begin(); it != input.end(); it++){							//loop from the begging of string until end of string

		char c = *it;																//value of iterator->char

		if(c == ' '){																//extracts token when the delimiter, ' ', is encountered

			double d;																//temporary variable for storing double literals
			const char* c_token = token.c_str();									//converts token to c style string (NULL terminator)
			auto first = token.begin();												//char* to first character in token
			char first_c = *(first);												//iterator->char value

			if(isdigit(first_c)){													//if value is digit, then token is number
				d = strtod(c_token, NULL);											//from beggining to NULL, interpret string as double FP #
				equation.push(d);													//Throw that number on the stack
				token = "";															//reset token (used many times)
				continue;
			}

			if(isalpha(first_c)){													//if first char is alphabetic, then token is variable
				if(token == "Pi"){													//pre-determined Pi
					equation.push(3.14169);											//push pre-dtermined value of pi onto stack
					token = "";
					continue;
				}
				if(token == "e"){													//pre-determined e
					equation.push(2.718);											//push pre-determined value of e onto stack
					token = "";
					continue;
				}
				else{																//non pre-declared variable

					mit = m.find(token);											//find desired token in hashmap, returns pointer to value

					if(mit == m.end()){												//not found
						cout << "Undeclared-Variable" << endl;						//desired error case
						return;
					}

					else{															//found
						equation.push(mit->second);									//push variable value on the stack
					}
					token = "";
					continue;
				}
			}
		}
		if(isoperator(c) || c == '!' || c == '@' || c == '#'){						//function/operator operations

			double op1 = equation.Top();											//operand 1
			equation.pop();															//remove operand 1

			if(c == '!' || c == '@' || c == '#'){									//if special function character (only needs top operand 1)

				if(c == '!')														// ! == sin function
					result = sin(op1);

				if(c == '@')														// @ == cos function
					result = cos(op1);

				if( c == '#')														// # == log function
					result = log(op1);
			}

			else{																	//else any other operation

				double op2 = equation.Top();										//operand 2
				equation.pop();														//pop operand 2

				if(c == '+')														//updates result as sum of operands
					result = op1 + op2;

				if(c == '-')											//updates result as subtraction of operands
					result = op2 - op1;

				if(c == '*')														//updates result as multiplication of operands
					result = op2 * op1;

				if(c == '/'){														//updates result as division of operands
					if(op1 == 0){													//if denominator == 0, throw error
						zero_error = true;
						break;
					}
					result = op2/op1;
				}

				if(c == '^')														//updates result as exponentiation of operands
					result = pow(op2,op1);

			}

			equation.push(result);													//push updated result into register
			continue;
		}
		if(isalpha(c) || isdigit(c) || c == '.'){									//extracts token until delimiter ' ' is found
			token += c;
			continue;
		}
	}

	if(!equation.isEmpty()){														//case where one number or variable is input
		result = equation.Top();
	}
	if(zero_error){																	//throws 0 error
		cout << "Division-By-Zero" << endl;
		return;
	}
	if(let_expression){																//if a let expression, update value of variable
		m[variable] = result;
		return;
	}
	cout<< result << endl;															//prints result and begins new line
}

void infix_to_postfix(string input, unordered_map<string, double> &m){				//modified shunting-yard algorithm

	int size = input.length();														//determines size of string for our stack
	Stack<char> operators = Stack<char>(size);										//initializes stack for characters
	string postfix_tokens = "";														//initializes postfix string variable
	string token = "";																//stores token
	string variable = "";															//if let expression, store desired variable name
	bool declaration_expression = false;											//value for storing let declaration

	for(auto it = input.begin(); it != input.end(); it++){							//loops through whole input

		char c = *it;																//value at location in string

		if(token == "let"){															//if let, set declaration to true
			declaration_expression = true;
			token = "";
			continue;
		}
		if(declaration_expression && c == '='){										//extract variable name as token, store value
			variable = token;
			token = "";
			continue;
		}
		if(c == '('){																//if c == '('
			if(!isfunction(token)){													//if current token is not a function type, store token
			postfix_tokens += token;
			postfix_tokens += ' ';													//delimiter for future evaluation
			token = "";
			}
			else{																	//if token is a function, push it's corresponding char
				char function = function_type(token);								//into stack before pushing in the '('
				operators.push(function);
				token = "";
			}
			operators.push(c);														//push '('
			continue;
		}
		if(c == ')'){																//if c==')'
			postfix_tokens += token;												//extract token
			postfix_tokens += ' ';
			token = "";
			while(operators.Top()!='(' && !operators.isEmpty()){					//append tokens into postfix equation until '(' found
				postfix_tokens += operators.Top();									// (if empty, unbalanced parenthesis)
				postfix_tokens += ' ';
				operators.pop();
			}
			operators.pop();																//pop the used '(' character from previous loop
			if(operators.Top() == '!' || operators.Top() == '@' || operators.Top() == '#'){	//if top is function, immediately append to postfix equation
				postfix_tokens += operators.Top();
				postfix_tokens += ' ';
				operators.pop();													//pop function character
			}
			continue;
		}
		if(isdigit(c)|| isalpha(c) || c == '.'){									//if character is alphabetic or numeric or a '.', it is a operand
			token += c;																//collect character for token extraction
			continue;

		}
		if(isoperator(c)){															//if character is operator
			postfix_tokens += token;												//extract token
			postfix_tokens += ' ';
			token = "";
			int precedence = op_precedence(c);										//determine and store operator preference
			if(operators.isEmpty()){												//if empty, simply push on to stack
				operators.push(c);
				continue;
			}
			if(precedence!=3){														//if not ^, process operators in left to right association loop
				while(!operators.isEmpty()
						&& precedence <= op_precedence(operators.Top())){
					postfix_tokens += operators.Top();
					postfix_tokens += ' ';
					operators.pop();
					}
				}
			if(precedence == 3){													//if c == '^', process exponentiation in right to left association
				while(!operators.isEmpty()
						&& precedence < op_precedence(operators.Top())){
					postfix_tokens += operators.Top();
					postfix_tokens += ' ';
					operators.pop();
					}
				}
			operators.push(c);														//push current operator on stack
		}

	}

	postfix_tokens += token;														//append last operand
	postfix_tokens += ' ';
	token = "";

	while(!operators.isEmpty()){													//pop the rest of the operators until stack is empty
		if(operators.Top() != '('){
		postfix_tokens += operators.Top();
		postfix_tokens += ' ';
		}
		operators.pop();
	}
	evaluate(postfix_tokens, variable,declaration_expression, m);					//send postfix version of input and desired to be evaluated
}	