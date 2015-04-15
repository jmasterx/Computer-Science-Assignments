/************************************************************************
* Filename: Set.cpp
* Author: Joshua Larouche
* Description: An O(log n) implementation of an ordered set
* Date: 01-24-2014
/************************************************************************/

#include "Set.h"
#include <iostream>
#define INITIAL_CAPACITY 1
#define CAPACITY_GROWTH_RATE 2

Set::Set(void)
: m_capacity(0),
  m_size(0),
  m_array(0)
{
	initArray(INITIAL_CAPACITY); //give the array an initial capacity
}

Set::Set( const Set& set )
: m_capacity(0),
  m_size(0),
  m_array(0)
{
	m_size = set.m_size;
	m_capacity = set.m_capacity;
	m_array = allocMemory(m_capacity); 
	//copy over the elements from the other set
	copy(set.m_array,m_array,card(),false);
}

Set::~Set(void)
{
	freeMemory(); //mark this memory as free
}

void Set::initArray( int initialCapacity )
{
	freeMemory();
	m_capacity = initialCapacity;
	m_size = 0;
	m_array = allocMemory(initialCapacity);
}

void Set::freeMemory()
{
	
	delete[] m_array; //free the memory of our array
	m_array = 0; 	  //avoid dangling pointer by setting the array to NULL
}

set_type* Set::allocMemory( int capacity )
{
	return new set_type[capacity];
}

unsigned int Set::capacity() const
{
	return m_capacity;
}

/*
   Grows the array capacity by a factor of 2.
   Copies all old elements from old array to new one.
*/
void Set::grow()
{
	unsigned int newCapacity = capacity() * CAPACITY_GROWTH_RATE;
	set_type* newArray = allocMemory(newCapacity);
	copy(m_array,newArray,card(),false);
	freeMemory();
	m_array = newArray;
	m_capacity = newCapacity;
}

unsigned int Set::card() const
{
	return m_size;
}

void Set::copy(const set_type* src, set_type* dest,
			   unsigned int count, bool forward )
{
	const set_type* first;
	const set_type* last;
	set_type* result;

	if(forward) //copies 'count * sizeof(type)' bytes from src into dest
	{
		first = src;        //current memory location in source array
		last = src + count; //final memory location in source array
		result = dest;      //current memory location in dest array
		
		//keep going until all the memory has been copied
		while (first != last) 
		{
			*result = *first;
			++result; 
			++first;
		}
	}
	else //reverse version
	{
		last = src;                //final memory location in source array
		first = src + count - 1;   //current memory location in source array
		result = dest + count - 1; //current memory location in dest array

		//keep going until all the memory has been copied
		while (first >= last) 
		{
			*result = *first;
			--result;
			--first;
		}
	}
}

bool Set::isMember( const set_type& val ) const
{
	return findIndexOf(val) != SET_ELEM_NOT_FOUND;
}

int Set::findIndexOf( const set_type& val ) const
{
	int low = 0;
	int high = card() - 1;
	int midpoint = 0;
	int position = SET_ELEM_NOT_FOUND; //return value

	while (low <= high) //perform binary search to find the index of val
	{
		midpoint = low + (high - low)/2;
		if (val == m_array[midpoint]) //if we found it, return it
		{
			return midpoint;
		}
		else if (val < m_array[midpoint]) //search the lower bound
			high = midpoint - 1;
		else                              //search the upper bound
			low = midpoint + 1;
	}
	return position;
}

unsigned int Set::findInsertionIndex( const set_type& val ) const
{
	int low = 0;
	int high = card() - 1;
	int midpoint = 0;

	while (low <= high) //perform binary search to find the insertion index
	{
		midpoint = low + (high - low)/2;
		if (val < m_array[midpoint]) //search the lower bound
			high = midpoint - 1;
		else                         //search the upper bound
			low = midpoint + 1;
	}
	
	return low; //return the lower bound
}

bool Set::insert( const set_type& val )
{
	if(!isMember(val)) //only add it if we do not already have it
	{
		//if we have reached the capacity of the array, increase its size
		if(card() == capacity()) 
		{
			grow();
		}

		unsigned int insertIndex = findInsertionIndex(val);
		//move everything starting at insertionIndex down by 1 element
		if(insertIndex < card()) 
		{
			set_type* source = m_array + insertIndex;
			set_type* destination = source + 1;
			copy(source,destination,card() - insertIndex,false);
		}
		m_array[insertIndex] = val;
		m_size++;
		return true;
	}
	return false;
}

void Set::insert( const Set& b )
{
	for(unsigned int i = 0; i < b.card(); i++)
	{
		insert(b.m_array[i]);
	}
}

bool Set::subset( const Set& b ) const
{
	//b must contain all elements of this
	for(unsigned int i = 0; i < card(); i++)
	{
		if(!b.isMember(m_array[i]))
		{
			return false;
		}
	}
	return true; //all elements of this were found in b
}

Set& Set::operator=( const Set& rhs )
{
	if(this == &rhs)
	{
		return *this;
	}

	freeMemory();

	m_size = rhs.m_size;
	m_capacity = rhs.m_capacity;
	m_array = allocMemory(m_capacity);
	copy(rhs.m_array,m_array,card(),false);
	return *this;
}

bool Set::operator==(const Set& rhs) const
{
	if(card() == rhs.card()) //make sure the cardinality matches
	{
		for(unsigned int i = 0; i < card(); i++)
		{
			if(!rhs.isMember(m_array[i])) //an element did not match
			{
				return false;
			}
		}
		return true; //all the elements match
	}
	return false; //cardinalities of this and b did not match
}

bool Set::operator!=( const Set& rhs ) const
{
	return !(*this == rhs);
}

Set Set::Union( const Set& b ) const
{
	Set u(*this); //add all the elements of this into a new set
	u.insert(b);  //insert all the elements of b
	return u;
}

Set Set::Intersection( const Set& b ) const
{
	Set intersection;
	set_type val = 0;
	const Set& small = card() < b.card() ? *this : b; //smaller set
	const Set& big = card() < b.card() ? b : *this; //bigger set

	for(unsigned int i = 0; i < small.card(); i++)
	{
	    val = small.m_array[i];
		if(big.isMember(val)) //big set must contain small set value
		{
			intersection.insert(val);
		}
	}
	return intersection;
}

bool Set::del( const set_type& val )
{
	int index = findIndexOf(val);
	if(index != SET_ELEM_NOT_FOUND)      //if the element is in the set
	{
		if((unsigned int)index < card()) //if not removing last item
		{
			//move everything up by 1
			set_type* source = m_array + index + 1;
			set_type* destination = source - 1;
			copy(source,destination,card() - index,true);
		}
		m_size--;
		return true;
	}
	return false;
}

void Set::del( const Set& b )
{
	for(unsigned int i = 0; i < b.card(); i++)
	{
		del(b.m_array[i]);
	}
}

void Set::display()
{
	//prints in the form of {#, #, #}
	std::cout << "{";

	if(card() > 1)
	for(unsigned int i = 0; i < card() - 1; i++)
	{	
		std::cout << m_array[i] << ", ";
	}

	if(card() > 0)
		std::cout << m_array[card() - 1];
	
	std::cout << "}" << std::endl;
}
