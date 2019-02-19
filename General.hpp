#include<stdint.h>
#include<cstdlib>
#include<cstdio>
#include<string.h>

#define TRUE 1
#define FALSE 0
#define TRUE_PROPA 2
#define FALSE_PROPA 3
#define UNDEFINE -1

template <class T> void swap ( T& a, T& b ){
  T c(a); a=b; b=c;
}

template <typename T> T max (T a, T b ){
  return a>b?a:b;
}

template <typename T> T abs (T a){
    return a>0?a:-a;
}

template <typename T1, typename T2> struct pair {
  T1 first;
  T2 second;
};

template <typename T1, typename T2>
pair<T1, T1> make_pair(const T1 &first, const T2 &second) {
  return pair<T1, T2>{first, second};
};

template <typename T> class less {
    public:
  bool operator()(const T &a, const T &b) { return a < b; }
};

