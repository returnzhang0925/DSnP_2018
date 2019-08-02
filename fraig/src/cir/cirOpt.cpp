/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include <vector>
#include <algorithm>

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
void
CirMgr::sweep()
{
	vector<unsigned> remove_ID;
	vector<bool> removegate(_miloa[0] + _miloa[3] + 1, false);
	unsigned a = _miloa[0] + _miloa[3];
	while(a >= 0)
	{
		for (unsigned i = 0; i < _dfsList.size(); i++)
		{
			if(_gateList[a] != NULL)
			{
				if(_gateList[a]->_id == _dfsList[i]->_id )
				{
					removegate[a] = true;
					break;
				}
			}	
		}
		if(a != 0) a--;
		else break;
	}
	for(unsigned i = 1; i < _gateList.size(); ++i)
    {
        if(_gateList[i] != NULL)
        {
			if (_gateList[i]->_typeID != 1 && removegate[i] == false)
            	remove_ID.push_back(i);
		}
    }
	for (unsigned i = 0; i < remove_ID.size(); i++)
    {
        if (_gateList[remove_ID[i]]->_typeID == 3)
        {
			for (unsigned j = 0; j < _aig.size(); j++)
            {
                if (_gateList[_aig[j]]->_id == remove_ID[i])
                    _aig.erase(_aig.begin() + j);
            }
		}
    }
    _miloa[4] = _aig.size();
	sort(remove_ID.begin(), remove_ID.end());
	if(!remove_ID.empty())
	{
		for(int i = 1 + _miloa[1]; i < _gateList.size(); i++)
		{
			if(_gateList[i] != NULL)
			{
				for(int j = 0; j < remove_ID.size(); j++)
				{
					if(_gateList[i]->_id == remove_ID[j])
					{
						if(_gateList[i]->_typeID == 0) // for undef gate
							_gateList[i]->removeFou();
						else
						{
							_gateList[i]->removeFin();
							_gateList[i]->removeFou();
						}
						cout << "Sweeping: " << _gateList[i]->_typeStr << '(' 
							<< _gateList[i]->_id << ") removed..." << endl;
						_gateList[i] = NULL;
						break;
					}
				}
			}
		}
	}
}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
	vector<unsigned> remove_ID;
	bool isDel = false;
	CirGate::setGlobalRef();
	for (unsigned i = 0; i < _dfsList.size(); i++) 
	{
		if ((_dfsList[i]->_fin1 || _dfsList[i]->_fin2) && _dfsList[i]->_typeID != 2) 
		{
			if(_dfsList[i]->_typeID == 1 || _dfsList[i]->_typeID == 4 || _dfsList[i]->_typeID == 0) {}
			else if(_dfsList[i]->_fin1->_typeID == 4 && _dfsList[i]->_fin2->_typeID != 4)
			{
				if(_dfsList[i]->_inv1 == true)
				{
					if(_dfsList[i]->_fin2->_id == 1 && !_dfsList[i]->_inv2) // PI = !0 & 1
						_dfsList[i]->mergeIdentical(_gateList, 2);
					else if(_dfsList[i]->_fin2->_id == 1 && _dfsList[i]->_inv2) //for PI = !0 & !1
						_dfsList[i]->mergeConstant(3, false);
					else
						_dfsList[i]->mergeConstant(1, true); // constant 1
					isDel = true;
				}
				else if(_dfsList[i]->_inv1 == false)
				{
					_dfsList[i]->mergeConstant(1, false); // constant 0
					isDel = true;
				}
			}
			else if(_dfsList[i]->_fin2->_typeID == 4 && _dfsList[i]->_fin1->_typeID != 4)
			{
				if(_dfsList[i]->_inv2 == true)
				{
					if(_dfsList[i]->_fin1->_id == 1 && !_dfsList[i]->_inv1)
						_dfsList[i]->mergeIdentical(_gateList, 2);
					else if(_dfsList[i]->_fin1->_id == 1 && _dfsList[i]->_inv1) //for PI = !0 & !1
						_dfsList[i]->mergeConstant(1, false);
					else
						_dfsList[i]->mergeConstant(2, true); // constant 1
					isDel = true;
				}
				else if(_dfsList[i]->_inv2 == false)
				{
					_dfsList[i]->mergeConstant(2, false);  // constant 0
					isDel = true;
				}
			}
			else if(_dfsList[i]->_fin2 && _dfsList[i]->_fin1->_id == _dfsList[i]->_fin2->_id)
			{
				if(_dfsList[i]->_inv1 && _dfsList[i]->_inv2) //Replaced with the (fanin+phase)
				{
					_dfsList[i]->mergeIdentical(_gateList, 1);
					isDel = true;
				}
				else if(!_dfsList[i]->_inv1 && !_dfsList[i]->_inv2) //Replaced with the (fanin+phase)
				{
					_dfsList[i]->mergeIdentical(_gateList, 2);
					isDel = true;
				}
				else
				{
					_dfsList[i]->mergeIdentical(_gateList, 3); //Replaced with 0
					isDel = true;
				}
			}
		}
		if(isDel) { remove_ID.push_back(_dfsList[i]->_id); }
		isDel = false;
	}

	for (unsigned i = 0; i < remove_ID.size(); i++)
    {
        if (_gateList[remove_ID[i]]->_typeID == AIG_GATE)
		{
			for (unsigned j = 0; j < _aig.size(); j++)
            {
                if (_gateList[_aig[j]]->_id == remove_ID[i])
                    _aig.erase(_aig.begin() + j);
            }
		}
		_gateList[remove_ID[i]]->removeFin();
		_gateList[remove_ID[i]]->removeFou();
		_gateList[remove_ID[i]] = NULL;
    }
    _miloa[4] = _aig.size();
	_dfsList.resize(0); 
	dfsTraversal(_output);
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/