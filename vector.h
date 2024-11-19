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
 *        vector::iterator       : An interator through Vector
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
 * Just like the std :: vector <T> class
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
   vector(size_t numElements                );
   vector(size_t numElements, const T & t   );
   vector(const std::initializer_list<T>& l );
   vector(const vector &  rhs);
   vector(      vector && rhs);
   ~vector();

   //
   // Assign
   //

   void swap(vector& rhs)
   {
       // temp variables to hold the current values so they don't get lost when swapping
       T* tempData = data;
       size_t tempCapacity = numCapacity;
       size_t tempElements = numElements;

       // swap rhs values with current values
       data = rhs.data;
       numCapacity = rhs.numCapacity;
       numElements = rhs.numElements;

       // swap our temp values with rhs values
       rhs.data = tempData;
       rhs.numCapacity = tempCapacity;
       rhs.numElements = tempElements;
   }

   vector & operator = (const vector & rhs);
   vector& operator = (vector&& rhs);

   //
   // Iterator
   //

   class iterator;
   iterator       begin() { return iterator(0, *this); }
   iterator       end() { return iterator(this->size(), *this); }

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

   void clear() // Deletes data and resets number of elements but maintains capacity 
   {
       delete[] data;
       numElements = 0;
   }
   void pop_back() // Decrements elements by 1, if at least 1 
   {
       if (numElements > 0)
           --numElements;
   }
   void shrink_to_fit(); // See below 

   //
   // Status
   //

   // size(), capacity(), and empty() are returning the member variables instead of the hardcoded values
   size_t  size()          const { return numElements;}
   size_t  capacity()      const { return numCapacity;}
   bool empty()            const { return numElements == 0;}
   
   // adjust the size of the buffer
   
   // vector-specific interfaces
   
private:
   
   T *  data;                 // user data, a dynamically-allocated array
   size_t  numCapacity;       // the capacity of the array
   size_t  numElements;       // the number of items currently used
};

/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T>
class vector <T> ::iterator
{
    friend class ::TestVector; // give unit tests access to the privates
    friend class ::TestStack;
    friend class ::TestPQueue;
    friend class ::TestHash;
public:
    // constructors, destructors, and assignment operator
    iterator() { this->p = nullptr; }
    iterator(T* p) { this->p = p; }
    iterator(const iterator& rhs) { this->p = rhs.p; }
    iterator(size_t index, vector<T>& v) { this->p = &(v.data[index]); }
    iterator& operator = (const iterator& rhs)
    {
        if (this != &rhs)
            this->p = rhs.p;
        return *this;
    }

