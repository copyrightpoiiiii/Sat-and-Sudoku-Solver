#ifndef GENERAL
#define GENERAL

#include"General.h"

#endif

template<typename T>
class myVector {
private:
	T *elem;
	unsigned long Listsize, num;

    inline void inc_capacity ();

	inline void dec_capacity ();

public:
	inline myVector () {
		num = 0;
		Listsize = 8;
		elem = (T *) malloc (Listsize * sizeof (T));
	}

	inline T &operator[] (const unsigned long x);

	const myVector &push_back (const T new_elem);

	const myVector &pop_back ();

	const myVector &erase (const unsigned long elem_id);

    bool del (const T val);

    void random_shuffle(T& head,T& tail);

    void clear();

    myVector &begin();

	inline unsigned long size ();

};