/*
* File:   main.cpp
* Author: Prasanna Subburaj
* Approximation Algorithm for Steiner Tree Implementaion
* Reference Paper: [1] http://monet.skku.ac.kr/course_materials/undergraduate/al/lecture/2006/TM.pdf
*/

#include <cstdlib>
#include <iostream>
#include <vector>

#define VERTICES 10           //Define the number of vertices |V| in the graph G = (V,E) 


using namespace std;

/**
*Class to implement Priority Queue
*/
class heap{
public:
	int left, right, largest, temp;
	void max_heapify(int*, int, int);
	void heapsort(int*, int);
	void build_max_heap(int*, int);
	void display(int*, int, string);
};

/**
* Function to correct the violation of heap property
* @param array
* @param i
* @param size
*/

void heap::max_heapify(int *array, int i, int size){
	left = 2 * i;
	right = (2 * i) + 1;
	if ((left <= size) && (array[left]>array[i]))
		largest = left;
	else
		largest = i;

	if ((right <= size) && (array[right]>array[largest]))
		largest = right;

	if (largest != i){
		temp = array[i];
		array[i] = array[largest];
		array[largest] = temp;
		max_heapify(array, largest, size);
	}
}

/**
* Perform heapsort on the max-heap
* @param array
* @param size
*/

void heap::heapsort(int *array, int size){
	int temp = 0;
	if (size>0){
		temp = array[1];
		array[1] = array[size];
		array[size] = temp;
		size--;
		max_heapify(array, 1, size);
		heapsort(array, size);
	}
}

/**
* Builds Max-Heap from an unsorted array
* @param array
* @param size
*/

void heap::build_max_heap(int* array, int size){

	for (int i = (size / 2); i >= 1; i--){
		max_heapify(array, i, size);
	}
}

/**
*Class to implement of Graph
*/
class Graph
{
	int i, j, numberofedges, v;
	int AdjacencyMatrix[VERTICES+1][VERTICES+1];
	int AdjacencyMatrix1[VERTICES+1][VERTICES+1];
	enum state{ white, gray, black };
	int key[VERTICES+1], pred[VERTICES+1], key1[VERTICES+1];
	struct edge{
		int vertex1, vertex2, weight;
	}edges[(VERTICES+1) * 3];
	int *path;
public:
	int steinerterminals[VERTICES];
	Graph(int n)
	{
		i = 0;
		j = 0;
		numberofedges = 0;
		for (i = 0; i <= n; i++)
		{
			key1[i] = 0;
			for (j = 0; j <= n; j++)
			{
				AdjacencyMatrix[i][j] = 0;
				AdjacencyMatrix1[i][j] = 0;
			}
		}
	}
	void assignterminals(int a[],int n){
		for (int i = 0; i < n;i++)
			steinerterminals[i] = a[i];
	}
	void insertedge(int, int, int);
	void printadjacencymatrix(int);
	int *dijkstra(int,int,int,int);
	void steiner(int);
	int *pathfind(vector<int>, int);
};


/**
* This method is used to insert one edge at a time into the graph
* @param origin
* @param destination
* @param weight
*/
void Graph::insertedge(int origin, int destination, int weight)
{
	if ((AdjacencyMatrix[origin][destination] != 0))
	{
		cout << "This edge already exits..\n";
	}
	else
	{
		AdjacencyMatrix[origin][destination] = weight;
		numberofedges = numberofedges + 1;
		edges[numberofedges].vertex1 = origin;
		edges[numberofedges].vertex2 = destination;
		edges[numberofedges].weight = weight;

		AdjacencyMatrix[destination][origin] = weight;
		numberofedges = numberofedges + 1;
		edges[numberofedges].vertex1 = destination;
		edges[numberofedges].vertex2 = origin;
		edges[numberofedges].weight = weight;
	}
}

/**
* Prints the Adjacency Matrix of the formed graph
* @param n
*/
void Graph::printadjacencymatrix(int n)
{
	cout << "The Adjacency Matrix for the given graph\n\n";
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; j++)
		{
			cout << AdjacencyMatrix[i][j] << "\t";
		}
		cout << "\n";
	}
}

