/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *     Joshua Sooaemalelagi & Brooklyn Sowards
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h"

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

/*************************************************
 * P QUEUE
 * Create a priority queue.
 *************************************************/
template<class T>
class priority_queue
{
   friend class ::TestPQueue; // give the unit test class access to the privates
   template <class TT>
   friend void swap(priority_queue<TT>& lhs, priority_queue<TT>& rhs);

private:
    void heapify();                            // convert the container in to a heap
    bool percolateDown(size_t indexHeap);      // fix heap from index down. This is a heap index!

	custom::vector<T> container;               //using our custom vector from previous assignment

public:

   //
   // construct
   //
   priority_queue() 
   {
   }
   priority_queue(const priority_queue &  rhs)  
   { 
       this->container = rhs.container;
   }
   priority_queue(priority_queue && rhs)  
   { 
       this->container = std::move(rhs.container);
   }
   template <class Iterator>
   priority_queue(Iterator first, Iterator last) 
   {
       size_t newCapacity = last - first;
       container.reserve(newCapacity);
       for (auto it = first; it != last; it++)
           container.push_back(*it);
   }
   explicit priority_queue (custom::vector<T> && rhs) 
   {
       this->container = rhs;
   }
   explicit priority_queue (custom::vector<T>& rhs)
   {
       this->container = std::move(rhs);
   }
  ~priority_queue() {}

   //
   // Access
   //
   const T & top() const;

   //
   // Insert
   //
   void  push(const T& t);
   void  push(T&& t);     

   //
   // Remove
   //
   void  pop(); 

   //
   // Status
   //
   size_t size()  const 
   { 
       return container.size();
   }
   bool empty() const 
   { 
       return container.empty();
   }
};

/************************************************
 * P QUEUE :: TOP
 * Get the maximum item from the heap: the top item.
 ***********************************************/
template <class T>
const T & priority_queue <T> :: top() const
{
    if (empty())
        return false;
        //throw "std:out_of_range";
    return container.front();
}

/**********************************************
 * P QUEUE :: POP
 * Delete the top item from the heap.
 **********************************************/
template <class T>
void priority_queue <T> :: pop()
{
    if (!empty())
        std::swap(container.front(), container.back());
    container.pop_back();
    percolateDown(1);
}

/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/
template <class T>
void priority_queue <T> :: push(const T & t)
{
	container.push_back(t);
	size_t i = container.size();
	while (i > 1 && container[i - 1] > container[i / 2 - 1])
	{
		std::swap(container[i - 1], container[i / 2 - 1]);
		i /= 2;
	}
}
template <class T>
void priority_queue <T> :: push(T && t)
{
    container.push_back(std::move(t));
    size_t index = container.size() / 2;
    while (index && percolateDown(index))
        index /= 2;
}

/************************************************
 * P QUEUE :: PERCOLATE DOWN
 * The item at the passed index may be out of heap
 * order. Take care of that little detail!
 * Return TRUE if anything changed.
 ************************************************/
template <class T>
bool priority_queue <T> :: percolateDown(size_t indexHeap)
{
    size_t indexLeft = indexHeap * 2;
    size_t indexRight = indexLeft + 1;
    size_t indexBigger;

    if (indexLeft > size())
        return false;

    if (indexRight <= size())
        indexBigger = container[indexLeft - 1] < container[indexRight - 1] ?
        indexRight : indexLeft;

    else
        indexBigger = indexLeft;

    if (container[indexHeap - 1] < container[indexBigger - 1])
    {
        std::swap(container[indexHeap - 1], container[indexBigger - 1]);
        percolateDown(indexBigger);
        return true;
    }

    return false;
}

/************************************************
 * P QUEUE :: HEAPIFY
 * Turn the container into a heap.
 ************************************************/
template <class T>
void priority_queue <T> ::heapify()
{
	for (size_t i = size() / 2; i > 0; i--)
		percolateDown(i);
}

/************************************************
 * SWAP
 * Swap the contents of two priority queues
 ************************************************/
template <class T>
inline void swap(custom::priority_queue <T>& lhs,
                 custom::priority_queue <T>& rhs)
{
    std::swap(lhs.container, rhs.container);
}

};

