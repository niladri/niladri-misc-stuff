#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class IDNumberVerification
{
public:
	string verify(string ID, vector<string> regCodes);

private:
	enum Months { JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };
};

string IDNumberVerification::verify(string ID, vector<string> regCodes)
{
	string inv = "Invalid";
	string regionCode(ID.begin(), ID.begin() + 6);
	string sYear(ID.begin() + 6, ID.begin() + 10);
	string sMonth(ID.begin() + 10, ID.begin() + 12);
	string sDay(ID.begin() + 12, ID.begin() + 14);
	string sSeqCode(ID.begin() + 14, ID.begin() + 17);
	char checkSum = ID[17];
	int year = atoi(sYear.c_str());
	int month = atoi(sMonth.c_str());
	int day = atoi(sDay.c_str());
	int seq = atoi(sSeqCode.c_str());

	vector<string>::iterator it = find(regCodes.begin(), regCodes.end(), regionCode);
	if( it == regCodes.end() )
		return inv;

	if( year < 1900 || year > 2011 ) return inv;
	if( month < 1 || month > 12 ) return inv;
	if( day < 1 ) return inv;

	if( month == FEB )
	{
		if( year % 4 == 0 && year != 1900 ) // Leap Year
		{
			if( day > 29 ) return inv;
		}
		else
		{
			if( day > 28 ) return inv;
		}
	}
	else
	{
		if( month == APR || month == JUN || month == SEP || month == NOV )
		{
			if( day > 30 ) return inv;
		}
		else
		{
			if( day > 31 ) return inv;
		}
	}

	if( !seq ) return inv;

	int inputcs = 11;
	if( checkSum == 'X' )
		inputcs = 10;
	else
		inputcs = checkSum - '0';

	if( inputcs < 0 || inputcs > 10 ) return inv; 

	int a[17];
	int cs = 0;
	for(int i=0; i<17; ++i)
	{
		a[i] = ID[i] - '0';
		cs += a[i] << (17 - i);
	}
	cs = cs % 11;

	if( (inputcs + cs)%11 != 1 ) return inv;

	if( seq % 2 ) return "Male";
	else return "Female";
}