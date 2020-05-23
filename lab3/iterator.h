#include "BinarySearchTree.h"

#pragma once

/* **********************************************************
 * Class to represent a bi-directional iterator for BSTs     *
 * Bi-directional iterators can be moved in both directions, *
 * increasing and decreasing order of items keys             *
 * ***********************************************************/

/*
William Uddmur - wilud321
Joel Paulsson -  joepa811
*/

template <typename Comparable>
class BinarySearchTree<Comparable>::Iterator {

public:

    //Nestled class. To get access to private functions in BST.
    friend class BinarySearchTree<Comparable>;

    Iterator() : current{ nullptr } {};

    ~Iterator() = default;
         
    //explicit Iterator(Node* n) : current{ n } {}

    Comparable& operator*() const {
        return current->element; // reference the element
    }

    Comparable* operator->() const {
        return &current->element; // address to the element
    }
    bool operator==(const Iterator& it) const {
        return current == it.current; // compaparison of nodes
    }
    bool operator!=(const Iterator& it) const {
        return current != it.current; // compaparison of nodes
    }

    Iterator& operator++() { //pre-increment
        current = find_successor(current); // go to node with higher value
        return *this;
    }

    Iterator& operator++(int) { //post-increment
        Iterator* temp = this;
        current = find_successor(current);
        return *temp;
    }


    Iterator& operator--() { //pre-decrement
        current = find_predecessor(current);
        return *this;
    }

    Iterator operator--(int) { //post-decreIterator
        Iterator* temp = this;
        current = find_predecessor(current);
        return *temp;
    }
    
private:

    Node* current;
    Iterator(Node* p ) : current{ p } { };
};