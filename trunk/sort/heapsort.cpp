#include <iostream>

using namespace std;

typedef unsigned int uint;

uint heapSize, arraySize;

uint parent(uint i) { return i>>1; }

uint left(uint i) { return i<<1; }

uint right(uint i) { return (i<<1) + 1; }

void MaxHeapify(int* A, uint index)
{
    uint l, r, largest;

    l = left(index);
    r = right(index);

    if ( l < heapSize && A[l] > A[index] )
        largest = l;
    else
        largest = index;

    if ( r < heapSize && A[r] > A[largest] )
        largest = r;

    if ( largest != index )
    {
        // Exchange A[index] and A[largest]
        int temp = A[index];
        A[index] = A[largest];
        A[largest] = temp;
        MaxHeapify( A, largest );
    }
}

void BuildMaxHeap(int* A)
{
    // Call MaxHeapify on all except the leaf nodes
    heapSize = arraySize;
    uint mid = arraySize >> 1;
    for (int i=mid; i>=0; i--)
        MaxHeapify( A, i );
}

void HeapSort(int* A, uint size)
{
    arraySize = size;
    BuildMaxHeap(A);
    for(int i=arraySize-1; i>0; i--)
    {
        // Exchange A[1] (which is max) with A[i]
        int heapTop = A[0];
        A[0] = A[i];
        A[i] = heapTop;

        // Reduce heap size
        heapSize--;

        MaxHeapify( A, 0 );
    }
}

int main(int c, char* argv[])
{
    cout << "Hello" << endl;
    int A[10] = { 6, 9, 2, 11, 45, 23, 1, 39, 17, 41 };
    HeapSort( A, 10 );
    for (int i=0; i<10; i++)
        cout << A[i] << " " ;
    cout << endl;
    
    return 0;
}

