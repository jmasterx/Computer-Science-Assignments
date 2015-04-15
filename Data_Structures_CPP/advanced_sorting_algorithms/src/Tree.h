/****************************************************************
 * File      :   Tree.h
 * Version   :   1.0
 * Date      :   April, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Templated Binary Tree Used for Sorting
 *****************************************************************/
#ifndef TREE_H
#define TREE_H
#include "BinaryNode.h"
template<class T>
class Tree {
BinaryNode<T>* m_root;
BinaryNode<T>* insert(BinaryNode<T> *&root, const T& data) {
	if (root == 0) {
		root = new BinaryNode<T>(data);
		return root;
	}
	else if (data < root->m_data) {
		return insert(root->m_left, data);
	}
	else {
		return insert(root->m_right, data);
	}
}

BinaryNode<T>* removeNode(const T& value,
						  BinaryNode<T> *parent, BinaryNode<T>* node) {
	//search on the left
	if (value < node->m_data) {
		if (node->m_left != 0)
			return removeNode(value,node,node->m_left);
		else
			return 0;
	//search on the right
	} else if (value > node->m_data) {
		if (node->m_right != 0)
			return removeNode(value,node,node->m_right);
		else
			return 0;
	} 
	else { //this node has the data
		//case of having both children
		if (node->m_left != 0 && node->m_right != 0) {
			node->m_data = minValue(node->m_right);
			return removeNode(value, node,node->m_right);
		} 
		//case of left child of parent
		else if (parent->m_left == node) {
			parent->m_left = (node->m_left != 0) ?
				node->m_left : node->m_right;
			return node;
		} 
		//case of right child of parent
		else if (parent->m_right == node) {
			parent->m_right = (node->m_left != 0) ? 
				node->m_left : node->m_right;
			return node;
		}
	}

	return 0;
}

const T& minValue(BinaryNode<T>* node) {
	//recursively returns the minimum value of the node
	if (node->m_left == 0)
		return node->m_data;
	else
		return minValue(node->m_left);
}

void binarySort(T array[], int size) {
	clear();
	for(int i = 0; i < size; i++) {
		insert(array[i]);
	}
	inorderTraverse(array,m_root,0);
}

int inorderTraverse(T array[], BinaryNode<T>* root, int index) {
	if(root == 0) {
		return index;
	}

	if(root->m_left)
	index = inorderTraverse(array,root->m_left,index);

	array[index] = root->m_data;
	index++;
	if(root->m_right)
	index = inorderTraverse(array,root->m_right,index);
	return index;
} 
public:
     Tree()
		: m_root(0)
	{}
	BinaryNode<T>* insert(const T& data) {
		return insert(m_root,data);
    };
	bool remove(const T& value) {
		if (m_root == 0) {
			return false;
		}
		else {
			//the root has the data
			if (m_root->m_data == value) {
				BinaryNode<T> temp;
				temp.m_left = m_root;
				BinaryNode<T>* removedNode = removeNode(value,&temp,m_root);
				m_root = temp.m_left;
				if (removedNode != 0) {
					delete removedNode;
					return true;
				} 
				else
					return false;
			} 
			else {	
				//a child of the root has the data
				BinaryNode<T>* removedNode = removeNode(value,0,m_root);
				if (removedNode != 0) {
					delete removedNode;
					return true;
				}
				else
					return false;
			}
		}
	}
   
	void clear() {
		//keep removing the top until we have nothing like a heap
		while(m_root != 0) {
			remove(m_root->m_data);
		}
	}

	void sort(T array[], int size) {
		binarySort(array,size);
	}

	~Tree() {
		clear();
	}
};
#endif