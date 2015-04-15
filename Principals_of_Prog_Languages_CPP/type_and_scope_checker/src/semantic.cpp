//Joshua Larouche
//jlarouche13@ubishops.ca
//Principals of Programming Languages Assignment #5
#include <stdlib.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>

/* the program must be used with commandline switches
   namely: -f and a filename of a program source code
           -s and a program in quotes ""
   The reason for doing this is so that the output can be piped to a file,
   or another command such as more if the output is too long.

   Output: 
   If there are any syntax error, these will be output
   If there are no syntax errors then semantic checks are performed
   If there are semantic errors, they will be printed
   If the program is correct, nothing will be printed at all
*/

/***** evidence of testing ********
included with the parser are a few programs including:
bubblesort   - Bubble sorts an array of 4 elements
               This program illustrates that a correct program outputs nothing.

semantictest - This program attempts to test the semantic analysis
               by having various mistakes.
			   The source code has line comments to show the errors.
			   All errors are detected as expected.

I also did a lot of other semantic testing but this would mean submitting 
a lot of small programs. I tried to put all my tests into a single program.
I could not find an instance of invalid output in my testing.
you can see it in action here: http://ideone.com/jOI6aK

Notes on the semantic analysis:
- Scope checking is implemented as a red-black tree (std::map) of lists.
  I would have preferred a HashMap but this would have meant using
  extra files which was not permitted.
- You cannot assign an array to an array even if that may be legal.
  This would mean dealing with a pointer type which was not mentioned.
- The type bool can be compared with relational operators.
- The grammar does not define how to assign to a char 
  so that has not been tested.
- char is not considered a numeric type, thus arithmetic cannot be performed
  on chars, but relational is possible.
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
	VINF_NONE, VINF_BOOL, VINF_INT, VINF_CHAR, VINF_DOUBLE,VINF_INVALID
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

	NodeList& getChildren() {
		return m_children;
	}

	void print(unsigned indent = 0) const {
		std::string symbol = printVal();
		if (isNonterminal())
			std::cout << std::string(indent, ' ') 
			<< "<" << symbol << ">" << '\n';
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

//represents a symbol with a type, scope, and number of array dimensions
class Symbol {
	int m_scope;
	vocabinfo m_type;
	int m_arrayDimensions;
public:
	Symbol() : m_scope(-1), m_type(VINF_NONE),m_arrayDimensions(0) {}
	Symbol(int scope, const vocabinfo& info,
		int arrayDimensions) : m_scope(scope),
		m_type(info), m_arrayDimensions(arrayDimensions) {}

	int getScope() const {
		return m_scope;
	}
	int getArrayDimensions() const {
		return m_arrayDimensions;
	}
	const vocabinfo& getType() const {
		return m_type;
	}
};

typedef std::list<Symbol> SymbolList;
typedef std::map<std::string,SymbolList> SymbolMap;
//an enumeration of semantic errors that are possible
enum SymbolError {
	SYMERR_NONE,
	SYMERR_UNDECLARED_IDENTIFIER,
	SYMERR_ALREADY_DECLARED_IN_SCOPE,
	SYMERR_TYPE_MISMATCH,
	SYMERR_NEGATIVE_ARRAY
};
//implements a symbol table as a map of lists where we map the identifier
//to a list of declarations. Each declaration has a type and a scope.
//TODO: use a HashMap instead of a red-black tree
class SymbolTable {
SymbolMap m_symbols;
int m_scopeNumber;
public:
	SymbolTable() : m_scopeNumber(0) {}
	Symbol* lookupSymbol(const std::string& identifier) {
		SymbolMap::iterator it;
		it = m_symbols.find(identifier);
		if(it != m_symbols.end() && !it->second.empty()) {
			return &it->second.back();
		}
		return NULL;
	}

	void beginScope() {
		m_scopeNumber++;
	}

	void endScope() {
		/* removes any declarations matching the scope
		   and removes the symbol altogether if no more declarations remain.*/
		for(SymbolMap::iterator it = m_symbols.begin();
			it != m_symbols.end();) {
			for (SymbolList::iterator lit = it->second.begin(); 
				lit != it->second.end();) {
				if (lit->getScope() == getCurrentScope()) {
					it->second.erase(lit++);
				}
				else {
					++lit;
				}
			}
			//std::list does not invalidate iterators on removal
			if (it->second.empty()) {
				m_symbols.erase(it++);
			}
			else {
				++it;
			}
		}
		m_scopeNumber--;
	}

	int getCurrentScope() const {
		return m_scopeNumber;
	}

	//adds the symbol if it is not already declared in the current scope
	SymbolError processDeclaration(const std::string& identifier,
		const vocabinfo& type,int arrayDimensions) {
			Symbol* s = lookupSymbol(identifier);
			if(s && s->getScope() == getCurrentScope()) {
				return SYMERR_ALREADY_DECLARED_IN_SCOPE;
			}
			//operator [] will add a new list if the identifier is not found
			SymbolList* list = &m_symbols[identifier];
			//add the symbol
			list->push_back(Symbol(getCurrentScope(),type,arrayDimensions));
			return SYMERR_NONE;
	}

	//checks declaration, # of arrays, and type
	SymbolError processUsage(const std::string& identifier, 
		const vocabinfo& type, int arrayDimensions) {
		Symbol* s = lookupSymbol(identifier);
		if( s == NULL)
			return SYMERR_UNDECLARED_IDENTIFIER;
		if(s->getArrayDimensions() != arrayDimensions || s->getType() != type)
			return SYMERR_TYPE_MISMATCH;
		return SYMERR_NONE;
	}
};

