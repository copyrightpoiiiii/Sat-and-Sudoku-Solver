#ifndef GENERAL
#define GENERAL

#include"General.h"

#endif
#include"vector.hpp"

template <typename value_t, typename Comp = less<value_t> >
class priority_queue {
public:
  priority_queue() {
    length = 0;
    data.clear();
    // data.push_back(value_t(0));
  }
  int size() const { return length; }
  void push(const value_t &x) {
    data.push_back(x);
    length++;
    int point = length;
    while (point != 1) {
      if (data[(point) / 2 - 1] < data[point - 1]) {
        swap(data[point - 1], data[(point) / 2 - 1]);
        point = (point) / 2;
      } else
        break;
    }
  };
  const value_t &top() const { return data[0]; }
  void pop() {
    data[0] = data[length - 1];
    data.pop_back();
    length--;
    if (!length)
      return;
    int point = 1;
    while (1) {
      int lChild = 2 * (point);
      int rChild = lChild + 1;
      if (lChild > length)
        break;
      if (rChild > length) {
        if (data[lChild - 1] > data[point - 1])
          swap(data[lChild - 1], data[point - 1]);
        break;
      }
      int minChild;
      if (data[lChild - 1] < data[rChild - 1])
        minChild = rChild;
      else
        minChild = lChild;
      if (data[minChild - 1] > data[point - 1]) {
        swap(data[minChild - 1], data[point - 1]);
        point = minChild;
      } else
        break;
    }
  }
  ~priority_queue() {
    data.clear();
    length = 0;
  }

private:
  int length;
  myVector<value_t> data;
};