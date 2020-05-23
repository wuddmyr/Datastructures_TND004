#include "set.h"
#include "node.h"

/*
William Uddmyr - wilud321
Joel Paulsson - joepa811
*/

int Set::Node::count_nodes = 0;  // initialize total number of existing nodes to zero

/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

// Used for debug purposes
// Return number of existing nodes
int Set::get_count_nodes() {
    return Set::Node::count_nodes;
}

// Default constructor
Set::Set() : counter{ 0 } {

    head = new Node();
    tail = new Node(0, nullptr, head);

    head->next = tail;
}

// Conversion constructor
Set::Set(int n)
    : Set{ }  // create an empty list
{
    insert(n, tail); 
}

// Constructor to create a Set from a sorted vector v
Set::Set(const std::vector<int>& v)
    : Set{}  // create an empty list
{
    for(size_t i = 0; i < v.size(); i++){
        insert(v[i], tail);
	}
}

// Make the set empty
void Set::make_empty() {
    
    Node* ptr = head->next;

    while(ptr != tail){
        ptr = ptr->next;
        deleteNode(ptr->prev);
	}
}

Set::~Set() {    make_empty();

    delete head;
    delete tail;
}

// Copy constructor
Set::Set(const Set& source)
    : Set{}  // create an empty list
{
    Node* ptr = source.head->next;

    while(ptr != source.tail){
        
        insert(ptr->value, tail);
        
        ptr = ptr->next;
	}
}

// Copy-and-swap assignment operator
Set& Set::operator=(Set source) {

    // Copy-and-swap idiom: Make use of existing copy constructor to create a temp copy (call by value),
    // swap the data and delete the temp copy. 

    std::swap(head, source.head);
    std::swap(tail, source.tail);
    std::swap(counter, source.counter);

    return *this;
}

// Test whether a set is empty
bool Set::is_empty() const {
    return (counter == 0);
}

// Test set membership
bool Set::is_member(int val) const {
    
    Node* ptr = head->next;

    while (ptr != tail) {

        if (ptr->value == val) {
            return true;
        }
        ptr = ptr->next;
    }
    return false;  
}

// Return number of elements in the set
size_t Set::cardinality() const {
    return counter;
}

// Return true, if the set is a subset of b, otherwise false
// a <= b if every member of a is a member of b
bool Set::operator<=(const Set& b) const {
   
    Node* A = head->next;
    Node* B = b.head->next;

    while (A != tail && B != b.tail) {

        if (A->value < B->value) return false;

        else if (A->value > B->value) B = B->next;

        else if (A->value == B->value) {
            B = B->next;
            A = A->next;
        }
    }
    return (A == tail); // Om vi har gått igenom hela A har vi kommit till Tailen av A, dvs true
}

// Return true, if the set is equal to set b
// a == b, if a <= b and b <= a
bool Set::operator==(const Set& b) const {
    
    return( b <= *this && *this <= b); //subset of eachother ==
    //return (counter == b.counter && (*this <= b));
}

// Return true, if the set is different from set b
// a == b, iff a <= b and b <= a
bool Set::operator!=(const Set& b) const {
  
    return !(b <= *this && *this <= b); 
}

// Return true, if the set is a strict subset of S, otherwise false
// a == b, iff a <= b but not b <= a
bool Set::operator<(const Set& b) const {
 
    //return (counter < b.counter && *this <= b); ALT 1
    
    //ALT 2:
    return *this <= b && !(b <= *this); //if *this subset of b and b is not of *this
}

// Modify *this such that it becomes the union of *this with Set S
// Add to *this all elements in Set S (repeated elements are not allowed)
Set& Set::operator+=(const Set& S) {

    Node* A = head->next;
    Node* B = S.head->next;

    // Insert union: Add all from A & B but no duplicates

    while (B != S.tail && A != tail) {

        if (A->value < B->value) {
            A = A->next;
        }
        else if (A->value > B->value) {
            
            insert(B->value, A); // Update A
            B = B->next;
        }
        else { //if (B->value == A->value) { // Already in A
            B = B->next;
            A = A->next;
        }
    }

    //If there is remaining elements in B - insert to set:
    while (B != S.tail) {
        insert(B->value, A);
        B = B->next;
    }

    return *this;
}

// Modify *this such that it becomes the intersection of *this with Set S
Set& Set::operator*=(const Set& S) {

    Node* A = head->next;
    Node* B = S.head->next;

    //Save equal numbers in sets, delete others
    while (A != tail && B != S.tail) {

        if (A->value < B->value) {
            A = A->next;
            deleteNode(A->prev);
        }
        else if (A->value > B->value) {
            B = B->next;
        }
        else {  //if (A->value == B->value) { // values to keep
            A = A->next;
            B = B->next;
        }
    }

    // Remove remaining nodes in A.
    while(A != tail ){
        A = A->next;
        deleteNode(A->prev);
	}
    return *this;
}

// Modify *this such that it becomes the Set difference between Set *this and Set S
Set& Set::operator-=(const Set& S) {

    Node* A = head->next;
    Node* B = S.head->next;

    //Set difference
    while (B != S.tail && A != tail) {

        if (A->value < B->value) {
            A = A->next;
        }
        else if (A->value > B->value) {
            B = B->next;
        }
        else //if (A->value == B->value) {
        {
            B = B->next;
            A = A->next;

            deleteNode(A->prev);
        }
    }
    return *this;
}

// Overloaded stream insertion operator<<
std::ostream& operator<<(std::ostream& os, const Set& b) {
    if (b.is_empty()) {
        os << "Set is empty!";
    } else {
        Set::Node* temp{b.head->next};

        os << "{ ";
        while (temp != b.tail) {
            os << temp->value << " ";
            temp = temp->next;
        }

        os << "}";
    }

    return os;
}

/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

//If you add any private member functions to class Set then write the implementation here

//Insert node
void Set::insert(int n, Set::Node* tailNode ) {

    Node* newNode = new Node(n, tailNode, tailNode->prev);

    tailNode->prev->next = newNode; // Point the heads next to newNode
    tailNode->prev = newNode; // Point the tails previous to newNode

    counter++;
}

void Set::deleteNode(Set::Node* node){
 
    node->next->prev = node->prev; // Point tail to head
    node->prev->next = node->next; // Point head to tail

    delete node;

    counter--;
}

