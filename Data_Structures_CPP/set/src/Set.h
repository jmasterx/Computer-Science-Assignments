/************************************************************************
* Filename: Set.h
* Author: Joshua Larouche
* Description: An O(log n) implementation of an ordered set
* Date: 01-24-2014
/************************************************************************/

#ifndef SET_H
#define SET_H
#define SET_ELEM_NOT_FOUND -1
typedef int set_type;
class Set
{
	private:
	//member variables
	set_type* m_array;
	unsigned int m_size;
	unsigned int m_capacity;

	//static private methods
	static void copy(const set_type* src, set_type* dest,
		unsigned int count, bool forward);
	//private member methods
	void initArray(int initialCapacity);
	void freeMemory();
	set_type* allocMemory(int capacity);
	void grow();
	unsigned int findInsertionIndex(const set_type& val) const;
	
public:
	//public methods
	//default constructor
	Set(void);
	//copy constructor
	Set(const Set& set);
	//assignment operator
	Set& operator=(const Set& rhs);
	//equality operators
	bool operator==(const Set& rhs) const;
	bool operator!=(const Set& rhs) const;
	//cardinality
	unsigned int card() const;
	unsigned int capacity() const;
	int findIndexOf(const set_type& val) const;
	bool isMember(const set_type& val) const;
	bool insert(const set_type& val);
	void insert(const Set& b);
	bool del(const set_type& val);
	void del(const Set& b);
	bool subset(const Set& b) const;
	Set Union(const Set& b) const;
	Set Intersection(const Set& b) const;
	void display();
	~Set(void);
};
#endif
