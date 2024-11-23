/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    Joshua Sooaemalelagi & Brooklyn Sowards
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

/*****************************************
 * VECTOR
 * Just like the std::vector<T> class
 ****************************************/
template <typename T>
class vector
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   
   // 
   // Construct
   //

   vector();
   vector(size_t numElements);
   vector(size_t numElements, const T & t);
   vector(const std::initializer_list<T>& l);
   vector(const vector &  rhs);
   vector(vector && rhs);
   ~vector();

   //
   // Assign
   //

   void swap(vector& rhs);
   vector & operator = (const vector & rhs);
   vector& operator = (vector&& rhs);

   //
   // Iterator
   //

   class iterator;
   iterator begin();
   iterator end();

   //
   // Access
   //

         T& operator [] (size_t index);
   const T& operator [] (size_t index) const;
         T& front();
   const T& front() const;
         T& back();
   const T& back() const;

   //
   // Insert
   //

   void push_back(const T& t);
   void push_back(T&& t);
   void reserve(size_t newCapacity);
   void resize(size_t newElements);
   void resize(size_t newElements, const T& t);

   //
   // Remove
   //

   void clear();
   void pop_back();
   void shrink_to_fit();

   //
   // Status
   //

   size_t size() const { return numElements; }
   size_t capacity() const { return numCapacity; }
   bool empty() const { return numElements == 0; }
   
private:
   T * data;                 // user data, a dynamically-allocated array
   size_t numCapacity;       // the capacity of the array
   size_t numElements;       // the number of items currently used

   void allocate(size_t newCapacity);
};

/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector. You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work. Not all iterators are that way.
 *************************************************/
template <typename T>
class vector<T>::iterator
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   // constructors, destructors, and assignment operator
   iterator() : p(nullptr) {}
   iterator(T* p) : p(p) {}
   iterator(const iterator& rhs) : p(rhs.p) {}
   iterator(size_t index, vector<T>& v) : p(&(v.data[index])) {}
   iterator& operator = (const iterator& rhs)
   {
      if (this != &rhs)
         p = rhs.p;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator& rhs) const { return p != rhs.p; }
   bool operator == (const iterator& rhs) const { return p == rhs.p; }

   // increment and decrement
   iterator& operator++()
   {
      ++p;
      return *this;
   }
   iterator operator++(int)
   {
      iterator temp = *this;
      ++p;
      return temp;
   }
   iterator& operator--()
   {
      --p;
      return *this;
   }
   iterator operator--(int)
   {
      iterator temp = *this;
      --p;
      return temp;
   }

   // dereference
   T& operator*() { return *p; }
   const T& operator*() const { return *p; }

private:
   T* p;
};

/*****************************************
 * VECTOR :: DEFAULT CONSTRUCTOR
 * Default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector<T>::vector() : data(nullptr), numCapacity(0), numElements(0) {}

/*****************************************
 * VECTOR :: NON-DEFAULT CONSTRUCTOR
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector<T>::vector(size_t numElements) : data(nullptr), numCapacity(numElements), numElements(numElements)
{
   if (numElements > 0)
   {
      allocate(numCapacity);
      for (size_t i = 0; i < numElements; ++i)
         data[i] = T();
   }
}

template <typename T>
vector<T>::vector(size_t numElements, const T & t) : data(nullptr), numCapacity(numElements), numElements(numElements)
{
   allocate(numCapacity);
   for (size_t i = 0; i < numElements; ++i)
      data[i] = t;
}

/*****************************************
 * VECTOR :: INITIALIZATION LIST CONSTRUCTOR
 * Create a vector with an initialization list.
 ****************************************/
template <typename T>
vector<T>::vector(const std::initializer_list<T>& l) : data(nullptr), numCapacity(l.size()), numElements(l.size())
{
   allocate(numCapacity);
   size_t i = 0;
   for (const T& item : l)
      data[i++] = item;
}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T>
vector<T>::vector(const vector& rhs) : data(nullptr), numCapacity(rhs.numElements), numElements(rhs.numElements)
{
   if (numElements > 0)
   {
      allocate(numCapacity);
      for (size_t i = 0; i < numElements; ++i)
         data[i] = rhs.data[i];
   }
}

