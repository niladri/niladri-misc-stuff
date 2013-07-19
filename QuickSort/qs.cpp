int Partition(vector<int>& input, int l, int r)
{
#ifdef LAST_PIVOT
	swap(input[l], input[r]);
#endif

#ifdef MID_PIVOT
	int sp = l;
	int mid = (l+r)/2;
	int midVal = input[mid];
	int lVal = input[l];
	int rVal = input[r];
	if(lVal < midVal)
	{
		if(rVal < lVal) { sp = l; /* lVal is median */ }
		else if(rVal < midVal) { sp = r; /* rVal is median */ }
		else { sp = mid; /* midVal is median */ }
	}
	else // lVal > midVal
	{
		if(rVal > lVal) { sp = l; /* lVal is median */ }
		else if(rVal > midVal) { sp = r; /* rVal is median */ }
		else { sp = mid; /* midVal is median */ }
	}
	swap(input[l], input[sp]);
#endif

	int pivot = l;
	int pivotVal = input[pivot];
	int i = l+1, j = l+1;
	for( ; j <= r; ++j)
	{
		if( input[j] < pivotVal )
		{
			swap(input[i], input[j]);
			i++;
		}
	}
	swap(input[l], input[i-1]);
	return i-1;
}

int QuickSort(vector<int>& input, int l, int r)
{
	if( (r - l) < 1 )
		return 0;
	int pivot = Partition(input, l, r);
	int lComparisons = QuickSort(input, l, pivot - 1);
	int rComparisons = QuickSort(input, pivot + 1, r);
	return lComparisons + rComparisons + (r-l);
}

void foo24(string filename)
{
	vector<int> intVec;

	ifstream inFile;
	inFile.open(filename, ifstream::in);
	if( inFile.is_open() )
	{
		int val;
		while( inFile.good() )
		{
			inFile >> val;
			intVec.push_back(val);
		}
		inFile.close();
	}
	else
	{
		cout << "Unable to open file " << filename << endl;
	}

	unsigned int comparisons = QuickSort(intVec, 0, intVec.size() - 1);
	cout << intVec.size() << endl;
	cout << comparisons << endl;
}