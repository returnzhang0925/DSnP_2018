/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ==" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
	HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
	~HashSet() { reset(); }

	// TODO: implement the HashSet<Data>::iterator
	// o An iterator should be able to go through all the valid Data
	//   in the Hash
	// o Functions to be implemented:
	//   - constructor(s), destructor
	//   - operator '*': return the HashNode
	//   - ++/--iterator, iterator++/--
	//   - operators '=', '==', !="
	//
	class iterator
	{
		friend class HashSet<Data>;

	public:
		iterator(vector<Data>* b, size_t id, size_t p, size_t n)
               : _id(id), _pos(p), _bucket(b), _numbkt(n)
			   { if(_id != _numbkt) _node = &_bucket[_id][_pos]; }
		iterator(const iterator& i) :
			_bucket(i._bucket), _id(i._id), _pos(i._pos), _node(i._node), _numbkt(i._numbkt) {}
		~iterator() {};

		const Data& operator * () const { return *_node; }
		Data& operator * () { return *_node; }
		iterator& operator ++ () 
		{
			if(_pos < _bucket[_id].size() - 1) { _pos++; }
			else
			{
				while(_id < _numbkt)
				{
					_id++;
					if(!_bucket[_id].empty()) { break; }
				}
				_pos = 0;
			}
			_node = &_bucket[_id][_pos];
			return (*this); 
		}
		iterator operator ++ (int) 
		{ 
			iterator tmp = *this;
			++(*this);
			return tmp; 
		}
		iterator& operator -- () 
		{
			if(_pos > 0) { _pos--; }
			else
			{
				while(_id > 0)
				{
					_id--;
					if(!(_bucket[_id].empty())) { break; }
				}
				_pos = _bucket[_id].size() - 1;
			}
			_node = &_bucket[_id][_pos];
			return (*this);  
		}
		iterator operator -- (int) 
		{ 
			iterator tmp = *this;
			--(*this);
			return tmp;  
		}
		iterator& operator = (const iterator& i) 
		{
			_numbkt = i._numbkt;
			_bucket = i._bucket; 
			_node = i._node;
			_id = i._id;
			_pos = i._pos;
			return *(this);  
		}

		bool operator != (const iterator& i) const { return !(i == (*this)); }
		bool operator == (const iterator& i) const { return (_id == i._id && _pos == i._pos); }
	private:
		vector<Data>* _bucket;
		Data* _node;
		size_t _id, _pos, _numbkt;
	};

	void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
	void reset() {
		_numBuckets = 0;
		if (_buckets) { delete [] _buckets; _buckets = 0; }
	}
	void clear() {
		for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
	}
	size_t numBuckets() const { return _numBuckets; }

	vector<Data>& operator [] (size_t i) { return _buckets[i]; }
	const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

	// TODO: implement these functions
	//
	// Point to the first valid data
	iterator begin() const 
	{
		if(size() == 0) return end();
		for(size_t i = 0; i < _numBuckets; i++)
		{
			if(!_buckets[i].empty())
				return iterator(_buckets, i, 0, _numBuckets);
		}
	}
	// Pass the end
	iterator end() const { return iterator(_buckets, _numBuckets, 0, _numBuckets); }
	// return true if no valid data
	bool empty() const { return (begin() == end()); }
	// number of valid data
	size_t size() const 
	{ 
		size_t s = 0; 
		for(size_t i = 0; i < _numBuckets; i++)
			s += _buckets[i].size();
		return s; 
	}

	// check if d is in the hash...
	// if yes, return true;
	// else return false;
	bool check(const Data& d) const
	{
		size_t pos = bucketNum(d);
		for(size_t i = 0; i < _buckets[pos].size(); i++)
			if(_buckets[pos][i] == d) return true;
		return false; 
	}

	// query if d is in the hash...
	// if yes, replace d with the data in the hash and return true;
	// else return false;
	bool query(Data& d) const 
	{
		size_t pos = bucketNum(d);
		for(size_t i = 0; i < _buckets[pos].size(); i++)
		{	
			if(_buckets[pos][i] == d)
			{
				d = _buckets[pos][i];
				return true;
			}
		}
		return false; 
	}

	// update the entry in hash that is equal to d (i.e. == return true)
	// if found, update that entry with d and return true;
	// else insert d into hash as a new entry and return false;
	bool update(const Data& d) 
	{ 
		size_t pos = bucketNum(d);
		for(size_t i = 0; i < _buckets[pos].size(); i++)
		{	
			if(_buckets[pos][i] == d)
			{
				_buckets[pos][i] = d;
				return true;
			}
		}
		_buckets[pos].push_back(d);
		return false; 
	}

	// return true if inserted successfully (i.e. d is not in the hash)
	// return false is d is already in the hash ==> will not insert
	bool insert(const Data& d) 
	{ 
		if(check(d)) { return false; }
		else
		{
			_buckets[bucketNum(d)].push_back(d);
			return true; 
		}
	}

	// return true if removed successfully (i.e. d is in the hash)
	// return fasle otherwise (i.e. nothing is removed)
	bool remove(const Data& d) 
	{
		size_t pos = bucketNum(d);
		typename vector<Data>:: iterator ptr(_buckets[pos].begin());
		if(check(d))
		{
			while(ptr != _buckets[pos].end())
			{
				if(d == *ptr)
				{
					*ptr = _buckets[pos].back();
					_buckets[pos].pop_back();
					return true;
				}
				ptr++;
			}
		}
		else { return false; }
	}

private:
	// Do not add any extra data member
	size_t            _numBuckets;
	vector<Data>*     _buckets;

	size_t bucketNum(const Data& d) const {
		return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H