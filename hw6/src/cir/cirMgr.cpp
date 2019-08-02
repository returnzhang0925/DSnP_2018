// /****************************************************************************
//   FileName     [ cirMgr.cpp ]
//   PackageName  [ cir ]
//   Synopsis     [ Define cir manager functions ]
//   Author       [ Chung-Yang (Ric) Huang ]
//   Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
// ****************************************************************************/

// #include <iostream>
// #include <iomanip>
// #include <cstdio>
// #include <fstream>
// #include <ctype.h>
// #include <cassert>
// #include <cstring>
// #include "cirMgr.h"
// #include "cirGate.h"
// #include "util.h"
// #include <stdio.h>
// #include <string.h>
// #include "stdlib.h"
// #include <sstream>
// #include <iomanip>
// #include <vector>
// #include <string>
// #include <algorithm>

// using namespace std;

// // TODO: Implement memeber functions for class CirMgr

// /*******************************/
// /*   Global variable and enum  */
// /*******************************/
// CirMgr* cirMgr = 0;

// enum CirParseError {
//    EXTRA_SPACE,
//    MISSING_SPACE,
//    ILLEGAL_WSPACE,
//    ILLEGAL_NUM,
//    ILLEGAL_IDENTIFIER,
//    ILLEGAL_SYMBOL_TYPE,
//    ILLEGAL_SYMBOL_NAME,
//    MISSING_NUM,
//    MISSING_IDENTIFIER,
//    MISSING_NEWLINE,
//    MISSING_DEF,
//    CANNOT_INVERTED,
//    MAX_LIT_ID,
//    REDEF_GATE,
//    REDEF_SYMBOLIC_NAME,
//    REDEF_CONST,
//    NUM_TOO_SMALL,
//    NUM_TOO_BIG,

//    DUMMY_END
// };

// /**************************************/
// /*   Static varaibles and functions   */
// /**************************************/
// static unsigned lineNo = 0;  // in printint, lineNo needs to ++
// static unsigned colNo  = 0;  // in printing, colNo needs to ++
// static char buf[1024];
// static string errMsg;
// static int errInt;
// static CirGate *errGate;

// static bool
// parseError(CirParseError err)
// {
// 	switch (err) {
// 		case EXTRA_SPACE:
// 			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
// 				<< ": Extra space character is detected!!" << endl;
// 			break;
// 		case MISSING_SPACE:
// 			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
// 				<< ": Missing space character!!" << endl;
// 			break;
// 		case ILLEGAL_WSPACE: // for non-space white space character
// 			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
// 				<< ": Illegal white space char(" << errInt
// 				<< ") is detected!!" << endl;
// 			break;
// 		case ILLEGAL_NUM:
// 			cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
// 				<< errMsg << "!!" << endl;
// 			break;
// 		case ILLEGAL_IDENTIFIER:
// 			cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
// 				<< errMsg << "\"!!" << endl;
// 			break;
// 		case ILLEGAL_SYMBOL_TYPE:
// 			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
// 				<< ": Illegal symbol type (" << errMsg << ")!!" << endl;
// 			break;
// 		case ILLEGAL_SYMBOL_NAME:
// 			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
// 				<< ": Symbolic name contains un-printable char(" << errInt
// 				<< ")!!" << endl;
// 			break;
// 		case MISSING_NUM:
// 			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
// 				<< ": Missing " << errMsg << "!!" << endl;
// 			break;
// 		case MISSING_IDENTIFIER:
// 			cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
// 				<< errMsg << "\"!!" << endl;
// 			break;
// 		case MISSING_NEWLINE:
// 			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
// 				<< ": A new line is expected here!!" << endl;
// 			break;
// 		case MISSING_DEF:
// 			cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
// 				<< " definition!!" << endl;
// 			break;
// 		case CANNOT_INVERTED:
// 			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
// 				<< ": " << errMsg << " " << errInt << "(" << errInt/2
// 				<< ") cannot be inverted!!" << endl;
// 			break;
// 		case MAX_LIT_ID:
// 			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
// 				<< ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
// 				<< endl;
// 			break;
// 		case REDEF_GATE:
// 			cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
// 				<< "\" is redefined, previously defined as "
// 				<< errGate->getTypeStr() << " in line " << errGate->getLineNo()
// 				<< "!!" << endl;
// 			break;
// 		case REDEF_SYMBOLIC_NAME:
// 			cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
// 				<< errMsg << errInt << "\" is redefined!!" << endl;
// 			break;
// 		case REDEF_CONST:
// 			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
// 				<< ": Cannot redefine constant (" << errInt << ")!!" << endl;
// 			break;
// 		case NUM_TOO_SMALL:
// 			cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
// 				<< " is too small (" << errInt << ")!!" << endl;
// 			break;
// 		case NUM_TOO_BIG:
// 			cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
// 				<< " is too big (" << errInt << ")!!" << endl;
// 			break;
// 		default: break;
//    }
//    return false;
// }