//represents a semantic error with a symbol error and additional information
class SemanticError {
	SymbolError m_symError;
	std::string m_identifier;
public:
	SemanticError() {}
	SemanticError(const SymbolError& error, const std::string& identifier) :
	m_symError(error),m_identifier(identifier) {}

	const SymbolError& getError() const {return m_symError;}
	const std::string& getIdentifier() const {return m_identifier;}
};

//a small helper structure to keep the code clean
struct SemanticType {
	vocabinfo type;
	int arrayDimensions;
};

//iterates every node of the parse tree and performs type and scope checking
//returns a list of errors that can be printed
class SemanticAnalyzer {
	SymbolTable m_symbolTable;
	std::vector<SemanticError> m_errors;

	//this function generates the errors (if any) for most nonterminals
	//it checks that the results meet certain conditions
	//for example, a relational and equality can only return a bool
	vocabinfo processResults(vocabinfo result, vocabinfo recursiveResult,
		bool restrictNumeric, bool restrictBool,
		const SymbolError& symbol, const std::string& error,
		vocabinfo returnType = VINF_NONE) {

	    //this helps reduce large chains of errors from a single error
		if(result == VINF_INVALID && recursiveResult == VINF_INVALID)
			return VINF_INVALID;

		bool restriction = true; //this means no restriction

		if(restrictBool) //result must be bool
			restriction = result == VINF_BOOL;
		else if(restrictNumeric) //result must be numeric
			restriction = result == VINF_INT || result == VINF_DOUBLE;

		if(recursiveResult != VINF_NONE) {
			//types must be the same and meet restrictions (if any)
			if(restriction && result == recursiveResult) {
				if(returnType == VINF_NONE)
				return result;
				else
					return returnType;
			}
			else {
				m_errors.push_back(SemanticError(
					symbol,
					error));
				return VINF_INVALID;
			}
		}

		//there was no closure, just return what we got
		return result;
	}

