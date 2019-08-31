/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
#include "stdlib.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream& operator >> (istream& is, DBJson& j)
{
    assert(j._obj.empty());

    char line[100];
	char key_temp[100];
	char value_temp[100];
	char *pch;
    DBJsonElem elem;

    while(is.getline(line,sizeof(line),'\n'))
    {
        if(strcmp(line,"}") == NULL)
        {
            break;
        }
        pch = strtok(line,": \",	");
        int count = 0;

        while(pch != NULL)
        {
            count++;
            if(count%2 == 1)	
            {
                string key(pch);
                elem.set_key(key);	
            }
            else if(count%2 == 0)
            {
                string value_1(pch);
                int value_2;
                istringstream temp(value_1);
                temp>>value_2;
                elem.set_value(value_2);
                j._obj.push_back(elem);
            }
            pch = strtok(NULL,": \",	");
        }
    }
    return is;
}

ostream& operator << (ostream& os, const DBJson& j)
{
    cout << "{" << endl;
	for(int i=0;i<j._obj.size();i++)
	{
		cout << "  " ;
        os << j._obj[i];
		if(i != j._obj.size()-1)
		{
			cout << "," << endl;
		}
		else
		{
			cout << endl;
		}
	}
	cout << "}" << endl;
    cout << "Total JSON elements: " << j._obj.size();
    return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()
{
   _obj.clear();
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
	int counter_2 = 0;
	for(int i = 0; i < _obj.size(); i++)
	{
		if(elm.key() == _obj[i].key())
		{
			counter_2 ++;
		}
	}
	if(counter_2 == 1)
	{
		return false;
	}
	else
	{
		_obj.push_back(elm);
		return true;
	}
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
    if(_obj.empty())
	{
		return NAN;
	}
	else
	{
		double _sum = 0;
		double _ave = 0;
		for(int i=0;i<_obj.size();i++)
		{
			_sum = _sum + _obj[i].value();
		}
		_ave = _sum/(_obj.size());
		return _ave;
	}
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   if(_obj.empty())
	{
		idx = _obj.size();
		int maxN = INT_MIN;
		return  maxN;
	}
	else
	{
		int i, temp = 0;

		for (i = 0; i < _obj.size()-1; i++)
		{
			if(_obj[temp].value()>_obj[i+1].value())
			{
			}
			else
			{
				temp = i+1;
			}
		}
		idx = temp;
	}
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::min(size_t& idx) const
{
    if(_obj.empty())
	{	
		idx = _obj.size();
		int minN = INT_MAX;
		return  minN;
	}
	else
	{
		int i, temp = 0;

		for (i = 0; i < _obj.size()-1; i++)
		{
			if(_obj[temp].value()<_obj[i+1].value())
			{
			}
			else
			{
				temp = i+1;
			}
		}
		idx = temp;
	}
}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
    if(_obj.empty())
	{
		int s = 0;
		return s;
	}
	else
	{
		int _sum = 0;
		for(int i=0;i<_obj.size();i++)
		{
			_sum = _sum + _obj[i].value();
		}
		return _sum;
	}
}
