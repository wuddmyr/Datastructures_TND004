/*********************************************
 * file:	~\tnd004\lab\lab4b\graph.h        *
 * remark:implementation of undirected graphs *
 **********************************************/
 /*
 William Uddmyr - wilud321
 Joel Paulsson - joepa811
 */

#include <iostream>
#include <iomanip>
#include <cassert>  //assert
#include <limits>   //std::numeric_limits
#include <queue>

// using namespace std;
#include "graph.h"
#include "edge.h"
#include "heap.h"
#include "dsets.h"

const int INF = 99999;

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

// Create an empty graph with n vertices
Graph::Graph(int n) : size{n}, n_edges{0} {
    assert(n >= 1);

    table.resize(size + 1);
}

Graph::Graph(const std::vector<std::tuple<int, int, int>> &V, int n) : Graph{n} {
    for (auto [u, v, w] : V)  // C++17
    {
        insertEdge(u, v, w);
    }
}

// -- MEMBER FUNCTIONS

// insert undirected edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Graph::insertEdge(int u, int v, int w) {
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    table[u].insert(v, w);
    table[v].insert(u, w);

    n_edges += 2;
}

// remove undirected edge (u, v)
void Graph::removeEdge(int u, int v) {
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    table[u].remove(v);
    table[v].remove(u);

    n_edges -= 2;
}

// Prim's minimum spanning tree algorithm
// Use for dens graphs
void Graph::mstPrim() const {

    int* dist = new int[size + 1]; // weight
    int* path = new int[size + 1]; // edges
    bool* done = new bool[size + 1]; // visited nodes
   
    // Fill arrays with default values
    for(int i = 1; i <= size; ++i){
        dist[i] = INF;
        path[i] = 0;
        done[i] = false;
	}

    // Start vertex
    int v = 1; 
    dist[v] = 0;
    done[v] = true;

    int totWeight = 0; 

    // Add one node to the tree each iteration
    while(true){

        Node* temp = table[v].getFirst(); 

        // Store/keep track of previous vertices in array dist
        // For all nodes adjecent to the current one
        while(temp){

            // Does temp belong to the tree? Is this a better edge then previous? => 
            if (!done[temp->vertex] && (dist[temp->vertex] > temp->weight )) { 

                // Record the best edge
                path[temp->vertex] = v;
                dist[temp->vertex] = temp->weight;
            }
            temp = temp->next;
        }

        int smallestDistance = INF;

		for(int i = 1; i <= size; ++i)
		{
			if(!done[i] && dist[i] < smallestDistance)
			{
				smallestDistance = dist[i]; // update shortest path
				v = i; // update vertex for the shorthest path
			}
		}

		if (smallestDistance == INF) break; // All nodes have been added to the tree => exit

        done[v] = true;   //Else, add to the tree.
        totWeight += dist[v];

        Edge tempEdge(path[v], v, dist[v]); //path to "find" every node in tree
        std::cout << tempEdge <<std::endl;    
    }

    std::cout << "Total weight: " << totWeight << std::endl;
}

// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const {

    Heap <Edge> H(size-1); // min-heap with priority = weights
    DSets D(size);

    int counter = 0;
    int totWeight = 0;

    // Add edges to the heap
    for(int i = 1; i <= size; ++i ){

        Node* temp = table[i].getFirst();
       
        //insert smallest weighted edge into heap 
        while (temp) {

            if(i < temp->vertex){

                // create edges
                Edge tempEdge(i,temp->vertex, temp->weight);
                // insert edge to heap
                H.insert(tempEdge);
			}

            //move to next vertex - use table[i].getNext() instead!
            temp = table[i].getNext();//temp->next;
        }
	}
    
    while(counter < size - 1){ //tree must have n-1 edges

        Edge UV = H.deleteMin(); //find an edge with lowest cost 

        //check that u and v are in differnet trees. Avoid cycles
        if(D.find(UV.head) != D.find(UV.tail)){ 

            std::cout << UV << std::endl;

            //merge two different trees
            D.join(D.find(UV.head), D.find(UV.tail));
            totWeight += UV.weight; 
            counter++;
		}
	}

    std::cout << "Total weight: " << totWeight << std::endl;
}

// print graph
void Graph::printGraph() const {
    std::cout << "------------------------------------------------------------------\n";
    std::cout << "vertex  adjacency list                                            \n";
    std::cout << "------------------------------------------------------------------\n";

    for (int v = 1; v <= size; v++) {
        std::cout << std::setw(4) << v << " : ";
        table[v].print();
    }

    std::cout << "------------------------------------------------------------------\n";
}
