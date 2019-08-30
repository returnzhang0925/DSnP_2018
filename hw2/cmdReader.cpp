/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"
#include <string>

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar getChar(istream&);
int count = 0;

string& trim(string &s) 
{
    if (s.empty()) 
    {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
	if (_dofile.is_open()) {
		readCmdInt(_dofile);
		_dofile.close();
	}
	else
		readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
	resetBufAndPrintPrompt();

	while (1) {
		ParseChar pch = getChar(istr);
		if (pch == INPUT_END_KEY) break;
		switch (pch) {
			case LINE_BEGIN_KEY :
			case HOME_KEY       : moveBufPtr(_readBuf); break;
			case LINE_END_KEY   :
			case END_KEY        : moveBufPtr(_readBufEnd); break;
			case BACK_SPACE_KEY : moveBufPtr(_readBufPtr-1);
								  deleteChar();
								  break;
			case DELETE_KEY     : deleteChar();break;
			case NEWLINE_KEY    : 
								  count = 0;
								  addHistory();
								  cout << char(NEWLINE_KEY);
								  resetBufAndPrintPrompt();
								  break;
			case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
			case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
			case ARROW_RIGHT_KEY: moveBufPtr(_readBufPtr+1); break;
			case ARROW_LEFT_KEY : moveBufPtr(_readBufPtr-1); break;
			case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
			case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
			case TAB_KEY        : insertChar(char(' '),8); break;
			case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
			case UNDEFINED_KEY  : mybeep(); break;
			default:  // printable character
				insertChar(char(pch)); break;
		}
		#ifdef TA_KB_SETTING
		taTestOnly();
		#endif
	}
}


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
	if(ptr >= _readBuf)
	{
		if((_readBufPtr - ptr) > 0) 
		{	
			int temp = _readBufPtr-ptr;

			for(int i = 0; i < temp; i++)
			{
				cout << '\b';
				_readBufPtr--;
			}
			return true;
		}
		else if((_readBufPtr - ptr) < 0 && _readBufPtr < _readBufEnd)	
		{
			int temp_2 = ptr - _readBufPtr;

			for(int i = 0; i < temp_2; i++)
			{
				cout << *_readBufPtr;
				_readBufPtr++;
			}
			return true;
		}
		else
		{
			mybeep();
			return false;
		}
	}
	else
	{
		mybeep();
		return false;
	}
}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
	if(_readBufEnd == _readBuf || _readBufPtr == _readBufEnd)
	{
		mybeep();
		return false;
	}
	else
	{
   		*_readBufPtr = 0;
		for(int i = 0; i < _readBufEnd - _readBufPtr; i++)
		{
			*(_readBufPtr + i)= *(_readBufPtr + i + 1);
		}
		for(int n = 0; n < _readBufEnd -_readBufPtr; n++)
		{
			if(n == _readBufEnd -_readBufPtr - 1)
			{
				cout << " ";
			}
			else
			{
				cout << *(_readBufPtr + n);
			}
		}
		for(int m = 0; m < _readBufEnd -_readBufPtr; m++)
		{
			cout << '\b';
		}
		_readBufEnd --;
		return true;
	}
}

// 1. Insert character 'ch' for "repeat" times at _readBufPtr
// 2. Move the remaining string right for "repeat" characters
// 3. The cursor should move right for "repeats" positions afterwards
// 4. Default value for "repeat" is 1. You should assert that (repeat >= 1).
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k', 3) ---
//
// cmd> This is kkkthe command
//                 ^


void
CmdParser::insertChar(char ch, int repeat)
{	
	assert(repeat >= 1);
	if(_readBufPtr >= _readBuf && _readBufPtr < _readBufEnd)
	{
		_readBufEnd += repeat;
		for(int i = _readBufEnd - _readBufPtr; i > 0; i--)
		{
			*(_readBufPtr + i) = *(_readBufPtr + i - repeat);
		}
		for(int j = 0; j < repeat; j++)
		{
			*_readBufPtr = ch;
			_readBufPtr++;
		}
		for(int n = 0; n < _readBufEnd -_readBufPtr + repeat; n++)
		{
			cout << *(_readBufPtr - repeat + n);
		}
		for(int m = 0; m < _readBufEnd -_readBufPtr ; m++)
		{
			cout << '\b';
		}
	}
	else
	{
		for(int i = 0; i < repeat; i++)
		{
			cout << ch;
			*_readBufPtr = ch;
			_readBufPtr++;
			_readBufEnd++;
		}
	}
}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
	if(_readBufPtr != _readBuf)
	{
		moveBufPtr(_readBuf);
		int temp_3 = _readBufEnd - _readBufPtr;
		for(int i = 0; i < temp_3; i++)
		{
			deleteChar();
		}
		_readBufEnd = _readBuf;
	}
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void
CmdParser::moveToHistory(int index)
{
	if(index < _historyIdx)
	{
		if(index < 0)
		{
			index = 0;
		}

		if(_historyIdx == _history.size())
		{
				count ++;
				_tempCmdStored = true;
				addHistory();
				_historyIdx = index;
				retrieveHistory();
		}
		else if(_historyIdx == 0)
		{
			mybeep();
		}
		else
		{
			_historyIdx = index;	
			retrieveHistory();	
		}
	}
	else if(index > _historyIdx)
	{
		int index_2 = 0;
		if(index > _history.size())
		{
			if(index == _history.size() + 1)
			{
				index = _history.size();
			}
			else if(_historyIdx == _history.size())
			{
				index = _history.size();
			}
			else if(index > _history.size() + 1)
			{
				index = _history.size() - 1;
				index_2 = _history.size();
			}
		}

		if(index == _history.size() || _historyIdx == _history.size() - 1)
		{
			mybeep();
		}
		else
		{
			if(index_2 == _history.size())
			{
				_historyIdx = index_2 - 1;
				retrieveHistory();
			}
			else
			{
				_historyIdx = index;
				retrieveHistory();
			}
		}
	}
}


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
//
void
CmdParser::addHistory()
{
	string temp;

	int counter = _readBufEnd - _readBuf;

	*_readBufEnd = 0;
	
	if(_tempCmdStored == true)
	{
		temp = _readBuf;
		if(count == 1)
		{
			_history.push_back(temp);
			_historyIdx = _history.size();
			for(int i = 0; i < counter; i++)
			{
				_readBuf[i] = '0';
			}
		}
		else
		{
			trim(temp);
			if(!temp.empty())
			{
				_history.pop_back();
				_history.push_back(temp);
				_historyIdx = _history.size();
				for(int i = 0; i < counter; i++)
				{
					_readBuf[i] = '0';
				}
				_tempCmdStored = false;
			}
			else
			{
				_history.pop_back();
				_historyIdx = _history.size();
				for(int i = 0; i < counter; i++)
				{
					_readBuf[i] = '0';
				}
				_tempCmdStored = false;
			}
		}
	}
	else
	{
		temp = _readBuf;
		trim(temp);
		if(!temp.empty())
		{
			_history.push_back(temp);
			_historyIdx = _history.size();
			for(int i = 0; i < counter; i++)
			{
				_readBuf[i] = '0';
			}
		}
	}
}


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
