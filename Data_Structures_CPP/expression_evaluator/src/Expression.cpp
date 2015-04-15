/****************************************************************
 * File      :   Expression.cpp
 * Version   :   1.0
 * Date      :   February, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Expression class implementation
 *****************************************************************/
#include "Expression.h"
#include <sstream>
#include <cmath>
using namespace std;
/*additional mathematical functions */

//factorial of a number
double factorial(double n){
	int fac = 1;
	for(int i = 1; i <= (int)n; ++i)
		fac *= i;
	return fac;
}

//summation formula ((n * n+1) / 2)
double sum(double n){
	return (n * (n+1)) / 2.0;
}

//Expression class implementation
Expression::Expression(string input, Notation notation)
: m_hasError(false) {
	//add the operators we need
	m_operators.push_back(Operator("+", ASSOCIATION_LEFT, 0));  //add
	m_operators.push_back(Operator("-", ASSOCIATION_LEFT, 0));  //subtract
	m_operators.push_back(Operator("*", ASSOCIATION_LEFT, 1));  //multiply
	m_operators.push_back(Operator("/", ASSOCIATION_LEFT, 1));  //divide
	m_operators.push_back(Operator("%", ASSOCIATION_LEFT, 1));  //modulo
	m_operators.push_back(Operator("N", ASSOCIATION_RIGHT, 2)); //negation
	m_operators.push_back(Operator("^", ASSOCIATION_RIGHT, 3)); //exponent

	//add in some standard functions (string, function pointer)
	m_functions.push_back(make_pair("sin",sin));
	m_functions.push_back(make_pair("cos",cos));
	m_functions.push_back(make_pair("tan",tan));
	m_functions.push_back(make_pair("asin",asin));
	m_functions.push_back(make_pair("acos",acos));
	m_functions.push_back(make_pair("atan",atan));
	m_functions.push_back(make_pair("log",log10));
	m_functions.push_back(make_pair("ln",log));
	m_functions.push_back(make_pair("floor",floor));
	m_functions.push_back(make_pair("ceil",ceil));
	m_functions.push_back(make_pair("sqrt",sqrt));
	m_functions.push_back(make_pair("abs",fabs));
	m_functions.push_back(make_pair("sum",sum));
	m_functions.push_back(make_pair("n!",factorial));

	//catch any errors that happen
	try {
		switch (notation) {
		case NOTATION_INFIX:
			m_infix = input;
			inToPost();
			inToPre();
			preToIn();
			break;
		case NOTATION_POSTFIX:
			m_postfix = input;
			postToIn();
			inToPost();
			inToPre();
			break;
		case NOTATION_PREFIX:
			m_prefix = input;
			preToIn();
			inToPost();
			inToPre();
		default:
			break;
		}
	} catch (runtime_error& error) //store the error if we encountered one
	{
		m_error = error.what();
		m_hasError = true;
	}
}

Expression::~Expression(void) {
}

const string& Expression::getPrefix() const {
	return m_prefix;
}

const string& Expression::getPostfix() const {
	return m_postfix;
}

const string& Expression::getInfix() const {
	return m_infix;
}

const string& Expression::getError() const {
	return m_error;
}

bool Expression::hasError() const {
	return m_hasError;
}

double Expression::evaluate() {
	/*evaluates an arithmetic expression that is in Reverse Polish Notation*/
	Stack <string> stack;
	std::vector <string> tokens;
	string token;

	//if there was already an error, do not try to evaluate
	if (hasError())
		throw runtime_error("Evaluation error: expression contains errors.");

	//tokenize the postfix notation to evaluate it
	tokens = tokenizePrefixAndPostfix(getPostfix());

	for (unsigned int i = 0; i < tokens.size(); i++) {
		token = tokens[i];

		if (isOperator(token)) {
			//assumes all operators take 2 arguments
			if (stack.size() < 2) {
				throw runtime_error(
						"Evaluation error: stack has too few elements.");
			}

			string b = stack.pop();
			string a = stack.pop();

			stack.push(evaluateOperation(token, a, b));
		} else if(isFunction(token))
		{
			//assumes all functions take 1 argument
			if (stack.size() < 1) {
				throw runtime_error(
					"Evaluation error: stack has too few elements.");
			}

			string a = stack.pop();
			stack.push(evaluateFunction(token, a));
		}
		else //push the value
		{
			stack.push(token);
		}
	}

	//stack size should only be 1
	if (stack.size() != 1) {
		throw runtime_error("Evaluation error: unexpected stack size.");
	}

	string result = stack.pop();
	return stringToDouble(result);
}

