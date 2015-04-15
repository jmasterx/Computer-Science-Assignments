//Joshua Larouche
//jlarouche13@ubishops.ca
//Principals of Programming Languages Assignment #4
#include <stdlib.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>

/* the parser must be used with commandline switches
   namely: -f and a filename of a program source code
           -s and a program in quotes ""
   The reason for doing this is so that the output can be piped to a file,
   or another command such as more if the output is too long.
*/

/***** evidence of testing ********
included with the parser are a few programs including:
fib        - computes the Fibonacci sequence for a given N
factorial  - computes the factorial of a given N
bubblesort - bubble sorts an array of 4 elements
syntaxtest - a program which aims to be as syntactically complex as possible
broken1    - declares an identifier after some statements which is illegal
broken2    - has a ! operator that is not preceded by a logical operator
broken3    - has an if statement immediately followed by an else instead of
a statement which is illegal. As the parser suggests:
                              expected nonterminal stmt got else
This link shows the parser in action for Fibonacci http://ideone.com/aVc2OJ

All the test programs I created seem to output as expected.
I could not find an instance of invalid output in my testing.
*/

/***** Fixed grammar *******
 <program>  ::= <block>
 <block>    ::= { <decls> <stmts> }
 <decls>    ::= e 
 |   <decl> <decls>
 <decl>     ::= <type> ID ;
 <type>     ::= BASIC <typecl>
 <typecl>   ::= e | [ NUM ] <typecl>
 <stmts>    ::= e | <stmt> <stmts>
 <stmt>     ::= <loc> = <bool> ;
 |   IF ( <bool> ) <stmt>
 |   IF ( <bool> ) <stmt> ELSE <stmt>
 |   WHILE ( <bool> ) <stmt>
 |   <block>
 <loc>      ::= ID <loccl>
 <loccl>    ::= e | [ <bool> ] <loccl>
 <bool>     ::= <join> <boolcl>
 <boolcl>   ::= e | || <join> <boolcl>
 <join>     ::= <equality> <joincl>
 <joincl>   ::= e 
 |   && <equality> <joincl>
 <equality> ::= <rel> <equalcl>
 <equalcl>  ::= e 
 |   == <rel> <equalcl> 
 |   != <rel> <equalcl>
 <rel>      ::= <expr> <reltail>
 <reltail>  ::= e 
 |   <= <expr>
 |   >= <expr>
 |   > <expr>
 |   < <expr>
 <expr>     ::= <term> <exprcl>
 <exprcl>   ::= e
 |   + <term> <exprcl>
 |   - <term> <exprcl>
 <term>     ::= <unary> <termcl>
 <termcl>   ::= e
 |   * <unary> <termcl>
 |   / <unary> <termcl>
 <unary>    ::= ! <unary>
 |   - <unary>
 |   <factor>
 <factor>   ::= ( <bool> )
 |   <loc>
 |   NUM
 |   REAL
 |   TRUE
 |   FALSE
 */

//utility functions for detecting numbers from strings
namespace util {
//http://stackoverflow.com/questions/5932391/
//determining-if-a-string-is-a-double
bool is_double(const std::string& s) {
	std::istringstream iss(s);
	double d;
	char c;
	return iss >> d && !(iss >> c);
}
//http://stackoverflow.com/questions/2844817/
//how-do-i-check-if-a-c-string-is-an-int
bool is_integer(const std::string& s) {
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
		return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
	}
}

class Node;
typedef std::list<Node*> NodeList;

//enumeration of all the possible nonterminals
enum nonterminal {
	NT_NONE,
	NT_PROGRAM,
	NT_BLOCK,
	NT_DECLS,
	NT_DECL,
	NT_TYPE,
	NT_TYPECL,
	NT_STMTS,
	NT_STMT,
	NT_LOC,
	NT_LOCCL,
	NT_BOOL,
	NT_BOOLCL,
	NT_JOIN,
	NT_JOINCL,
	NT_EQUALITY,
	NT_EQUALCL,
	NT_REL,
	NT_RELTAIL,
	NT_EXPR,
	NT_EXPRCL,
	NT_TERM,
	NT_TERMCL,
	NT_UNARY,
	NT_FACTOR
};

