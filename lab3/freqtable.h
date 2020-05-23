#include <iostream>
#include <iomanip>  //setw
#include <utility>  //std::move

#pragma once

#include "BinarySearchTree.h"
#include "iterator.h" //needed?

using namespace std;


class freqtable {
public:

	// Constructors
	freqtable(): word{""}, counter{0} {}
	
	freqtable(string s, int c) : word{ s }, counter{ c }{}

	~freqtable() = default;

	// Overloaded operator <
	bool operator<(const freqtable &ft) const {
		return word < ft.word;
	}

	int updateCounter() {
		return ++counter;
	}

	friend ostream& operator<<(ostream& os, const freqtable &ft){

		os << setw(20) << ft.word << setw(10) << ft.counter;
		return os;
	}

private:
	int counter;
	string word;
};

