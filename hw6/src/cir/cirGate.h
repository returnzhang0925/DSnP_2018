/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include "cirMgr.h"

using namespace std;

class CirGate;
static unsigned _globalRef;
//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{

public:
	CirGate(GateType _type, unsigned int i, unsigned int l, unsigned int v):
		_gatetype(_type), _id(i), _line(l), _val(v), _symbol(""), _ref(0) {}
	~CirGate() {}
	// virtual ~CirGate() {}

	// Basic access methods
	string getTypeStr() const { return ""; }
	unsigned getLineNo() const { return 0; }

	// Printing functions
	void printGate() const;
	// virtual void printGate() const = 0;
	void reportGate() const;
	void reportFanin(int level);
	void reportFanout(int level);

	GateList& get_fin() { return fanin; }
	GateList& get_fout(){ return fanout; }
	void set_Id(unsigned int i) { _id = i; }
	unsigned int get_Id()const { return _id; }
	void set_Line(unsigned int l) { _line = l; }
	unsigned int get_Line() { return _line; }
	void set_Symbol(string s) { _symbol = s; }
	vector<unsigned>& get_invert1(){ return fin_invert; }
	vector<unsigned>& get_invert2(){ return fou_invert; }
	unsigned int get_Val() { return _val; }
	string get_Symbol() { return _symbol; }
	GateType get_type() { return _gatetype; }
	void set_invert() { _isinvert = true; }
	void set_choosefl() { _choosefl = true; }
	bool get_choosefl() { return _choosefl;}

	void FaninTraversal(CirGate*, int, int);
	void FanoutTraversal(CirGate*, int, int);

	bool isGlobalRef(){ return (_ref == _globalRef); }
	void setToGlobalRef(){ _ref = _globalRef; }
	static void setGlobalRef() { _globalRef++; }

	void get_GateType(GateType);

private:

protected:
	unsigned int _id, _line, _val;
	GateList fanin, fanout;
	GateType _gatetype;
	bool _isinvert;
	vector<unsigned> fin_invert;
	vector<unsigned> fou_invert;
	bool _choosefl;
	string _symbol;

	unsigned _ref; 
};

#endif // CIR_GATE_H
