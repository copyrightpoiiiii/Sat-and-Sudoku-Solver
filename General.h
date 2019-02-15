#include<stdint.h>
#include<cstdlib>
#include<cstdio>
#include<string.h>
#define binary_conversion(x,y,z) ((x*9*9)+(y*9)+z)

template <class T> void swap ( T& a, T& b ){
  T c(a); a=b; b=c;
}

template <typename T> T max (T a, T b ){
  return a>b?a:b;
}
