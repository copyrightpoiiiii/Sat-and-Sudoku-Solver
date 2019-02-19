#ifndef GENERAL
#define GENERAL

#include"General.h"

#endif

template <class T>
void selectionSort(T* array, int size)
{
    int     i, j, best_i;
    T       tmp;

    for (i = 0; i < size-1; i++){
        best_i = i;
        for (j = i+1; j < size; j++){
            if (abs(array[j])<abs( array[best_i]))
                best_i = j;
        }
        tmp = array[i]; array[i] = array[best_i]; array[best_i] = tmp;
    }
}

template <class T>
void sort(T* array, T* end)
{
    int size=end-array;
    if (size <= 15)
        selectionSort(array, size);

    else{
        T           pivot = array[rand()%size];
        int         i = -1;
        int         j = size;

        for(;;){
            do i++; while(abs(array[i])<abs( pivot));
            do j--; while(abs(pivot)<abs( array[j]));

            if (i >= j) break;

            swap(array[i],array[j]);
        }

        sort(array    , array+i);
        sort(&array[i], (&array[i])+(size-i));
    }
}