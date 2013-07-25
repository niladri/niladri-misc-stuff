#include <iostream>
#include <string>
#include <algorithm>
using std::string;
using std::swap;
using std::cout;

class SwappingDigits
{
public:
	SwappingDigits()
	{
		m_maxIndex = -1;
		m_minIndex = -1;
	}
	string minNumber(string&);
	
private:
	void findMaxMinFirst();
	void findMaxMinRest();
	
private:
	string m_str;
	int m_len;
	int m_maxIndex;
	int m_minIndex;
};

string SwappingDigits::minNumber(string& str)
{
	cout << str;
	m_str = str;
	m_len = m_str.length();
	findMaxMinFirst();
	if( m_minIndex != -1 )
	{
		swap(m_str[m_maxIndex], m_str[m_minIndex]);
		return m_str;
	}
	findMaxMinRest();
	if( m_minIndex != -1 )
	{
		swap(m_str[m_maxIndex], m_str[m_minIndex]);
		return m_str;
	}
	return m_str;
}

void SwappingDigits::findMaxMinFirst()
{
	char first = m_str[0];
	char min = 'a';
	m_maxIndex = 0;
	for(int i=m_len-1; i>0; --i)
	{
		if(m_str[i] < first && m_str[i] != '0' && m_str[i] < min)
		{
			m_minIndex = i;
			min = m_str[i];
		} 
	}
}

void SwappingDigits::findMaxMinRest()
{
	for(int i=1; i<m_len-1; i++)
	{
		char first = m_str[i];
		char min = 'a';
		m_maxIndex = i;
		bool found = false;
		for(int j=m_len-1; j>i; --j)
		{
			if(m_str[j] < first && m_str[j] < min)
			{
				m_minIndex = j;
				min = m_str[j];
				found = true;
			}
		}
		if(found) break;
	}
}