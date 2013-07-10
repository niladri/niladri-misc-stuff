#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

int MergeAndCount(vector<int>& input, vector<int>& scratch, int l, int mid, int r)
{
	int lIndex = l;
	int rIndex = mid + 1;
	int sInv = 0;
	for(int i = l; i <= r; ++i)
	{
		if(rIndex > r || input[lIndex] < input[rIndex])
		{
			if(lIndex <= mid)
			{
				scratch[i] = input[lIndex];
				lIndex++;
			}
		}
		else
		{
			scratch[i] = input[rIndex];
			rIndex++;
			sInv += (mid - lIndex + 1);
		}
	}
	// Copy to Input
	for(int i = l; i <= r; ++i)
	{
		input[i] = scratch[i];
	}
	return sInv;
}

unsigned int SortAndCount(vector<int>& input, vector<int>& scratch, int l, int r)
{
	int length = r - l + 1;
	if( length <= 1 )
		return 0;

	int mid = (l + r)/2;
	unsigned int lInv = SortAndCount(input, scratch, l, mid);
	unsigned int rInv = SortAndCount(input, scratch, mid + 1, r);
	unsigned int sInv = MergeAndCount(input, scratch, l, mid, r);
	return lInv + rInv + sInv;
}

void foo23(string filename)
{
	vector<int> intVec;
	vector<int> scratch;

	ifstream inFile;
	inFile.open(filename, ifstream::in);
	if( inFile.is_open() )
	{
		int val;
		while( inFile.good() )
		{
			inFile >> val;
			intVec.push_back(val);
			scratch.push_back(0);
		}
		inFile.close();
	}
	else
	{
		cout << "Unable to open file " << filename << endl;
	}

	unsigned int inversions = SortAndCount(intVec, scratch, 0, intVec.size() - 1);
	cout << intVec.size() << " " << sizeof(long long) << endl;
	cout << inversions << endl;
}