	vocabinfo processLoc(Node* root) {
		int numArrays = 0;
		std::string identifier;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getToken().getVocab() == VOC_ID)
					identifier = child->getToken().getString();
				else if(child->getNonterminal() == NT_LOCCL)
					numArrays = processLoccl(child);
		}

		Symbol* s = m_symbolTable.lookupSymbol(identifier);
		if(s == NULL) {
			m_errors.push_back(
				SemanticError(SYMERR_UNDECLARED_IDENTIFIER,identifier));
			return VINF_INVALID;
		}
		else if(s->getArrayDimensions() != numArrays) {
			//to simplify things:
			/* int [ 6 ] [ 6 ] a ;
			   int [ 6 ] b ;
			   b = a [ 0 ] ;
			   is not permitted (cannot assign arrays)
			*/
			m_errors.push_back(
				SemanticError(SYMERR_TYPE_MISMATCH,identifier));
			return VINF_INVALID;
		}
		return s->getType();
	}

	//returns number of array dimensions
	int processLoccl(Node* root) {
		int numArrays = 0;
		Node* nextRoot = root;
		while(nextRoot != NULL) {
			Node* cur = nextRoot;
			nextRoot = NULL;
			numArrays++;
			for(NodeList::iterator it = cur->getChildren().begin(); it !=
				cur->getChildren().end(); ++it) {
					Node* child = (*it);
					if(child->getNonterminal() == NT_LOCCL)
						nextRoot = child;
					else if(child->getNonterminal() == NT_BOOL) {
						vocabinfo boolResult = processBool(child);
						//subscripts must be int
						if(boolResult != VINF_INT) {
							m_errors.push_back(SemanticError(
								SYMERR_TYPE_MISMATCH,
								"Array subscripts must be int"));
						}
					}
			}
		}
		return numArrays;
	}

	vocabinfo processFactor(Node* root) {
		vocabinfo factorResult = VINF_NONE;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_BOOL)
					factorResult = processBool(child);
				else if(child->getNonterminal() == NT_LOC)
					factorResult = processLoc(child);
				else if(child->getToken().getVocab() == VOC_NUM)
					factorResult = VINF_INT;
				else if(child->getToken().getVocab() == VOC_REAL)
					factorResult = VINF_DOUBLE;
				else if(child->getToken().getVocab() == VOC_TRUE)
					factorResult = VINF_BOOL;
				else if(child->getToken().getVocab() == VOC_FALSE)
					factorResult = VINF_BOOL;
		}
		return factorResult;
	}

	vocabinfo processUnary(Node* root) {
		vocabinfo unaryResult = VINF_NONE;
		vocab token = VOC_EOS;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_FACTOR)
					unaryResult = processFactor(child);
				else if(child->getNonterminal() == NT_UNARY)
					unaryResult = processUnary(child);
				else if(!child->isNonterminal())
					token = child->getToken().getVocab();
		}

		if(unaryResult == VINF_INVALID)
			return VINF_INVALID;

		if(token == VOC_NOT && unaryResult != VINF_BOOL) {
			m_errors.push_back(SemanticError(
				SYMERR_TYPE_MISMATCH,
				"! expects a boolean expression"));
			return VINF_INVALID;
		}
		else if(token == VOC_MINUS && unaryResult != VINF_INT && 
			unaryResult != VINF_DOUBLE) {
			m_errors.push_back(SemanticError(
				SYMERR_TYPE_MISMATCH,
				"- expects an int or double"));
			return VINF_INVALID;
		}
		return unaryResult;		
	}

	vocabinfo processTerm(Node* root) {
		vocabinfo termResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_UNARY)
					termResult = processUnary(child);
				else if(child->getNonterminal() == NT_TERMCL)
					recursiveResult = processTermcl(child);
		}
		return processResults(termResult,recursiveResult,true,false,
			SYMERR_TYPE_MISMATCH,"term expects 2 matching ints or doubles");
	}

	vocabinfo processTermcl(Node* root) {
		vocabinfo termclResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		vocab token = VOC_EOS;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_UNARY)
					termclResult = processUnary(child);
				else if(child->getNonterminal() == NT_TERMCL)
					recursiveResult = processTermcl(child);
				else if(!child->isNonterminal())
					token = child->getToken().getVocab();
		}

		if(termclResult == VINF_INVALID && recursiveResult == VINF_INVALID)
			return VINF_INVALID;

		if(token == VOC_MUL && termclResult != VINF_INT 
			&& termclResult != VINF_DOUBLE) {
			m_errors.push_back(SemanticError(
				SYMERR_TYPE_MISMATCH,
				"* expects a numeric expression"));
			return VINF_INVALID;
		}
		else if(token == VOC_DIV && termclResult != VINF_INT 
			&& termclResult != VINF_DOUBLE) {
			m_errors.push_back(SemanticError(
				SYMERR_TYPE_MISMATCH,
				"/ expects a numeric expression"));
			return VINF_INVALID;
		}

		return processResults(termclResult,recursiveResult,false,false,
			SYMERR_TYPE_MISMATCH,"term");
	}

	vocabinfo processExpr(Node* root) {
		vocabinfo exprResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		vocab token = VOC_EOS;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_TERM)
					exprResult = processTerm(child);
				else if(child->getNonterminal() == NT_EXPRCL)
					recursiveResult = processExprcl(child);
		}

		//expression requires numeric types
		return processResults(exprResult,recursiveResult,true,false,
			SYMERR_TYPE_MISMATCH,"expr expects 2 matching ints or doubles");
	}

	vocabinfo processExprcl(Node* root) {
		vocabinfo exprclResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		vocab token = VOC_EOS;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_TERM)
					exprclResult = processTerm(child);
				else if(child->getNonterminal() == NT_EXPRCL)
					recursiveResult = processExprcl(child);
				else if(!child->isNonterminal())
					token = child->getToken().getVocab();
		}
		if(exprclResult == VINF_INVALID && recursiveResult == VINF_INVALID)
			return VINF_INVALID;

		if(token == VOC_PLUS && exprclResult != VINF_INT 
			&& exprclResult != VINF_DOUBLE) {
				m_errors.push_back(SemanticError(
					SYMERR_TYPE_MISMATCH,
					"+ expects a numeric expression"));
				return VINF_INVALID;
		}
		else if(token == VOC_MINUS && exprclResult != VINF_INT 
			&& exprclResult != VINF_DOUBLE) {
				m_errors.push_back(SemanticError(
					SYMERR_TYPE_MISMATCH,
					"- expects a numeric expression"));
				return VINF_INVALID;
		}

		return processResults(exprclResult,recursiveResult,false,false,
			SYMERR_TYPE_MISMATCH,"expression");
	}

	vocabinfo processRel(Node* root) {
		vocabinfo relResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_EXPR)
					relResult = processExpr(child);
				else if(child->getNonterminal() == NT_RELTAIL)
					recursiveResult = processReltail(child);
		}

		return processResults(relResult,recursiveResult,false,false,
			SYMERR_TYPE_MISMATCH,"relational",
			VINF_BOOL);
	}

	vocabinfo processReltail(Node* root) {
		vocabinfo exprResult = VINF_NONE;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_EXPR)
					exprResult = processExpr(child);
		}
		return exprResult;
	}

	vocabinfo processEquality(Node* root) {
		vocabinfo eqResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_REL)
					eqResult = processRel(child);
				else if(child->getNonterminal() == NT_EQUALCL)
					recursiveResult = processEqualcl(child);
		}

		return processResults(eqResult,recursiveResult,false,false,
			SYMERR_TYPE_MISMATCH,"equality",
			VINF_BOOL);
	}

	vocabinfo processEqualcl(Node* root) {
		vocabinfo eqclResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_REL)
					eqclResult = processRel(child);
				else if(child->getNonterminal() == NT_EQUALCL)
					recursiveResult = processEqualcl(child);
		}

		return processResults(eqclResult,recursiveResult,false,false,
			SYMERR_TYPE_MISMATCH,"equality");
	}

	vocabinfo processJoin(Node* root) {
		vocabinfo joinResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_EQUALITY)
					joinResult = processEquality(child);
				else if(child->getNonterminal() == NT_JOINCL)
					recursiveResult = processJoincl(child);
		}

		return processResults(joinResult,recursiveResult,false,true,
			SYMERR_TYPE_MISMATCH,"&&");
	}

	vocabinfo processJoincl(Node* root) {
		vocabinfo joinclResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_EQUALITY)
					joinclResult = processEquality(child);
				else if(child->getNonterminal() == NT_JOINCL)
					recursiveResult = processJoincl(child);
		}

		//must be boolean
		return processResults(joinclResult,recursiveResult,false,true,
			SYMERR_TYPE_MISMATCH,"&&");
	}

	vocabinfo processBoolcl(Node* root) {
		vocabinfo boolclResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_JOIN)
					boolclResult = processJoin(child);
				else if(child->getNonterminal() == NT_BOOLCL)
					recursiveResult = processBoolcl(child);
		}

		//must be boolean
		return processResults(boolclResult,recursiveResult,false,true,
			SYMERR_TYPE_MISMATCH,"||");
	}

	vocabinfo processBool(Node* root) {
		vocabinfo boolResult = VINF_NONE;
		vocabinfo recursiveResult = VINF_NONE;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_JOIN)
					boolResult = processJoin(child);
				else if(child->getNonterminal() == NT_BOOLCL)
					recursiveResult = processBoolcl(child);
		}

		return processResults(boolResult,recursiveResult,false,true,
			SYMERR_TYPE_MISMATCH,"||");
	}

	void processStmt(Node* root) {
		//process an assignment
		if(root->getChildren().front()->getNonterminal() == NT_LOC)
		{
			vocabinfo loctype = VINF_NONE;
			vocabinfo assignResult = VINF_NONE;
			for(NodeList::iterator it = root->getChildren().begin(); it !=
				root->getChildren().end(); ++it) {
					Node* child = (*it);
			       if(child->getNonterminal() == NT_BOOL)
						assignResult = processBool(child);
					else if(child->getNonterminal() == NT_LOC)
					loctype = processLoc(child);
			}

			if(loctype != VINF_INVALID && assignResult != VINF_INVALID
				&& loctype != assignResult)
			{
				m_errors.push_back(SemanticError(
					SYMERR_TYPE_MISMATCH,
					"assignment"));
			}
		}
		else //process everything else 
		{
			for(NodeList::iterator it = root->getChildren().begin(); it !=
				root->getChildren().end(); ++it) {
					Node* child = (*it);
					if(child->getNonterminal() == NT_BOOL) {
						vocabinfo result = processBool(child);
						if(result != VINF_BOOL && result != VINF_INVALID) {
							m_errors.push_back(SemanticError(
							SYMERR_TYPE_MISMATCH,
							"conditional expression expects boolean result"));
						}
					}
					else if(child->getNonterminal() == NT_STMT)
						processStmt(child);
					else if(child->getNonterminal() == NT_BLOCK)
						processBlock(child);
			}
		}
	}

	void processStmts(Node* root) {
		Node* nextRoot = root;
		while(nextRoot != NULL) {
			Node* cur = nextRoot;
			nextRoot = NULL;
			for(NodeList::iterator it = cur->getChildren().begin(); it !=
				cur->getChildren().end(); ++it) {
					Node* child = (*it);
					if(child->getNonterminal() == NT_STMTS)
						nextRoot = child;
					else if(child->getNonterminal() == NT_STMT)
						processStmt(child);
			}
		}
	}

	//returns the number of arrays
	//also checks for array indices < 0
	int processTypecl(Node* root) {
		int numArrays = 0;
		Node* nextRoot = root;
		while(nextRoot != NULL) {
			Node* cur = nextRoot;
			nextRoot = NULL;
			numArrays++;
			for(NodeList::iterator it = cur->getChildren().begin(); it !=
				cur->getChildren().end(); ++it) {
					Node* child = (*it);
					if(child->getNonterminal() == NT_TYPECL)
						nextRoot = child;
					else if(child->getToken().getVocab() == VOC_NUM) {
						if(child->getToken().getString()[0] == '-') {
							m_errors.push_back(SemanticError(
								SYMERR_NEGATIVE_ARRAY,
								child->getToken().getString()));
						}
					}
			}
		}
		return numArrays;
	}

	SemanticType processType(Node* root) {
		SemanticType t;
		t.type = VINF_NONE;
		t.arrayDimensions = 0;

		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_TYPECL)
					t.arrayDimensions = processTypecl(child);
				else if(child->getToken().getVocab() == VOC_BASIC)
					t.type = child->getToken().getVocabInfo();
		}
		return t;
	}

	void processDecl(Node* root) {
		SemanticType t;
		std::string identifier;
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_TYPE)
					t = processType(child);
				else if(child->getToken().getVocab() == VOC_ID)
					identifier = child->getToken().getString();
		}
		SymbolError e = m_symbolTable.processDeclaration(identifier,
			t.type,t.arrayDimensions);
		if(e != SYMERR_NONE)
		{
			m_errors.push_back(SemanticError(e,identifier));
		}
	}

	void processDecls(Node* root) {
		Node* nextRoot = root;
		while(nextRoot != NULL) {
			Node* cur = nextRoot;
			nextRoot = NULL;
			for(NodeList::iterator it = cur->getChildren().begin(); it !=
				cur->getChildren().end(); ++it) {
					Node* child = (*it);
					if(child->getNonterminal() == NT_DECLS)
						nextRoot = child;
					else if(child->getNonterminal() == NT_DECL)
						processDecl(child);
			}
		}
	}

	void processBlock(Node* root) {
		m_symbolTable.beginScope();
		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_DECLS)
					processDecls(child);
				else if(child->getNonterminal() == NT_STMTS)
					processStmts(child);
		}
		m_symbolTable.endScope();
	}

