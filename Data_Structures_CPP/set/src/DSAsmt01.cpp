/************************************************************************
* Filename: DSAsmt01.cpp
* Author: Joshua Larouche
* Description: Main file for Data Structures Assignment 1
* Date: 01-24-2014
/************************************************************************/

#include "Set.h"
#include <iostream>
using namespace std;
int main()
{
	//output program information
	cout << "Data Structures Assignment #1." << endl;
	cout << "Author: " << "Joshua Larouche." << endl;
	cout << endl;

	Set A; // make new set A
	A.insert(10); // insert items in Set A
	A.insert(40);
	A.insert(60);
	A.insert(80);
	cout << "A = ";
	A.display();
	if (A.isMember(40)) // if 40 is a member of set A
		cout << "40 found " << endl;
	if (A.del(80)) // delete A if it's in set A
		cout << "80 found and deleted"<<endl;
	else cout << "could not delete 80 ... not there";
	cout << "A = ";
	A.display(); // Display the contents of set A
	Set B; // make a new Set B
	B.insert(20); // insert items in Set B
	B.insert(40);
	B.insert(60);
	B.insert(80);
	cout << "B = ";
	B.display();
	if (A.subset(B)) // check if A is a subset of B
		cout << "A is a subset of B"<<endl;
	else cout << "A is not a subset of B"<<endl;
	Set result = A.Intersection(B); // find the set intersection of A, B
	cout << "Intersection of A and B = ";
	result.display();
	Set result2 = A.Union(B); // find the set Union of A, B
	cout << "Union of A and B = ";
	result2.display();
	return 0;
}