// /**************************************************************/
// /*   class CirMgr member functions for circuit construction   */
// /**************************************************************/
// CirGate* 
// CirMgr::getGate(unsigned gid) const 
// {
// 	for(unsigned i = 0; i < gates.size(); i++) 
// 	{
// 		if(gates[i] == NULL) { return 0; }
// 		else if(gid == gates[i]->get_Id()) { return gates[i]; }
// 	}
// }
// bool
// CirMgr::readCircuit(const string& fileName)
// {
//    	char line[1000];
// 	char *pch;
// 	int count_l = 0;
// 	ifstream fin;
// 	fin.open(fileName);
// 	if(fin.is_open())
// 	{
// 		bool check_in = false;	
// 		while(fin.getline(line, sizeof(line), '\n'))
// 		{
// 			pch = strtok(line, " ");
// 			count_l ++;
// 			if(*pch == 'i0' || *pch == 'c' || *pch == 'o0') { break; }
// 			while(pch != NULL)
// 			{
// 				if(count_l == 1)
// 				{
// 					if(check_in == false){ check_in = true; }
// 					else
// 					{
// 						string value_1(pch);
// 						unsigned int value_2;
// 						istringstream temp(value_1);
// 						temp>>value_2;
// 						_MILOA.push_back(value_2);
// 					}
// 				}
// 				else if(count_l >= _MILOA[1] + _MILOA[3] + 2 && count_l < _MILOA[1] + _MILOA[3] +_MILOA[4] + 2)
// 				{
// 					string key(pch);
// 					andgate_tmp.push_back(key);
// 				}
// 				else if(count_l >= _MILOA[1] + _MILOA[3] +_MILOA[4] + 2){ break; }
// 				else
// 				{
// 					string key(pch);
// 					gate_tmp.push_back(key);
// 				}
// 				pch = strtok(NULL, " ");
// 			}
// 		}
// 		fin.close();

// 		for(unsigned i = 0; i < _MILOA[0] + _MILOA[3] + 1; ++i) 
// 			gates.push_back(NULL);

