#ifndef GENERAL
#define GENERAL

#include"General.h"

#endif

template<typename T>
class myQueue {
private:
	T *elem;
	unsigned long Listsize, l,r,num;

	inline void inc_capacity () {
		Listsize<<=1;
        elem=(T*)realloc(Listsize*sizeof(T));
	}

public:
	inline myQueue () {
		l = r = 0;
        num=0;
		Listsize = 8;
		elem = (T *) malloc (Listsize * sizeof (T));
	}

	inline T &operator[] (const unsigned long x){
        return elem[x];
    }

	void push (const T new_elem) {
		elem[++r]=new_elem;
        num++;
        if(num==Listsize)
            inc_capacity();
	}

	T pop () {
		if(!empty()){
            num--;
            return elem[++l];
        }
	}

	void clear () {
		num = l = r = 0;
		Listsize = 8;
		if (elem != NULL)free (elem);
		elem = (T *) malloc (Listsize * sizeof (T));
	}

	T &begin () {
		return &elem[l];
	}

    T &end () {
		return &elem[r];
	}

    bool empty(){
        return num==0?true:false;
    }

	inline unsigned long size () {
		return num;
	}

};