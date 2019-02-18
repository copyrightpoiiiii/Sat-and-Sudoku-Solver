#ifndef GENERAL
#define GENERAL

#include"General.h"

#endif
#define red 0
#define black 1

template<typename T>
class set {
public:
	struct node {
		bool color = red;
		T data = 0;
		node *parent = NULL;
		node *left = NULL;
		node *right = NULL;
		node *pre = NULL;
		node *next = NULL;

		node *grandParent (node *n) {
			if (n->parent == NULL)
				return NULL;
			return n->parent->parent;
		}

		node *uncle (node *n) {
			if (n->grandParent (n) == NULL)
				return NULL;
			if (n->parent == n->grandParent (n)->left)
				return n->grandParent (n)->right;
			else
				return n->grandParent (n)->left;
		}

		node *brother (node *n) {
			if (n->parent == NULL)
				return NULL;
			if (n == n->parent->left)
				return n->parent->right;
			else
				return n->parent->left;
		}
	};

	set () {
		node *treeRoot = NULL;
		length = 0;
	};

	int size () const { return length; }

	void insert (const T &item);

	void insert_case1 (node *n);

	void insert_case2 (node *n);

	void insert_case3 (node *n);

	void insert_case4 (node *n);

	void insert_case5 (node *n);

	void rotate_left (node *n);

	void rotate_right (node *n);

	class iterator {
	public:
		node *current;

		const T &operator* () { return current->data; }

		iterator (node *currentTmp) { current = currentTmp; }
		/*iterator operator++(int) {
		  node *leftChild = this->current->left;
		  node *rightChild = this->current->right;
		  node *now = rightChild;
		  while (now->left) {
			now = now->left;
		  }
		  return iterator(now);
		}; */// for iter++
		iterator &operator++ () {
			current = current->next;
			return *this;
		}

		iterator &operator-- () {
			current = current->pre;
			return *this;
		}

		iterator operator++ (int) // for iter++
		{
			iterator iter = this;
			++this;
			return iter;
		}

		iterator operator-- (int) {
			iterator iter = this;
			--this;
			return iter;
		}

		bool operator== (const iterator &another) const {
			return current == another.current;
		};

		bool operator!= (const iterator &another) const {
			return current != another.current;
		};

		// private:
		// int length;
		// add your data and function here
	};

	iterator find (const T &item);

	void erase (iterator it);

	void delete_case1 (node *n);

	void delete_case2 (node *n);

	void delete_case3 (node *n);

	void delete_case4 (node *n);

	void delete_case5 (node *n);

	void delete_case6 (node *n);

	void delete_one_child (node *n);

	void del (node *now);

	iterator begin ();

	iterator end ();

	void clear ();

	~set () { clear (); };

private:
	int length;
	node *data;
	node *beg = NULL;
	node *en = NULL;
	node *treeRoot = NULL;
	// add your data and function here
};

template<typename T>
typename set<T>::iterator set<T>::begin () {
	return iterator (beg);
}

template<typename T>
typename set<T>::iterator set<T>::end () {
	return iterator (en);
}

template<typename T>
void set<T>::rotate_left (node *n) {
	node *gParent = n->grandParent (n);
	node *father = n->parent;
	node *son = n->left;
	father->right = son;
	if (son)
		son->parent = father;
	n->left = father;
	father->parent = n;
	if (treeRoot == father)
		treeRoot = n;
	n->parent = gParent;
	if (gParent) {
		if (gParent->left == father)
			gParent->left = n;
		else
			gParent->right = n;
	}
}

template<typename T>
void set<T>::rotate_right (node *n) {
	node *gParent = n->grandParent (n);
	node *father = n->parent;
	node *son = n->right;
	father->left = son;
	if (son)
		son->parent = father;
	n->right = father;
	father->parent = n;
	if (treeRoot == father)
		treeRoot = n;
	n->parent = gParent;
	if (gParent) {
		if (gParent->left == father)
			gParent->left = n;
		else
			gParent->right = n;
	}
}