// 		gates[0]= new CirGate(GateType::CONST_GATE, 0, 0, 0);
// 		for(unsigned i = 1; i < _MILOA[1] + 1; i++)
// 		{
// 			unsigned int id_tmp;
// 			istringstream temp(gate_tmp[i - 1]);
// 			temp>>id_tmp;
// 			if(id_tmp % 2 == 0)
// 			{
// 				gates[i] = new CirGate(GateType::PI_GATE, id_tmp / 2, i + 1, id_tmp);
// 			}
// 			else
// 			{
// 				gates[i] = new CirGate(GateType::PI_GATE, (id_tmp - 1)/2, i + 1, id_tmp);
// 				gates[i]->set_invert();
// 			}
// 		}
// 		for(unsigned i = _MILOA[1] + 1; i < _MILOA[1] + _MILOA[3] + 1; i++)
// 		{
// 			unsigned id_tmp;
// 			istringstream temp(gate_tmp[i - 1]);
// 			temp>>id_tmp;
// 			if(id_tmp % 2 == 0)
// 			{
// 				gates[i] = new CirGate(GateType::PO_GATE, _MILOA[0] + i - (_MILOA[1]), i + 1, id_tmp);
// 			}
// 			else
// 			{
// 				gates[i] = new CirGate(GateType::PO_GATE, _MILOA[0] + i - (_MILOA[1]), i + 1, id_tmp);
// 				gates[i]->set_invert();
// 			}
// 		}
// 		for(unsigned i = _MILOA[1] + _MILOA[3] + 1; i < _MILOA[1] + _MILOA[3] +_MILOA[4] + 1; i++)
// 		{
// 			unsigned id_tmp;
// 			istringstream temp(andgate_tmp[(i - (_MILOA[1] + _MILOA[3] + 1)) * 3]);
// 			temp>>id_tmp;
// 			if(id_tmp % 2 == 0)
// 			{
// 				gates[i] = new CirGate(GateType::AIG_GATE, id_tmp / 2, i + 1, id_tmp);
// 			}
// 			else
// 			{
// 				gates[i] = new CirGate(GateType::AIG_GATE, (id_tmp - 1) / 2, i + 1, id_tmp);
// 				gates[i]->set_invert();
// 			}
// 		}
// 		ifstream fin2(fileName, ios::in);
// 		string line_2;
// 		int count_l2 = 0;
// 		stringstream ss;
// 		if(fin2.is_open())
// 		{
// 			while(getline(fin2,line_2))
// 			{
// 				count_l2 ++;
// 				cir_tmp.push_back(line_2);
// 				if(count_l2 >= _MILOA[1] + _MILOA[3] + _MILOA[4] + 2)
// 				{
// 					char tmp[1000];
// 					char dest[1000];
// 					string str;
// 					unsigned num;
// 					ss << line_2;
// 					ss >> tmp;
// 					ss.clear();
// 					ss.str("");
// 					strncpy(dest, tmp + 1, strlen(tmp));
// 					ss << dest;
// 					ss >> num;
// 					if(tmp[0] == 'i')
// 					{
// 						gates[num + 1]->set_Symbol(line_2.substr(strlen(tmp) + 1,line_2.length() - 3));
// 					}
// 					else if(tmp[0] == 'o')
// 					{
// 						gates[_MILOA[1] + num + 1]->set_Symbol(line_2.substr(strlen(tmp) + 1,line_2.length() - 3));
// 					}
// 					else{ break; }
// 					ss.clear();
// 					ss.str("");
// 				}
// 			}
// 		}
// 		fin2.close();
// 		/*************construct AIG_netlist*************/
// 		for(unsigned i = 0; i < _MILOA[4]; i++)
// 		{
// 			unsigned count = i * 3;
// 			unsigned count_undef = 0;
// 			unsigned id_tmp;
// 			istringstream temp(andgate_tmp[count + 1]);
// 			temp>>id_tmp;
// 			if(id_tmp % 2 == 0)
// 			{
// 				id_tmp = id_tmp / 2;
// 				for(unsigned j = 0; j < _MILOA[1] + _MILOA[3] + _MILOA[4] + 1; j++)
// 				{
// 					if(id_tmp == gates[j]->get_Id() && gates[j] != NULL)
// 					{
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j]);
// 						gates[j]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
// 						break;
// 					}
// 					else if(j == _MILOA[1] + _MILOA[3] + _MILOA[4])
// 					{
// 						count_undef ++;
// 						gates[j + count_undef] = new CirGate(GateType::UNDEF_GATE, id_tmp, 0, id_tmp*2);
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j + count_undef]);
// 						gates[j + count_undef]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
// 						break;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				id_tmp = (id_tmp - 1) / 2;
// 				for(unsigned j = 0; j < _MILOA[1] + _MILOA[3] + _MILOA[4] + 1; j++)
// 				{
// 					if(id_tmp == gates[j]->get_Id() && gates[j] != NULL)
// 					{
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j]);
// 						gates[j]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_invert1().push_back(true);
// 						gates[j]->get_invert2().push_back(true);
// 						break;
// 					}
// 					else if(j == _MILOA[1] + _MILOA[3] + _MILOA[4])
// 					{
// 						count_undef ++;
// 						gates[j + count_undef] = new CirGate(GateType::UNDEF_GATE, id_tmp, 0, (id_tmp*2) + 1);
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j + count_undef]);
// 						gates[j + count_undef]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_invert1().push_back(true);
// 						gates[j + count_undef]->get_invert2().push_back(true);
// 						break;
// 					}
// 				}
// 			}
// 			unsigned id_tmp2;
// 			istringstream temp2(andgate_tmp[count + 2]);
// 			temp2>>id_tmp2;
// 			if(id_tmp2 % 2 == 0)
// 			{
// 				id_tmp2 = id_tmp2 / 2;
// 				for(unsigned j = 0; j < _MILOA[1] + _MILOA[3] + _MILOA[4] + 1; j++)
// 				{
// 					if(id_tmp2 == gates[j]->get_Id() && gates[j] != NULL)
// 					{
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j]);
// 						gates[j]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
// 						break;
// 					}
// 					else if(j == _MILOA[1] + _MILOA[3] + _MILOA[4])
// 					{
// 						count_undef ++;
// 						gates[j + count_undef] = new CirGate(GateType::UNDEF_GATE, id_tmp2, 0, id_tmp2*2);
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j + count_undef]);
// 						gates[j + count_undef]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
// 						break;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				id_tmp2 = (id_tmp2 - 1) / 2;
// 				for(unsigned j = 0; j < _MILOA[1] + _MILOA[3] + _MILOA[4] + 1; j++)
// 				{
// 					if(id_tmp2 == gates[j]->get_Id() && gates[j] != NULL)
// 					{
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j]);
// 						gates[j]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);						
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_invert1().push_back(true);
// 						gates[j]->get_invert2().push_back(true);
// 						break;
// 					}
// 					else if(j == _MILOA[1] + _MILOA[3] + _MILOA[4])
// 					{
// 						count_undef ++;
// 						gates[j + count_undef] = new CirGate(GateType::UNDEF_GATE, id_tmp2, 0, (id_tmp2*2) + 1);
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j + count_undef]);
// 						gates[j + count_undef]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + count_undef]);
// 						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_invert1().push_back(true);
// 						gates[j + count_undef]->get_invert2().push_back(true);
// 						break;
// 					}
// 				}
// 			}
// 		}
// 		/*************construct PO_netlist*************/
// 		unsigned countundef_2;
// 		for(unsigned i = 0; i < _MILOA[3]; i++)
// 		{
// 			bool check_1 = false;
// 			bool check_2 = false;
// 			for(unsigned j = 0; j < _MILOA[4]; j++) // for AIG
// 			{
// 				unsigned id_tmp = (gates[_MILOA[1] + 1 + i]->get_Val()%2 == 0) ? (gates[_MILOA[1] + 1 + i]->get_Val())/2 : (gates[_MILOA[1] + 1 + i]->get_Val()-1)/2;
// 				if(id_tmp == gates[_MILOA[1] + _MILOA[3] + 1 + j]->get_Id())
// 				{
// 					gates[_MILOA[1] + 1 + i]->get_fin().push_back(gates[_MILOA[1] + _MILOA[3] + 1 + j]);
// 					gates[_MILOA[1] + _MILOA[3] + 1 + j]->get_fout().push_back(gates[_MILOA[1] + 1 + i]);
// 					if(gates[_MILOA[1] + 1 + i]->get_Val() % 2 != 0)
// 					{
// 						gates[_MILOA[1] + 1 + i]->get_invert1().push_back(true);
// 						gates[_MILOA[1] + _MILOA[3] + 1 + j]->get_invert2().push_back(true);
// 					}
// 					check_1 = true;
// 				}
// 			}
// 			for(unsigned m = 0; m < _MILOA[1] + 1; m++) // for PI
// 			{
// 				unsigned id_tmp = (gates[_MILOA[1] + 1 + i]->get_Val()%2 == 0) ? (gates[_MILOA[1] + 1 + i]->get_Val())/2 : (gates[_MILOA[1] + 1 + i]->get_Val()-1)/2;
// 				if(id_tmp == gates[m]->get_Id())
// 				{
// 					gates[_MILOA[1] + 1 + i]->get_fin().push_back(gates[m]);
// 					gates[m]->get_fout().push_back(gates[_MILOA[1] + 1 + i]);
// 					if(gates[_MILOA[1] + 1 + i]->get_Val() % 2 != 0)
// 					{
// 						gates[_MILOA[1] + 1 + i]->get_invert1().push_back(true);
// 						gates[m]->get_invert2().push_back(true);
// 					}
// 					check_2 = true;
// 				}
// 			}
// 			if(!check_1 && !check_2)
// 			{
// 				countundef_2 ++;
// 				unsigned id_tmp3;
// 				if(gates[_MILOA[1] + 1 + i]->get_Val()%2 != 0)
// 				{
// 					id_tmp3 = (gates[_MILOA[1] + 1 + i]->get_Val() - 1)/2;
// 					gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2] = new CirGate(GateType::UNDEF_GATE, id_tmp3, 0, (id_tmp3*2) + 1);
// 					gates[_MILOA[1] + 1 + i]->get_fin().push_back(gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2]);
// 					gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2]->get_fout().push_back(gates[_MILOA[1] + 1 + i]);
// 					gates[_MILOA[1] + 1 + i]->get_invert1().push_back(true);
// 					gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2]->get_invert2().push_back(true);
// 				}
// 				else
// 				{
// 					id_tmp3 = gates[_MILOA[1] + 1 + i]->get_Val()/2;
// 					gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2] = new CirGate(GateType::UNDEF_GATE, id_tmp3, 0, id_tmp3*2);
// 					gates[_MILOA[1] + 1 + i]->get_fin().push_back(gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2]);
// 					gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2]->get_fout().push_back(gates[_MILOA[1] + 1 + i]);
// 				}
// 			}
// 		}
// 		buildDFSOrder();
// 		return true;
// 	}
// 	else
// 	{
// 		cerr << "Cannot open design \""  << fileName<< " \"!!" << endl; 
// 		return false;
// 	}
// }

