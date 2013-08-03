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

// Need to increase stack size for large size input
// For approx 1 million vertices, 5 million edges - 8 MB of stack was not sufficient
// Next run with 160 MB of stack was successful

static const int START_VERTEX = 1;
static const int VERTEX_COUNT = 875714 + START_VERTEX;
int currentLeader = -1;
list<pair<int, int>> listOfLeaders;
vector<int> leader;
vector<int> nodesOrderedByFinishTime;
list<int> maxSCCs;
int t = 0; // finish time

void DFS(vector<int>* graph, int node, bool* explored, bool calcFinishTimes)
{
	explored[node] = true;
	if(!calcFinishTimes)
	{
		leader[node] = currentLeader;
		(listOfLeaders.back().second)++;
	}
	vector<int>::iterator it;
	for(it = graph[node].begin(); it != graph[node].end(); ++it)
	{
		if(false == explored[*it])
			DFS(graph, *it, explored, calcFinishTimes);
	}
	if(calcFinishTimes)
	{
		t++;
		nodesOrderedByFinishTime.push_back(node);
	}
}

void SCCLoop1(vector<int>* graph)
{
	bool explored[VERTEX_COUNT];
	nodesOrderedByFinishTime.reserve(VERTEX_COUNT);
	for(int i=VERTEX_COUNT-1; i>=0; i--)
	{
		explored[i] = false;
	}
	for(int i=VERTEX_COUNT-1; i>=START_VERTEX; i--)
	{
		if(true != explored[i])
		{
			DFS(graph, i, explored, true);
		}
	}
}

void SCCLoop2(vector<int>* graph)
{
	bool explored[VERTEX_COUNT];
	leader.reserve(VERTEX_COUNT);
	for(int i=VERTEX_COUNT-1; i>=0; i--)
	{
		explored[i] = false;
		leader.push_back(0);
	}
	vector<int>::reverse_iterator rit;
	for(rit = nodesOrderedByFinishTime.rbegin(); rit != nodesOrderedByFinishTime.rend(); ++rit)
	{
		if(true != explored[*rit])
		{
			currentLeader = *rit;
			listOfLeaders.push_back(make_pair(currentLeader, 0));
			DFS(graph, *rit, explored, false);

			// Update list of max SCCs
			int LeaderSize = listOfLeaders.back().second;
			if(maxSCCs.size() < 6)
			{
				maxSCCs.push_back(LeaderSize);
			}
			else
			{
				list<int>::iterator it;
				list<int>::iterator itMin = maxSCCs.end();
				int minSize = 0x7FFFFFFF;
				for(it = maxSCCs.begin(); it != maxSCCs.end(); ++it)
				{
					if(*it < minSize)
					{
						minSize = *it;
						itMin = it;
					}
				}
				if(minSize < LeaderSize)
				{
					maxSCCs.erase(itMin);
					maxSCCs.push_back(LeaderSize);
				}
			}
		}
	}
}

void scc(string filename)
{
	vector<int>* g = new vector<int>[VERTEX_COUNT];
	vector<int>* gRev = new vector<int>[VERTEX_COUNT];
	ifstream inFile;
	inFile.open(filename, ifstream::in);
	if( inFile.is_open() )
	{
		int tail, head;
		time_t time1 = time(NULL);
		while( inFile >> tail >> head )
		{
			g[tail].push_back(head);
			gRev[head].push_back(tail);
		}
		time_t time2 = time(NULL);
		cout << "Time Taken = " << difftime(time2, time1) << " seconds" <<endl;

		inFile.close();
	}
	else
	{
		cout << "Unable to open file " << filename << endl;
	}

	// DFS on reverse graph
	SCCLoop1(gRev);

	// DFS on forward graph
	SCCLoop2(g);

	delete [] g;
	delete [] gRev;

	cout << "Done "  << sizeof(pair<int, int>) << endl;
}