/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T>
vector<T>::vector(vector&& rhs) : data(rhs.data), numCapacity(rhs.numCapacity), numElements(rhs.numElements)
{
   rhs.data = nullptr;
   rhs.numCapacity = 0;
   rhs.numElements = 0;
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
template <typename T>
vector<T>::~vector()
{
   clear();
}

/*****************************************
 * VECTOR :: ASSIGNMENT OPERATOR
 * Copy assignment operator
 ****************************************/
template <typename T>
vector<T>& vector<T>::operator=(const vector& rhs)
{
   if (this != &rhs)
   {
      if (rhs.numElements <= numCapacity)
      {
         // Enough capacity, just copy elements
         numElements = rhs.numElements;
         for (size_t i = 0; i < numElements; ++i)
            data[i] = rhs.data[i];
      }
      else
      {
         // Not enough capacity, reallocate
         delete[] data;
         numCapacity = rhs.numElements;
         numElements = rhs.numElements;
         allocate(numCapacity);
         for (size_t i = 0; i < numElements; ++i)
            data[i] = rhs.data[i];
      }
   }
   return *this;
}

/*****************************************
 * VECTOR :: MOVE ASSIGNMENT OPERATOR
 * Move assignment operator
 ****************************************/
template <typename T>
vector<T>& vector<T>::operator=(vector&& rhs)
{
   if (this != &rhs)
   {
      clear();
      data = rhs.data;
      numCapacity = rhs.numCapacity;
      numElements = rhs.numElements;
      rhs.data = nullptr;
      rhs.numCapacity = 0;
      rhs.numElements = 0;
   }
   return *this;
}

/*****************************************
 * VECTOR :: SWAP
 * Swap the contents of two vectors
 ****************************************/
template <typename T>
void vector<T>::swap(vector& rhs)
{
   std::swap(data, rhs.data);
   std::swap(numCapacity, rhs.numCapacity);
   std::swap(numElements, rhs.numElements);
}

/*****************************************
 * VECTOR :: PUSH BACK
 * Add a new element to the end of the vector
 ****************************************/
template <typename T>
void vector<T>::push_back(const T& t)
{
   if (numElements == numCapacity)
      reserve(numCapacity == 0 ? 1 : numCapacity * 2);
   data[numElements++] = t;
}

template <typename T>
void vector<T>::push_back(T&& t)
{
   if (numElements == numCapacity)
      reserve(numCapacity == 0 ? 1 : numCapacity * 2);
   data[numElements++] = std::move(t);
}

/*****************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity. It will also copy all
 * the data from the old buffer into the new
 ****************************************/
template <typename T>
void vector<T>::reserve(size_t newCapacity)
{
   if (newCapacity > numCapacity)
   {
      T* newData = new T[newCapacity];
      for (size_t i = 0; i < numElements; ++i)
         newData[i] = std::move(data[i]);
      delete[] data;
      data = newData;
      numCapacity = newCapacity;
   }
}

/*****************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 ****************************************/
template <typename T>
void vector<T>::resize(size_t newElements)
{
   if (newElements > numCapacity)
      reserve(newElements);
   for (size_t i = numElements; i < newElements; ++i)
      data[i] = T();
   numElements = newElements;
}

template <typename T>
void vector<T>::resize(size_t newElements, const T& t)
{
   if (newElements > numCapacity)
      reserve(newElements);
   for (size_t i = numElements; i < newElements; ++i)
      data[i] = t;
   numElements = newElements;
}

/*****************************************
 * VECTOR :: CLEAR
 * Deletes data and resets number of elements but maintains capacity
 ****************************************/
template <typename T>
void vector<T>::clear()
{
   // Call destructor on each element
   for (size_t i = 0; i < numElements; ++i)
      data[i].~T();
   numElements = 0;
   // data remains allocated
   // numCapacity remains unchanged
}

/*****************************************
 * VECTOR :: POP BACK
 * Decrements elements by 1, if at least 1
 ****************************************/
template <typename T>
void vector<T>::pop_back()
{
   if (numElements > 0)
      --numElements;
}

/*****************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 ****************************************/
template <typename T>
void vector<T>::shrink_to_fit()
{
   if (numCapacity > numElements)
   {
      if (numElements == 0)
      {
         delete[] data;
         data = nullptr;
         numCapacity = 0;
      }
      else
      {
         T* newData = new T[numElements];
         for (size_t i = 0; i < numElements; ++i)
            newData[i] = std::move(data[i]);
         delete[] data;
         data = newData;
         numCapacity = numElements;
      }
   }
}

/*****************************************
 * VECTOR :: ALLOCATE
 * Allocate memory for the vector
 ****************************************/
template <typename T>
void vector<T>::allocate(size_t newCapacity)
{
   data = new T[newCapacity];
}

/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T>
T& vector<T>::operator[](size_t index)
{
   assert(index >= 0 && index < numElements);
   return data[index];
}

template <typename T>
const T& vector<T>::operator[](size_t index) const
{
   assert(index >= 0 && index < numElements);
   return data[index];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T& vector<T>::front()
{
   assert(numElements > 0);
   return data[0];
}

template <typename T>
const T& vector<T>::front() const
{
   assert(numElements > 0);
   return data[0];
}

/*****************************************
 * VECTOR :: BACK
 * Read-Write access
 ****************************************/
template <typename T>
T& vector<T>::back()
{
   assert(numElements > 0);
   return data[numElements - 1];
}

template <typename T>
const T& vector<T>::back() const
{
   assert(numElements > 0);
   return data[numElements - 1];
}

/*****************************************
 * VECTOR :: BEGIN
 * Return an iterator to the beginning
 ****************************************/
template <typename T>
typename vector<T>::iterator vector<T>::begin()
{
   return iterator(data);
}

/*****************************************
 * VECTOR :: END
 * Return an iterator to the end
 ****************************************/
template <typename T>
typename vector<T>::iterator vector<T>::end()
{
   return iterator(data + numElements);
}

} // namespace custom