#ifndef GENERAL
#define GENERAL

#include"../General.hpp"

#endif

#include"sort.hpp"

template<typename T>
class myVector {
private:
	T *elem;
	unsigned long Listsize, num;

	inline void inc_capacity () {
		Listsize <<= 1;
		elem = (T *) realloc (elem, Listsize * sizeof (T));
	}

	inline void dec_capacity () {
		if (Listsize <= 8)return;
		Listsize >>= 1;
		elem = (T *) realloc (elem, Listsize * sizeof (T));
	}

public:
	inline myVector () {
		num = 0;
		Listsize = 8;
		elem = (T *) malloc (Listsize * sizeof (T));
	}

	inline T &operator[] (const unsigned long x){
        return elem[x];
    }

	const myVector &push_back (const T new_elem) {
		if (num == Listsize)
			inc_capacity ();
		elem[num++] = new_elem;
		return *this;
	}

	const myVector &pop_back () {
		num--;
		if (num == (Listsize / 2))
			dec_capacity ();
		return *this;
	}

	const myVector &erase (const unsigned long elem_id) {
		num--;
		for (register int i = elem_id; i < num; i++)
			elem[i] = elem[i + 1];
		if (num == (Listsize / 2))
			dec_capacity ();
		return *this;
	}

	bool del (const T val) {
		int l = 0, r = num - 1;
		while (l <= r) {
			int mid = (l + r) >> 1;
			if (abs(elem[mid]) == val) {
				erase (mid);
				return true;
			} else if (abs(elem[mid]) < val)
				l = mid + 1;
			else r = mid - 1;
		}
		return false;
	}

	void random_shuffle (T &head, T &tail) {
		int len = tail - head;
		for (int i = 0; i < len; i++) {
			swap (head + i, head + rand () % len);
		}
	}

	void clear () {
		num = 0;
		Listsize = 8;
		if (elem != NULL)free (elem);
		elem = (T *) malloc (Listsize * sizeof (T));
	}

	myVector &begin () {
		return elem;
	}

    myVector &end () {
        return elem+num;
    }

	inline unsigned long size () {
		return num;
	}

    inline void capacity(int input_size){
        Listsize=max(Listsize,input_size);
        elem=(T*)realloc(Listsize*sizeof(T));
    }

    void qsort(int l,int r){
        sort(elem+l,r-l+1);
    }

};