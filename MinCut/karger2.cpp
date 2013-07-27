#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

typedef list<pair<int, int>> EdgeListType;

int GetRandom(int max)
{
	srand((unsigned int)time(NULL));
	int r = rand() % max;
	return r;
}

int GetMinCutSize(int vertexCount, EdgeListType& edgeList)
{
	EdgeListType edgeListCopy(edgeList);

	for(int i=0; i<vertexCount-2; i++)
	{
		int rV = GetRandom(edgeListCopy.size());
		EdgeListType::iterator it = edgeListCopy.begin();
		for(int j=0; j<rV; j++) ++it;
		pair<int, int> edge = *it;
		int vID1 = edge.first;
		int vID2 = edge.second;

		// Changes all vID1 to vID2 in edgeList
		for(it = edgeListCopy.begin(); it != edgeListCopy.end(); )
		{
			pair<int, int> edge = *it;
			if(edge.first == vID1) it->first = vID2;
			if(edge.second == vID1) it->second = vID2;
			// Remove self loops of vID2
			if(it->first == it->second)
				it = edgeListCopy.erase(it);
			else
				++it;
		}
	}

	return edgeListCopy.size();
}

void karger(string filename)
{
	EdgeListType edgeList;
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

			while (iss >> n)
			{
				if(n>vID)
					edgeList.push_back(make_pair(vID, n));
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
		unsigned int cutSize = GetMinCutSize(vertexCount, edgeList);
		if(cutSize < minCutSize)
			minCutSize = cutSize;
	}
	time_t time2 = time(NULL);
	cout << "Time Taken = " << difftime(time2, time1) << " seconds" <<endl;
	cout << minCutSize << endl;
}