template<typename T>
void set<T>::insert (const T &item) {
	/*std::cout << "ok"
			  << " ";*/
	node *newNode = new node;
	newNode->data = item;
	node *now = treeRoot;
	T max = item;
	node *maxAddress = NULL;
	T min = item;
	node *minAddress = NULL;
	while (now) {
		node *left = now->left;
		node *right = now->right;
		if (newNode->data == now->data)
			return;
		if (left && newNode->data < now->data) {
			if (now->data < max || max == item) {
				max = now->data;
				maxAddress = now;
			}
			now = left;
			continue;
		}
		if (right && newNode->data > now->data) {
			if (now->data > min || min == item) {
				min = now->data;
				minAddress = now;
			}
			now = right;
			continue;
		}
		// newNode= now;
		if (newNode->data < now->data) {
			if (now->data < max || max == item) {
				max = now->data;
				maxAddress = now;
			}
			now->left = newNode;
			newNode->parent = now;
			if (minAddress) {
				newNode->pre = minAddress;
				minAddress->next = newNode;
			}
			if (maxAddress) {
				newNode->next = maxAddress;
				maxAddress->pre = newNode;
			}
			break;
		} else {
			if (now->data > min || min == item) {
				min = now->data;
				minAddress = now;
			}
			now->right = newNode;
			newNode->parent = now;
			if (minAddress) {
				newNode->pre = minAddress;
				minAddress->next = newNode;
			}
			if (maxAddress) {
				newNode->next = maxAddress;
				maxAddress->pre = newNode;
			}
			break;
		}
	}
	// if(now->left->data<newNode->data)
	if (newNode->pre == NULL)
		beg = newNode;
	if (newNode->next == NULL)
		en = newNode;
	length++;
	insert_case1 (newNode);
} // namespace Lab
template<typename T>
void set<T>::insert_case1 (node *n) {
	if (n->parent == NULL) {
		treeRoot = n;
		n->color = black;
	} else {
		insert_case2 (n);
	}
}

template<typename T>
void set<T>::insert_case2 (node *n) {
	if (n->parent->color == black)
		return;
	insert_case3 (n);
}

template<typename T>
void set<T>::insert_case3 (node *n) {
	if (n->parent->color == red && n->uncle (n) != NULL &&
	    n->uncle (n)->color == red) {
		n->uncle (n)->color = black;
		n->parent->color = black;
		n->grandParent (n)->color = red;
		insert_case1 (n->grandParent (n));
	} else
		insert_case4 (n);
}

template<typename T>
void set<T>::insert_case4 (node *n) {
	if (n == n->parent->right && n->parent == n->grandParent (n)->left) {
		rotate_left (n);
		n = n->left;
	}
	if (n == n->parent->left && n->parent == n->grandParent (n)->right) {
		rotate_right (n);
		n = n->right;
	}
	insert_case5 (n);
}

template<typename T>
void set<T>::insert_case5 (node *n) {
	n->parent->color = black;
	n->grandParent (n)->color = red;
	if (n == n->parent->left && n->parent == n->grandParent (n)->left) {
		rotate_right (n->parent);
	} else {
		rotate_left (n->parent);
	}
}

template<typename T>
typename set<T>::iterator set<T>::find (const T &item) {
	node *now = treeRoot;
	while (now) {
		// node *leftChild=now->left;
		// node *rightChild=now->right;
		if (now->data == item)
			return iterator (now);
		if (now->data < now)
			now = now->right;
		else if (now->data > now)
			now = now->left;
	}
	return end ();
}

template<typename T>
void set<T>::erase (iterator it) {
	node *leftChild = it.current->left;
	node *rightChild = it.current->right;
	node *tmp = it.current;
	if (leftChild && rightChild) {
		tmp = leftChild;
		while (tmp->right)
			tmp = tmp->right;
		it.current->data = tmp->data;
		// delete tmp;
	}
	delete_one_child (tmp);
	length--;
}