/**
*Dijkstra Shortest Path Algorithm implementation for Graph G={V,E}
*/
int* Graph::dijkstra(int src,int dest,int cost,int par){
	heap h;
	int u;						//Holds the elements of the tree as we traverse through the graph finding the shortest path
	int i = 1, j = 1, k = VERTICES, l = 1, r = 0;
	edge adj[VERTICES+1];					//Holds the set of edges to the vertices that are adjacent from a selected Vertex
	int out[VERTICES+1];					//Array to hold the final set of the vertices in the traversed order
	state *vstate = new state[VERTICES+1];                  //Holds the state of each vertices

	for (i = 0; i <= VERTICES; i++)
	{
		pred[i] = 0;				//Initialize the parent set 
		key[i] = 10000;				//Initialize the weight of all the Vertex as maximum
		vstate[i] = white;			//Mark the state of all the Vertex as unvisited(white)

		adj[i].vertex1 = 0;
		adj[i].vertex2 = 0;
		adj[i].weight = 0;
		out[i] = 0;
	}

	//Assign the starting parameters of the shortest path algorithm
	key[src] = cost;
	pred[src] = par;
	vstate[src] = black;

	u = src;							//Source vertex to which we have to find the shortest path
	while (k>0){
		j = 1;
		for (i = 1; i <= (VERTICES+1) * 3; i++){
			if (u == edges[i].vertex1){		//find all the adjacent elements of the selected vertex
				adj[j].vertex1 = edges[i].vertex1;
				adj[j].vertex2 = edges[i].vertex2;
				adj[j].weight = edges[i].weight;
				j++;
			}
		}

		for (i = 1; i<j; i++){
			r = key[u] + adj[i].weight;
			if ((vstate[adj[i].vertex2] == white) && (r<key[adj[i].vertex2])){

				key[adj[i].vertex2] = r;	//update the key value of the vertex if an smallest reachable path is found
				pred[adj[i].vertex2] = adj[i].vertex1;

			}
		}

		vstate[u] = black;		//Mark the visited Vertex as black
		out[l] = u;				//Fill up the visited elements of the graph in order
		
		key1[0] = 10000;

		for (i = 1; i <= VERTICES; i++){
			if (vstate[i] != black)
				key1[i] = key[i];
			else
				key1[i] = 10000;
		}

		h.build_max_heap(key1, VERTICES);
		h.heapsort(key1, VERTICES);

		for (i = 1; i <= VERTICES; i++)
		{															//Select the vertex with the shortest path using Heap
			if (key1[1] == key[i]){
				if (vstate[i] != black){
					u = i;  //Extract Minimum edge from the set based on key value
					break;
				}

			}
		}

		l++;
		k--;
	}

	j = 1;
	int d = pred[dest];
	path = new int;

	//Return the Cost to the destination and the path to the destination by trying out all the Vj-1 to vi

	path[0] = key[dest];
	path[j] = dest;
	j++;
	while (d != 0){
		path[j] = d;
		d = pred[d];
		j++;
	}
	
	return path;
}

