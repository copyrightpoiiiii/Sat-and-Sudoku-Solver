#ifndef GENERAL
#define GENERAL
#include"General.h"
#endif
#include"Sudoku.h"
#include"vector.hpp"
#include"set.hpp"
#include"queue.hpp"

Cnf clauses;


int readint(){
    int x=0,f=1;char ch=getchar();
    while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
    while(ch>='0'&&ch<='9'){x=10*x+ch-'0';ch=getchar();}
    return x*f;
}

bool solve();

void init();