//enumeration of all the possible tokens
enum vocab {
	VOC_EOS,
	VOC_EPSILON,
	VOC_OPEN_PAR,
	VOC_CLS_PAR,
	VOC_OPEN_BRACE,
	VOC_CLS_BRACE,
	VOC_OPEN_BRACKET,
	VOC_CLS_BRACKET,
	VOC_IF,
	VOC_ELSE,
	VOC_WHILE,
	VOC_ID,
	VOC_AND,
	VOC_OR,
	VOC_ASSIGN,
	VOC_EQUALITY,
	VOC_INEQUALITY,
	VOC_LESSEQ,
	VOC_GREATEREQ,
	VOC_LESS,
	VOC_GREATER,
	VOC_PLUS,
	VOC_MINUS,
	VOC_NUM,
	VOC_REAL,
	VOC_TRUE,
	VOC_FALSE,
	VOC_NOT,
	VOC_MUL,
	VOC_DIV,
	VOC_BASIC,
	VOC_SEMICOLON
};

//enumeration of metadata for certain tokens
enum vocabinfo {
	VINF_NONE, VINF_BOOL, VINF_INT, VINF_CHAR, VINF_DOUBLE
};

//stores a vocabulary enumeration, the string token, and metadata on the token
class Token {
	std::string m_strTok;
	vocab m_vocab;
	vocabinfo m_vocabInfo;
public:
	Token(const std::string& strTok, const vocab& voc, const vocabinfo& info) :
			m_strTok(strTok), m_vocab(voc), m_vocabInfo(info) {
	}
	Token(const std::string& strTok, const vocab& voc) :
			m_strTok(strTok), m_vocab(voc), m_vocabInfo(VINF_NONE) {
	}
	Token() :
			m_vocab(VOC_EOS), m_vocabInfo(VINF_NONE) {
	}

	const std::string& getString() const {
		return m_strTok;
	}

	const vocab& getVocab() const {
		return m_vocab;
	}

	const vocabinfo& getVocabInfo() const {
		return m_vocabInfo;
	}
};

//provides string equivalents for nonterminal enumerations
std::string printNonterminal(const nonterminal& nt) {
	switch (nt) {
	case NT_NONE:
		return "none";
	case NT_PROGRAM:
		return "program";
	case NT_BLOCK:
		return "block";
	case NT_DECLS:
		return "decls";
	case NT_DECL:
		return "decl";
	case NT_TYPE:
		return "type";
	case NT_TYPECL:
		return "typecl";
	case NT_STMTS:
		return "stmts";
	case NT_STMT:
		return "stmt";
	case NT_LOC:
		return "loc";
	case NT_LOCCL:
		return "loccl";
	case NT_BOOL:
		return "bool";
	case NT_BOOLCL:
		return "boolcl";
	case NT_JOIN:
		return "join";
	case NT_JOINCL:
		return "joincl";
	case NT_EQUALITY:
		return "equality";
	case NT_EQUALCL:
		return "equalitycl";
	case NT_REL:
		return "rel";
	case NT_RELTAIL:
		return "reltail";
	case NT_EXPR:
		return "expr";
	case NT_EXPRCL:
		return "exprcl";
	case NT_TERM:
		return "term";
	case NT_TERMCL:
		return "termcl";
	case NT_UNARY:
		return "unary";
	case NT_FACTOR:
		return "factor";
	default:
		return "???";
	}
}

//represents the parse tree as a Node with N children
//a node is either a nonterminal or an actual token
class Node {
	NodeList m_children;
	Token m_token;
	nonterminal m_nonterminal;
public:
	Node* addChild(Node* child) {
		m_children.push_back(child);
		return child;
	}

	Node(nonterminal nt) :
			m_nonterminal(nt) {
	}
	Node(const Token& token) :
			m_token(token), m_nonterminal(NT_NONE) {
	}
	bool isNonterminal() const {
		return m_nonterminal != NT_NONE;
	}
	bool isToken() const {
		return !isNonterminal();
	}
	const nonterminal& getNonterminal() const {
		return m_nonterminal;
	}
	const Token& getToken() const {
		return m_token;
	}

	std::string printVal() const {
		if (!isNonterminal()) {
			return m_token.getString();
		} else {
			return printNonterminal(m_nonterminal);
		}
	}

