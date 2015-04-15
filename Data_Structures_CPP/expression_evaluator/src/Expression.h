/****************************************************************
 * File      :   Expression.h
 * Version   :   1.0
 * Date      :   February, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Expression class header
 *****************************************************************/
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "Stack.h"
#include <string>
#include <vector>
using namespace std;
class Expression {
public:
	enum Notation {
		NOTATION_INFIX, NOTATION_PREFIX, NOTATION_POSTFIX
	};

	enum Association {
		ASSOCIATION_LEFT, ASSOCIATION_RIGHT
	};
private:
	//defines an arithmetic operator
	class Operator {
	private:
		string m_operator;
		Association m_association;
		int m_precedence;
	public:
		Operator(const string& op, Association association, int precedence);
		bool isLeftAssociative() const;
		bool isRightAssociative() const;
		int getPrecedence() const;
		const string& get() const;
	};

	//private variables
	vector<Operator> m_operators;
	vector<pair<string, double(*)(double)> > m_functions;
	string m_infix;
	string m_postfix;
	string m_prefix;
	string m_error;
	bool m_hasError;

	//private methods
	string inToPost();
	string inToPre();
	string postToIn();
	string preToIn();
	bool isParenthesisMatch(const string& input) const;
	vector<string> tokenizeInfix(const string& input);
	vector<string> tokenizePrefixAndPostfix(const string& input);
	bool isNumber(const string& input) const;
	bool isOperator(const string& token) const;
	bool isOperator(char c) const;
	bool isParenthesis(const std::string& token) const;
	bool isParenthesis(char c) const;
	bool isDigit(char c) const;
	bool isFullStop(char c) const;
	bool isLeftAssociative(const string& token) const;
	bool isRightAssociative(const string& token) const;
	bool isLeftParenthesis(const string& token) const;
	bool isRightParenthesis(const string& token) const;
	bool isLeftParenthesis(char c) const;
	bool isRightParenthesis(char c) const;
	bool isNegative(const string& token) const;
	bool isNegative(char c) const;
	int compareOp(const string& lhs, const string& rhs) const;
	Operator getOperator(const string& token) const;
	string evaluateOperation(const string& op, const string& a,
		const string& b) const;
	string evaluateFunction(const string& op, const string& a) const;
	string doubleToString(double d) const;
	double stringToDouble(const string& str) const;
	bool isUnaryMinus(const string& token, const string& prev) const;
	bool isFunction(const string& token) const;
	void pushFunction(std::vector<string>& tokens, string& token);
	void pushNumber(std::vector<string>& tokens, string& token);
	void checkForImplicitMultiplication(std::vector<string>& tokens, char c);

public:
	Expression(string input, Notation notation);
	~Expression();
	const string& getPrefix() const;
	const string& getPostfix() const;
	const string& getInfix() const;
	const string& getError() const;
	bool hasError() const;
	double evaluate();
};

#endif
