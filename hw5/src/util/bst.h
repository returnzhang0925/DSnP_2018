/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
  	// TODO: design your own class!!
   	friend class BSTree<T>;
	friend class BSTree<T>::iterator;

	BSTreeNode(const T& d, BSTreeNode<T>* l = 0, BSTreeNode<T>* r = 0, BSTreeNode<T>* p = 0):
		_data(d), leftchild(l), rightchild(r), parent(p) {}

	T               _data;
	BSTreeNode<T>*  leftchild;
	BSTreeNode<T>*  rightchild;
  	BSTreeNode<T>*  parent;
};


template <class T>
class BSTree
{
public:
    // TODO: design your own class!!
	BSTree()
	{
		_root = new BSTreeNode<T>(T(""));
		_root->rightchild = _root->parent = _tail = _root;
	}
	~BSTree() { clear(); delete _root; }
	class iterator
	{
		friend class BSTree;

	public:
		iterator(BSTreeNode<T>* n= 0): _node(n) {}
		iterator(const iterator& i) : _node(i._node) {}
		~iterator() {}

		const T& operator * () const { return _node->_data; }
		T& operator * () { return _node->_data; }
		iterator& operator ++ ()
		{
			if(_node->_data == T("")) {}
			else if(_node->rightchild == NULL)
			{
				while(_node->parent->rightchild == _node)
				{
					_node = _node->parent;
				}
				_node = _node->parent;
			}
			else
			{
				_node = _node->rightchild;
				while(_node->leftchild != NULL)
				{
					_node = _node->leftchild;
				}
			}
			return *(this); 
		}
		iterator operator ++ (int)
		{ 
			iterator temp = *this; 
			++(*this);
			return temp;
		}
		iterator& operator -- ()
		{ 
			if(_node->leftchild == NULL)
			{
				while(_node->parent->leftchild == _node)
				{
					_node = _node->parent;
				}
				_node = _node->parent;
			}
			else
			{
				_node = _node->leftchild;
				while(_node->rightchild != NULL)
				{
					_node = _node->rightchild;
				}
			}
			if (_node->_data == T("")) 
				while(_node->leftchild) { _node = _node->leftchild; }

			return *(this); 
		}
		iterator operator -- (int) 
		{ 
			iterator temp = *this;
			--(*this);
			return temp; 
		}

		iterator& operator = (const iterator& i) 
		{
			_node = i._node;
			return *(this); 
		}		

		bool operator != (const iterator& i) const { return (_node != i._node); }
		bool operator == (const iterator& i) const { return (_node == i._node); }

		BSTreeNode<T>* _node;
	};

	iterator begin() const 
	{
		return iterator(min(_root));
	}
	iterator end() const 
	{
		return iterator(_tail);
	}
	bool empty() const 
	{ 
		if(_root == _tail) { return true; }
		else { return false; }
	}
	size_t size() const 
	{
		size_t count = 0;
		if(!empty()) { for(iterator ptr = begin(); ptr != end(); ptr ++) { count ++; } }
		return count; 
	}

	void insert(const T& x) 
	{ 
		BSTreeNode<T>* preparent = 0;        
		BSTreeNode<T>* soldier = 0;   

		if(empty())
		{
			_root = new BSTreeNode<T>(x);
			_root->parent = _tail;
			_root->rightchild = _tail;
			_tail->rightchild = _tail;
			_tail->parent = _root;
		}
		else
		{
			BSTreeNode<T>* newnode = new BSTreeNode<T>(x);
			soldier = _root;
			while (soldier != NULL)
			{
				preparent = soldier; 
				if(soldier->rightchild == _tail && newnode->_data > soldier->_data) { break; }            				     
				
				if(newnode->_data <= soldier->_data)
				{
					if(soldier->leftchild) { soldier = soldier->leftchild; }
					else{ break; }
					
				}
				else
				{
					if(soldier->rightchild) { soldier = soldier->rightchild; }
					else{ break; }
				}
			}                                 
			if(soldier->rightchild == _tail && newnode->_data > soldier->_data)
			{
				soldier->rightchild = newnode;
				newnode->parent = soldier;
				newnode->rightchild = _tail;
				_tail->parent = newnode;
			}
			else
			{
				newnode->parent = preparent;        
				if (newnode->_data <= preparent->_data)
				{
					preparent->leftchild = newnode;
				}
				else
				{
					preparent->rightchild = newnode;
				}
			}
		}
	}
	void pop_front() 
	{
		if(!empty())
		{ 
			BSTreeNode<T>* deletenode = begin()._node;
			if(deletenode == _root)
			{
				delete deletenode;
				_root = _tail;
			}
			else
			{
				if(deletenode->rightchild != NULL)
				{
					deletenode->parent->leftchild = deletenode->rightchild;
					deletenode->rightchild->parent = deletenode->parent;
				}
				else
				{
					deletenode->parent->leftchild = NULL;
				}
				delete deletenode;
			}
		}
		else { return; }
	}
	void pop_back() 
	{
		if(!empty())
		{
			BSTreeNode<T>* deletenode = (--end())._node;
			if(deletenode->leftchild != NULL)
			{
				BSTreeNode<T>* tmp = max(deletenode->leftchild);
				deletenode->parent->rightchild = deletenode->leftchild;
				deletenode->leftchild->parent = deletenode->parent;
				tmp->rightchild =_tail;
				_tail->parent = tmp;
				if(deletenode == _root) { _root = _root->leftchild;}
			}
			else
			{
				deletenode->parent->rightchild = _tail;
				_tail->parent = deletenode->parent;
				if(deletenode == _root) { _root = _tail;}
			}
			delete deletenode;
		}
		else { return; }

	}

