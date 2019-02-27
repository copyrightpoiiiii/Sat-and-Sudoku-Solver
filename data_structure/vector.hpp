#pragma once
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

	void inc_capacity () {
		Listsize <<= 1;
		elem = (T *) realloc (elem, Listsize * sizeof (T));
	}

	void dec_capacity () {
		if (Listsize <= 8)return;
		Listsize >>= 1;
		elem = (T *) realloc (elem, Listsize * sizeof (T));
	}

public:
	myVector () {
		num = 0;
		Listsize = 8;
		elem = (T *) malloc (Listsize * sizeof (T));
	}
    myVector(int size):Listsize(8){
        num = 0;
		Listsize = size;
		elem = (T *) malloc (Listsize * sizeof (T));
    }
    myVector(int size, const T& pad) : elem(NULL) , num(0)   , Listsize(0){
        Listsize = size;
		elem = (T *) malloc (Listsize * sizeof (T));
        for (int i = num; i < size; i++)
            new(&elem[i])T(pad);
        num = size;
    }
    myVector(T* array, int size)     : elem(array), num(size), Listsize(size) { } 

	T &operator[] (const unsigned long x){
        return elem[x];
    }

    operator T*       (void)           { return elem; }

    operator const T* (void) const     { return elem; }

    const  T &operator[] (const unsigned long x)const{
        return elem[x];
    }

	void push_back (const T new_elem) {
		if (num == Listsize)
			inc_capacity ();
		elem[num++] = new_elem;
	}

    void push_back () {
		if (num == Listsize)
			inc_capacity ();
		num++;
	}

	void pop_back () {
		num--;
		if (num == (Listsize / 2))
			dec_capacity ();
	}

	void erase (const unsigned long elem_id) {
		num--;
		for (register int i = elem_id; i < num; i++)
			elem[i] = elem[i + 1];
		if (num == (Listsize / 2))
			dec_capacity ();
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

    bool empty(){
        return num==0;
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

	T* begin () {
		return elem;
	}

    T* end () {
        return elem+num;
    }

	unsigned long size () const{
		return num;
	}

    void capacity(int input_size){
        Listsize=max(Listsize,input_size);
        elem=(T*)realloc(Listsize*sizeof(T));
    }

    void qsort(int l,int r){
        sort(elem+l,r-l+1);
    }

    template <class LessThan> void qsort(int l,int r,LessThan lt){
        sort(elem+l, r-l+1, lt);
    }

    void unique(){
        int tmp=num,now=0;
        for(int i=1;i<tmp;i++)
            if(elem[i]==elem[now]){
                num--;
            }
            else{
                elem[++now]=elem[i];
            }
    }
    void copyTo(myVector<T>& copy)const{
        copy.clear();
        copy.capacity(num);
        for(int i=0;i<num;i++)
            copy.push_back(elem[i]);
    }

};