template<typename T>
void set<T>::delete_one_child (node *n) {
	if (!n)
		return;
	if (beg == n)
		beg = n->next;
	if (en == n)
		en = n->pre;
	if (n->pre)
		n->pre->next = n->next;
	if (n->next)
		n->next->pre = n->pre;
	node *child;
	if (n->left)
		child = n->left;
	else
		child = n->right;
	if (n->left == NULL && n->right == NULL && n->parent == NULL) {
		n = NULL;
		treeRoot = n;
		return;
	} // if root
	if (n->parent == NULL) {
		delete n;
		treeRoot = child;
		treeRoot->color = black;
		child->parent = NULL;
		return;
	}
	if (n->parent->left == n)
		n->parent->left = child;
	else
		n->parent->right = child;
	if (child)
		child->parent = n->parent;
	if (n->color == black) {
		if (child && child->color == red) {
			child->color = black;
		} else
			delete_case1 (child);
	}
	delete n;
}

template<typename T>
void set<T>::delete_case1 (node *n) {
	if (!n)
		return;
	if (n->parent == NULL) {
		n->color = black;
		return;
	}
	delete_case2 (n);
}

template<typename T>
void set<T>::delete_case2 (node *n) {
	node *bro = n->brother (n);
	if (bro && bro->color == red) {
		n->parent->color = red;
		bro->color = black;
		if (n->parent->left == n)
			rotate_left (bro);
		else
			rotate_right (bro);
	}
	delete_case3 (n);
}

template<typename T>
void set<T>::delete_case3 (node *n) {
	node *bro = n->brother (n);
	if ((n->parent->color == black) && (bro) &&
	    ((!bro->left) || bro->left->color == black) && (bro->color == black) &&
	    ((!bro->right) || bro->right->color == black)) {
		bro->color = red;
		delete_case1 (n->parent);
	} else
		delete_case4 (n);
}

template<typename T>
void set<T>::delete_case4 (node *n) {
	node *bro = n->brother (n);
	if ((n->parent->color == red) && (bro) &&
	    ((!bro->left) || bro->left->color == black) && (bro->color == black) &&
	    ((!bro->right) || bro->right->color == black)) {
		bro->color = red;
		bro->parent->color = black;
	} else
		delete_case5 (n);
}

template<typename T>
void set<T>::delete_case5 (node *n) {
	node *bro = n->brother (n);
	if ((bro) && bro->color == black) {
		if ((n == n->parent->left) &&
		    ((!bro->right) || bro->right->color == black) && (bro->left) &&
		    (bro->left->color == red)) {
			bro->color = red;
			bro->left->color = black;
			rotate_right (bro->left);
		} else if ((n == n->parent->right) && (bro->right) &&
		           (bro->right->color == red) &&
		           ((!bro->left) || bro->left->color == black)) {
			bro->color = red;
			bro->right->color = black;
			rotate_left (bro->right);
		}
		delete_case6 (n);
	}
}

template<typename T>
void set<T>::delete_case6 (node *n) {
	node *bro = n->brother (n);
	bro->color = n->parent->color;
	n->parent->color = black;

	if (n == n->parent->left) {
		bro->right->color = black;
		rotate_left (bro);
	} else {
		bro->left->color = black;
		rotate_right (bro);
	}
}

template<typename T>
void set<T>::clear () {
	del (treeRoot);
	length = 0;
	treeRoot = NULL;
	beg = en = NULL;
}

template<typename T>
void set<T>::del (node *now) {
	if (!now)
		return;
	node *leftChild = now->left;
	node *rightChild = now->right;
	if (leftChild)
		del (leftChild);
	if (rightChild)
		del (rightChild);
	if (beg == now)
		beg = now->next;
	if (en == now)
		en = now->pre;
	if (now->pre)
		now->pre->next = now->next;
	if (now->next)
		now->next->pre = now->pre;
	delete now;
	length--;
}