	void print(unsigned indent = 0) const {
		std::string symbol = printVal();
		if (isNonterminal())
			std::cout << std::string(indent, ' ') << "<" << symbol << ">" << '\n';
		else
			std::cout << std::string(indent, ' ') << symbol << '\n';

		for (NodeList::const_iterator it = m_children.begin();
				it != m_children.end(); ++it) {
			Node* child = (*it);
			child->print(indent + 2);
		}

		if (isNonterminal())
			std::cout << std::string(indent, ' ') << 
			"</" << symbol << ">" << '\n';
	}
};

class Tokenizer {
	std::vector<std::string> m_tokens;
	int m_arrayPos;
	std::map<std::string, vocab> m_vocabMap;
	std::map<std::string, vocabinfo> m_vocabInfoMap;

	//these characters will separate tokens
	static bool isTokenSeparator(char c) {
		return c == '\n' || c == ' ' || c == '\r' || c == '\t';
	}

	//we map the expected string to an enumeration to avoid a long switch case
	void fillVocabInfoMap() {
		m_vocabInfoMap["int"] = VINF_INT;
		m_vocabInfoMap["char"] = VINF_CHAR;
		m_vocabInfoMap["bool"] = VINF_BOOL;
		m_vocabInfoMap["double"] = VINF_DOUBLE;
	}

	//we map the expected string to an enumeration to avoid a long switch case
	void fillVocabMap() {
		m_vocabMap["("] = VOC_OPEN_PAR;
		m_vocabMap[")"] = VOC_CLS_PAR;
		m_vocabMap["{"] = VOC_OPEN_BRACE;
		m_vocabMap["}"] = VOC_CLS_BRACE;
		m_vocabMap["["] = VOC_OPEN_BRACKET;
		m_vocabMap["]"] = VOC_CLS_BRACKET;
		m_vocabMap["if"] = VOC_IF;
		m_vocabMap["else"] = VOC_ELSE;
		m_vocabMap["while"] = VOC_WHILE;
		m_vocabMap["&&"] = VOC_AND;
		m_vocabMap["||"] = VOC_OR;
		m_vocabMap["="] = VOC_ASSIGN;
		m_vocabMap["=="] = VOC_EQUALITY;
		m_vocabMap["!="] = VOC_INEQUALITY;
		m_vocabMap["<="] = VOC_LESSEQ;
		m_vocabMap[">="] = VOC_GREATEREQ;
		m_vocabMap["<"] = VOC_LESS;
		m_vocabMap[">"] = VOC_GREATER;
		m_vocabMap["+"] = VOC_PLUS;
		m_vocabMap["-"] = VOC_MINUS;
		m_vocabMap["true"] = VOC_TRUE;
		m_vocabMap["false"] = VOC_FALSE;
		m_vocabMap["!"] = VOC_NOT;
		m_vocabMap["*"] = VOC_MUL;
		m_vocabMap["/"] = VOC_DIV;
		m_vocabMap[";"] = VOC_SEMICOLON;
		m_vocabMap["int"] = VOC_BASIC;
		m_vocabMap["char"] = VOC_BASIC;
		m_vocabMap["bool"] = VOC_BASIC;
		m_vocabMap["double"] = VOC_BASIC;
	}

public:
	Tokenizer() :
			m_arrayPos(-1) {
		fillVocabMap();
		fillVocabInfoMap();
	}

	//this will split the input based on the separator chars defined above
	void tokenize(const std::string& input) {
		m_tokens.clear();
		m_arrayPos = -1;
		std::string tempToken;
		char prev = '\0';

		for (std::string::const_iterator it = input.begin(); it != input.end();
				++it) {
			char c = *it;
			if (tempToken.length() > 0 && !isTokenSeparator(c)
					&& isTokenSeparator(prev)) {
				m_tokens.push_back(tempToken);
				tempToken = "";
				tempToken += c;
			} else if (!isTokenSeparator(c)) {
				tempToken += c;
			}
			prev = c;
		}

		if (tempToken.length() > 0) {
			m_tokens.push_back(tempToken);
		}
	}

