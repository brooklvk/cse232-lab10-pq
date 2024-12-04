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

   // default constructor
   priority_queue() 
   {
   }

    // copy constructor
   priority_queue(const priority_queue &  rhs)  
   { 
       this->container = rhs.container;
   }

    // move constructor
   priority_queue(priority_queue && rhs)  
   { 
       this->container = std::move(rhs.container);
   }

    // initializer list constructor using iterators
   template <class Iterator>
   priority_queue(Iterator first, Iterator last) 
   {
       size_t newCapacity = last - first;
       container.reserve(newCapacity); // allocate as much as needed 

       // Add each new item 
       for (auto it = first; it != last; it++)
           container.push_back(*it);
   }

    // initializer list constructor using our custom vector
   priority_queue (custom::vector<T> && rhs) 
   {
        // Move the vector into the container
        container = std::move(rhs);
        // Build the heap from the moved data
        heapify();
   }

    // initializer list constructor using our custom vector
   priority_queue (custom::vector<T>& rhs) // 
   {
       this->container = std::move(rhs);
   }

    // destructor thats here but does nothing. 
    // Not needed because the underlying vector structure takes care of it 
  ~priority_queue() {}

   //
   // Access
   //
   const T & top() const; // Get the maximum item the top item.

   //
   // Insert
   //
   void  push(const T& t); // Add a new element to the heap
   void  push(T&& t);      // also add a new element to the heap

   //
   // Remove
   //
   void  pop(); // Remove the top item from the heap

   //
   // Status
   //
   size_t size()  const 
   { 
       return container.size(); // get the size from the container using our vector
   }
   bool empty() const 
   { 
       return container.empty(); //uses vectors empty
   }
};



/************************************************
 * P QUEUE :: TOP
 * Get the maximum item from the heap: the top item.
 ***********************************************/
template <class T>
const T & priority_queue <T> :: top() const
{
    if (empty()) // Check if the queue is empty
    {
        // If we don't throw an error here, something invalid would be returned 
        std::cerr << "Throwing std::out_of_range in top()" << std::endl; // Send this specific error message for the unit test
        throw std::out_of_range("std:out_of_range"); // Throw an out of range for test
    }

    return container.front(); // Return the front (or top) element of the container
}

/**********************************************
 * P QUEUE :: POP
 * Delete the top item from the heap.
 **********************************************/
template <class T>
void priority_queue <T> :: pop()
{
    if (!empty()) // Check if the queue is empty
        std::swap(container.front(), container.back()); // if not empty then we need to swap the front and back elements to remove the top element
    container.pop_back(); // now we can pop/remove the back element
    percolateDown(1); // percolate down to fix the heap
}

/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/

// push takes a const reference and adds it to the container, then percolates it to the correct positions and fixes the heap
template <class T>
void priority_queue <T> :: push(const T & t)
{
	container.push_back(t); // add item 

    // fix the heap 
    // similar to percolateDown() 
	size_t i = container.size(); 
	while (i > 1 && container[i - 1] > container[i / 2 - 1])
	{
		std::swap(container[i - 1], container[i / 2 - 1]);
		i /= 2;
	}
}

// same as above but with rvalue reference
template <class T>
void priority_queue <T> :: push(T && t)
{
    container.push_back(std::move(t)); // add item 

    // fix the heap 
    size_t index = container.size() / 2;
    while (index && percolateDown(index)) // here we can just call percolateDown() 
        index /= 2;
}

/************************************************
 * P QUEUE :: PERCOLATE DOWN
 * The item at the passed index may be out of heap
 * order. Take care of that little detail!
 * Return TRUE if anything changed.
 ************************************************/

// percolates down the heap (the heap is a binary tree where the parent is always greater than the children) 
// we need to make sure the heap is in order so we percolate down the heap to fix it when needed
template <class T>
bool priority_queue <T> :: percolateDown(size_t indexHeap)
{
    size_t indexLeft = indexHeap * 2; // indexHeap is the current element 
    size_t indexRight = indexLeft + 1;
    size_t indexBigger; // we will find the largest element 

    if (indexLeft > size())
        return false; // no change is needed 

    // Find the largest element, should be on top of the heap 

    if (indexRight <= size())

        // if left element is smaller, set right element to indexBigger. else left is indexBigger 
        indexBigger = container[indexLeft - 1] < container[indexRight - 1] ?
        indexRight : indexLeft;

    else // if right > size() 
        indexBigger = indexLeft;

    if (container[indexHeap - 1] < container[indexBigger - 1]) // compare the current index with whichever we found was larger, above 
    {
        std::swap(container[indexHeap - 1], container[indexBigger - 1]); // swap if: current element is less than the larger element we found above. 
        // Because the largest value has to be on top of the heap 

        percolateDown(indexBigger); // recursively call method to fix subtrees 
        return true; // we did make a change by swapping 
    }

    return false; // no change was needed 
}

/************************************************
 * P QUEUE :: HEAPIFY
 * Turn the container into a heap.
 ************************************************/

// heapify converts the container (the container is a vector) into a heap (a heap is like a BST but the parent is always greater than the children)
// it does this by percolating down the heap and while it is moving through the heap adjusting the elements so that lower elements are moved down and higher elements are moved up
template <class T>
void priority_queue <T> ::heapify()
{
	for (size_t i = size() / 2; i > 0; i--)  
		percolateDown(i); // apply to all elements in the heap 
}

/************************************************
 * SWAP
 * Swap the contents of two priority queues
 ************************************************/

// swap swaps...
template <class T>
inline void swap(custom::priority_queue <T>& lhs,
                 custom::priority_queue <T>& rhs)
{
    std::swap(lhs.container, rhs.container); // swappy swap swap 
}

};