public:
	SemanticAnalyzer() {}
	const std::vector<SemanticError>& analyze(Node* root) {
		m_errors.clear();

		for(NodeList::iterator it = root->getChildren().begin(); it !=
			root->getChildren().end(); ++it) {
				Node* child = (*it);
				if(child->getNonterminal() == NT_BLOCK)
					processBlock(child);

		}
		return m_errors;
	}

	std::string printSymbolError(const SymbolError& e) {
		switch(e) {
			case SYMERR_ALREADY_DECLARED_IN_SCOPE:
				return "Identifier already declared in scope:";
			case SYMERR_NEGATIVE_ARRAY:
				return "Negative array:";
			case SYMERR_TYPE_MISMATCH:
				return "Type mismatch:";
			case SYMERR_UNDECLARED_IDENTIFIER:
				return "Undeclared identifier:";
				default:
				return "No error:";
		}
	}
	void printErrors() {
		for(std::vector<SemanticError>::iterator it = m_errors.begin(); it !=
			m_errors.end(); ++it) {
				std::string error;
				std::cout << printSymbolError(it->getError()) << " "
					<< it->getIdentifier() << std::endl;
		}
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

		bool ignoreMode = false; //added support for //line comments
		for (std::string::const_iterator it = input.begin(); it != input.end();
				++it) {
			char c = *it;
			if(tempToken == "//") {
				ignoreMode = true;
				tempToken = "";
			}
			if(ignoreMode)
			{
				if(c == '\n')
					ignoreMode = false;
				continue;
			}
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
		SemanticAnalyzer s;
		Node* tree = p.parse(prog);
		if (tree) {
				s.analyze(tree);
			    s.printErrors();
		}
		return 0;
	}

	std::cout << "Usage:" << std::endl;
	std::cout << "-s \"program statements\"" << std::endl;
	std::cout << "-f <path to file containing a program>" << std::endl;
	std::cout << std::endl;
	std::cout << "Joshua Larouche" << std::endl;
	std::cout << "Principals of Programming Languages Assignment #5"
			<< std::endl;
}