	Token getToken() {
		//return end of string if we are done
		if (m_arrayPos + 1 >= (int) m_tokens.size())
			return Token();
		//move forward one token
		m_arrayPos++;
		//create the next token
		std::string t = m_tokens[m_arrayPos];

		//try to match the string to our grammar
		if (m_vocabMap.find(t) != m_vocabMap.end()) {
			vocab v = m_vocabMap[t];
			vocabinfo info = VINF_NONE;
			//try to match a type (int,char,bool,double)
			if (m_vocabInfoMap.find(t) != m_vocabInfoMap.end()) {
				info = m_vocabInfoMap[t];
			}
			return Token(t, v, info);
		}

		//if we get here then it is a number or an identifier
		if (util::is_integer(t)) {
			return Token(t, VOC_NUM);
		} else if (util::is_double(t)) {
			return Token(t, VOC_REAL);
		}

		return Token(t, VOC_ID);
	}
};

//takes an input string, tokenizes it and then generates a parse tree
class Parser {
	Tokenizer m_tokenizer;
	Token m_t;
	Node* m_root;
	std::vector<Node*> m_allocNodes;

	//return the next token
	void getToken() {
		m_t = m_tokenizer.getToken();
	}

	//free the heap memory for all the Nodes in the tree
	void clear() {
		for (std::vector<Node*>::iterator it = m_allocNodes.begin();
				it != m_allocNodes.end(); ++it) {
			delete (*it);
		}
		m_root = NULL;
		m_allocNodes.clear();
	}

	//allocates a Node based on a nonterminal
	Node* allocNode(const nonterminal& nt) {
		Node* n = new Node(nt);
		m_allocNodes.push_back(n);
		return n;
	}

	//allocates a Node based on a Token
	Node* allocNode(const Token& t) {
		Node* n = new Node(t);
		m_allocNodes.push_back(n);
		return n;
	}
/* what follows are functions that determine if the current
   token is the beginning of a given nonterminal */
	bool isBlock() const {
		return m_t.getVocab() == VOC_OPEN_BRACE;
	}

	bool isProgram() const {
		return isBlock();
	}

	bool isType() const {
		return m_t.getVocab() == VOC_BASIC;
	}

	bool isDecl() const {
		return isType();
	}

	bool isDecls() const {
		return isDecl();
	}

	bool isTypecl() const {
		return m_t.getVocab() == VOC_OPEN_BRACKET;
	}

	bool isLoc() const {
		return m_t.getVocab() == VOC_ID;
	}

	bool isLoccl() const {
		return m_t.getVocab() == VOC_OPEN_BRACKET;
	}

	bool isStmt() const {
		return isLoc() || m_t.getVocab() == VOC_IF
				|| m_t.getVocab() == VOC_WHILE || isBlock();
	}

	bool isStmts() const {
		return isStmt();
	}

	bool isFactor() const {
		return m_t.getVocab() == VOC_OPEN_PAR || isLoc()
				|| m_t.getVocab() == VOC_NUM || m_t.getVocab() == VOC_REAL
				|| m_t.getVocab() == VOC_TRUE || m_t.getVocab() == VOC_FALSE;
	}

	bool isUnary() const {
		return m_t.getVocab() == VOC_NOT || m_t.getVocab() == VOC_MINUS
				|| isFactor();
	}

	bool isTerm() const {
		return isUnary();
	}

	bool isTermcl() {
		return m_t.getVocab() == VOC_MUL || m_t.getVocab() == VOC_DIV;
	}
	bool isExpr() const {
		return isTerm();
	}

	bool isExprcl() const {
		return m_t.getVocab() == VOC_PLUS || m_t.getVocab() == VOC_MINUS;
	}

	bool isRel() const {
		return isExpr();
	}

	bool isRelTail() const {
		return m_t.getVocab() == VOC_LESSEQ 
			    || m_t.getVocab() == VOC_LESS
				|| m_t.getVocab() == VOC_GREATEREQ
				|| m_t.getVocab() == VOC_GREATER;
	}

	bool isEquality() const {
		return isRel();
	}

	bool isEqualcl() const {
		return m_t.getVocab() == VOC_EQUALITY
				|| m_t.getVocab() == VOC_INEQUALITY;
	}

	bool isJoin() const {
		return isEquality();
	}

	bool isJoincl() const {
		return m_t.getVocab() == VOC_AND;
	}

	bool isBool() const {
		return isJoin();
	}

	bool isBoolcl() const {
		return m_t.getVocab() == VOC_OR;
	}

