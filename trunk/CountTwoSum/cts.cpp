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

static const int PRIME1 = 2562337;
static const int INPUT_SIZE = 1000000;
long long inputArr[INPUT_SIZE];
int listSize[PRIME1];
int distinctEntries;
long long hashTable[PRIME1][10];

bool FindTwoSum(int tVal)
{
	bool bFound = false;
	for(int i=0; i<distinctEntries; i++)
	{
		long long x = inputArr[i];
		long long y = tVal - x;
		int bucket = y % PRIME1;
		if(bucket < 0) bucket += PRIME1;

		for(int j=0; j<listSize[bucket]; j++)
		{
			if(y == hashTable[bucket][j])
			{
				bFound = true;
				return bFound;
			}
		}
	}
	return bFound;
}

int FindValidT()
{
	int count = 0;
	for(int tVal = -10000; tVal < 10001; tVal++)
	{
		//time_t time1 = time(NULL);
		if(FindTwoSum(tVal))
			count++;
		//time_t time2 = time(NULL);
		//cout << "Time Taken = " << difftime(time2, time1) << " seconds" <<endl;
	}
	return count;
}

void CountTwoSum(string filename)
{
	int index = 0;

	ifstream inFile;
	inFile.open(filename, ifstream::in);
	if( inFile.is_open() )
	{
		int bucket;
		long long val;
		while( inFile >> val )
		{
			bucket = val % PRIME1;
			if(bucket < 0) bucket += PRIME1;

			bool alreadyInTable = false;
			for(int j=0; j<listSize[bucket]; j++)
			{
				if(val == hashTable[bucket][j])
					alreadyInTable = true;
			}
			if(false == alreadyInTable)
			{
				hashTable[bucket][listSize[bucket]] = val;
				listSize[bucket]++;
				inputArr[index] = val;
				index++;
			}
		}
		inFile.close();
	}
	else
	{
		cout << "Unable to open file " << filename << endl;
	}

	distinctEntries = index;
	int count;
	count = FindValidT();

	//int maxSize = 0;
	//int maxID;
	//for(int i=0; i<PRIME1; i++)
	//{
	//	int size = hashTable[i].size();
	//	if( size > maxSize )
	//	{
	//		maxSize = size;
	//		maxID = i;
	//	}
	//}
	//cout << maxSize << endl;

	cout << count << endl; // 427
}