// /**********************************************************/
// /*   class CirMgr member functions for circuit printing   */
// /**********************************************************/
// /*********************
// Circuit Statistics
// ==================
//   PI          20
//   PO          12
//   AIG        130
// ------------------
//   Total      162
// *********************/
// void
// CirMgr::printSummary() const
// {
// 	cout << "\nCircuit Statistics\n"
// 		 << "==================\n"
// 		 << "  PI" << right << setw(12) << _MILOA[1] << "\n"
// 		 << "  PO" << right << setw(12) << _MILOA[3] << "\n"
// 		 << "  AIG" << right << setw(11) << _MILOA[4] << "\n"
// 		 << "------------------\n"
// 		 << "  Total" << right << setw(9) <<  _MILOA[1] + _MILOA[3] + _MILOA[4] << endl;
// }

// void
// CirMgr::printNetlist() const
// {
// 	if(No_PO == true)
// 		cout << "\n";
// 	else
// 	{
// 		cout << "\n";
// 		for(unsigned i = 0; i < _dfsgate.size(); i++)
// 		{
// 			cout << "[" << i << "] ";
// 			_dfsgate[i]->printGate();
// 			cout << endl;
// 		}
// 	}
// }

// void
// CirMgr::printPIs() const
// {
// 	cout << "PIs of the circuit:";
// 	for(unsigned i = 1; i < _MILOA[1] + 1; i++)
// 		cout << " " << gates[i]->get_Id();
// 	cout << endl;
// }

// void
// CirMgr::printPOs() const
// {
// 	cout << "POs of the circuit:";
// 	for(unsigned i = 1 + _MILOA[1]; i < _MILOA[1] +_MILOA[3] + 1; i++)
// 		cout << " " << gates[i]->get_Id();
// 	cout << endl;
// }

// void
// CirMgr::printFloatGates() const
// {
// 	/*****************Check floating*****************/
// 	vector<unsigned int> floattmp;
// 	bool check_inf = false;
// 	for(unsigned i = 0; i < gates.size(); i++)
// 	{
// 		if(gates[i] == NULL) { break; }
// 		if(gates[i]->get_type() == UNDEF_GATE && gates[i]->get_fout()[0]->get_choosefl() != true)
// 		{
// 			if(!check_inf) 
// 			{ 
// 				check_inf =true;
// 				cout << "Gates with floating fanin(s):";
// 			}
// 			floattmp.push_back(gates[i]->get_fout()[0]->get_Id());
// 			gates[i]->get_fout()[0]->set_choosefl();
// 			if(gates[i + 1] == NULL) { break; }
// 		}
// 	}
// 	sort(floattmp.begin(), floattmp.end());
// 	for(unsigned i = 0; i < floattmp.size(); i++)
// 		cout << " " << floattmp[i];
// 	if(check_inf) { cout << endl; }
// 	/*****************Check notused*****************/
// 	vector<unsigned int> nousedtmp;
// 	bool check_nou = false;
// 	for(unsigned i = 0; i < gates.size(); i++)
// 	{
// 		if(gates[i] == NULL) { break; }
// 		if(gates[i]->get_fout().empty())
// 		{
// 			if(gates[i]->get_type() == PO_GATE || gates[i]->get_type() == CONST_GATE) {}
// 			else
// 			{
// 				if(!check_nou) 
// 				{ 
// 					check_nou =true;
// 					cout << "Gates defined but not used  :";
// 				}
// 				nousedtmp.push_back(gates[i]->get_Id());	
// 			}
// 		}
// 		if(gates[i]->get_type() == UNDEF_GATE)
// 			if(gates[i + 1] == NULL) { break; }
// 	}
// 	sort(nousedtmp.begin(), nousedtmp.end());
// 	for(unsigned i = 0; i < nousedtmp.size(); i++)
// 		cout << " " << nousedtmp[i];
// 	if(check_nou == true)
// 		cout << endl;
// }

