/****************************************************************
 * File      :   BinaryNode.h
 * Version   :   1.0
 * Date      :   April, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Templated Node for Binary Tree
 *****************************************************************/

#ifndef BINARY_NODE_H
#define BINARY_NODE_H
template<class T>
class BinaryNode {
	template<class T> friend class Tree;
	BinaryNode* m_left;
	BinaryNode* m_right;
	T m_data;
public:
	BinaryNode(const T& data, BinaryNode* prev, BinaryNode* next)
		: m_left(prev),m_right(next),m_data(data)
	{

	}

	BinaryNode()
		: m_left(0),m_right(0)
	{

	}

	BinaryNode(const T& data)
		: m_left(0),m_right(0),m_data(data)
	{

	}
};
#endif