	//this will throw an exception because we hit an unexpected token
	void error(const std::string& e) {
		std::cout << "error: " << e << std::endl;
		clear();
		throw std::exception();
	}

	//this will cause an exception to be thrown. It asserts that
	//the current token is the vocab v
	void mustBe(const vocab& v) {
		if (v != m_t.getVocab()) {
			error("unexpected token: " + m_t.getString());
		}
	}

	//determines if the current token is the start of the parameter nonterminal
	bool isNonterminal(const nonterminal& nt) {
		switch (nt) {
		case NT_NONE:
			return false;
		case NT_PROGRAM:
			return isProgram();
		case NT_BLOCK:
			return isBlock();
		case NT_DECLS:
			return isDecls();
		case NT_DECL:
			return isDecl();
		case NT_TYPE:
			return isType();
		case NT_TYPECL:
			return isTypecl();
		case NT_STMTS:
			return isStmts();
		case NT_STMT:
			return isStmt();
		case NT_LOC:
			return isLoc();
		case NT_LOCCL:
			return isLoccl();
		case NT_BOOL:
			return isBool();
		case NT_BOOLCL:
			return isBoolcl();
		case NT_JOIN:
			return isJoin();
		case NT_JOINCL:
			return isJoincl();
		case NT_EQUALITY:
			return isEquality();
		case NT_EQUALCL:
			return isEqualcl();
		case NT_REL:
			return isRel();
		case NT_RELTAIL:
			return isRelTail();
		case NT_EXPR:
			return isExpr();
		case NT_EXPRCL:
			return isExprcl();
		case NT_TERM:
			return isTerm();
		case NT_TERMCL:
			return isTermcl();
		case NT_UNARY:
			return isUnary();
		case NT_FACTOR:
			return isFactor();
		default:
			return false;
		}
	}

	//this will cause an exception to be thrown. It asserts that
	//the current token is the nonterminal t
	void mustBe(const nonterminal& t) {
		if (!isNonterminal(t)) {
			error("expected nonterminal: " + printNonterminal(t) + " got: "
							+ m_t.getString());
		}
	}

	/* What follows is the logic for each nonterminal in the grammar.
	   A nonterminal that has an epsilon is optional and will not
	   cause the parser to halt if it is not found. */

	Node* unary() {
		Node* current = allocNode(NT_UNARY);
		mustBe(NT_UNARY);
		if (isNonterminal(NT_FACTOR)) {
			current->addChild(factor());
		} else {
			current->addChild(allocNode(m_t));
			getToken();
			mustBe(NT_UNARY);
			current->addChild(unary());
		}
		return current;
	}

	Node* termcl() {
		Node* current = allocNode(NT_TERMCL);
		mustBe(NT_TERMCL);
		current->addChild(allocNode(m_t));
		getToken();
		mustBe(NT_UNARY);
		current->addChild(unary());
		if (isNonterminal(NT_TERMCL)) {
			current->addChild(termcl());
		}
		return current;
	}

	Node* term() {
		Node* current = allocNode(NT_TERM);
		mustBe(NT_TERM);
		current->addChild(unary());
		if (isNonterminal(NT_TERMCL)) {
			current->addChild(termcl());
		}
		return current;
	}

	Node* exprcl() {
		Node* current = allocNode(NT_EXPRCL);
		mustBe(NT_EXPRCL);
		current->addChild(allocNode(m_t));
		getToken();
		current->addChild(term());
		if (isNonterminal(NT_EXPRCL)) {
			current->addChild(exprcl());
		}
		return current;
	}

	Node* expr() {
		Node* current = allocNode(NT_EXPR);
		mustBe(NT_EXPR);
		current->addChild(term());
		if (isNonterminal(NT_EXPRCL)) {
			current->addChild(exprcl());
		}
		return current;
	}

	Node* reltail() {
		Node* current = allocNode(NT_RELTAIL);
		mustBe(NT_RELTAIL);
		current->addChild(allocNode(m_t));
		getToken();
		mustBe(NT_EXPR);
		current->addChild(expr());
		return current;
	}

	Node* rel() {
		Node* current = allocNode(NT_REL);
		mustBe(NT_REL);
		current->addChild(expr());
		if (isNonterminal(NT_RELTAIL)) {
			current->addChild(reltail());
		}
		return current;
	}

