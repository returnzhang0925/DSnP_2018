/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>
#include <cstdlib>
#include <iostream>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
	friend class DList<T>;
	friend class DList<T>::iterator;

	DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
		_data(d), _prev(p), _next(n) {}

	// [NOTE] DO NOT ADD or REMOVE any data member
	T              _data;
	DListNode<T>*  _prev;
	DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
	// TODO: decide the initial value for _isSorted
	DList() {
		_head = new DListNode<T>(T());
		_head->_prev = _head->_next = _head; // _head is a dummy node
		_isSorted = false;
	}
	~DList() { clear(); delete _head; }

	// DO NOT add any more data member or function for class iterator
	class iterator
	{
		friend class DList;

	public:
		iterator(DListNode<T>* n= 0): _node(n) {}
		iterator(const iterator& i) : _node(i._node) {}
		~iterator() {} // Should NOT delete _node

		// TODO: implement these overloaded operators
		const T& operator * () const { return _node->_data; }
		T& operator * () { return _node->_data; }
		iterator& operator ++ ()
		{
			_node = _node->_next;
			return *(this); 
		}
		iterator operator ++ (int)
		{ 
			iterator temp = *this;
			_node = _node->_next;
			return temp;
		}
		iterator& operator -- ()
		{ 
			_node = _node->_prev;
			return *(this); 
		}
		iterator operator -- (int) 
		{ 
			iterator temp = *this;
			_node = _node->_prev;
			return temp; 
		}

		iterator& operator = (const iterator& i) 
		{
			_node = i._node;
			return *(this); 
		}

		bool operator != (const iterator& i) const { return (_node != i._node); }
		bool operator == (const iterator& i) const { return (_node == i._node); }

	private:
		DListNode<T>* _node;
	};

	// TODO: implement these functions
	iterator begin() const { return iterator(_head->_next); }
	iterator end() const { return iterator(_head); }
	bool empty() const 
	{ 
		if(_head == _head->_prev) { return true; }
		else { return false; }
	}
	size_t size() const 
	{  
		size_t count = 0;
		for(iterator ptr = begin(); ptr != end(); ptr ++)
		{
			count ++;
		}
		return count; 
	}

	void push_back(const T& x) 
	{ 
		DListNode<T>* newnode = new DListNode<T>(x, _head->_prev, _head);
		_head->_prev->_next = newnode;
		_head->_prev = newnode;
		_isSorted = false;
	}
	void pop_front() 
	{
		if(!empty()) 
		{
			DListNode<T>* deletenode = _head->_next;
			_head->_next = deletenode->_next;
			deletenode->_next->_prev = _head;
			delete deletenode;
		}
		else { return; }
	}
	void pop_back() 
	{
		if(!empty())
		{
			DListNode<T>* deletenode = _head->_prev;
			_head->_prev->_prev->_next = _head;
			_head->_prev = deletenode->_prev;
			delete deletenode;
		}
		else { return; }
	}

	// return false if nothing to erase
	bool erase(iterator pos) 
	{
		if(empty()) { return false; }
		else
		{
			if(pos == begin()) { pop_front(); return true; }
			pos._node->_prev->_next = pos._node->_next;
			pos._node->_next->_prev = pos._node->_prev;
			delete pos._node;
			return true;
		}
	}
	bool erase(const T& x) 
	{
		if(empty()) { return false; }
		else
		{
			iterator ptr = begin();
			while(ptr._node->_data != x)
			{
				if(ptr == end()) { return false; }
				ptr ++;
			}
			if(ptr == begin())
			{
				pop_front();
				return true;
			}
			else
			{
				ptr._node->_prev->_next = ptr._node->_next;
				ptr._node->_next->_prev = ptr._node->_prev;
				delete ptr._node;
				return true;
			}
		}
	}

	iterator find(const T& x) 
	{ 
		iterator ptr = begin();
		while(ptr._node->_data != x)
		{
			if(ptr == end()) { return end(); }
			ptr = ptr._node->_next;
		}
		return ptr;
	}

	void clear() 
	{ 
		if (empty()) { return; }
		while(_head->_next != _head) 
		{
			pop_front();
		}
		_head->_prev = _head->_next = _head;
	}  // delete all nodes except for the dummy node

	void sort() const
	{ 
		// insertion_sort(begin(), size());
		// if(_isSorted == false)
		// {
		// 	quicksort(0, size(), begin(), (--end()));
		// 	_isSorted == true;
		// }
		// int length = size();
		// iterator ptr;

		// if(_isSorted == false)
		// {
		// 	quicksort(begin(), (end()));
		// 	_isSorted == true;
		// }

		// iterator a = begin();
		// 	int length = size();
		// 	for (size_t i = 0; i < length - 1; i++) {
		// 	for (size_t j = 0; j < length - 1 - i; j++) {
		// 		if (a._node->_data > a._node->_next->_data) {
		// 		T temp = a._node->_data;
		// 		a._node->_data = a._node->_next->_data;
		// 		a._node->_next->_data = temp;
		// 		}
		// 		a++;
		// 	}
		// 	a = begin();
		// 	}
		// 	_isSorted = true;
		// }
                     
		iterator ptr = begin();
		int length = size();
		if(_isSorted == false)
		{
			while (length > 1)
			{
				length--;
				for(int i = 0; i < length; i++)
				{
					if(ptr._node->_data > (ptr._node->_next)->_data)
					{
						T tmp = ptr._node->_data;
						ptr._node->_data = (ptr._node->_next)->_data;
						(ptr._node->_next)->_data = tmp;
					}
					ptr ++;
				}
				ptr = begin();
			}
			_isSorted == true;
		}
	}