	bool erase(iterator pos) 
	{
		BSTreeNode<T>* deletenode = pos._node;
		if(empty() || pos == end()) { return false; }
		// else if(pos == begin()){ pop_front(); return true; }
		else if(pos == (--end())){ pop_back(); return true; }
		else
		{
			if(deletenode->leftchild == NULL && deletenode->rightchild == NULL) { delete_case1(pos); }
			else if(deletenode->leftchild == NULL || deletenode->rightchild == NULL) { delete_case2(pos); }
			else { delete_case3(pos); }		
			return true;
		}
	}
	bool erase(const T& x) 
	{
		if(empty()) { return false;}
		else
		{
			iterator ptr = begin();
			while(ptr._node->_data != x)
			{
				if(ptr == end()) { return false; }
				ptr ++;
			}
			return erase(ptr);
		}
	}

	iterator find(const T& x) 
	{ 
		iterator ptr = begin();
		while(ptr._node->_data != x)
		{
			if(ptr == end()) { return end(); }
			ptr ++;
		}
		return ptr;
	}

	void clear() { while(begin() != end()) { erase(begin()); }}
	void sort() const {}
	void print() const {}

private:
	BSTreeNode<T>*  _root;
	BSTreeNode<T>*  _tail;
void erase0(iterator pos) {
      BSTreeNode<T>* tmp = pos._node;
      if (tmp->parent->leftchild == tmp)
         tmp->parent->leftchild = 0;
      else
         tmp->parent->rightchild = 0;
      delete tmp;
   }
   void erase1(iterator pos) {
      BSTreeNode<T>* tmp = pos._node;
      BSTreeNode<T>* newChild = (tmp->leftchild)? tmp->leftchild : tmp->rightchild;
      if (tmp->parent->leftchild == tmp)
         tmp->parent->leftchild = newChild;
      else
         tmp->parent->rightchild = newChild;
      if (tmp->leftchild || tmp->rightchild)
         newChild->parent = tmp->parent;
      if (tmp == _root)
         _root = newChild;
      delete tmp;
   }
	void delete_case1(iterator pos)
	{
		BSTreeNode<T>* tmp = pos._node;
		if(tmp->parent->leftchild == tmp)
		{
			tmp->parent->leftchild = NULL;
		}
		else
		{
			tmp->parent->rightchild = NULL;
		}
		delete tmp;
	}
	void delete_case2(iterator pos)
	{
		BSTreeNode<T>* tmp = pos._node;
		BSTreeNode<T>* newnode = 0;
		if(tmp->leftchild != NULL)
		{
			newnode = tmp->leftchild;
			if(tmp->parent->leftchild == tmp)
			{
				tmp->parent->leftchild = newnode;
			}
			else
			{
				tmp->parent->rightchild = newnode;
			}
			if(tmp == _root) { _root = newnode; }
		}
		else
		{
			newnode = tmp->rightchild;
			if(tmp->parent->leftchild == tmp)
			{
				tmp->parent->leftchild = newnode;
			}
			else
			{
				tmp->parent->rightchild = newnode;
			}
			if(tmp == _root) { _root = newnode; }
		}

		if(tmp->leftchild != NULL || tmp->rightchild != NULL)
		{
			newnode->parent = tmp->parent;
		}
		delete tmp;	
	}
	void delete_case3(iterator pos)
	{
		iterator ptr = Successor(pos);
		swap(*pos, *ptr);
		delete_case2(ptr);
		// BSTreeNode<T>* tmp = pos._node;
		// BSTreeNode<T>* top = min(tmp->rightchild);
		
		// if(tmp->rightchild != top )
		// {
		// 	if(top->rightchild != NULL)
		// 	{
		// 		top->parent->leftchild = top->rightchild;
		// 		top->rightchild->parent = top->parent;
		// 	}
		// 	else
		// 	{
		// 		top->parent->leftchild = NULL;
		// 	}
		// 	top->leftchild = tmp->leftchild;
		// 	top->rightchild = tmp->rightchild;
		// 	top->parent = tmp->parent;
		// 	if(tmp->parent->leftchild == tmp)
		// 	{
		// 		tmp->parent->leftchild = top;
		// 	}
		// 	else
		// 	{
		// 		tmp->parent->rightchild = top;
		// 	}
		// 	if(tmp == _root) { _root = top; }
		// }
		// else
		// {
		// 	if(tmp->parent->leftchild == tmp)
		// 	{
		// 		tmp->parent->leftchild = tmp->rightchild;
		// 	}
		// 	else
		// 	{
		// 		tmp->parent->rightchild = tmp->rightchild;
		// 	}
		// 	tmp->rightchild->parent = tmp->parent;
		// 	if(tmp->leftchild != NULL)
		// 	{
		// 		tmp->rightchild->leftchild = tmp->leftchild;
		// 	}
		// 	else
		// 	{
		// 		tmp->rightchild->leftchild = NULL;
		// 	}
		// 	if(tmp == _root) { _root = tmp->rightchild; }
		// }
		// delete tmp;
	}
	BSTreeNode<T>* min(BSTreeNode<T>* node) const
	{
		if (node->leftchild == NULL) { return node; }
		else { return min(node->leftchild); }
	}

	BSTreeNode<T>* max(BSTreeNode<T>* node) const
	{
		if (node->rightchild == NULL && node->rightchild != _tail) { return node; }
		else { return max(node->rightchild); }
	}

	iterator Successor(iterator current) const { return min(current._node->rightchild); }
};

#endif // BST_H
