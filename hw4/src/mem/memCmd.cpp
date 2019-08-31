/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"
#include <string.h>
#include <sstream>
#include <vector>

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
	vector<string> options;
	if (!CmdExec::lexOptions(option, options))
		return CMD_EXEC_ERROR;

	if (options.empty())
		return CmdExec::errorOption(CMD_OPT_MISSING, "");

	bool doArray = false;
	size_t num;
	int numObject;
	int arraySize;

	if(myStrNCmp("-Array", options[0], 2) == 0)
	{
		if(options.size() < 3)
		{
			return CmdExec::errorOption(CMD_OPT_MISSING, "");
		}
		else if(myStr2Int(options[1], arraySize) == false)
		{
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
		}
		else if(arraySize <= 0)
		{
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
		}
		else if(myStr2Int(options[2], numObject) == false)
		{
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
		}
		else if(numObject <= 0)
		{
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
		}
		else if(options.size() > 3)
		{
			return CmdExec::errorOption(CMD_OPT_EXTRA, options[3]);
		}
		try
		{
			size_t numObject_2 = numObject;
			size_t arraySize_2 = arraySize;
			mtest.newArrs(numObject_2, arraySize_2);
		}
		catch(bad_alloc)
		{
			return CMD_EXEC_ERROR;
		}
		return CMD_EXEC_DONE;
	}
	else
	{
		if(myStr2Int(options[0], numObject) == false)
		{
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
		}
		else if(options.size() == 1)
		{
			if(numObject <= 0)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
			}
			try
			{
				size_t numObject_2 = numObject;
				mtest.newObjs(numObject_2);
			}
			catch(bad_alloc)
			{
				return CMD_EXEC_ERROR;
			}
			return CMD_EXEC_DONE;
		}
		else if(options.size() == 2)
		{
			if(myStrNCmp("-Array", options[1], 2) == 0)
			{
				return CmdExec::errorOption(CMD_OPT_MISSING, options[1]);
			}
			else
			{
				return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);
			}
		}
		else if(options.size() == 3)
		{
			for(size_t i = 0; i < options.size(); i++)
			{
				if(myStrNCmp("-Array", options[i], 2) == 0)
				{
					if(doArray == true)
					{
						return CmdExec::errorOption(CMD_OPT_EXTRA, options[i]);
					}
					doArray = true;
				}
			}
			if(myStr2Int(options[0], numObject) == false)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
			}
			else if(numObject <= 0)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
			}
			else if(myStr2Int(options[2], arraySize) == false)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
			}
			else if(arraySize <= 0)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
			}
			try
			{
				size_t numObject_2 = numObject;
				size_t arraySize_2 = arraySize;
				mtest.newArrs(numObject_2, arraySize_2);
			}
			catch(bad_alloc)
			{
				return CMD_EXEC_ERROR;
			}
			return CMD_EXEC_DONE;
		}
		else
		{
			if(myStr2Int(options[0], numObject) == false)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
			}
			else if(numObject <= 0)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
			}
			else if(myStrNCmp("-Array", options[1], 2) != 0)
			{
				return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);
			}
			else if(myStr2Int(options[2], arraySize) == false)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
			}
			else if(arraySize <= 0)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
			}
			else
			{
				return CmdExec::errorOption(CMD_OPT_EXTRA, options[3]);
			}
		}
	}
}
void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
	vector<string> options;
	if (!CmdExec::lexOptions(option, options))
		return CMD_EXEC_ERROR;

	if (options.empty())
		return CmdExec::errorOption(CMD_OPT_MISSING, "");

	bool flag = false;
	int num = 0;
	int objId;
	int numRandId;
	
	if(myStrNCmp("-Array", options[0], 2) == 0)
	{
		if(options.size() == 1)
		{
			return CmdExec::errorOption(CMD_OPT_MISSING, "");
		}
		else if(options.size() == 2)
		{
			if(myStrNCmp("-Index", options[1], 2) == 0 || myStrNCmp("-Random", options[1], 2) == 0)
			{
				return CmdExec::errorOption(CMD_OPT_MISSING, options[1]);
			}
			else
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
			}
		}
		else if(options.size() == 3)
		{
			if(myStrNCmp("-Index", options[1], 2) != 0 && myStrNCmp("-Random", options[1], 2) != 0)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
			}
			else if(myStrNCmp("-Index", options[1], 2) == 0)
			{
				if(myStr2Int(options[2], objId) == false)
				{
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
				}
				size_t objId_2 = objId;
				if(objId < 0)
				{
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
				}
				else if(objId_2 >= mtest.getArrListSize())
				{
					cerr << "Size of array list (" << mtest.getArrListSize() << ") is <= " << objId_2 << "!!" << endl;
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
				}
				mtest.deleteArr(objId_2);
				return CMD_EXEC_DONE;
			}
			else if(myStrNCmp("-Random", options[1], 2) == 0)
			{
				if(myStr2Int(options[2], numRandId) == false)
				{
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
				}
				size_t numRandId_2 = numRandId;
				if(numRandId <= 0)
				{
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
				}
				else if(mtest.getArrListSize() == 0)
				{
					cerr << "Size of array list is 0!!" << endl;
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
				}
				for(int i = 0; i < numRandId; i++)
				mtest.deleteArr(rnGen(mtest.getArrListSize()));
				return CMD_EXEC_DONE;				
			}
			else
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
			}
		}
		else
		{
			if(myStrNCmp("-Index", options[1], 2) != 0 && myStrNCmp("-Random", options[1], 2) != 0)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
			}
			else
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
			}		
		}
	}

	for (size_t i = 0, n = options.size(); i < n; ++i)
	{
		if (myStrNCmp("-Index", options[i], 2) == 0)
		{
			if (flag) 
			{
				if(i == 1)
				{
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
				}
				return CmdExec::errorOption(CMD_OPT_EXTRA, options[i]);
			}
			flag = true;
		}
		else if(myStrNCmp("-Random", options[i], 2) == 0)
		{
			if (flag) 
			{
				if(i == 1)
				{
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
				}
				return CmdExec::errorOption(CMD_OPT_EXTRA, options[i]);
			}
			flag = true;
		}
		else if(myStrNCmp("-Index", options[0], 2) != 0 && myStrNCmp("-Random", options[0], 2) != 0)
		{
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
		}
	}
	if (myStrNCmp("-Index", options[0], 2) == 0)
	{
		if(options.size() == 1)
		{
			return CmdExec::errorOption(CMD_OPT_MISSING, options[0]);
		}
		else if(!myStr2Int(options[1], num))
		{
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
		}
		else if(myStr2Int(options[1], num))
		{
			size_t val;
			istringstream is(options[1]);
			is>>val;
			if(val < 0)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
			}
			else if(options.size() > 2)
			{
				if(myStrNCmp("-Array", options[2], 2) == 0)
				{
					if(options.size() > 3)
					{
						return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[3]);
					}
					else if(val >= mtest.getArrListSize())
					{
						cerr << "Size of array list (" << mtest.getArrListSize() << ") is <= " << val << "!!" << endl;
						return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
					}
					mtest.deleteArr(val);
				}
				else
				{
					return CmdExec::errorOption(CMD_OPT_EXTRA, options[2]);
				}
			}
			else if(options.size() == 2)
			{
				if(val >= mtest.getObjListSize())
				{
					cerr << "Size of object list (" << mtest.getObjListSize() << ") is <= " << val << "!!" << endl;
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
				}
				mtest.deleteObj(val);
				
			}
		}
	}
	else if(myStrNCmp("-Random", options[0], 2) == 0)
	{
		if(options.size() == 1)
		{
			return CmdExec::errorOption(CMD_OPT_MISSING, options[0]);
		}
		size_t val;
		istringstream is(options[1]);
		is>>val;
		if(!myStr2Int(options[1], num))
		{
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
		}
		else if(myStr2Int(options[1], num))
		{
			if(num < 0)
			{
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
			}
			else if(options.size() > 2)
			{
				if(myStrNCmp("-Array", options[2], 2) == 0)
				{
					if(options.size() > 3)
					{
						return CmdExec::errorOption(CMD_OPT_EXTRA, options[3]);
					}
					else if(mtest.getArrListSize() == 0)
					{
						cerr << "Size of array list is 0!!" << endl;
						return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
					}
					for(int i = 0; i < val; i++)
					mtest.deleteArr(rnGen(mtest.getArrListSize()));
				}
				else
				{
					return CmdExec::errorOption(CMD_OPT_EXTRA, options[2]);
				}
			}
			else if(options.size() == 2)
			{
				if(mtest.getObjListSize() == 0)
				{
					cerr << "Size of object list is 0!!" << endl;
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
				}
				for(int i = 0; i < val; i++)
				mtest.deleteObj(rnGen(mtest.getObjListSize()));
			}
		}
	}
	return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