// void
// CirMgr::writeAag(ostream& outfile) const
// {
// 	stringstream ss;
// 	string str;
	
// 	for(int i = 0; i < _MILOA[1] + _MILOA[3] + 1; i++)
// 	{
// 		outfile << cir_tmp[i] << endl;
// 	}
//    	for(unsigned j = 0; j < AIGinDFSorder.size(); j++)
// 	{
// 		ss << AIGinDFSorder[j]->get_Val();
// 		ss >> str;
// 		for(unsigned i = 0; i < andgate_tmp.size(); i += 3)
// 		{
// 			if(andgate_tmp[i] == str)
// 			{
// 				outfile << andgate_tmp[i] << " " 
// 						<< andgate_tmp[i + 1] << " "
// 						<< andgate_tmp[i + 2] << endl;
// 				break;
// 			}
// 		}
// 		ss.clear();
// 		ss.str("");
// 	}
// 	for(int i = _MILOA[1] + _MILOA[3] + _MILOA[4] + 1; i < cir_tmp.size(); i++)
// 	{
// 		if(i == cir_tmp.size())
// 			outfile << cir_tmp[i];
// 		else
// 			outfile << cir_tmp[i] << endl;
// 	}
// }
// void
// CirMgr::buildDFSOrder()
// {
// 	CirGate::setGlobalRef();
// 	if(_MILOA[3] == 0) { No_PO = true; return; }
// 	for(unsigned i = _MILOA[1] + 1; i < _MILOA[1] + _MILOA[3] + 1; i++)
// 		dfsTraversal_2(gates[i]);
// }
// void
// CirMgr::dfsTraversal_2(CirGate* dfsgate)
// {
// 	assert(dfsgate->get_type() != UNDEF_GATE);
// 	for(unsigned i = 0; i <  dfsgate->get_fin().size() ; i++)
// 	{
// 		if (!dfsgate->get_fin()[i]->isGlobalRef() && dfsgate->get_fin()[i]->get_type() != UNDEF_GATE)
// 			dfsTraversal_2(dfsgate->get_fin()[i]);
// 	}
// 	_dfsgate.push_back(dfsgate);
// 	dfsgate->setToGlobalRef();
// 	if(dfsgate->get_type() == AIG_GATE) { AIGinDFSorder.push_back(dfsgate); }
// }
/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
	switch (err) {
		case EXTRA_SPACE:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Extra space character is detected!!" << endl;
			break;
		case MISSING_SPACE:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Missing space character!!" << endl;
			break;
		case ILLEGAL_WSPACE: // for non-space white space character
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Illegal white space char(" << errInt
				<< ") is detected!!" << endl;
			break;
		case ILLEGAL_NUM:
			cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
				<< errMsg << "!!" << endl;
			break;
		case ILLEGAL_IDENTIFIER:
			cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
				<< errMsg << "\"!!" << endl;
			break;
		case ILLEGAL_SYMBOL_TYPE:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Illegal symbol type (" << errMsg << ")!!" << endl;
			break;
		case ILLEGAL_SYMBOL_NAME:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Symbolic name contains un-printable char(" << errInt
				<< ")!!" << endl;
			break;
		case MISSING_NUM:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Missing " << errMsg << "!!" << endl;
			break;
		case MISSING_IDENTIFIER:
			cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
				<< errMsg << "\"!!" << endl;
			break;
		case MISSING_NEWLINE:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": A new line is expected here!!" << endl;
			break;
		case MISSING_DEF:
			cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
				<< " definition!!" << endl;
			break;
		case CANNOT_INVERTED:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": " << errMsg << " " << errInt << "(" << errInt/2
				<< ") cannot be inverted!!" << endl;
			break;
		case MAX_LIT_ID:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
				<< endl;
			break;
		case REDEF_GATE:
			cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
				<< "\" is redefined, previously defined as "
				<< errGate->getTypeStr() << " in line " << errGate->getLineNo()
				<< "!!" << endl;
			break;
		case REDEF_SYMBOLIC_NAME:
			cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
				<< errMsg << errInt << "\" is redefined!!" << endl;
			break;
		case REDEF_CONST:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Cannot redefine constant (" << errInt << ")!!" << endl;
			break;
		case NUM_TOO_SMALL:
			cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
				<< " is too small (" << errInt << ")!!" << endl;
			break;
		case NUM_TOO_BIG:
			cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
				<< " is too big (" << errInt << ")!!" << endl;
			break;
		default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
CirGate* 
CirMgr::getGate(unsigned gid) const 
{
	for(unsigned i = 0; i < gates.size(); i++) 
	{
		if(gates[i] == NULL) { return 0; }
		else if(gid == gates[i]->get_Id()) { return gates[i]; }
	}
}
bool
CirMgr::readCircuit(const string& fileName)
{
   	char line[1000];
	char *pch;
	int count_l = 0;
	ifstream fin;
	fin.open(fileName);
	if(fin.is_open())
	{
		bool check_in = false;	
		while(fin.getline(line, sizeof(line), '\n'))
		{
			pch = strtok(line, " ");
			count_l ++;
			if(*pch == 'i0' || *pch == 'c' || *pch == 'o0') { break; }
			while(pch != NULL)
			{
				if(count_l == 1)
				{
					if(check_in == false){ check_in = true; }
					else
					{
						string value_1(pch);
						unsigned int value_2;
						istringstream temp(value_1);
						temp>>value_2;
						_MILOA.push_back(value_2);
					}
				}
				else if(count_l >= _MILOA[1] + _MILOA[3] + 2 && count_l < _MILOA[1] + _MILOA[3] +_MILOA[4] + 2)
				{
					string key(pch);
					andgate_tmp.push_back(key);
				}
				else if(count_l >= _MILOA[1] + _MILOA[3] +_MILOA[4] + 2){ break; }
				else
				{
					string key(pch);
					gate_tmp.push_back(key);
				}
				pch = strtok(NULL, " ");
			}
		}
		fin.close();

		for(unsigned i = 0; i < _MILOA[0] + _MILOA[3] + 1; ++i) 
			gates.push_back(NULL);

		gates[0]= new CirGate(GateType::CONST_GATE, 0, 0, 0);
		for(unsigned i = 1; i < _MILOA[1] + 1; i++)
		{
			unsigned int id_tmp;
			istringstream temp(gate_tmp[i - 1]);
			temp>>id_tmp;
			if(id_tmp % 2 == 0)
			{
				gates[i] = new CirGate(GateType::PI_GATE, id_tmp / 2, i + 1, id_tmp);
			}
			else
			{
				gates[i] = new CirGate(GateType::PI_GATE, (id_tmp - 1)/2, i + 1, id_tmp);
				gates[i]->set_invert();
			}
		}
		for(unsigned i = _MILOA[1] + 1; i < _MILOA[1] + _MILOA[3] + 1; i++)
		{
			unsigned id_tmp;
			istringstream temp(gate_tmp[i - 1]);
			temp>>id_tmp;
			if(id_tmp % 2 == 0)
			{
				gates[i] = new CirGate(GateType::PO_GATE, _MILOA[0] + i - (_MILOA[1]), i + 1, id_tmp);
			}
			else
			{
				gates[i] = new CirGate(GateType::PO_GATE, _MILOA[0] + i - (_MILOA[1]), i + 1, id_tmp);
				gates[i]->set_invert();
			}
		}
		for(unsigned i = _MILOA[1] + _MILOA[3] + 1; i < _MILOA[1] + _MILOA[3] +_MILOA[4] + 1; i++)
		{
			unsigned id_tmp;
			istringstream temp(andgate_tmp[(i - (_MILOA[1] + _MILOA[3] + 1)) * 3]);
			temp>>id_tmp;
			if(id_tmp % 2 == 0)
			{
				gates[i] = new CirGate(GateType::AIG_GATE, id_tmp / 2, i + 1, id_tmp);
			}
			else
			{
				gates[i] = new CirGate(GateType::AIG_GATE, (id_tmp - 1) / 2, i + 1, id_tmp);
				gates[i]->set_invert();
			}
		}
		ifstream fin2(fileName, ios::in);
		string line_2;
		int count_l2 = 0;
		stringstream ss;
		if(fin2.is_open())
		{
			while(getline(fin2,line_2))
			{
				count_l2 ++;
				cir_tmp.push_back(line_2);
				if(count_l2 >= _MILOA[1] + _MILOA[3] + _MILOA[4] + 2)
				{
					char tmp[1000];
					char dest[1000];
					string str;
					unsigned num;
					ss << line_2;
					ss >> tmp;
					ss.clear();
					ss.str("");
					strncpy(dest, tmp + 1, strlen(tmp));
					ss << dest;
					ss >> num;
					if(tmp[0] == 'i')
					{
						gates[num + 1]->set_Symbol(line_2.substr(strlen(tmp) + 1,line_2.length() - 3));
					}
					else if(tmp[0] == 'o')
					{
						gates[_MILOA[1] + num + 1]->set_Symbol(line_2.substr(strlen(tmp) + 1,line_2.length() - 3));
					}
					else{ break; }
					ss.clear();
					ss.str("");
				}
			}
		}
		fin2.close();
		/*************construct AIG_netlist*************/
		for(unsigned i = 0; i < _MILOA[4]; i++)
		{
			unsigned count = i * 3;
			unsigned count_undef = 0;
			unsigned id_tmp;
			istringstream temp(andgate_tmp[count + 1]);
			temp>>id_tmp;
			if(id_tmp % 2 == 0)
			{
				id_tmp = id_tmp / 2;
				for(unsigned j = 0; j < _MILOA[1] + _MILOA[3] + _MILOA[4] + 1; j++)
				{
					if(id_tmp == gates[j]->get_Id() && gates[j] != NULL)
					{
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j]);
						gates[j]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
						break;
					}
					else if(j == _MILOA[1] + _MILOA[3] + _MILOA[4])
					{
						count_undef ++;
						gates[j + count_undef] = new CirGate(GateType::UNDEF_GATE, id_tmp, 0, id_tmp*2);
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j + count_undef]);
						gates[j + count_undef]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
						break;
					}
				}
			}
			else
			{
				id_tmp = (id_tmp - 1) / 2;
				for(unsigned j = 0; j < _MILOA[1] + _MILOA[3] + _MILOA[4] + 1; j++)
				{
					if(id_tmp == gates[j]->get_Id() && gates[j] != NULL)
					{
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j]);
						gates[j]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_invert1().push_back(id_tmp);
						gates[j]->get_invert2().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]->get_Id());
						break;
					}
					else if(j == _MILOA[1] + _MILOA[3] + _MILOA[4])
					{
						count_undef ++;
						gates[j + count_undef] = new CirGate(GateType::UNDEF_GATE, id_tmp, 0, (id_tmp*2) + 1);
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j + count_undef]);
						gates[j + count_undef]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_invert1().push_back(id_tmp);
						gates[j + count_undef]->get_invert2().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]->get_Id());
						break;
					}
				}
			}
			unsigned id_tmp2;
			istringstream temp2(andgate_tmp[count + 2]);
			temp2>>id_tmp2;
			if(id_tmp2 % 2 == 0)
			{
				id_tmp2 = id_tmp2 / 2;
				for(unsigned j = 0; j < _MILOA[1] + _MILOA[3] + _MILOA[4] + 1; j++)
				{
					if(id_tmp2 == gates[j]->get_Id() && gates[j] != NULL)
					{
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j]);
						gates[j]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
						break;
					}
					else if(j == _MILOA[1] + _MILOA[3] + _MILOA[4])
					{
						count_undef ++;
						gates[j + count_undef] = new CirGate(GateType::UNDEF_GATE, id_tmp2, 0, id_tmp2*2);
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j + count_undef]);
						gates[j + count_undef]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);
						break;
					}
				}
			}
			else
			{
				id_tmp2 = (id_tmp2 - 1) / 2;
				for(unsigned j = 0; j < _MILOA[1] + _MILOA[3] + _MILOA[4] + 1; j++)
				{
					if(id_tmp2 == gates[j]->get_Id() && gates[j] != NULL)
					{
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j]);
						gates[j]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]);						
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_invert1().push_back(id_tmp2);
						gates[j]->get_invert2().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]->get_Id());
						break;
					}
					else if(j == _MILOA[1] + _MILOA[3] + _MILOA[4])
					{
						count_undef ++;
						gates[j + count_undef] = new CirGate(GateType::UNDEF_GATE, id_tmp2, 0, (id_tmp2*2) + 1);
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_fin().push_back(gates[j + count_undef]);
						gates[j + count_undef]->get_fout().push_back(gates[_MILOA[1] + _MILOA[3] + i + count_undef]);
						gates[_MILOA[1] + _MILOA[3] + i + 1]->get_invert1().push_back(id_tmp2);
						gates[j + count_undef]->get_invert2().push_back(gates[_MILOA[1] + _MILOA[3] + i + 1]->get_Id());
						break;
					}
				}
			}
		}
		/*************construct PO_netlist*************/
		unsigned countundef_2;
		for(unsigned i = 0; i < _MILOA[3]; i++)
		{
			bool check_1 = false;
			bool check_2 = false;
			for(unsigned j = 0; j < _MILOA[4]; j++) // for AIG
			{
				unsigned id_tmp = (gates[_MILOA[1] + 1 + i]->get_Val()%2 == 0) ? (gates[_MILOA[1] + 1 + i]->get_Val())/2 : (gates[_MILOA[1] + 1 + i]->get_Val()-1)/2;
				if(id_tmp == gates[_MILOA[1] + _MILOA[3] + 1 + j]->get_Id())
				{
					gates[_MILOA[1] + 1 + i]->get_fin().push_back(gates[_MILOA[1] + _MILOA[3] + 1 + j]);
					gates[_MILOA[1] + _MILOA[3] + 1 + j]->get_fout().push_back(gates[_MILOA[1] + 1 + i]);
					if(gates[_MILOA[1] + 1 + i]->get_Val() % 2 != 0)
					{
						gates[_MILOA[1] + 1 + i]->get_invert1().push_back(id_tmp);
						gates[_MILOA[1] + _MILOA[3] + 1 + j]->get_invert2().push_back(gates[_MILOA[1] + 1 + i]->get_Id());
					}
					check_1 = true;
				}
			}
			for(unsigned m = 0; m < _MILOA[1] + 1; m++) // for PI
			{
				unsigned id_tmp = (gates[_MILOA[1] + 1 + i]->get_Val()%2 == 0) ? (gates[_MILOA[1] + 1 + i]->get_Val())/2 : (gates[_MILOA[1] + 1 + i]->get_Val()-1)/2;
				if(id_tmp == gates[m]->get_Id())
				{
					gates[_MILOA[1] + 1 + i]->get_fin().push_back(gates[m]);
					gates[m]->get_fout().push_back(gates[_MILOA[1] + 1 + i]);
					if(gates[_MILOA[1] + 1 + i]->get_Val() % 2 != 0)
					{
						gates[_MILOA[1] + 1 + i]->get_invert1().push_back(id_tmp);
						gates[m]->get_invert2().push_back(gates[_MILOA[1] + 1 + i]->get_Id());
					}
					check_2 = true;
				}
			}
			if(!check_1 && !check_2)
			{
				countundef_2 ++;
				unsigned id_tmp3;
				if(gates[_MILOA[1] + 1 + i]->get_Val()%2 != 0)
				{
					id_tmp3 = (gates[_MILOA[1] + 1 + i]->get_Val() - 1)/2;
					gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2] = new CirGate(GateType::UNDEF_GATE, id_tmp3, 0, (id_tmp3*2) + 1);
					gates[_MILOA[1] + 1 + i]->get_fin().push_back(gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2]);
					gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2]->get_fout().push_back(gates[_MILOA[1] + 1 + i]);
					gates[_MILOA[1] + 1 + i]->get_invert1().push_back(id_tmp3);
					gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2]->get_invert2().push_back(gates[_MILOA[1] + 1 + i]->get_Id());
				}
				else
				{
					id_tmp3 = gates[_MILOA[1] + 1 + i]->get_Val()/2;
					gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2] = new CirGate(GateType::UNDEF_GATE, id_tmp3, 0, id_tmp3*2);
					gates[_MILOA[1] + 1 + i]->get_fin().push_back(gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2]);
					gates[_MILOA[1] + _MILOA[3] + _MILOA[4] + countundef_2]->get_fout().push_back(gates[_MILOA[1] + 1 + i]);
				}
			}
		}
		buildDFSOrder();
		return true;
	}
	else
	{
		cerr << "Cannot open design \""  << fileName<< " \"!!" << endl; 
		return false;
	}
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
	cout << "\nCircuit Statistics\n"
		 << "==================\n"
		 << "  PI" << right << setw(12) << _MILOA[1] << "\n"
		 << "  PO" << right << setw(12) << _MILOA[3] << "\n"
		 << "  AIG" << right << setw(11) << _MILOA[4] << "\n"
		 << "------------------\n"
		 << "  Total" << right << setw(9) <<  _MILOA[1] + _MILOA[3] + _MILOA[4] << endl;
}

