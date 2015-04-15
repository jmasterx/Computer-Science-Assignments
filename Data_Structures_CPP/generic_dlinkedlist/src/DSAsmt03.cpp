/****************************************************************
 * File      :   DSAsmt03.cpp
 * Version   :   1.0
 * Date      :   March, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Main file for DLinkedList Assignment
 *****************************************************************/
#include "DLinkedList.h"
#include <ctime>
using namespace std;
int main()
{
	//output program information
	cout << "Data Structures Assignment #3." << endl;
	cout << "Author: " << "Joshua Larouche." << endl;
	cout << endl;

	DLinkedList<int> theList; //initialize an empty list
	int size; // the size of the list to be sorted
	int n;
	cout << "Enter a size less than 500" << endl;
	cin >> size;
	if(size < 500 && size >= 0) {
		srand(time(0));
		Node<int> *temp =theList.isFirst();
		for (int i=1; i<=size; i++) {
			n = rand() % 100; // generate the elements randomly
			temp = theList.insert(n, temp);
		}
		theList.display(); // display the generated list
		theList.sort(); // Sort using recursive selection sort
		theList.sort(); // Sort using recursive selection sort
		theList.display(); // display the sorted list
	}
	else if(size > 500) {
		cout << "size exceeded 500. Stack Overflow risk." << endl;
	}
	else {
		cout << "Invalid input." << endl;
	}

}