private:
	// [NOTE] DO NOT ADD or REMOVE any data member
	DListNode<T>*  _head;     // = dummy node if list is empty
	mutable bool   _isSorted; // (optionally) to indicate the array is sorted

	// iterator iteratorsize_head(size_t src) const
	// {
	// 	iterator count = begin();
	// 	while(src != 0)
	// 	{
	// 		count ++;
	// 		src --;
	// 	}
	// 	cerr << "hey3" << endl;
	// 	return count;
	// }
	// iterator iteratorsize_end(size_t src) const
	// {
	// 	iterator count = (--end());
	// 	while(src != size())
	// 	{
	// 		count --;
	// 		src ++;
	// 	}
	// 	return count;
	// }
	// iterator iteratorsize_pivot1(size_t src) const
	// {
	// 	iterator count = begin();
	// 	while(src != 0)
	// 	{
	// 		src --;
	// 		count ++;
	// 	}
	// 	cerr << "hey2" << endl;
	// 	return count;
	// }
	// iterator iteratorsize_pivot2(size_t src) const
	// {
	// 	iterator count = (--end());
	// 	while(src != size())
	// 	{
	// 		src ++;
	// 		count --;
	// 	}
	// 	return count;
	// }
	
	// void quicksort(iterator head, iterator end) const
	// {
	// 	iterator pivot;
	// 	if(head._node != _head->_prev && head._node != end._node && head._node != end._node->_next)
	// 	{
	// 		pivot = Partition(head, end);
	// 		quicksort(head, pivot--);
	// 		quicksort(pivot++, end);
	// 	}
	// }
	// iterator Partition(iterator head, iterator end) const
	// {
	// 	T t = end._node->_data;
    //     iterator i = head--;

    //     for (iterator j = head; j != end; j++)
    //     {
    //         if(j._node->_data < t)
    //         {
    //             if(i._node == _head->_prev)
    //                 i = head;
    //             else
    //                i++ ;
                
    //             T tmp = i._node->_data;
    //             i._node->_data = j._node->_data;
    //             j._node->_data = tmp;
    //         }
    //     }
    //     if(i._node == _head->_prev)
    //         i = head;
    //     else
    //         i++;
            
    //     T ttmp = i._node->_data;
    //     i._node->_data = end._node->_data;
    //     end._node->_data = ttmp;
    //     return i;

	// }
	void swap(iterator a, iterator b) const
	{
		T tmp = a._node->_data;
		a._node->_data = b._node->_data;
		b._node->_data = tmp;
	}
	// void insertion_sort(iterator a, size_t len) const
	// {
	// 	iterator ptr(a);
	// 	iterator get(a);
	// 	iterator min(a);

	// 	for(size_t i = 0; i < len; i++)
	// 	{
	// 		for(size_t j = 1; j < len - 1; j++)
	// 		{
	// 			get ++;
	// 			if(get._node->_data < min._node->_data) { min = get;}	
	// 		}
	// 		if(min != ptr) { swap(ptr, min); }
	// 		ptr++;
	// 		get = ptr;
	// 		min = ptr;
	// 	}
	// }
	// void quickSort(iterator first, iterator last, size_t size) const
	// {
	// 	if(size == 0) return;
	// 	int top = 0;
	// 	pair<iterator, iterator> stack[size];
	// 	stack[top++] = make_pair(first, last);
	// 	while(top)
	// 	{
	// 		--top;
	// 		iterator head(stack[top].first);
	// 		iterator tail(stack[top].second);
	// 		iterator start(head);
	// 		iterator end(tail--);
	// 		if(start == end) continue;
	// 		iterator mid(end);
	// 		while(head != tail && head != tail._node->_next)
	// 		{
	// 			while(head._node->_data < mid._node->_data && head != tail) ++head;
	// 			while(tail._node->_data >= mid._node->_data && head != tail) --tail;
	// 			swap(head, tail);
	// 		}
	// 		if(head._node->_data >= mid._node->_data) swap(head, mid);
	// 		else ++head;
	// 		iterator newEnd(head);
	// 		if(newEnd != start) stack[top++] = make_pair(start, --newEnd);
	// 		if(head != end) stack[top++] = make_pair(++head, end);
	// 	}
	// }
	// [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
