/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include "p2Json.h"
#include <stdio.h>
#include <string.h>

using namespace std;

int main()
{
	Json json;
	// Read in the csv file. Do NOT change this part of code.
	string jsonFile;
	cout << "Please enter the file name: ";
	cin >> jsonFile;

 	if (json.read(jsonFile))
		cout << "File \"" << jsonFile << "\" was read in successfully." << endl;
 	else {
 		cerr << "Failed to read in file \"" << jsonFile << "\"!" << endl;
		exit(-1); // jsonFile does not exist.
 	}

    // TODO read and execute commands
   
   while (true) 
	{
     	char command[100];
		char _key_1[100];
		int _value_1;
		cout << "Enter command: ";
		cin >> command;
		if(strcmp(command,"ADD") == 0)
		{
			cin >> _key_1;
			cin >> _value_1;
			json.add(_key_1,_value_1);

		}
		else if(strcmp(command,"PRINT") == 0)
		{
			json.print();
		}
		else if(strcmp(command,"SUM") == 0)
		{
			json.sum();
		}
		else if(strcmp(command,"AVE") == 0)
		{
			json.ave();
		}
		else if(strcmp(command,"MAX") == 0)
		{
			json.max();
		}
		else if(strcmp(command,"MIN") == 0)
		{
			json.min();
		}
		else if(strcmp(command,"EXIT") == 0)
		{
			break;
		}
	}
}