string Expression::inToPost() {
	/*converts infix to postfix (Reverse Polish Notation)*/
	std::vector <string> result;
	Stack <string> stack;
	std::vector <string> tokens;
	string token;
	string prevToken;
	string nextToken;

	tokens = tokenizeInfix(getInfix());

	//implements the Shunting-yard algorithm
	//en.wikipedia.org/wiki/Shunting-yard_algorithm
	for (unsigned int i = 0; i < tokens.size(); i++) {
		token = tokens[i];

		//functions get pushed right on the stack
		if(isUnaryMinus(token,prevToken)){
			token = "N"; //negation
			stack.push(token);
		}
		else if(isFunction(token)) {
			stack.push(token);
		}
		else if (isOperator(token)) {
			/*
			 While there is an operator (y) at the top of the operators stack 
			 and either (x) is left-associative and its precedence is 
			 less or equal to that of (y), or (x) is right-associative
			 and its precedence is less than (y) 
			 */
			while (!stack.isEmpty() && isOperator(stack.peek())) {
				//get the precedence of the token and the top of the stack
				int prec = compareOp(token, stack.peek());

				if ((isLeftAssociative(token) && prec <= 0)
						|| (isRightAssociative(token) && prec < 0)) {
					if (stack.isEmpty()) {
						throw runtime_error(
								"Parse error: expected a token on the stack.");
					}

					result.push_back(stack.pop());
					continue;
				}
				break; //exit early because condition no longer met
			}
				stack.push(token);

		} else if (isLeftParenthesis(token)) {
			stack.push(token);
		} else if (isRightParenthesis(token)) {
			//until we reach a left parenthesis
			while (!stack.isEmpty() && !isLeftParenthesis(stack.peek())) {
				result.push_back(stack.pop());
			}

			//pop the left parenthesis
			if (!stack.isEmpty())
				stack.pop();

			//push function to results
			if (!stack.isEmpty() && isFunction(stack.peek()))
				result.push_back(stack.pop());
		} else{  //it is a number, push it to the result array
			result.push_back(token);
		}
		prevToken = token;
	}

	//push the rest of the tokens on the stack to the result array
	while (!stack.isEmpty()) {
		result.push_back(stack.pop());
	}

	string converted; //convert to string
	for (unsigned int i = 0; i < result.size(); i++) {
		string s = result[i];
		if(s == "N") //convert negation into multiplication by -1
			s = "-1 *";
		converted += s;
		//add a space except at the end
		if (i + 1 != result.size()) {
			converted += " ";
		}
	}

	m_postfix = converted;
	return converted;
}

string Expression::inToPre() {
	/*converts Reverse Polish Notation to Polish Notation.
	 *requires that the infix was already converted to postfix.
	 *this is done to simplify the code.
	 */

	Stack <string> result;
	Stack <string> stack;
	std::vector <string> tokens = tokenizePrefixAndPostfix(getPostfix());
	string token;

	for (unsigned int i = 0; i < tokens.size(); i++) {
		token = tokens[i];

		//if it is a number, push on the stack
		if (isOperator(token)) {
			if (stack.size() < 2) {
				throw runtime_error(
					"Parse error: expected at least 2 tokens on the stack.");
			}
			//concatenate in this form: + 5 10
			string b = stack.pop();
			string a = stack.pop();
			string concat = token + " " + a + " " + b;

			//push the concatenated form back on the stack
			stack.push(concat);
		} else if(isFunction(token)) {
			if (stack.size() < 1) {
				throw runtime_error(
					"Parse error: expected at least 1 token on the stack.");
			}
			//concatenate in this form: f 5
			string a = stack.pop();
			string concat = token + " " + a;

			//push the concatenated form back on the stack
			stack.push(concat);
		}
		else //it is a number
		{
			stack.push(token);
		}
	}

	//pop the stack until it is empty and store in result stack
	while (!stack.isEmpty()) {
		result.push(stack.pop());
	}

	string converted;

	//generate the converted string
	while (!result.isEmpty()) {
		converted += result.pop() + " ";
	}

	//remove the ' ' space at the end
	if (converted.length() > 0) {
		converted = converted.substr(0, converted.length() - 1);
	}

	m_prefix = converted;

	return converted;
}

