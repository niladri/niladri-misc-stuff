#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>

static const int MAX_VERTICES = 201;
list<int> adjList[MAX_VERTICES];
deque<int> vList;

int GetRandom(int max)
{
	srand(time(NULL));
	int r = rand() % max;
	return r;
}

int GetMinCutSize(int vertexCount)
{
	list<int>* adjListCopy;
	deque<int> vListCopy(vList);
	adjListCopy = new list<int>[vertexCount+1];
	for(int i=1; i<=vertexCount; i++)
	{
		adjListCopy[i] = adjList[i];
	}
	int finalVID;
	for(int i=0; i<vertexCount-2; i++)
	{
		int rV = GetRandom(vertexCount-i);
		int vID1 = vListCopy[rV];
		int rE = GetRandom(adjListCopy[vID1].size());
		list<int>::iterator it = adjListCopy[vID1].begin();
		for(int i=0; i<rE; i++) ++it;
		int vID2 = *it;

		// Merge the vertices vID1 to vID2, thus eliminating vID1
		// Add the edges of vID1 to adjListCopy of vID2
		it = adjListCopy[vID2].end();
		adjListCopy[vID2].insert(it, adjListCopy[vID1].begin(), adjListCopy[vID1].end());

		// Delete adjListCopy of vID1
		adjListCopy[vID1].erase(adjListCopy[vID1].begin(), adjListCopy[vID1].end());

		// Change all entries of vID1 with vID2
		for(int j=1; j<=vertexCount; j++)
		{
			for(it = adjListCopy[j].begin(); it != adjListCopy[j].end(); ++it)
			{
				if(*it == vID1) *it = vID2;
			}
		}

		// Remove self loops of vID2
		for(it = adjListCopy[vID2].begin(); it != adjListCopy[vID2].end(); )
		{
			if(*it == vID2) it = adjListCopy[vID2].erase(it);
			else ++it;
		}

		vListCopy.erase(vListCopy.begin() + rV);

		finalVID = vID2;
	}

	int minCutSize = adjListCopy[finalVID].size();

	delete [] adjListCopy;

	return minCutSize;
}

void karger(string filename)
{
	ifstream inFile;
	inFile.open(filename, ifstream::in);
	int vertexCount = 0;
	if( inFile.is_open() )
	{
		std::string line;

		while (getline(inFile, line))
		{
			std::istringstream iss(line);
			int n;
			
			// Read vertex ID
			int vID;
			iss >> vID;
			vertexCount++;
			vList.push_back(vID);

			while (iss >> n)
			{
				adjList[vID].push_back(n);
			}
		}
		inFile.close();
	}
	else
	{
		cout << "Unable to open file " << filename << endl;
	}

	time_t time1 = time(NULL);
	unsigned int minCutSize = 0xFFFFFFFF;
	for(int i=0; i<vertexCount*vertexCount; i++)
	{
		unsigned int cutSize = GetMinCutSize(vertexCount);
		if(cutSize < minCutSize)
			minCutSize = cutSize;
	}
	time_t time2 = time(NULL);
	cout << "Time Taken = " << difftime(time2, time1) << " seconds" <<endl;
	cout << minCutSize << endl;
}