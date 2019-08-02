/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data[0]; }
   void insert(const Data& d)
   { 
      size_t tmp = size();
      _data.push_back(d);
      while (tmp > 0) 
      {
         int p = (tmp - 1) / 2;
         if (d < _data[p])
         {
            _data[tmp] = _data[p];
            tmp = p;
         }
         else { break; }
      }
      _data[tmp] = d;
   }
   void delMin() { delData(0); }
   void delData(size_t i)
   { 
      size_t p = i + 1, t = 2 * p, n = _data.size();
      _data.pop_back();
      Data& d = _data[n - 1];
      if(i == n) return;
      while (t <= n) 
      {
         if (t < n)
            if (_data[t] < _data[t - 1])
               t++; 
         if (!(_data[t - 1] < _data[n - 1]))
            break;
         swap(_data[p - 1], _data[t - 1]);
         swap(p, t);
         t = 2 * p;
      }
      while(p > 1)
		{
         t = p / 2;
			if(d < _data[t - 1])
			{
            swap(_data[p - 1], _data[t - 1]);
            swap(p, t);
         }
         else break;
		}
		swap(_data[p - 1], d);
   }

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif  MY_MIN_HEAP_H