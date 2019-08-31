/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include "p2Json.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include <sstream>
#include <iomanip>

using namespace std;

// Implement member functions of class Row and Table here
bool Json::read(const string& jsonFile)
{
	char line[100];
	char key_temp[100];
	char value_temp[100];
	char *pch;
	ifstream fin;
	fin.open(jsonFile);
	if(fin.is_open())
	{
		JsonElem elem;
		while(fin.getline(line,sizeof(line),'\n'))
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
					_obj.push_back(elem);
				}
				pch = strtok(NULL,": \",	");
			}
		}
		return true; // TODO
	}
	else
	{
		return false;
	}
	
}

ostream&
operator << (ostream& os, const JsonElem& j)
{
   	return (os << "\"" << j._key << "\" : " << j._value);
}
void JsonElem::set_key(string key)
{
	_key = key;
}
void JsonElem::set_value(int value)
{
	_value = value;
}
string JsonElem::get_key()
{
	return _key;
}
int JsonElem::get_value()
{
	return _value;
}
void Json::print()
{
	cout << "{" << endl;
	for(int i=0;i<_obj.size();i++)
	{
		cout << "  " << _obj[i];
		if(i != _obj.size()-1)
		{
			cout << "," << endl;
		}
		else
		{
			cout << endl;
		}
	}
	cout << "}" << endl;
}
void Json::sum()
{
	if(_obj.size()==0)
	{
		cout << "Error: No element found!!" << endl;
	}
	else
	{
		int _sum = 0;
		for(int i=0;i<_obj.size();i++)
		{
			_sum = _sum + _obj[i].get_value();
		}
		cout << "The summation of the values is: " << _sum << "." << endl;
	}
}
void Json::ave()
{
	if(_obj.size()==0)
	{
		cout << "Error: No element found!!" << endl;
	}
	else
	{
		double _sum = 0;
		double _ave = 0;
		for(int i=0;i<_obj.size();i++)
		{
			_sum = _sum + _obj[i].get_value();
		}
		_ave = _sum/(_obj.size());
		cout << "The average of the values is: " << fixed <<setprecision(1) << _ave << "." << endl;
	}
}
void Json::max()
{
	if(_obj.size()==0)
	{
		cout << "Error: No element found!!" << endl;
	}
	else
	{
		int i, j, temp = 0;

		for (i = 0; i < _obj.size()-1; i++)
		{
			if(_obj[temp].get_value()>_obj[i+1].get_value())
			{
			}
			else
			{
				temp = i+1;
			}
		}
		cout << "The maximum element is: { " << _obj[temp] << " }." << endl;
	}
}
void Json::min()
{
	if(_obj.size()==0)
	{
		cout << "Error: No element found!!" << endl;
	}
	else
	{
		int i, j, temp = 0;

		for (i = 0; i < _obj.size()-1; i++)
		{
			if(_obj[temp].get_value()<_obj[i+1].get_value())
			{
			}
			else
			{
				temp = i+1;
			}
		}
		cout << "The minimum element is: { " << _obj[temp] << " }." << endl;
	}
}
void Json::add(string _key_1,int _value_1)
{
	JsonElem elem_2;
	elem_2.set_key(_key_1);	
	elem_2.set_value(_value_1);
	_obj.push_back(elem_2);
}