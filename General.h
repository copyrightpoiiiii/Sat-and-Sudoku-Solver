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

