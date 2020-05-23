#include <iostream>
#include <iomanip>  //setw
#include <utility>  //std::move

#pragma once

#include "dsexceptions.h"

/*
William Uddmur - wilud321
Joel Paulsson -  joepa811
*/

using namespace std;

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinarySearchTree {
private:
    struct Node;  // nested class defined in node.h

   
public:
    class Iterator;
    BinarySearchTree() : root{nullptr} {
    }

    /**
     * Copy constructor
     */
    BinarySearchTree(const BinarySearchTree &rhs) : root{nullptr} {
        root = clone(rhs.root);
    }

    /**
     * Destructor for the tree
     */
    ~BinarySearchTree() {
        makeEmpty();
    }

    /**
     * Copy assignment: copy and swap idiom
     */
    BinarySearchTree &operator=(BinarySearchTree _copy) {
        std::swap(root, _copy.root);
        return *this;
    }

    /**
     * Find the smallest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable &findMin() const {
        if (isEmpty()) {
            throw UnderflowException{};
        }
        return findMin(root)->element;
    }

    /**
     * Find the largest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable &findMax() const {
        if (isEmpty()) {
            throw UnderflowException{};
        }
        return findMax(root)->element;
    }

    
     //* Returns true if x is found in the tree.
   
    // EXC. 1 (test 1 & 2)

    bool contains(const Comparable& x) const {

        return (contains(x, root) != nullptr);
    }

    //EXC. 2 (test 3)
    /*Iterator contains(const Comparable& x) const {

        return Iterator{ contains(x, root) };
    }*/


    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty() const {
        return root == nullptr;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void printTree(ostream &out = cout) const {
        if (isEmpty()) {
            out << "Empty tree";
        } else {
            out << "Pre-order:" << endl;
            preOrder(root, out); 
            //inorder(root, out);
        }
    }

    /**
     * Make the tree logically empty.
     */
    void makeEmpty() {
        root = makeEmpty(root);
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert(const Comparable &x) {
        root = insert(x, root);
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove(const Comparable &x) {
        root = remove(x, root);
    }

    /** Return total number of existing nodes
     *
     * Used for debug purposes
     */
    static int get_count_nodes() {
        return Node::count_nodes;
    }


    /**
    * get_parent: Get x parent from tree. Nothing if x is not found
    */

    Comparable get_parent(const Comparable &x){

        Node* temp = root;
        Comparable dummy{ 0 };

        while(temp != nullptr){

            if (temp->element < x) {
                temp = temp -> right;

            } else if (temp->element > x) {
                temp = temp -> left;
            }else{

                return (temp->parent != nullptr) ? temp->parent->element : dummy; // Dummy if we're at the root
			}
		}

        // We've reached the bottom
        return dummy;
	}


    /**
     * Find predecessor and succusser for x from the tree.
    */
    std::pair<Comparable, Comparable> find_pred_succ(const Comparable& x) const {

        std::pair<Comparable, Comparable> result;

        Node* temp = root;
        Comparable suc = temp->element;
        Comparable pred = temp->element;

        // If x is the root of the tree:
        if (temp->element == x) {

            Node* predecessor = findMax(temp);
            Node* successor = findMin(temp);

            if (predecessor != nullptr) suc = predecessor->element;
            if (successor != nullptr) pred = successor->element;

            result.first = pred;
            result.second = suc;

            return result;

        }

        // Traverse the tree
        while (temp != nullptr) {

            // look further down right
            if (temp->element < x) {

                pred = temp->element; // set predecessor to the element of temp
                temp = temp->right; // update pointer to look for a larger predecessor int the right part of the subtree

            }
            // look further down left
            else if (temp->element > x) {
                suc = temp->element; // set successor to the element of temp
                temp = temp->left;  // update pointer to look for a smaller successor in the left part of the subtree

            } // if we find the value somewhere else in the tree
            else {

                Node* predecessor = findMax(temp->left);
                Node* successor = findMin(temp->right);

                // Did we find a pred?
                if (predecessor) {
                    pred = predecessor->element;
                }
                else {
                    // On the left side of parent
                    pred = (temp->element < pred) ? temp->element : pred;
                }

                // Did we find a succ?
                if (successor) {
                    suc = successor->element;
                }
                else {
                    // On the right side of parent
                    suc = (temp->element > suc) ? temp->element : suc;
                }

                break;
            }
        }

        result.first = pred;
        result.second = suc;

        return result;
    }


/*OLD version, not approved*/

    /**
     * Find predcusserand succusser for x from the tree.
     */
    //std::pair<Comparable, Comparable> find_pred_succ(const Comparable& x) const {

    //    std::pair<Comparable, Comparable> result{};
    //    Comparable suc; // Higher than x
    //    Comparable pred; // Lower than x
    //    Node* tempRoot = root;

    //    // check if x is in the tree
    //    Node* temp = contains(x, root);

    //    // x is in tree
    //    if (temp != nullptr) {

    //        if (findMin(root)->element >= x) pred = x;
    //        else pred = find_predecessor(temp)->element;

    //        if (findMax(root)->element <= x) suc = x;
    //        else suc = find_successor(temp)->element;

    //    }
    //    else { // x is not in the tree

    //       //Node* tempRoot = root;

    //        while (tempRoot != nullptr) {

    //            if (tempRoot->element < x) {
    //                pred = tempRoot->element;
    //                tempRoot = tempRoot->right;
    //            }
    //            else {
    //                suc = tempRoot->element;
    //                tempRoot = tempRoot->left;
    //            }
    //        }

    //    }
    //    result.first = pred;
    //    result.second = suc;

    //    return result;
    //}

//---------------------------------------------------------------------



    Iterator begin() const { //return iterator to the node storing smallest value
        return ( isEmpty() ) ? end() : Iterator(findMin(root)); 
	}

    Iterator end() const {
        return Iterator(nullptr);
	}
    

private:
    Node *root;

    /**
     * Private member function to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Return a pointer to the node storing x.
     */
    Node *insert(const Comparable &x, Node *t) {
        if (t == nullptr) {
            t = new Node{x, nullptr, nullptr};
        } else if (x < t->element) {
            t->left = insert(x, t->left);
            t->left->parent = t; // Pointer to the parent
        } else if (t->element < x) {
            t->right = insert(x, t->right);
            t->right->parent = t; // Pointer to the parent
        } else {
            ;  // Duplicate; do nothing
        }

        return t;
    }

    /**
     * Private member function to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Return a pointer to the new root of the subtree that had root x
     */

    /*
    -------- EXC.4 Iterator validity ----------
    
    The given statement is not correct. In our Implementation we overwrite value x and 
    delete the node holding the value that ovwerwrites value x further down the tree.
    This means that the node that the iterator points to is not the one that gets invalidated. 
    
        6                                             7
    4       7      --> (overwrite element 6) -->  4      removed


    */

    Node *remove(const Comparable &x, Node *t) {
       
        if (t == nullptr) {
            return t;  // Item not found
        }
        if (x < t->element) {
            t->left = remove(x, t->left);
        } else if (t->element < x) {
            t->right = remove(x, t->right);
        }

        // x == t->element
        else if (t->left != nullptr && t->right != nullptr)  // Two children
        {
            t->element = findMin(t->right)->element;
            t->right = remove(t->element, t->right);

        // one children
        } else { 
            Node *oldNode = t;
            
            if(t->left != nullptr){
                t->left->parent = t->parent;
		    }

            if(t->right != nullptr){
                t->right->parent = t->parent;
		    }
            
            t = (t->left != nullptr) ? t->left : t->right;

            delete oldNode;

        }

        return t;
    }

    /**
     * Private member function to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    static Node *findMin(Node *t) {
        if (t == nullptr) {
            return nullptr;
        }
        if (t->left == nullptr) {
            return t;
        }

        // Tail recursion can be replaced by a loop
        return findMin(t->left);  // recursive call
    }

    /**
     * Private member function to find the largest item in a subtree t.
     * Return node containing the largest item.
     */
   static Node *findMax(Node *t)  {
        if (t != nullptr) {
            while (t->right != nullptr) {
                t = t->right;
            }
        }

        return t;
    }

    /**
     * Private member function to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the subtree.
     * Return a pointer to the node storing x, if x is found
     * Otherwise, return nullptr
     */
    Node* contains(const Comparable &x, Node *t) const {
        if (t == nullptr) {
            return t;
        } else if (x < t->element) {
            return contains(x, t->left);
        } else if (t->element < x) {
            return contains(x, t->right);
        } else {
            return t;  // Match
        }
    }

    /****** NONRECURSIVE VERSION*************************
    Node *contains(const Comparable &x, Node *t) const {
        while (t != nullptr) {
            if (x < t->element) {
                t = t->left;
            } else if (t->element < x) {
                t = t->right;
            } else {
                return t;  // Match
            }
        }

        return t;  // No match
    }
    *****************************************************/
    /**
     * Private member function to make subtree empty.
     */
    Node *makeEmpty(Node *t) {
        if (t != nullptr) {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }

        return nullptr;
    }

    /**
     * Private member function to print a subtree rooted at t in sorted order.
     * In-order traversal is used
     */
    void inorder(Node *t, ostream &out) const {
        if (t != nullptr) {
            inorder(t->left, out);
            out << t->element << '\n';
            inorder(t->right, out);
        }
    }

    void preOrder(Node* t, ostream &out, string s = "") const {

        if( t != nullptr ){

            out << s << t->element << std::endl;
            preOrder(t->left, out, s + "  ");
            preOrder(t->right, out, s + "  ");
		}
    }

    /**
     * Private member function to clone subtree.
     */
    Node *clone(Node *t) const {
        if (t == nullptr) {
            return nullptr;
        } else {
                 
            Node* temp = new Node{ t->element, clone(t->left), clone(t->right) };

            if (temp->left != nullptr) temp->left->parent = temp;

            if (temp->right != nullptr) temp->right->parent = temp;

            return temp;
        }
    }

    static Node* find_successor(Node* t) {
        
        if (t == nullptr) return nullptr;
       
        if (t->right != nullptr) { // t has a right tree
            return findMin(t->right);
        } 
        
        // climb up until parent has a left node
        while (t->parent != nullptr && t != t->parent->left) {
            t = t->parent;
        }

        return t = t->parent;
    }

    static Node* find_predecessor(Node* t) {

        if (t != nullptr && t->left != nullptr) { // t has a right tree
            return findMax(t->left);
        }

        // Climb back up until parent has a right node, then return parent
        while(t->parent != nullptr && t != t->parent->right){
            t = t->parent;
		}

        return t = t->parent;
    }
};

//Include the definition of class Node
#include "node.h"