std::string Expression::postToIn() {
	/*converts Reverse Polish Notation to Infix Notation*/
	std::vector <string> result; //result array
	Stack <string> stack;
	std::vector <string> tokens; //input token array
	string token;

	tokens = tokenizePrefixAndPostfix(getPostfix());

	//go through each token in forward order
	for (unsigned int i = 0; i < tokens.size(); i++) {
		token = tokens[i];

		if (isOperator(token)) {
			//these are all binary operators that need 2 operands
			if (stack.size() < 2) {
				throw runtime_error(
					"Parse error: expected at least 2 tokens on the stack.");
			}

			//need at least 2 operands
			string a = stack.pop();
			string b = stack.pop();
			//convert to infix
			string expr = "(" + b + token + a + ")";
			//push the infix subexpression
			stack.push(expr);
		} else if(isFunction(token)) {
			//these are all 1 arg functions
			if (stack.size() < 1) {
				throw runtime_error(
					"Parse error: expected at least 1 token on the stack.");
			}

			//need at least 1 operand
			string a = stack.pop();
			//convert to infix
			string expr = token + "(" + a + ")";
			//push the infix subexpression
			stack.push(expr);
		} 
		else //push numbers on the stack
		{
			stack.push(token);
		}
	}

	//only token should be the infix expression
	if (stack.size() != 1) {
		throw runtime_error(
			"Parse error: expected only 1 token on the stack.");
	}

	string converted = stack.pop();
	m_infix = converted;
	return m_infix;
}

std::string Expression::preToIn() {
	/*converts Polish Notation to Infix Notation*/

	std::vector <string> result; //result array
	Stack <string> stack;
	std::vector <string> tokens; //input token array
	string token; //current token

	tokens = tokenizePrefixAndPostfix(getPrefix());

	//go through each token in reverse order
	for (int i = tokens.size() - 1; i >= 0; i--) {
		token = tokens[i];

		if (isOperator(token)) {
			//these are all binary operators that need 2 operands
			if (stack.size() < 2) {
				throw runtime_error(
					"Parse error: expected at least 2 tokens on the stack.");
			}

			//need at least 2 operands
			string a = stack.pop();
			string b = stack.pop();
			//convert to infix
			string expr = "(" + a + token + b + ")";
			//push the infix subexpression
			stack.push(expr);
		} else if(isFunction(token)) {
			//these are all 1 arg functions
			if (stack.size() < 1) {
				throw runtime_error(
					"Parse error: expected at least 1 token on the stack.");
			}

			//need at least 1 operand
			string a = stack.pop();
			//convert to infix
			string expr = token + "(" + a + ")";
			//push the infix subexpression
			stack.push(expr);
		}  
		else //push numbers on the stack
		{
			stack.push(token);
		}
	}

	//only token should be the infix expression
	if (stack.size() != 1) {
		throw runtime_error(
			"Parse error: expected only 1 token on the stack.");
	}

	string converted = stack.pop();
	m_infix = converted;
	return m_infix;
}

bool Expression::isOperator(const string& token) const {
	for (unsigned int i = 0; i < m_operators.size(); i++) {
		if (m_operators[i].get() == token) {
			return true;
		}
	}

	return false;
}

bool Expression::isOperator(char c) const {
	string s;
	s += c;
	return isOperator(s);
}

bool Expression::isParenthesis(const std::string& token) const {
	return isLeftParenthesis(token)|| isRightParenthesis(token);
}

bool Expression::isParenthesis(char c) const {
	string s;
	s += c;
	return isParenthesis(s);
}

