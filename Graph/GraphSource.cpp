// A C / C++ program for Dijkstra's single source shortest path algorithm.
// The program is for adjacency matrix representation of the graph

#include <stdio.h>
#include <chrono>
#include <limits.h>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>

// Number of vertices in the graph
#define V 10

using namespace std::chrono;

// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(int dist[], bool sptSet[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < V; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

// A utility function to print the constructed distance array
void printSolution(int dist[], int n)
{
	printf("Vertex   Distance from Sourcen");
	for (int i = 0; i < V; i++)
		printf("%d tt %dn ", i, dist[i]);
}

// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void help(bool **sptSet, int** graph, int** dist, int u, int from ,int to)
{


		// Update dist value of the adjacent vertices of the picked vertex.
	for (int v = from; v < to; v++)
	{

// Update dist[v] only if is not in sptSet, there is an edge from 
// u to v, and total weight of path from src to  v through u is 
// smaller than current value of dist[v]
		if (!(*sptSet)[v] && graph[u][v] && (*dist)[u] != INT_MAX
			&& (*dist)[u] + graph[u][v] < (*dist)[v])
			(*dist)[v] = (*dist)[u] + graph[u][v];

		std::this_thread::sleep_for(10ms);
	}
}
void dijkstra(int** graph, int src)
{
	int* dist = new int[V];     // The output array.  dist[i] will hold the shortest
					 // distance from src to i

	bool *sptSet = new bool[V]; // sptSet[i] will true if vertex i is included in shortest
					// path tree or shortest distance from src to i is finalized

					// Initialize all distances as INFINITE and stpSet[] as false
	for (int i = 0; i < V; i++)
		dist[i] = INT_MAX, sptSet[i] = false;

	// Distance of source vertex from itself is always 0
	dist[src] = 0;

	// Find shortest path for all vertices
	for (int count = 0; count < V - 1; count++)
	{
		// Pick the minimum distance vertex from the set of vertices not
		// yet processed. u is always equal to src in first iteration.
		int u = minDistance(dist, sptSet);

		// Mark the picked vertex as processed
		sptSet[u] = true;

		// Update dist value of the adjacent vertices of the picked vertex.
		/*for (int v = 0; v < V; v++)
		std::thread();

			// Update dist[v] only if is not in sptSet, there is an edge from 
			// u to v, and total weight of path from src to  v through u is 
			// smaller than current value of dist[v]
			if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
				&& dist[u] + graph[u][v] < dist[v])
				dist[v] = dist[u] + graph[u][v];*/
		help(&sptSet, graph, &dist, u, 0, V);
	}

	// print the constructed distance array
	//printSolution(dist, V);
}

void dijkstraParallel(int** graph, int src, int p)
{
	int* dist = new int[V];     // The output array.  dist[i] will hold the shortest
					 // distance from src to i

	bool *sptSet = new bool[V]; // sptSet[i] will true if vertex i is included in shortest
					// path tree or shortest distance from src to i is finalized

					// Initialize all distances as INFINITE and stpSet[] as false
	for (int i = 0; i < V; i++)
		dist[i] = INT_MAX, sptSet[i] = false;

	// Distance of source vertex from itself is always 0
	dist[src] = 0;

	// Find shortest path for all vertices
	for (int count = 0; count < V - 1; count++)
	{
		// Pick the minimum distance vertex from the set of vertices not
		// yet processed. u is always equal to src in first iteration.
		int u = minDistance(dist, sptSet);

		// Mark the picked vertex as processed
		sptSet[u] = true;

		// Update dist value of the adjacent vertices of the picked vertex.
		double h = (V-1) / (double)p;
		std::vector<std::thread> thrs;
		for (int i = 0; i < p; i++)
		{
			thrs.push_back(std::thread(help, &sptSet, graph, &dist, u, i*h, (i+1)*h));
			//help(&sptSet, graph, &dist, u, i*h, (i + 1)*h);
		}

		for (auto& i : thrs)
		{
			i.join();
		}
	}

	// print the constructed distance array
	//printSolution(dist, V);
}

// driver program to test above function
int main()
{
	/* Let us create the example graph discussed above */
	int** graph = new int*[V];

	for (int i = 0; i < V; i++)
	{
		graph[i] = new int[V];
		for (int j = 0; j < V; j++)
		{
			graph[i][j] = rand() % 10;
		}
	}

	auto t0 = clock();
	dijkstra(graph, 0);
	auto t1 = clock();
	dijkstraParallel(graph, 0,4);
	auto t2 = clock();
	
	std::cout << std::endl << "t0 = " << t1 - t0 << "\tt1 = " << t2 - t1 << std::endl;
	std::getchar();
	return 0;
}