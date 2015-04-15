/****************************************************************
 * File      :   Node.h
 * Version   :   1.0
 * Date      :   March, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Template Double Linked List Node
 *****************************************************************/
#ifndef NODE_H
#define NODE_H
template<class T>
class Node {
	template<class T> friend class DLinkedList;
	Node* m_prev;
	Node* m_next;
	T m_data;
public:
	Node(const T& data, Node* prev, Node* next)
	: m_prev(prev),m_next(next),m_data(data)
	{

	}

	Node()
	: m_prev(0),m_next(0)
	{
		
	}

	Node(const T& data)
	: m_prev(0),m_next(0),m_data(data)
	{
		
	}


};
#endif