	Node* equalcl() {
		Node* current = allocNode(NT_EQUALCL);
		mustBe(NT_EQUALCL);
		current->addChild(allocNode(m_t));
		getToken();
		mustBe(NT_REL);
		current->addChild(rel());
		if (isNonterminal(NT_EQUALCL)) {
			current->addChild(equalcl());
		}
		return current;
	}

	Node* equality() {
		Node* current = allocNode(NT_EQUALITY);
		mustBe(NT_EQUALITY);
		current->addChild(rel());
		if (isNonterminal(NT_EQUALCL)) {
			current->addChild(equalcl());
		}
		return current;
	}
	Node* joincl() {
		Node* current = allocNode(NT_JOINCL);
		mustBe(NT_JOINCL);
		current->addChild(allocNode(m_t));
		getToken();
		mustBe(NT_EQUALITY);
		current->addChild(equality());
		if (isNonterminal(NT_JOINCL)) {
			joincl();
		}
		return current;
	}
	Node* join() {
		Node* current = allocNode(NT_JOIN);
		mustBe(NT_JOIN);
		current->addChild(equality());
		if (isNonterminal(NT_JOINCL)) {
			current->addChild(joincl());
		}
		return current;
	}

	Node* boolclf() {
		Node* current = allocNode(NT_BOOLCL);
		mustBe(NT_BOOLCL);
		current->addChild(allocNode(m_t));
		getToken();
		mustBe(NT_JOIN);
		current->addChild(join());
		if (isNonterminal(NT_BOOLCL)) {
			current->addChild(boolclf());
		}
		return current;
	}

	Node* boolf() {
		Node* current = allocNode(NT_BOOL);
		mustBe(NT_BOOL);
		current->addChild(join());
		if (isNonterminal(NT_BOOLCL)) {
			current->addChild(boolclf());
		}
		return current;
	}

	Node* loccl() {
		Node* current = allocNode(NT_LOCCL);
		mustBe(NT_LOCCL);
		current->addChild(allocNode(m_t));
		getToken();
		mustBe(NT_BOOL);
		current->addChild(boolf());
		mustBe(VOC_CLS_BRACKET);
		current->addChild(allocNode(m_t));
		getToken();
		if (isNonterminal(NT_LOCCL)) {
			current->addChild(loccl());
		}
		return current;
	}

	Node* loc() {
		Node* current = allocNode(NT_LOC);
		mustBe(NT_LOC);
		current->addChild(allocNode(m_t));
		getToken();
		if (isNonterminal(NT_LOCCL)) {
			current->addChild(loccl());
		}
		return current;
	}

	Node* factor() {
		Node* current = allocNode(NT_FACTOR);
		mustBe(NT_FACTOR);
		if (isNonterminal(NT_LOC)) {
			current->addChild(loc());
		} else if (m_t.getVocab() == VOC_OPEN_PAR) {
			current->addChild(allocNode(m_t));
			getToken();
			mustBe(NT_BOOL);
			current->addChild(boolf());
			mustBe(VOC_CLS_PAR);
			current->addChild(allocNode(m_t));
			getToken();
		} else {
			current->addChild(allocNode(m_t));
			getToken();
		}
		return current;
	}

	Node* stmt() {
		Node* current = allocNode(NT_STMT);
		mustBe(NT_STMT);
		if (isNonterminal(NT_LOC)) {
			current->addChild(loc());
			mustBe(VOC_ASSIGN);
			current->addChild(allocNode(m_t));
			getToken();
			mustBe(NT_BOOL);
			current->addChild(boolf());
			mustBe(VOC_SEMICOLON);
			current->addChild(allocNode(m_t));
			getToken();
		} else if (m_t.getVocab() == VOC_IF) {
			current->addChild(allocNode(m_t));
			getToken();
			mustBe(VOC_OPEN_PAR);
			current->addChild(allocNode(m_t));
			getToken();
			mustBe(NT_BOOL);
			current->addChild(boolf());
			mustBe(VOC_CLS_PAR);
			current->addChild(allocNode(m_t));
			getToken();
			mustBe(NT_STMT);
			current->addChild(stmt());
			if (m_t.getVocab() == VOC_ELSE) {
				current->addChild(allocNode(m_t));
				getToken();
				mustBe(NT_STMT);
				current->addChild(stmt());
			}
		} else if (m_t.getVocab() == VOC_WHILE) {
			current->addChild(allocNode(m_t));
			getToken();
			mustBe(VOC_OPEN_PAR);
			current->addChild(allocNode(m_t));
			getToken();
			mustBe(NT_BOOL);
			current->addChild(boolf());
			mustBe(VOC_CLS_PAR);
			current->addChild(allocNode(m_t));
			getToken();
			mustBe(NT_STMT);
			current->addChild(stmt());
		} else if (isNonterminal(NT_BLOCK)) {
			current->addChild(block());
		}
		return current;
	}