void
CirMgr::printNetlist() const
{
	if(No_PO == true)
		cout << "\n";
	else
	{
		cout << "\n";
		for(unsigned i = 0; i < _dfsgate.size(); i++)
		{
			cout << "[" << i << "] ";
			_dfsgate[i]->printGate();
			cout << endl;
		}
	}
}

void
CirMgr::printPIs() const
{
	cout << "PIs of the circuit:";
	for(unsigned i = 1; i < _MILOA[1] + 1; i++)
		cout << " " << gates[i]->get_Id();
	cout << endl;
}

void
CirMgr::printPOs() const
{
	cout << "POs of the circuit:";
	for(unsigned i = 1 + _MILOA[1]; i < _MILOA[1] +_MILOA[3] + 1; i++)
		cout << " " << gates[i]->get_Id();
	cout << endl;
}

void
CirMgr::printFloatGates() const
{
	/*****************Check floating*****************/
	vector<unsigned int> floattmp;
	bool check_inf = false;
	for(unsigned i = 0; i < gates.size(); i++)
	{
		if(gates[i] == NULL) { break; }
		if(gates[i]->get_type() == UNDEF_GATE && gates[i]->get_fout()[0]->get_choosefl() != true)
		{
			if(!check_inf) 
			{ 
				check_inf =true;
				cout << "Gates with floating fanin(s):";
			}
			floattmp.push_back(gates[i]->get_fout()[0]->get_Id());
			gates[i]->get_fout()[0]->set_choosefl();
			if(gates[i + 1] == NULL) { break; }
		}
	}
	sort(floattmp.begin(), floattmp.end());
	for(unsigned i = 0; i < floattmp.size(); i++)
		cout << " " << floattmp[i];
	if(check_inf) { cout << endl; }
	/*****************Check notused*****************/
	vector<unsigned int> nousedtmp;
	bool check_nou = false;
	for(unsigned i = 0; i < gates.size(); i++)
	{
		if(gates[i] == NULL) { break; }
		if(gates[i]->get_fout().empty())
		{
			if(gates[i]->get_type() == PO_GATE || gates[i]->get_type() == CONST_GATE) {}
			else
			{
				if(!check_nou) 
				{ 
					check_nou =true;
					cout << "Gates defined but not used  :";
				}
				nousedtmp.push_back(gates[i]->get_Id());	
			}
		}
		if(gates[i]->get_type() == UNDEF_GATE)
			if(gates[i + 1] == NULL) { break; }
	}
	sort(nousedtmp.begin(), nousedtmp.end());
	for(unsigned i = 0; i < nousedtmp.size(); i++)
		cout << " " << nousedtmp[i];
	if(check_nou == true)
		cout << endl;
}

