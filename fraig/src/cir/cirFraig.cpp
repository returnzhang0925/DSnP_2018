/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
HashKey
CirMgr::gatekey(CirGate * g)
{
	unsigned a = g->getFin1()->getId() * 2 + (g->getInv1() == 0);
	unsigned b = g->getFin2()->getId() * 2 + (g->getInv2() == 0);
	if(a < b) 
		return HashKey(a, b);
	else 	
		return HashKey(b, a);
}
/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
// _floatList may be changed.
// _unusedList and _undefList won't be changed
void
CirMgr::strash()
{
	HashMap<HashKey, CirGate *> strashMap(getHashSize(_dfsList.size()));
	vector<unsigned> remove_ID;

    for (unsigned i = 0; i < _dfsList.size(); i++)
	{
        if (_dfsList[i]->_typeID == 3)
		{
            HashKey key = gatekey(_dfsList[i]);
            CirGate* gate;
            if (strashMap.check(key, gate))
			{
				gate->mergeStrash(_dfsList[i]);
				cout << "Strashing: " << gate->_id << " merging " << _dfsList[i]->_id << "...\n";
                _dfsList[i]->removeFin();
				_dfsList[i]->removeFou();
                remove_ID.push_back(_dfsList[i]->_id);
            }
            else
                strashMap.insert(key, _dfsList[i]);
        }
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
	CirGate::setGlobalRef();
	_dfsList.resize(0); 
	dfsTraversal(_output);
}

void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
