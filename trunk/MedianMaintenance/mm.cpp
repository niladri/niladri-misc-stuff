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

typedef unsigned int uint;

uint maxHeapSize, minHeapSize, arraySize;

uint parent(uint i) { return (i-1)>>1; }
uint left(uint i) { return (i<<1) + 1; }
uint right(uint i) { return (i<<1) + 2; }

void MaxHeapify(int* A, uint index)
{
    uint l, r, largest;

    l = left(index);
    r = right(index);

    if ( l < maxHeapSize && A[l] > A[index] )
        largest = l;
    else
        largest = index;

	if ( r < maxHeapSize && A[r] > A[largest] )
        largest = r;

    if ( largest != index )
    {
		swap(A[index], A[largest]);
        MaxHeapify( A, largest );
    }
}

void MinHeapify(int* A, uint index)
{
    uint l, r, smallest;

    l = left(index);
    r = right(index);

	if ( l < minHeapSize && A[l] < A[index] )
        smallest = l;
    else
        smallest = index;

	if ( r < minHeapSize && A[r] < A[smallest] )
        smallest = r;

    if ( smallest != index )
    {
		swap(A[index], A[smallest]);
        MinHeapify( A, smallest );
    }
}

void MaxSiftUp(int* A, uint index)
{
	if(index == 0)
		return;

	uint p = parent(index);
	if(A[p] < A[index])
	{
		swap(A[p], A[index]);
		MaxSiftUp(A, p);
	}
}

void MinSiftUp(int* A, uint index)
{
	if(index == 0)
		return;

	uint p = parent(index);
	if(A[p] > A[index])
	{
		swap(A[p], A[index]);
		MinSiftUp(A, p);
	}
}

void BuildMaxHeap(int* A)
{
    // Call MaxHeapify on all except the leaf nodes
	maxHeapSize = arraySize;
    uint mid = arraySize >> 1;
    for (int i=mid; i>=0; i--)
        MaxHeapify( A, i );
}

void BuildMinHeap(int* A)
{
    // Call MinHeapify on all except the leaf nodes
	minHeapSize = arraySize;
    uint mid = arraySize >> 1;
    for (int i=mid; i>=0; i--)
        MinHeapify( A, i );
}

void HeapSort(int* A, uint size)
{
    arraySize = size;
    BuildMaxHeap(A);
    for(int i=arraySize-1; i>0; i--)
    {
        // Exchange A[0] (which is max) with A[i]
		swap(A[0], A[i]);

        // Reduce heap size
		maxHeapSize--;

        MaxHeapify( A, 0 );
    }
}

void HeapSort2(int* A, uint size)
{
    arraySize = size;
    BuildMinHeap(A);
    for(int i=arraySize-1; i>0; i--)
    {
        // Exchange A[0] (which is min) with A[i]
        swap(A[0], A[i]);

        // Reduce heap size
		minHeapSize--;

        MinHeapify( A, 0 );
    }
}

int minHeap[5001]; // for upper half
int maxHeap[5001]; // for lower half

void MaintainMedian(string filename)
{
	int index = 0;
	int input[10000];

	ifstream inFile;
	inFile.open(filename, ifstream::in);
	if( inFile.is_open() )
	{
		int val;
		while( inFile >> val )
		{
			input[index] = val;
			index++;
		}
		inFile.close();
	}
	else
	{
		cout << "Unable to open file " << filename << endl;
	}

	long long medianSum = 0;
	for(int i=0; i<index; i++)
	{
		int newVal = input[i];
		if(maxHeapSize <= minHeapSize)
		{
			if(newVal < minHeap[0] || maxHeapSize == 0)
			{
				// add new value to lower half
				maxHeap[maxHeapSize] = newVal;
				MaxSiftUp(maxHeap, maxHeapSize);
				maxHeapSize++;
			}
			else
			{
				// Remove top (min) of upper half
				int transferVal = minHeap[0];
				swap(minHeap[0], minHeap[minHeapSize-1]);
				minHeapSize--;
				MinHeapify(minHeap, 0);
				minHeap[minHeapSize] = 0;

				// Add it to lower half
				maxHeap[maxHeapSize] = transferVal;
				MaxSiftUp(maxHeap, maxHeapSize);
				maxHeapSize++;

				// add new value to upper half
				minHeap[minHeapSize] = newVal;
				MinSiftUp(minHeap, minHeapSize);
				minHeapSize++;
			}
		}
		else
		{
			if(minHeapSize == 0) minHeap[0] = maxHeap[0];

			if(newVal > maxHeap[0])
			{
				// add new value to upper half
				minHeap[minHeapSize] = newVal;
				MinSiftUp(minHeap, minHeapSize);
				minHeapSize++;
			}
			else
			{
				// Remove top (max) of lower half
				int transferVal = maxHeap[0];
				swap(maxHeap[0], maxHeap[maxHeapSize-1]);
				maxHeapSize--;
				MaxHeapify(maxHeap, 0);
				maxHeap[maxHeapSize] = 0;

				// Add it to upper half
				minHeap[minHeapSize] = transferVal;
				MinSiftUp(minHeap, minHeapSize);
				minHeapSize++;

				// add new value to lower half
				maxHeap[maxHeapSize] = newVal;
				MaxSiftUp(maxHeap, maxHeapSize);
				maxHeapSize++;
			}
		}

		int currentMedian = maxHeap[0];
		medianSum += currentMedian;
	}

	cout << medianSum % 10000 << endl;
}