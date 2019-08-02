/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "cirGate.h"

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
	CirMgr(){}
	~CirMgr() 
	{
		if(gates.empty()){ return; }
		for(unsigned int i = 0;i < (_MILOA[1] + _MILOA[2] + _MILOA[4]); i++)
			delete gates[i];
	}

	// Access functions
	// return '0' if "gid" corresponds to an undefined gate.
	CirGate* getGate(unsigned gid) const ;

	// Member functions about circuit construction
	bool readCircuit(const string&);

	// Member functions about circuit reporting
	void printSummary() const;
	void printNetlist() const;
	void printPIs() const;
	void printPOs() const;
	void printFloatGates() const;
	void writeAag(ostream&) const;

	void builddfsorder() const;
	void buildDFSOrder();
	void dfsTraversal_2(CirGate*);

private:
	vector<unsigned int> _MILOA;
	GateList gates;
	vector<string> gate_tmp;
	vector<string> andgate_tmp;
	vector<string> cir_tmp;
	GateList _dfsgate;
	GateList AIGinDFSorder;
	bool No_PO = false;
};

#endif // CIR_MGR_H
