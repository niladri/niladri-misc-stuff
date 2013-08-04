#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <stack>
#include <utility>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
using namespace std;

struct _edge
{
	int tail;
	int head;
	int weight;
};
typedef struct _edge Edge;

void CalculateShortestPaths(list<Edge>& edgeList, int n, int s, vector<int>& shortestDistances)
{
	vector<bool> vertexDone(n, false);
	int vertexDoneCount = 0;

	shortestDistances[s] = 0;
	vertexDone[s] = true;
	vertexDoneCount++;

	while(vertexDoneCount < n - 1)
	{
		list<Edge>::iterator it;
		int minDist = 1000000;
		int selectedVertex = 0;
		for(it = edgeList.begin(); it != edgeList.end(); ++it)
		{
			// Check that the edge crosses from calculated vertex set to uncalculated set
			if(true == vertexDone[it->tail] && false == vertexDone[it->head])
			{
				int newDistance = shortestDistances[it->tail] + it->weight;
				if(newDistance < minDist)
				{
					minDist = newDistance;
					selectedVertex = it->head;
				}
			}
		}

		shortestDistances[selectedVertex] = minDist;
		vertexDone[selectedVertex] = true;
		vertexDoneCount++;
	}
}

void dijkstra(string filename)
{
	ifstream inFile;
	int vertexCount = 0;
	list<Edge> edgeList;
	inFile.open(filename, ifstream::in);
	int firstVertexID = -1;
	if( inFile.is_open() )
	{
		std::string line;

		while (getline(inFile, line))
		{
			std::istringstream iss(line);
			vertexCount++;

			Edge e;
			iss >> e.tail;

			if(firstVertexID == -1) firstVertexID = e.tail;
			if(firstVertexID == 0) { e.tail++; }

			string str;
			int pos;
			while( iss >> str )
			{
				pos = str.find(',');
				e.head = stoi(str.substr(0, pos));
				e.weight = stoi(str.substr(pos+1, str.length()));
				if(firstVertexID == 0) { e.head++; }
				edgeList.push_back(e);
			}
		}
		inFile.close();
	}
	else
	{
		cout << "Unable to open file " << filename << endl;
	}

	// To take care of vertex IDs strating from 1
	vertexCount += 1;

	int source = 1;
	vector<int> shortestDistances(vertexCount, 1000000);
	CalculateShortestPaths(edgeList, vertexCount, source, shortestDistances);
}