/*********************************************
 * file:	~\tnd004\lab\lab4a\digraph.cpp       *
 * remark: implementation of directed graphs  *
 **********************************************/

/*
William Uddmyr - wilud321
Joel Paulsson - joepa811
*/


#include <iostream>
#include <iomanip>
#include <cassert>
#include <queue> 

//using namespace std;

#include "digraph.h"
#include "queue.h"

const int INF = 999999;

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

Digraph::Digraph(int n) : size{n}, n_edges{0} {
    assert(n >= 1);

    table.resize(size + 1);
    dist.resize(size + 1);
    path.resize(size + 1);
    done.resize(size + 1);
}

// Create a digraph with n vertices and the edges in V
Digraph::Digraph(const std::vector<std::tuple<int, int, int>> &V, int n) : Digraph{n} {
    for (auto [u, v, w] : V)  // C++17
    {
        insertEdge(u, v, w);
    }
}

// -- MEMBER FUNCTIONS

// insert directed edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Digraph::insertEdge(int u, int v, int w) {
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    table[u].insert(v, w);

    n_edges += 1;
}

// remove directed edge (u, v)
void Digraph::removeEdge(int u, int v) {
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    table[u].remove(v);

    n_edges -= 1;
}

// unweighted single source shortest paths
/*
    Idea is to:
    - process vertices in layers: closest -> furthest
    - store dist and path
*/

void Digraph::uwsssp(int s) const {
    
    if (s < 1 || s > size) {
        std::cout << "\nERROR: expected source s in range 1.." << size << " !\n";
        return;
    }

    Queue<int> Q;

    // Defaults
    for(int i = 1; i <= size; ++i ){
        dist[i] = INF;
        path[i] = 0; 
	}

    dist[s] = 0; // starting distance
    Q.enqueue(s);

    while(!Q.isEmpty()){

        int v = Q.getFront(); // Vertex v
        Q.dequeue(); // Reduce que
        
        // Using adjecent list - set p as the first node in table. refers to u in slides. 
        Node* p = table[v].getFirst();

        while(p != nullptr){
          
            if( dist[p->vertex] == INF ){

                dist[p->vertex] = dist[v] + 1; // distance to neighbour is always 1 more than the distance to where we are
                path[p->vertex] = v;
                Q.enqueue(p->vertex); 
			}
            p = table[v].getNext();
		}
	}
}

// positive weighted single source shortest paths
// Dijktra’s algorithm 
void Digraph::pwsssp(int s) const {

    if (s < 1 || s > size) {
        cout << "\nERROR: expected source s in range 1.." << size << " !\n";
        return;
    }

    //Set default
    for(int i = 1; i <= size; ++i){
        dist[i] = INF;
        path[i] = 0;
        done[i] = false; 
	}

    dist[s] = 0;
    done[s] = true;
    int v = s; 

    while (true) {
        
        Node* p = table[v].getFirst();

        // check all adjecent nodes to p
        while (p != nullptr) {
               
            // check if min distance and not included
            if (!done[p->vertex] && dist[p->vertex] > (dist[v] + p->weight)) {

                //update
                dist[p->vertex] = dist[v] + p->weight; 
                path[p->vertex] = v;
            }
            p = table[v].getNext(); // move to next adjecent node
        }
  
        int smallestDistance = INF;

        // Update vertex according to shortest distance
        for (int i = 1; i <= size; ++i)
        {
            if (!done[i] && dist[i] < smallestDistance)
            {
                smallestDistance = dist[i]; // update shortest path
                v = i; // update vertex for the shorthest path
            }
        }

        if (smallestDistance == INF) break; // All nodes have been added to the tree => exit

        done[v] = true;   //Else, add to the tree.


        // create a min heap for priority queue
        /*std::priority_queue <int, std::vector<int>, std::greater<int>> minHeap;

        for(int i = 1; i <= size; ++i){
            if (!done[i]) minHeap.push(dist[i]);
        }

        if (minHeap.empty()) break; 
        else {

            // find shortest path 
            for (int i = 1; i <= size; ++i) {
                if ( dist[i] == minHeap.top()) v = i;
            }


            done[v] = true;
        }*/
   
    }
}


// print graph
void Digraph::printGraph() const {
    std::cout << "------------------------------------------------------------------\n";
    std::cout << "vertex  adjacency list                  \n";
    std::cout << "------------------------------------------------------------------\n";

    for (int v = 1; v <= size; v++) {
        std::cout << setw(4) << v << " : ";
        table[v].print();
    }

    std::cout << "------------------------------------------------------------------\n";
}

// print shortest path tree for s
void Digraph::printTree() const {
    std::cout << "----------------------\n";
    std::cout << "vertex    dist    path\n";
    std::cout << "----------------------\n";

    for (int v = 1; v <= size; v++) {
        std::cout << std::setw(4) << v << " :" << std::setw(8)
                  << ((dist[v] == std::numeric_limits<int>::max()) ? -1 : dist[v]) << std::setw(8)
                  << path[v] << "\n";
    }

    std::cout << "----------------------\n";
}

// print shortest path from s to t
void Digraph::printPath(int t) const {

    if (t < 1 || t > size) {
        std::cout << "\nERROR: expected target t in range 1.." << size << " !\n";
        return;
    }

    Digraph::printPathOnly(path[t]);

    std::cout << " " << t << " ";
    std::cout << "(" << dist[t] << ")";
   
}

void Digraph::printPathOnly(int t) const{

   if(path[t] == 0){
       std::cout << " " << t << " ";
       return;
   }

   //Visit the previous node in the path
   printPathOnly(path[t]);
   std::cout << " " << t << " "; 

}
