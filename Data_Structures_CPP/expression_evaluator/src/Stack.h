/****************************************************************
 * File      :   Stack.h
 * Version   :   1.1
 * Date      :   Winter, 2014
 * Author    :   Madjid Allili (mallili@ubishops.ca)
 * Subject   :   Template Class Stack
 *****************************************************************/

#ifndef _Stack_h
#define _Stack_h
#include <stdlib.h>
#include <assert.h>
template<class TYPE>
class Stack {
private:
	struct Node {
		TYPE value;
		Node * next;
		Node(const TYPE& t, Node * n = NULL) {
			value = t;
			next = n;
		}
	};

	Node *top;
	int m_size;

//--------------------------------------------------------------
public:
	//Constructor
	Stack() :
			top(NULL), m_size(0) {
	}

	//Destructor
	~Stack() {
		Node *nodePtr, *nextNode;

		//Position nodePtr at the top of the stack.
		nodePtr = top;

		// Traverse the list deleting each node.
		while (nodePtr != NULL) {
			nextNode = nodePtr->next;
			delete nodePtr;
			nodePtr = nextNode;
		}
	}

	//Put item on top of stack
	void push(const TYPE& e) {
		top = new Node(e, top);
		m_size++;
	}

	//Take item from top of stack
	TYPE pop() {
		Node *temp = top; // Temporary pointer

		// First make sure the stack isn't empty.
		assert(!isEmpty());
		TYPE data = top->value;
		top = top->next;
		delete temp;
		m_size--;
		return data;
	}

	//Peek at top of stack
	const TYPE& peek() const {
		assert(!isEmpty());
		return top->value;
	}

	//True if stack is empty
	bool isEmpty() const {
		return (top == NULL);
	}

	int size() const {
		return m_size;
	}
};

#endif  
