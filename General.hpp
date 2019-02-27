#pragma once
#include<stdint.h>
#include<cstdlib>
#include<cstdio>
#include<string.h>
#include <new>

#define TRUE 1
#define FALSE 0
#define TRUE_PROPA 2
#define FALSE_PROPA 3
#define UNDEFINE -1

template<class T>
void swap (T &a, T &b) {
	T c (a);
	a = b;
	b = c;
}



template<typename T>
T abs (T a) {
	return a > 0 ? a : -a;
}

template<typename T1, typename T2>
struct pair {
	T1 first;
	T2 second;
};

template<class T1, class T2>
pair<T1, T2> make_pair (T1 x, T2 y) {
	return (pair<T1, T2> (x, y));
};

template<class T1, class T2>
inline bool operator< (const pair<T1, T2> &p1, const pair<T1, T2> &p2) {
	return p1.first == p2.first ? p1.second < p2.second : p1.first < p2.first;
}

template<class T1, class T2>
inline bool operator== (const pair<T1, T2> &p1, const pair<T1, T2> &p2) {
	return (p1.first == p2.first) && (p1.second == p2.second);
}

template<class T1, class T2>
inline bool operator> (const pair<T1, T2> &p1, const pair<T1, T2> &p2) {
	return p1.first == p2.first ? p1.second > p2.second : p1.first > p2.first;
}

template<typename T>
class less {
public:
	bool operator() (const T &a, const T &b) { return a < b; }
};


template<class T>
struct LessThan_default {
    bool operator () (T x, T y) { return x < y; }
};

template <class T, class LessThan>
void selectionSort(T* array, int size, LessThan lt)
{
    int     i, j, best_i;
    T       tmp;

    for (i = 0; i < size-1; i++){
        best_i = i;
        for (j = i+1; j < size; j++){
            if (lt(array[j], array[best_i]))
                best_i = j;
        }
        tmp = array[i]; array[i] = array[best_i]; array[best_i] = tmp;
    }
}
template <class T> static inline void selectionSort(T* array, int size) {
    selectionSort(array, size, LessThan_default<T>()); }


template <class T, class LessThan>
void sort(T* array, int size, LessThan lt)
{
    if (size <= 15)
        selectionSort(array, size, lt);

    else{
        T           pivot = array[rand()%size];
        T           tmp;
        int         i = -1;
        int         j = size;

        for(;;){
            do i++; while(lt(array[i], pivot));
            do j--; while(lt(pivot, array[j]));

            if (i >= j) break;

            tmp = array[i]; array[i] = array[j]; array[j] = tmp;
        }

        sort(array    , i     , lt);
        sort(&array[i], size-i, lt);
    }
}
template <class T> static inline void sort(T* array, int size) {
    sort(array, size, LessThan_default<T>()); }

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
        
        Listsize=(Listsize>input_size)?Listsize:input_size;
        elem=(T*)realloc(elem,Listsize*sizeof(T));
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



static inline int left  (int i) { return i+i; }
static inline int right (int i) { return i+i + 1; }
static inline int parent(int i) { return i >> 1; }

template<class C>
class Heap {
  public:
    C        comp;
    myVector<int> heap;     // heap of ints
    myVector<int> indices;  // int -> index in heap

    inline void percolateUp(int i){
        int x = heap[i];
        while (parent(i) != 0 && comp(x,heap[parent(i)])){
            heap[i]          = heap[parent(i)];
            indices[heap[i]] = i;
            i                = parent(i);
        }
        heap   [i] = x;
        indices[x] = i;
    }

    inline void percolateDown(int i)
    {
        int x = heap[i];
        while (left(i) < heap.size()){
            int child = right(i) < heap.size() && comp(heap[right(i)],heap[left(i)]) ? right(i) : left(i);
            if (!comp(heap[child],x)) break;
            heap[i]          = heap[child];
            indices[heap[i]] = i;
            i                = child;
        }
        heap   [i] = x;
        indices[x] = i;
    }

    bool ok(int n) { return n >= 0 && n < (int)indices.size(); }

  public:
    Heap(C c) : comp(c) { heap.push_back(-1); }

    void setBounds (int size) {  indices.capacity(size); }
    bool inHeap    (int n)    {  return indices[n] != 0; }
    void increase  (int n)    {  percolateUp(indices[n]); }
    bool empty     ()         { return heap.size() == 1; }

    void insert(int n) {
        indices[n] = heap.size();
        heap.push_back(n);
        percolateUp(indices[n]); }

    int  getmin() {
        int r            = heap[1];
        heap[1]          = heap[heap.size()-1];
        indices[heap[1]] = 1;
        indices[r]       = 0;
        heap.pop_back();
        if (heap.size() > 1)
            percolateDown(1);
        return r; }

    bool heapProperty() {
        return heapProperty(1); }

    bool heapProperty(int i) {
        return (size_t)i >= heap.size()
            || ((parent(i) == 0 || !comp(heap[i],heap[parent(i)])) && heapProperty(left(i)) && heapProperty(right(i))); }
};

struct info{
    bool exist;
    int size,l1,l2;
};

class Cnf{
    private:
        typedef myVector<int> clause;
        typedef myVector<int> rec_lit_in_clause;
    public:
        myVector<clause> cnf_set;
        myVector<int> lit_set;
        myVector<info> clause_info;
        myVector<rec_lit_in_clause> rec_pos_lit;
        myVector<rec_lit_in_clause> rec_neg_lit;
        unsigned long literal_size;
        unsigned long clause_size;
        inline Cnf(){
            cnf_set.clear();
            lit_set.clear();
            clause_info.clear();
            rec_pos_lit.clear();
            rec_neg_lit.clear();
            literal_size=0;
            clause_size=0;
        }
        inline void init(){
            lit_set.capacity(literal_size);
            clause_info.capacity(cnf_set.size());
            for(int i=0;i<literal_size;i++)
                lit_set[i]=UNDEFINE;
            for(int i=0;i<cnf_set.size();i++){
                clause_info[i].exist=true;
                clause_info[i].size=clause_info[i].l2=cnf_set[i].size();
                clause_info[i].l1=0;
            }
        }
        inline void clear(){
            cnf_set.clear();
            lit_set.clear();
            rec_pos_lit.clear();
            rec_neg_lit.clear();
            clause_info.clear();
            literal_size=0;
            clause_size=0;
        }
        bool empty(){
            return clause_size==0;
        }
};

