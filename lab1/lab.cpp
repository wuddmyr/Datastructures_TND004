// lab1.cpp : stable partition 
// Iterative and divide-and-conquer
//
// Joel Paulsson, joepa811
// William Uddmyr, wilud321
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <iomanip>
#include <functional>  //std::function
#include <cassert>     //assert

//using namespace std;

/****************************************
 * Declarations                          *
 *****************************************/

 // generic class to write an item to a stream
template <typename T>
class Formatter {
public:
    Formatter(std::ostream& os, int width, int per_line)
        : os_{ os }, per_line_{ per_line }, width_{ width } {
    }

    void operator()(const T& t) {
        os_ << std::setw(width_) << t;
        if (++outputted_ % per_line_ == 0)
            os_ << "\n";
    }

private:
    std::ostream& os_;    // output stream
    const int per_line_;  // number of columns per line
    const int width_;     // column width
    int outputted_{ 0 };    // counter of number of items written to os_
};

namespace TND004 {
    // Divide-and-conquer algorithm
    void stable_partition(std::vector<int>& V, std::function<bool(int)> p);

    // Iterative algorithm
    void stable_partition_iterative(std::vector<int>& V, std::function<bool(int)> p);
}  // namespace TND004

// To test the Divide-and-conquer/iterative algorithms with input sequence V
// Expected output sequence is in res
void execute(std::vector<int>& V, const std::vector<int>& res);

bool even(int i);

/****************************************
 * Main:test code                        *
 *****************************************/

int main() {
    /*****************************************************
     * TEST PHASE 1                                       *
     ******************************************************/
    {
        std::cout << "TEST PHASE 1\n\n";

        std::vector<int> seq{ 1, 2 };

        std::cout << "Sequence: ";
        std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>{std::cout, " "});

        execute(seq, std::vector<int>{2, 1});
    }

    /*****************************************************
     * TEST PHASE 2                                       *
     ******************************************************/
    {
        std::cout << "\n\nTEST PHASE 2\n\n";

        std::vector<int> seq{ 2 };

        std::cout << "Sequence: ";
        std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>{std::cout, " "});

        execute(seq, std::vector<int>{2});
    }

    /*****************************************************
     * TEST PHASE 3                                       *
     ******************************************************/
    {
        std::cout << "\n\nTEST PHASE 3\n\n";

        std::vector<int> seq{ 3 };

        std::cout << "Sequence: ";
        std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>{std::cout, " "});

        execute(seq, std::vector<int>{3});
    }

    /*****************************************************
     * TEST PHASE 4                                       *
     ******************************************************/
    {
        std::cout << "\n\nTEST PHASE 4\n\n";

        std::vector<int> seq{ 3, 3 };

        std::cout << "Sequence: ";
        std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>(std::cout, " "));

        execute(seq, std::vector<int>{3, 3});
    }

    /*****************************************************
     * TEST PHASE 5                                       *
     ******************************************************/
    {
        std::cout << "\n\nTEST PHASE 5\n\n";

        std::vector<int> seq{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        std::cout << "Sequence: ";
        std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>(std::cout, " "));

        execute(seq, std::vector<int>{2, 4, 6, 8, 1, 3, 5, 7, 9});
    }

    /*****************************************************
     * TEST PHASE 6                                       *
     ******************************************************/
    {
        std::cout << "\n\nTEST PHASE 6: test with long sequence loaded from a file\n\n";

        std::ifstream file("../test_data.txt");

        if (!file) {
            std::cout << "Could not open test_data.txt!!\n";
            return 0;
        }

        // read the input sequence from file
        std::vector<int> seq{ std::istream_iterator<int>{file}, std::istream_iterator<int>() };
        file.close();

        std::cout << "Number of items in the sequence: " << seq.size() << '\n';

        // display sequence
        // std::for_each(std::begin(seq), std::end(seq), Formatter<int>(std::cout, 8, 5));

        // read the result sequence from file
        file.open("../test6_res.txt");

        if (!file) {
            std::cout << "Could not open test6_res.txt!!\n";
            return 0;
        }

        std::vector<int> res{ std::istream_iterator<int>{file}, std::istream_iterator<int>() };

        std::cout << "\nNumber of items in the result sequence: " << res.size() << '\n';

        // display sequence
        //std::for_each(std::begin(res), std::end(res), Formatter<int>(std::cout, 8, 5));

        assert(seq.size() == res.size());

        execute(seq, res);
    }
    return 0;
}

