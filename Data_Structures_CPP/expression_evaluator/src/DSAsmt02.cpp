/****************************************************************
 * File      :   DSAsmt02.cpp
 * Version   :   1.0
 * Date      :   February, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Main file for Expression assignment
 *****************************************************************/
#include "Expression.h"
#include <iostream>
using namespace std;

/*
Ask the user what type of expression
Let the user enter an expression
Display the conversions and value, or an error
*/
void runNormal(){
	string choice;
	string expressionStr;
	Expression *expression = NULL;
	Expression::Notation notation = Expression::NOTATION_INFIX;
	bool isValidSelection = true;

	cout << "Select the notation of your expression:" << endl;
	cout << "1) Infix   (Traditional)." << endl;
	cout << "2) Postfix (Reverse Polish)." << endl;
	cout << "3) Prefix  (Polish)." << endl;
	cout << endl;
	getline(cin,choice);
	cout <<endl;
	if(choice == "1" || choice == "infix" || choice == "Infix"){
		notation = Expression::NOTATION_INFIX;
	}
	else if(choice == "2" || choice == "postfix" || choice == "Postfix"){
		notation = Expression::NOTATION_POSTFIX;
	}
	else if(choice == "3" || choice == "prefix" || choice == "Prefix"){
		notation = Expression::NOTATION_PREFIX;
	}
	else{
		isValidSelection = false;
	}

	if(isValidSelection){
		if(notation == Expression::NOTATION_INFIX){
			cout << "Enter your expression in infix notation:" << endl;
		}
		else if(notation == Expression::NOTATION_POSTFIX){
			cout << "Enter your expression in postfix notation:" << endl;
		}
		else if(notation == Expression::NOTATION_PREFIX){
			cout << "Enter your expression in prefix notation:" << endl;
		}
		
		getline(cin,expressionStr);
		cout << endl;
		expression = new Expression(expressionStr,notation);

		if(!expression->hasError()){
			string outputType = "infix";

			if(notation == Expression::NOTATION_INFIX)
			{
				cout << 
					"Would you like to convert to prefix or postfix?" << endl;
				getline(cin,outputType);

				if(outputType != "postfix")
					outputType == "prefix";
				cout << endl;
			}

			if(outputType == "infix")
			cout << "Infix: " << expression->getInfix() << endl;
			else if(outputType == "postfix")
			cout << "Postfix: " << expression->getPostfix() << endl;
			else
			cout << "Prefix: " << expression->getPrefix() << endl;
			try{
				cout << "Result: " << expression->evaluate() << endl;
			}
			catch (runtime_error& e)
			{
				cout << e.what() << endl;
			}
		}
		else{
			cout << "There was a problem ";
			cout << "with the expression you entered:" << endl;
			cout << expression->getError() << endl;
		}
	}
	else{
		cout << "The selection you entered was not valid." << endl;
	}

	delete expression;
}

/* performs and outputs the choice expression in the output style */
void runSilent(std::string choice,std::string strExpr,std::string output) {
	Expression::Notation notation = Expression::NOTATION_INFIX;
	Expression* expression = NULL;
	bool isValidSelection = true;
	
	if(choice == "infix"){
		notation = Expression::NOTATION_INFIX;
	}
	else if(choice == "postfix"){
		notation = Expression::NOTATION_POSTFIX;
	}
	else if(choice == "prefix"){
		notation = Expression::NOTATION_PREFIX;
	}
	else{
		isValidSelection = false;
	}

	if(isValidSelection){
		expression = new Expression(strExpr,notation);

		if(!expression->hasError()){
			if(output == "infix")
			cout << expression->getInfix() << endl;
			else if(output == "postfix")
			cout << expression->getPostfix() << endl;
			else if(output == "prefix")
			cout << expression->getPrefix() << endl;
			else if(output == "value"){
				try{
					cout << expression->evaluate() << endl;
				}
				catch (runtime_error& e)
				{
					cout << e.what() << endl;
				}
			}
			else
				cout << "invalid output format." << endl;
		}
		else{
			cout << expression->getError() << endl;
		}
	}
	else{
		cout << "The selection you entered was not valid." << endl;
	}

	delete expression;
}

/*will keep asking the user if they want to enter another expression */
void runNormalLoop()
{
	bool isRunning = true;
	cout << "Data Structures Assignment #2 - Joshua Larouche";
	cout << endl << endl;
	while(isRunning){
		isRunning = false;
		string runAgain;
		runNormal();
		cout << endl;
		cout << "Would you like to enter another expression? (y/n)" << endl;
		getline(cin,runAgain);

		//does the user want to enter another expression?
		if(runAgain.length() > 0 && 
			(runAgain[0] == 'y' || runAgain[0] == 'Y')) {
				isRunning = true;
				cout << endl;
		}
	}
}

void showConsoleUsage(){
	cout << "Usage: input_type output_type expression" << endl;
	cout << "input_type: infix, postfix, prefix" << endl;
	cout << "output type: infix, postfix, prefix, value" << endl;
	cout << "expression: \"2sin(2^5 + 12(3 ^ 2))\"" << endl;
}

int main(int argc, char *argv[]) {
	//run like normal
	if(argc < 2){
		runNormalLoop();
	}
	else if(argc == 4){
		runSilent(argv[1],argv[3],argv[2]);
	}
	else{
		showConsoleUsage();
	}
	return 0;
}