vector<string> Expression::tokenizeInfix(const string& input) {
	/*generates a token array from a sequence of characters in infix notation. 
	 *will also parse for functions.
	 *will also add: implicit multiplication.
	 *will also ensure that negative numbers are handled correctly.
	 */

	vector <string> tokens; //the result array of tokens
	string token; //the current token being built up
	bool isMakingNum = false; //true if a number is being parsed

	//make sure the parenthesis match
	if (!isParenthesisMatch(input)) {
		throw runtime_error("Parse error: parenthesis mismatch.");
	}

	//go through each character of the input
	for (unsigned int i = 0; i < input.length(); i++) {
		char c = input[i];
		
		//parse numbers and the decimal
		if (isDigit(c) || isFullStop(c)) {
			//we are now working on a number
			//push any function we might have been building
			if(!isMakingNum){
				isMakingNum = true;
				pushFunction(tokens,token);
			}
			
			//if the user only put '.' then implicitly make it '0.'
			if (isFullStop(c) && token.length() == 0) {
					token += '0';
			}

			//add the character to the current token
			token += c;
		} else if (isParenthesis(c) || isOperator(c)) {
			//if we were making a number, the number is finished
			if (isMakingNum) {
				isMakingNum = false;
				pushNumber(tokens,token);
			}
			else if(isFunction(token)){ //function is finished
				pushFunction(tokens,token);
			}

			//implicit multiplication (x)(y) -> (x)*(y)
			checkForImplicitMultiplication(tokens,c);

			//push the current character
			token += c;
			tokens.push_back(token);
			token = "";
		} 
		else {
			if(isMakingNum){ //finished making a number
				isMakingNum = false;
				pushNumber(tokens,token);
			}

			if(c == ' '){ //hitting a space means this is a function
				pushFunction(tokens,token);
			}
			else { //add the character and keep building the function
				token += c;
			}
		}
	}
	//push the last token, if any
	if (token.length() > 0) {
		if(isNumber(token))
			pushNumber(tokens,token);
		else if(isFunction(token))
		pushFunction(tokens,token);
		else
			tokens.push_back(token);
	}
	return tokens;
}

bool Expression::isDigit(char c) const {
	//check ascii range for 0 to 9
	return c >= '0' && c <= '9';
}

bool Expression::isFullStop(char c) const {
	return c == '.';
}

int Expression::compareOp(const string& lhs, const string& rhs) const {
	/*compares the precedence of 2 operators*/

	//returns < 0 if lhs < rhs
	//returns 0 if lhs == rhs
	//returns > 0 if lhs > rhs
	if (isOperator(lhs) && isOperator(rhs)) {
		Operator a = getOperator(lhs);
		Operator b = getOperator(rhs);

		return a.getPrecedence() - b.getPrecedence();
	}

	throw runtime_error("Compare error: expected 2 operator tokens.");
}

Expression::Operator Expression::getOperator(const string& token) const {
	for (unsigned int i = 0; i < m_operators.size(); i++) {
		if (m_operators[i].get() == token) {
			return m_operators[i];
		}
	}

	throw runtime_error("Operator error: token is not a valid operator.");
}

bool Expression::isLeftAssociative(const string& token) const {
	if (isOperator(token)) {
		return getOperator(token).isLeftAssociative();
	}

	throw runtime_error("Associative error: token is not a valid operator.");
}

bool Expression::isRightAssociative(const string& token) const {
	if (isOperator(token)) {
		return getOperator(token).isRightAssociative();
	}
	throw runtime_error("Associative error: token is not a valid operator.");
}

bool Expression::isRightParenthesis(const string& token) const {
	return token == ")" || token == "}" || token == "]";
}

bool Expression::isRightParenthesis(char c) const {
	string t;
	t += c;
	return isRightParenthesis(t);
}

bool Expression::isLeftParenthesis(const string& token) const {
	return token == "(" || token == "{" || token == "[";
}

bool Expression::isLeftParenthesis(char c) const {
	string t;
	t += c;
	return isLeftParenthesis(t);
}

bool Expression::isNegative(const string& token) const {
	return token == "-";
}

bool Expression::isNegative(char c) const {
	string s;
	s += c;
	return isNegative(s);
}

std::string Expression::evaluateOperation(
	const string& op, const string& a, const string& b) const {
	double result = 0.0;
	double operA = stringToDouble(a);
	double operB = stringToDouble(b);

	if (operB == 0 && op == "/") {
		throw runtime_error(
			"Evaluation error: division by zero was detected.");
	}

	if (op == "+")
		result = operA + operB;
	else if (op == "-")
		result = operA - operB;
	else if (op == "*")
		result = operA * operB;
	else if (op == "/")
		result = operA / operB;
	else if (op == "%")
		result = fmod(operA, operB);
	else if (op == "^")
		result = pow(operA, operB);

	return doubleToString(result);
}

std::string Expression::doubleToString(double d) const {
	stringstream ss;
	ss << d;
	if (ss.fail()) {
		throw runtime_error("Cast error: could not convert double to string.");
	}
	return ss.str();
}

double Expression::stringToDouble(const string& str) const {
	stringstream ss;
	ss << str;
	double d;
	ss >> d;
	if (ss.fail()) {
		throw runtime_error("Cast error: could not convert string to double.");
	}
	return d;
}