/****************************************
 * Functions definitions                 *
 *****************************************/

bool even(int i) {
    return i % 2 == 0;
}

// Iterative algorithm
void TND004::stable_partition_iterative(std::vector<int>& V, std::function<bool(int)> p) {

    // Alt 1:
    /*
    std::vector<int> evenV;
    std::vector<int> oddV;

    for (int i = 0; i < V.size(); i++) {
        if (p(V[i])) {
            evenV.insert(evenV.end(), V[i]);
        }
        else {
            oddV.insert(oddV.end(), V[i]);
        }
    }
    evenV.insert(evenV.end(), oddV.begin(), oddV.end()); // Odd  Vi s�tter in de udda fr�n och med sista platsen f�r de j�mna
    V = evenV;
    */

    // Alt 2:

    // Defining a vector of fixed size
    std::vector<int> tempVec(V.size());

    int evenCounter = 0;
    int oddCounter = V.size() - 1;

    for (unsigned int i = 0; i < V.size(); i++) {

        if (p(V[i])) {
            tempVec[evenCounter] = V[i];
            evenCounter++;
        }
        else {
            tempVec[oddCounter] = V[i];
            oddCounter--;
        }
    }

    std::reverse(tempVec.begin() + evenCounter, tempVec.end());

    V = tempVec;

}

// Auxiliary function that performs the stable partition recursivelly
namespace TND004 {
    // Divide-and-conquer algorithm: stable-partition the sub-sequence in V starting at first and ending
    // at last-1. If there are items with property p then return an iterator to the end of the block
    // containing the items with property p. If there are no items with property p then return first.
    std::vector<int>::iterator stable_partition(std::vector<int>& V, std::vector<int>::iterator first,
        std::vector<int>::iterator last,
        std::function<bool(int)> p) {

        // 0. BASE CASE:  Only the same numbers in the sequence
        if (first == last - 1) {
            return p(*first) ? last : first;
        }
        else {
            // 1. Find mid point 
            int dist = std::distance(first, last);
            std::vector<int>::iterator middle = first + (dist / 2);

            // 2. Call stable_partition again for left and right part of the sequence
            std::vector<int>::iterator left = stable_partition(V, first, middle, p);
            std::vector<int>::iterator right = stable_partition(V, middle, last, p);
           
            // Rotate [even -> odd]
            return std::rotate(left, middle, right);
        }
    }
}  // namespace TND004

void TND004::stable_partition(std::vector<int>& V, std::function<bool(int)> p) {
    TND004::stable_partition(V, std::begin(V), std::end(V), p);  // call auxiliary function
}

// To test the divide-and-conquer/iterative algorithms with input sequence V
// Expected output sequence is in res
void execute(std::vector<int>& V, const std::vector<int>& res) {
    std::vector<int> _copy{ V };

    std::cout << "\n\nIterative stable partition\n";
    TND004::stable_partition_iterative(V, even);
    assert(V == res);  // compare with the expected result

    //Display
    std::cout << std::endl;
    std::copy(std::begin(V), std::end(V), std::ostream_iterator<int>(std::cout, " "));


    std::cout << "\n\nDivide-and-conquer stable partition\n";
    TND004::stable_partition(_copy, even);
    assert(_copy == res);  // compare with the expected result

    //Display
    std::cout << std::endl;
    std::copy(std::begin(_copy), std::end(_copy), std::ostream_iterator<int>(std::cout, " "));
}