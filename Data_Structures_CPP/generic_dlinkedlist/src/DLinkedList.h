/****************************************************************
 * File      :   DLinkedList.h
 * Version   :   1.0
 * Date      :   March, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Template Double Linked List With Recursive Selection Sort
 *****************************************************************/
#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H
#ifndef NULL
#define NULL 0
#endif
#include "Node.h"
#include <iostream>
template<class T>
class DLinkedList
{
	Node<T>* m_head;
	Node<T>* m_tail;

	void swap(Node<T>* a, Node<T>* b) {
		if ( a == b ){
			return;
		}
		
		//store the current pointers
		Node<T>* curANext = a->m_next;
		Node<T>* curAPrev = a->m_prev;
		Node<T>* curBNext = b->m_next;
		Node<T>* curBPrev = b->m_prev;

		// a b adjacent
		if ( a->m_next == b ){
			a->m_next = curBNext;
			a->m_prev = b;
			b->m_next = a;
			b->m_prev = curAPrev;

			if ( curAPrev )
				curAPrev->m_next = b;
			if ( curBNext )
				curBNext->m_prev = a;
		}
		// not adjacent
		else {
			a->m_next = curBNext;
			a->m_prev = curBPrev;
			b->m_next = curANext;
			b->m_prev = curAPrev;

			if ( curANext )
				curANext->m_prev = b;
			if ( curAPrev )
				curAPrev->m_next = b;
			if ( curBNext )
				curBNext->m_prev = a;
			if ( curBPrev )
				curBPrev->m_next = a;
		}

		//update the tail
		if(a == m_tail) {
			m_tail = b;
		}
		else if(b == m_tail) {
			m_tail = a;
		}
	}

	void selectionSort(Node<T>* head) {
		if(next(head) == NULL) {
			return;
		}
		Node<T>* minimum = min(head);

		swap(head,minimum);
		selectionSort(minimum->m_next);
	}
public:
	DLinkedList()
		: m_head(NULL),m_tail(NULL) {
			m_head = new Node<T>();
	}

	//copy constructor
	DLinkedList(const DLinkedList& rhs) {
		m_tail = NULL;
		m_head = new Node<T>();
		Node<T>* n = rhs.isFirst()->m_next;
		while(n) {
			pushBack(n->m_data);
			n = next(n);
		}
	}
	//assignment operator
	DLinkedList& operator=(const DLinkedList& rhs) {
		clear();

		Node<T>* n = rhs.isFirst()->m_next;
		while(n) {
			pushBack(n->m_data);
			n = next(n);
		}
		return *this;
	}

	bool isEmpty() const {
		return m_head->m_next == NULL;
	}
	Node<T>* pushFront(const T& t) {
		Node<T>* n = new Node<T>(t);
		if(m_head->m_next) {
			m_head->m_next->m_prev = n;
		}

		n->m_next = m_head->m_next;
		n->m_prev = m_head;
		if(m_head->m_next == NULL) {
			m_tail = n;
		}

		m_head->m_next = n;
		return n;
	}

	Node<T>* pushBack(const T& t) {
		Node<T>* n = new Node<T>(t);

		if(m_tail == NULL) {
			m_head->m_next = n;
			m_tail = n;
			m_tail->m_prev = m_head;
		}
		else {
			m_tail->m_next = n;
			n->m_prev = m_tail;
			m_tail = n;
		}
		return n;
	}

	Node<T>* insert(const T& t,Node<T>* pos){
		if(pos == m_head) {
			return pushFront(t);
		}
		else if(pos == m_tail && m_tail != NULL) {
			return pushBack(t);
		}
		else {
			Node<T>* n = new Node<T>(t);
			pos->m_next->m_prev = n;
			n->m_next = pos->m_next;
			n->m_prev = pos;
			pos->m_next = n;

			return n;
		}
	}

	void clear() {
		Node<T>* n = isFirst()->m_next;
		while(n) {
			Node<T>* temp = next(n);
			delete n;
			n = temp;
		}

		m_head->m_next = NULL;
		m_tail = NULL;
	}

	Node<T>* isFirst() const {
		return m_head;
	}

	Node<T>* isLast() const {
		return m_tail;
	}

	Node<T>* next(Node<T>* n) const {
		return n->m_next;
	}

	Node<T>* precedent(Node<T>* n) const {
		return n->m_prev;
	}

	Node<T>* min(Node<T>* h) {
		Node<T>* smallest = NULL;
		Node<T>* n = h;

		while(n) {
			if(smallest == NULL || n->m_data < smallest->m_data) {
				smallest = n;
			}
			n = next(n);
		}

		return smallest;
	}

	void display() {
		Node<T>* n = isFirst()->m_next;
		std::cout << "DLinkedList:" << std::endl;
		while(n) {
			std::cout << n->m_data << std::endl;
			n = next(n);
		}
		std::cout << std::endl;
	}

	void sort() {
		if(!isEmpty())
		selectionSort(m_head->m_next);
	}

	Node<T>* removeAfter(Node<T>* n) {
		if(n == NULL) {return n;}
		Node<T>* r = n->m_next;
		if(r == NULL) { return n;}

		r->m_prev->m_next  = r->m_next;
		if (r->m_next == NULL)
			m_tail = r->m_prev;
		else
		r->m_next->m_prev  = r->m_prev;
		
		delete r;

		if(m_tail == m_head)
			m_tail = NULL;

		return n;
	}

	~DLinkedList() {
		clear();
		delete m_head;
		m_head = NULL;
		m_tail = NULL;
	}
};
#endif