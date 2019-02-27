#pragma once
#ifndef GENERAL
#define GENERAL

#include"../General.hpp"

#endif
#include"vector.hpp"

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
    bool inHeap    (int n)    { assert(ok(n)); return indices[n] != 0; }
    void increase  (int n)    { assert(ok(n)); assert(inHeap(n)); percolateUp(indices[n]); }
    bool empty     ()         { return heap.size() == 1; }

    void insert(int n) {
        assert(ok(n));
        indices[n] = heap.size();
        heap.push_back(n);
        percolateUp(indices[n]); }

    int  getmin() {
        int r            = heap[1];
        heap[1]          = heap.end();
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