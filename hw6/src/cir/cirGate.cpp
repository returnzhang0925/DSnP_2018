/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
	stringstream secondline;
	cout << "==================================================\n";
	secondline << "= ";
	if(this->_gatetype == CONST_GATE)
	{
		secondline << "CONST(0), line 0";
	}
	else if(this->_gatetype == PI_GATE)
	{
		secondline << "PI(" << this->_id << ")";
		if(this->_symbol != "")
		{
			secondline << "\"" << this->_symbol << "\"";
		}
		secondline << ", line " << this->_line;
	}
	else if(this->_gatetype == PO_GATE)
	{
		secondline << "PO(" << this->_id << ")";
		if(this->_symbol != "")
		{
			secondline << "\"" << this->_symbol << "\"";
		}
		secondline << ", line " << this->_line;
	}
	else if(this->_gatetype == AIG_GATE)
		secondline << "AIG(" << this->_id << "), line " << this->_line;
	else if(this->_gatetype == UNDEF_GATE)
		secondline << "UNDEF(" << this->_id << "), line " << this->_line;

	int len = secondline.str().size();
	if(len < 49)
		cout << secondline.str() << string(49 - len, ' ') << "=\n";
	else
		cout << secondline.str() << " =\n";
	cout << "==================================================\n";
}

void
CirGate::reportFanin(int level)
{
   	assert(level >= 0);
	setGlobalRef();
	FaninTraversal(this, 0, level);
}

void
CirGate::reportFanout(int level)
{
  	assert(level >= 0);
	setGlobalRef();
	FanoutTraversal(this, 0, level);
}
void 
CirGate::FaninTraversal(CirGate* dofingate, int start, int level)
{
	assert(start <= level);
	bool mark_1 = false;
	for(int i = 0; i < start; i ++)
		cout << "  ";
	for(unsigned i = 0; i < dofingate->fanout.size(); i++)
	{
		if(!dofingate->fou_invert.empty())
		{
			for(unsigned j = 0; j < dofingate->fou_invert.size(); j++)
			{
				if(dofingate->fou_invert[j] == dofingate->fanout[i]->get_Id())
					{cout << "!"; mark_1 = true; break; }
			}
		}
		if(mark_1 == true) { break; }
	}
	dofingate->get_GateType(dofingate->get_type());
	cout << dofingate->get_Id();

	if(start == level) { cout << endl; return; }

	if(dofingate->isGlobalRef())
		cout << " (*)" << endl;
	else
	{
		cout << "\n";
		for(unsigned i = 0; i <  dofingate->get_fin().size(); i++)
		{
			if (!dofingate->get_fin()[i]->isGlobalRef())
				FaninTraversal(dofingate->get_fin()[i], start + 1, level);
		}
	}
	dofingate->setToGlobalRef();
}
void 
CirGate::FanoutTraversal(CirGate* dofougate, int start, int level)
{
	assert(start <= level);
	for(int i = 0; i < start; i ++)
		cout << "  ";
	bool mark_2 = false;
	for(unsigned i = 0; i < dofougate->fanout.size(); i++)
	{
		if(!dofougate->fou_invert.empty())
		{
			for(unsigned j = 0; j < dofougate->fou_invert.size(); j++)
			{
				if(dofougate->fou_invert[j] == dofougate->fanout[i]->get_Id())
					{cout << "!"; mark_2 = true; break; }
			}
		}
		if(mark_2 == true) { break; }
	}
	dofougate->get_GateType(dofougate->get_type());
	cout << dofougate->get_Id();

	if(start == level) { cout << endl; return; }

	if(dofougate->isGlobalRef())
		cout << " (*)" << endl;
	else
	{
		cout << "\n";
		for(unsigned i = 0; i < dofougate->get_fout().size(); i++)
		{
			if (!dofougate->get_fout()[i]->isGlobalRef())
				FaninTraversal(dofougate->get_fout()[i], start + 1, level);
		}
	}
	dofougate->setToGlobalRef();
}
void
CirGate::printGate() const
{
	if(this->_gatetype == CONST_GATE)
	{
		cout << "CONST0";
		return;
	}
	switch(this->_gatetype)
	{
		case PI_GATE: 
			cout << "PI  ";
			break;
        case PO_GATE: 
            cout << "PO  ";
            break; 
        case AIG_GATE: 
            cout << "AIG ";
            break; 
	}
	cout << this->_id;
	for(unsigned i = 0; i < this->fanin.size(); i++)
	{
		cout << " ";
		if(this->fanin[i]->get_type() == UNDEF_GATE)
			cout << "*";
		if(!this->fin_invert.empty())
		{
			for(unsigned j = 0; j < this->fanin.size(); j++)
			{
				if(this->fin_invert[j] == this->fanin[i]->get_Id())
					{cout << "!"; break;}
			}
		}
		cout << this->fanin[i]->get_Id();
	}
	if(this->_symbol != "")
		cout << " (" << this->_symbol << ")";
}
void
CirGate::get_GateType(GateType t)
{
	switch(t)
	{
		case UNDEF_GATE:
			cout << "UNDEF ";
			break;
		case PI_GATE:
			cout << "PI ";
			break;
		case PO_GATE:
			cout << "PO ";
			break;
		case AIG_GATE:
			cout << "AIG ";
			break;
		case CONST_GATE:
			cout << "CONST ";
			break;
	}
}