/**
*Steiner Tree implementation to find the smallest cost subgraph of G that contains all the vertices in the subset S
*/
void Graph::steiner(int numterminal){
	int u,cost=10000,*path;
	static int k = 0,v=0;
	static vector<int> finalvertices,edges1;
	edge finaledges[VERTICES * 2], finaledges1[VERTICES * 2];

	//Assign the first Vertex to the tree T1 so that V1={v1} and E1={empty}
	u = steinerterminals[0];
	finalvertices.push_back(u);

	for (int i = 1; i <numterminal; i++){

		//select a vertex vi from S-Vi-1 and find the minimum path from any vertices in S-Vi-1 to vi
		std::vector<int>::iterator it;
		it = find(finalvertices.begin(), finalvertices.end(), steinerterminals[i]);
		if (it == finalvertices.end()){
			path = pathfind(finalvertices, steinerterminals[i]);

			//Adding the vertices to the tree Ti-1
			for (int p = 1; *(p + path) >0; p++)
			{
				if (!(find(finalvertices.begin(), finalvertices.end(), *(p + path)) != finalvertices.end()))
				{
					finalvertices.push_back(*(p + path));
				}
				edges1.push_back(*(p + path));
			}

			//Adding edges to the Tree Ti-1
			while (!edges1.empty())
			{
				finaledges[k].vertex1 = edges1.back();
				edges1.pop_back();
				if (!edges1.empty()){
					finaledges[k].vertex2 = edges1.back();
					k++;
				}
			}

			for (int s = 0; s < k; s++){
				int count = 0, dn = 0;
				for (int p = 1; p <= numberofedges; p++){
					if ((finaledges[s].vertex1 == edges[p].vertex1) && (finaledges[s].vertex2 == edges[p].vertex2)){		//find all the adjacent elements of the selected vertex	
						for (int ck = 0; ck < v; ck++){
							if ((finaledges1[ck].vertex1 == edges[p].vertex1) && (finaledges1[ck].vertex2 == edges[p].vertex2))
							{
								count++;
							}
						}
						if (count == 0){
							for (int gk = 0; gk < v; gk++){
								//Remove the costly edge pair between two vertices and replace it with the cheaper out of the two
								if ((finaledges1[gk].vertex1 == edges[p].vertex2) && (finaledges1[gk].vertex2 == edges[p].vertex1))
								{
									if (finaledges1[gk].weight>edges[p].weight){
										finaledges1[gk].vertex1 = edges[p].vertex1;
										finaledges1[gk].vertex2 = edges[p].vertex2;
										finaledges1[gk].weight = edges[p].weight;
									}
									dn++;
								}
							}
							if (dn == 0){
								finaledges1[v].vertex1 = edges[p].vertex1;
								finaledges1[v].vertex2 = edges[p].vertex2;
								finaledges1[v].weight = edges[p].weight;
								v++;
							}
						}
					}
				}
			}
		}
	}


	//Print the Steiner Points in the Steiner Tree 
	cout << "The following are the steiner points (if empty there is no Steiner Points in the Steiner Tree):" << endl;
	for (auto &i : finalvertices){
		int steinercount = 0;
		for (int pk = 0; pk < numterminal; pk++)
		{
			if (i == steinerterminals[pk])
			{
				steinercount++;
			}
		}
		if (steinercount == 0){
			cout << i <<endl;
		}
		
	}

	//Compute and print the cost of the Steiner Tree and also print the edges of the Steiner Tree
	int sum = 0;
	cout <<endl<< "The following are the edges in the steiner tree:" << endl;
	for (int z = 0; z < v; z++){
		cout <<endl <<" V1= " << finaledges1[z].vertex1 << " V2= " << finaledges1[z].vertex2 << " W= " << finaledges1[z].weight;
		sum += finaledges1[z].weight;
	}

	cout<<endl<<endl<< "Cost of the Steiner Tree = " << sum;
}

/**
*PATH(W,v) function as mentioned in [1] to compute the minimum cost path for the set of vertices in W to vertex v
*@param W It will contain the set of vertices in T which is a subset of S
*@param v It is the new vertex for which we need to find the shortest path from all the vertices in W to v
*@return R Address of the path whos cost is less than all the other shortest path in W
*/
int *Graph::pathfind(vector<int> W, int v){
	int size = W.size();
	int *p[VERTICES+1],j=0,*R,cost=10000;

	for (int k = 0; k<VERTICES; k++)
		p[k] = NULL;

	//Run Dijkstra from all the vertex in W to v
	for (auto &i : W){
		if (i != v){
		p[j]=dijkstra(i, v, 0, 0);	
		j++;
		}
	}

	//Based on the cost select a shortest path that has minimum cost
	for (int k = 0; p[k] !=NULL; k++)
	{
		if (p[k][0] < cost)
		{
			R = p[k];
			cost = p[k][0];
		}
	}

	//Return the address of path containing the minimum cost shortest path from a vertex in W to v
	return R;
}

int main(int argc, char** argv) {

	/*
	*Given graph G=(V,E) where V is the set of vertices and E is the set of edges
	*The cost function c defines the edges by relating vertices vi,vj in the form (vi,vj,c)
	*Give all the edges with the cost in the above format here to construct a graph
	*/
	
	Graph g(VERTICES);

	g.insertedge(1, 2, 2);
	g.insertedge(1, 5, 3);
	g.insertedge(2, 3, 3);
	g.insertedge(2, 6, 9);
	g.insertedge(3, 7, 2);
	g.insertedge(3, 4, 8);
	g.insertedge(4, 8, 9);
	g.insertedge(5, 6, 4);
	g.insertedge(5, 9, 6);
	g.insertedge(7, 6, 1);
	g.insertedge(7, 8, 4);
	g.insertedge(9, 10, 7);
	g.insertedge(6, 10, 4);
	g.insertedge(8, 10, 5);

	/*
	*Set S contains the subset of V for which we have to find the minimum cost Steiner Tree
	*/
	
	int set[5] = { 1,4,5,9,10};
	g.assignterminals(set, 5);

	/*
	*Call the Steiner Tree function to compute the minimum Steiner Tree in the Graph G
	*/
	g.steiner(5);
	
	cin.get();
	return 0;
}



