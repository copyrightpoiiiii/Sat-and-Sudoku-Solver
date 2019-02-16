#include<stdint.h>
#include<cstdlib>
#include<cstdio>
#include<string.h>

template <class T> void swap ( T& a, T& b ){
  T c(a); a=b; b=c;
}

template <typename T> T max (T a, T b ){
  return a>b?a:b;
}