void
CirMgr::writeAag(ostream& outfile) const
{
	stringstream ss;
	string str;
	
	for(int i = 0; i < _MILOA[1] + _MILOA[3] + 1; i++)
	{
		outfile << cir_tmp[i] << endl;
	}
   	for(unsigned j = 0; j < AIGinDFSorder.size(); j++)
	{
		ss << AIGinDFSorder[j]->get_Val();
		ss >> str;
		for(unsigned i = 0; i < andgate_tmp.size(); i += 3)
		{
			if(andgate_tmp[i] == str)
			{
				outfile << andgate_tmp[i] << " " 
						<< andgate_tmp[i + 1] << " "
						<< andgate_tmp[i + 2] << endl;
				break;
			}
		}
		ss.clear();
		ss.str("");
	}
	for(int i = _MILOA[1] + _MILOA[3] + _MILOA[4] + 1; i < cir_tmp.size(); i++)
	{
		if(i == cir_tmp.size())
			outfile << cir_tmp[i];
		else
			outfile << cir_tmp[i] << endl;
	}
}
void
CirMgr::buildDFSOrder()
{
	CirGate::setGlobalRef();
	if(_MILOA[3] == 0) { No_PO = true; return; }
	for(unsigned i = _MILOA[1] + 1; i < _MILOA[1] + _MILOA[3] + 1; i++)
		dfsTraversal_2(gates[i]);
}
void
CirMgr::dfsTraversal_2(CirGate* dfsgate)
{
	assert(dfsgate->get_type() != UNDEF_GATE);
	for(unsigned i = 0; i <  dfsgate->get_fin().size() ; i++)
	{
		if (!dfsgate->get_fin()[i]->isGlobalRef() && dfsgate->get_fin()[i]->get_type() != UNDEF_GATE)
			dfsTraversal_2(dfsgate->get_fin()[i]);
	}
	_dfsgate.push_back(dfsgate);
	dfsgate->setToGlobalRef();
	if(dfsgate->get_type() == AIG_GATE) { AIGinDFSorder.push_back(dfsgate); }
}