    // equals, not equals operator
    bool operator != (const iterator& rhs) const
    {
        if (this->p == rhs.p)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    bool operator == (const iterator& rhs) const
    {
        if (this->p != rhs.p)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // dereference operator
    T& operator * ()
    {
        return *p;
    }

    // prefix increment
    iterator& operator ++ ()
    {
        ++p;
        return *this;
    }

    // postfix increment
    iterator operator ++ (int postfix)
    {
        ++p;
        return *this;
    }

    // prefix decrement
    iterator& operator -- ()
    {
        --p;
        return *this;
    }

    // postfix decrement
    iterator operator -- (int postfix)
    {
        --p;
        return *this;
    }

private:
    T* p;
};

/*****************************************
 * VECTOR :: DEFAULT constructors
 * Default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> ::vector() // Start with empty values 
{
    data = nullptr;
    numCapacity = 0;
    numElements = 0;
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> ::vector(size_t num, const T& t)
{
    if (num > 0) {
        data = new T[num]; // number of elements 

        for (size_t i = 0; i < num; ++i) {
            data[i] = t; // insert values 
        }
    }
    else {
        data = nullptr;
    }

    numCapacity = num;
    numElements = num;
}
/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 ****************************************/
template <typename T>
vector <T> ::vector(const std::initializer_list<T>& l)
{
    data = new T[l.size()]; // create vector 
    std::copy(l.begin(), l.end(), data); // copy the list 
    numCapacity = l.size();
    numElements = l.size();
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> ::vector(size_t num)
{
    if (num > 0) {
        data = new T[num]; // number of elements 

        for (size_t i = 0; i < num; ++i) {
            data[i] = T(); // insert values 
        }
    }
    else {
        data = nullptr;
    }
    numCapacity = num;
    numElements = num;
}


/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T>
vector <T> ::vector(const vector& rhs)
{
    if (rhs.numElements > 0) 
    { // make sure it's not empty 

        data = new T[rhs.numElements]; // new vector, same size 

        for (size_t i = 0; i <= rhs.numElements; i++) 
        {
            data[i] = rhs.data[i]; // insert values 
        }
    }
    else 
    {
        data = nullptr;
    }
    numCapacity = rhs.numElements;
    numElements = rhs.numElements;
}

/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T>
vector <T> ::vector(vector&& rhs)
{
    if (rhs.numElements > 0) // make sure it's not empty 
    {
        data = rhs.data; // copy the values 
    }
    else 
    {
        data = nullptr;
    }
    
    // copy these attributes 
    numCapacity = rhs.numCapacity;
    numElements = rhs.numElements;

    // reset rhs values to default (empty) 
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
vector <T> :: ~vector()
{
    // delete 
    delete[] data;

    // reset each attribute to default (empty) 
    numCapacity = 0;
    numElements = 0;
    data = nullptr;
}

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> ::resize(size_t newElements)
{
    // case: empty 
    if (newElements == 0)
    {
        numElements = 0;
        return;
    }

    int t = NULL;

    // create a temporary vector 
    T* tempDataArray = new T[newElements];

    // case: make larger (set temp data)
    if (newElements > numCapacity)
    {
        for (size_t i = 0; i < numElements; ++i)
        {
            tempDataArray[i] = data[i];
        }
        for (size_t i = numElements; i < newElements; ++i)
        {
            tempDataArray[i] = t;
        }
        numCapacity = newElements;

    }

    // case: make smaller (set temp data) 
    else if (newElements <= numCapacity)
    {
        for (size_t i = 0; i < numElements; ++i)
        {
            tempDataArray[i] = data[i];
        }

    }

    // now that we've changed the data and capacity, set numElements 
    numElements = newElements;

    // reset data to the temporary data 
    delete[] data;
    data = tempDataArray;
}

template <typename T>
void vector <T> ::resize(size_t newElements, const T& t)
{
    // create a temporary vector 
    T* tempDataArray = new T[newElements];

    // case: make larger (set temp data) 
    if (newElements > numCapacity)
    {
        for (size_t i = 0; i < numElements; ++i)
        {
            tempDataArray[i] = data[i];
        }
        for (size_t i = numElements; i < newElements; ++i)
        {
            tempDataArray[i] = t;
        }
    }

    // case: make smaller (set temp data) 
    else if (newElements <= numCapacity)
    {
        for (size_t i = 0; i < numElements; ++i)
        {
            tempDataArray[i] = data[i];
        }

    }

    // now that we've moved data, set numCapacity and numElements 
    numCapacity = newElements;
    numElements = newElements;

    // reset data to the temporary data 
    delete[] data;
    data = tempDataArray;
}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: reserve(size_t newCapacity)
{
    // case: empty 
    if (newCapacity == 0)
    {
        return;
    }

    // create temporary vector 
    T* tempDataArray = new T[newCapacity];

    // regardless of size, set temp data 
    for (size_t i = 0; i < numElements; ++i)
    {
        tempDataArray[i] = data[i];
    }

    // set capacity 
    numCapacity = newCapacity;

    // reset data to temporary data 
    delete[] data;
    data = tempDataArray;
}

/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: shrink_to_fit()
{
    // case: no extra capacity 
    if (numElements == numCapacity)
    {
        return;
    }

    // case: no elements (reset data) 
    if (numElements == 0)
    {
        delete[] data;
        data = nullptr;
        numCapacity = 0;
        return;
    }

    // case: extra capacity (shrink) 
    
    // made a new array to hold the data while we delete the old array
    T* tempData = new T[numElements];
    // copy the data from the old array to the new array
    for (size_t i = 0; i < numElements; ++i)
    {
        tempData[i] = data[i];
    }
    // delete the old/this array, this resets the arrays deallocating the memory it was using
    delete[] data;
    // set the old/this array to the new temp array reallocating this arrays pointers back to its original data in memory
    data = tempData;
    // set the capacity to the number of elements since we just resized the array
    numCapacity = numElements;
}

/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: operator [] (size_t index)
{
   return data[index];
   
}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: operator [] (size_t index) const
{
   return data[index];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: front ()
{
   
   return data[0];
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: front () const
{
   return data[0];
}

/*****************************************
 * VECTOR :: BACK
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: back()
{
    return data[numElements - 1];
}

/******************************************
 * VECTOR :: BACK
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: back() const
{
    return data[numElements - 1];
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
template <typename T>
void vector <T> :: push_back (const T & t)
{
    if (numElements >= numCapacity)
    {
        // If adding element to empty vector, capacity should be 1 
        if (data == nullptr) {
            numCapacity = 1;
        }
        else { // If adding element to vector which already has elements, double capacity 
            numCapacity = numCapacity * 2;
        }

        T* newData = new T[numCapacity]; // create new data 

        for (int i = 0; i < numElements; i++)
        {
            newData[i] = data[i]; // insert value
        }

        // reset data 
        delete[] data;
        data = newData;
    }

    // adding one element 
    data[numElements] = t;
    numElements++;
}

template <typename T>
void vector <T> ::push_back(T&& t)
{
    if (numElements >= numCapacity)
    {
        // If adding element to empty vector, capacity should be 1 
        if (data == nullptr) {
            numCapacity = 1;
        }
        else { // If adding element to vector which already has elements, double capacity 
            numCapacity = numCapacity * 2;
        }

        T* newData = new T[numCapacity]; // create new data 

        for (int i = 0; i < numElements; i++)
        {
            newData[i] = data[i]; // insert value 
        }

        // reset data 
        delete[] data;
        data = newData;
    }

    // adding one element 
    data[numElements] = t;
    numElements++;

}

/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
template <typename T>
vector <T> & vector <T> :: operator = (const vector & rhs)
{
    // Check if this object is already the same as rhs, otherwise why copy the same data
    if (this == &rhs)
        return *this;

    // Don't copy if empty 
    if (rhs.data == nullptr) {
        data = nullptr;
        return *this;
    }

    // if not delete ALL the data on the current/this object
    // delete[] is shorthand for deleting an array, found this when talking to my friend Mr.GPT
    // the other way of doing this is to loop through the array and delete or setting each element to nullptr
    delete[] data;

    // Now that we have a fresh object, we can size it to the same size as rhs, if rhs is bigger.
    numElements = rhs.numElements;

    if (rhs.numCapacity > numCapacity)
        numCapacity = rhs.numCapacity;

    data = new T[numCapacity];

    // Then assign all the rsh values to the newly resized current/this object
    for (size_t i = 0; i < numElements; ++i)
    {
        data[i] = rhs.data[i];
    }
    return *this;
}
template <typename T>
vector <T>& vector <T> :: operator = (vector&& rhs)
{
    // Only do this if there is already data (ie not nullptr) 
    if (data)
    {
        // Clean up any existing resources 
        delete[] data;

        // Steal the resources from rhs
        data = rhs.data;
        numElements = rhs.numElements;
        numCapacity = rhs.numCapacity;

        // Reset rhs to a valid empty state
        rhs.data = nullptr;
        rhs.numElements = 0;
        rhs.numCapacity = 0;
    }
        
   return *this;
}




} // namespace custom

