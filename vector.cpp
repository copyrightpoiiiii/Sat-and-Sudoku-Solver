#include"vector.h"

template<class T>

inline void myVector<T>::inc_capacity () {
	Listsize <<= 1;
	elem = (T *) realloc (elem, Listsize * sizeof (T));
}

template<class T>
inline void myVector<T>::dec_capacity () {
    if(Listsize<=8)return ;
	Listsize >>= 1;
	elem = (T *) realloc (elem, Listsize * sizeof (T));
}

template<class T>
const myVector <T> &myVector<T>::push_back (const T new_elem) {
	if (num == Listsize)
		inc_capacity ();
	elem[num++] = new_elem;
	return *this;
}

template<class T>
const myVector <T> &myVector<T>::pop_back () {
	num--;
	if (num == (Listsize / 2))
		dec_capacity ();
	return *this;
}

template<class T>
const myVector <T> &myVector<T>::erase (const unsigned long elem_id) {
    num--;
	for (register int i = elem_id; i < num; i++)
		elem[i] = elem[i+1];
	if (num == (Listsize / 2))
		dec_capacity ();
	return *this;
}

template<class T>
void myVector<T>::random_shuffle(T& head,T& tail){
    int len=tail-head;
    for(int i=0;i<len;i++){
        swap(head+i,head+rand()%len);
    }
}

template<class T>
myVector <T> &myVector<T>::begin () {
	num = 0;
	Listsize = 8;
	if(elem!=NULL)free(elem);
    elem = (T *) malloc (Listsize * sizeof (T));
}

template<class T>
void myVector<T>::clear () {
	return elem;
}

template<class T>
inline unsigned long myVector<T>::size () {
	return num;
}