	Node* stmts() {
		Node* current = allocNode(NT_STMTS);
		mustBe(NT_STMTS);
		current->addChild(stmt());
		if (isNonterminal(NT_STMTS)) {
			current->addChild(stmts());
		}
		return current;
	}

	Node* typeclf() {
		Node* current = allocNode(NT_TYPECL);
		mustBe(NT_TYPECL);
		current->addChild(allocNode(m_t));
		getToken();
		mustBe(VOC_NUM);
		current->addChild(allocNode(m_t));
		getToken();
		mustBe(VOC_CLS_BRACKET);
		current->addChild(allocNode(m_t));
		getToken();
		if (isNonterminal(NT_TYPECL)) {
			current->addChild(typeclf());
		}
		return current;
	}

	Node* typef() {
		Node* current = allocNode(NT_TYPE);
		mustBe(NT_TYPE);
		current->addChild(allocNode(m_t));
		getToken();
		if (isNonterminal(NT_TYPECL)) {
			current->addChild(typeclf());
		}
		return current;
	}

	Node* decl() {
		Node* current = allocNode(NT_DECL);
		current->addChild(typef());
		mustBe(VOC_ID);
		current->addChild(allocNode(m_t));
		getToken();
		mustBe(VOC_SEMICOLON);
		current->addChild(allocNode(m_t));
		getToken();
		return current;
	}
	Node* decls() {
		Node* current = allocNode(NT_DECLS);
		mustBe(NT_DECLS);
		current->addChild(decl());
		if (isNonterminal(NT_DECLS)) {
			current->addChild(decls());
		}
		return current;
	}

	Node* block() {
		Node* current = allocNode(NT_BLOCK);
		mustBe(NT_BLOCK);
		current->addChild(allocNode(m_t));
		getToken();
		if (isNonterminal(NT_DECLS)) {
			current->addChild(decls());
		}
		if (isNonterminal(NT_STMTS)) {
			current->addChild(stmts());
		}
		mustBe(VOC_CLS_BRACE);
		current->addChild(allocNode(m_t));
		getToken();
		return current;
	}

	Node* program() {
		Node* current = allocNode(NT_PROGRAM);
		m_root = current;
		getToken();
		mustBe(NT_BLOCK);
		current->addChild(block());
		return current;
	}
public:
	Node* parse(const std::string& code) {
		clear();
		m_tokenizer.tokenize(code);
		Node* ret = NULL;
		try {
			ret = program();
		} catch (...) {
		}
		return ret;
	}

	void print() {
		if (m_root) {
			m_root->print();
		}
	}

	~Parser() {
		clear();
	}
};

int main(int argc, char *argv[]) {
	std::string prog;
	std::string choice;
	if (argc == 3)
		choice = argv[1];

	if (choice == "-f" || choice == "-s") {
		if (choice == "-s") {
			prog = argv[2];

			//erase ""
			if (prog[0] == '"') {
				prog.erase(0, 1);
				prog.erase(prog.size() - 1);
			}
		} else {
			std::string path = argv[2];

			//erase ""
			if (path[0] == '"') {
				path.erase(0, 1);
				path.erase(path.size() - 1);
			}

			std::ifstream t(path.c_str());
			std::string app((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());
			prog = app;
		}

		Parser p;
		Node* tree = p.parse(prog);
		if (tree)
			p.print();
		if (tree)
			return 0;
		else
			return 1;
	}

	std::cout << "Usage:" << std::endl;
	std::cout << "-s \"program statements\"" << std::endl;
	std::cout << "-f <path to file containing a program>" << std::endl;
	std::cout << std::endl;
	std::cout << "Joshua Larouche" << std::endl;
	std::cout << "Principals of Programming Languages Assignment #4"
			<< std::endl;
}