vector<string> Expression::tokenizePrefixAndPostfix(const string& input) {
	stringstream ss(input);
	string token;
	vector <string> tokens;
	//split the string into tokens with the ' ' delimiter
	while (getline(ss, token, ' ')) {
		//prefix/postfix can only contain numbers, functions, and operators
		if (!isOperator(token) && !isNumber(token) && !isFunction(token)) {
			throw runtime_error("Parse error: invalid token found.");
		}
		tokens.push_back(token);
	}
	return tokens;
}

bool Expression::isUnaryMinus(const string& token, const string& prev) const {
	/* detects a unary minus by looking at surrounding tokens */
	return token == "-" && 
		(prev == "" || isOperator(prev) || isLeftParenthesis(prev));
}

bool Expression::isParenthesisMatch(const string& input) const {
	/*checks for parenthesis mismatch
	 *returns true if the parenthesis match*/

	Stack<char> stack;
	for (unsigned int i = 0; i < input.size(); i++) {
		char token = input[i];

		//push a left parenthesis
		if (isLeftParenthesis(token)) {
			stack.push(token);
		} else if (isRightParenthesis(token)) { //pop a right parenthesis
			if (stack.isEmpty()) //stack is empty, mismatch
				return false;

			stack.pop();
		}
	}
	//if stack is not empty, mismatch
	return stack.isEmpty();
}

bool Expression::isNumber(const string& input) const {
	/*checks if the input string is a number */

	//nothing is not a number
	if (input.length() == 0) {
		return false;
	}

	if(input == "-")
		return false;

	bool hasDecimal = false;
	for (unsigned int i = 0; i < input.length(); i++) {
		char c = input[i];
		//make sure we do not have more than 1 decimal
		if (isFullStop(c)) {
			if (hasDecimal)
				return false; //multiple decimals found
			else
				hasDecimal = true; //first decimal found
		}
		//make sure it is a digit or the sign of the number
		//the sign can only be the first character
		else if (!isDigit(c) && !(isNegative(c) && i == 0)) {
			return false;
		}
	}
	return true;
}

bool Expression::isFunction( const string& token ) const {
	for(unsigned int i = 0; i < m_functions.size(); i++) {
		if(token == m_functions[i].first) //first is a string
			return true;
	}
	return false;
}

std::string Expression::evaluateFunction( 
	const string& op, const string& a ) const
{
	double result = 0.0;
	double operA = stringToDouble(a);

	for(unsigned int i = 0; i < m_functions.size(); i++){
		if(m_functions[i].first == op)
		{
			result = m_functions[i].second(operA);
			break;
		}
	}

	return doubleToString(result);
}

void Expression::pushFunction( std::vector<string>& tokens, string& token )
{
	if(isFunction(token)) {
		//implicit multiplication for functions
		if(tokens.size() > 0 && isNumber(tokens.back())) {
			tokens.push_back("*");
		}
		tokens.push_back(token);
	}

	token = "";
}

void Expression::checkForImplicitMultiplication( 
	std::vector<string>& tokens, char c ){

	if (tokens.size() > 0 && (isNumber(tokens.back()) 
		|| isRightParenthesis(tokens.back()))
		&& isLeftParenthesis(c)) {
			tokens.push_back("*");
	}
}

void Expression::pushNumber( std::vector<string>& tokens, string& token ){
	//turn func2 into func(2)
	if(tokens.size() > 0 && isFunction(tokens.back())){
		tokens.push_back("(");
		tokens.push_back(token);
		tokens.push_back(")");
	}
	else if(tokens.size() > 1 && //turn func-3 into func(-3)
		tokens.back() == "-" &&
		isFunction(tokens[tokens.size() - 2])) {
			tokens.insert(tokens.begin() + tokens.size() - 1,"(");
			tokens.push_back(token);
			tokens.push_back(")");
	}
	else{ //just add the number
		tokens.push_back(token);
	}

	token = "";
}

Expression::Operator::Operator(
		const string& op, Association association,int precedence) :
		m_operator(op), m_association(association), m_precedence(precedence) {
}

bool Expression::Operator::isLeftAssociative() const {
	return m_association == ASSOCIATION_LEFT;
}

bool Expression::Operator::isRightAssociative() const {
	return m_association == ASSOCIATION_RIGHT;
}

int Expression::Operator::getPrecedence() const {
	return m_precedence;
}

const string& Expression::Operator::get() const {
	return m_operator;
}
