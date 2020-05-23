/*********************************************
 * file:	~\tnd004\lab\lab4b\dsets.cpp         *
 * remark: implementation of disjoint sets    *
 **********************************************/

 /*
 William Uddmyr - wilud321
 Joel Paulsson - joepa811
 */

#include <iostream>
#include <iomanip>
#include <cassert>

//using namespace std;

#include "dsets.h"

// -- CONSTRUCTORS

DSets::DSets(int theSize) {
    assert(theSize > 0);
    size = theSize;
    array = new int[size + 1];
    init();
}

// -- DESTRUCTOR

DSets::~DSets() {
    delete[] array;
    array = nullptr;
    size = 0;
}

// -- MEMBER FUNCTIONS

// create initial sets
void DSets::init() {
    for (int i = 1; i <= size; i++) {
        array[i] = -1;  // every disjoint set has one node -- singleton
    }
}

// join sets named r and s where r != s
// i.e. join trees with roots r and s
void DSets::join(int r, int s) {
    assert(r != s);
    assert(r >= 1 && r <= size);
    assert(s >= 1 && s <= size);
    assert(array[r] < 0);
    assert(array[s] < 0);
    // simple union
    // array[r] = s;

    // *** Weighted union (by size) ***

    // IN THE BOOK, page 377:
    // Does not work! why? 
    /*
    if (array[s] < array[r]) { //s is deeper
        array[r] = s; //Make s as new root
    }
    else {
        if (array[s] == array[r]) {
            --array[r]; //update height if same
            array[s] = r; //Make r as new root
        }
    } 
    */    

    //Hang the tree with less nodes to tree that have more
    if(array[s] < array[r]) //s is deeper
	{
        array[s] += array[r];
		array[r] = s;
		//--array[s]; // update size
        
	}
	else
	{
        array[r] += array[s]; //update size
		array[s] = r;
		//--array[r];
	}
}

// return name of current set for x
// i.e. return root of tree for x
int DSets::find(int x) {
    assert(x >= 1 && x <= size);

    //if x is root from beginning
    if (array[x] < 0) {
        return x;
    } else {
        //return find(array[x]);  // simple find
        return array[x] = find(array[x]);
    }

}

// just in case ...
void DSets::print() const {
    std::cout << "\n";

    for (int i = 1; i <= size; i++) {
        std::cout << std::setw(4) << i;
    }

    std::cout << "\n";

    for (int i = 1; i <= size; i++) {
        std::cout << std::setw(4) << array[i];
    }

    std::cout << "\n";
}
