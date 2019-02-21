#include"Sudoku.h"
#include"vector.hpp"
#include"set.hpp"
#include"queue.hpp"
#include"heap.hpp"
#include"sort.hpp"
#ifndef GENERAL
#define GENERAL
#include"General.hpp"
#endif

#define inf 1e9

Cnf clauses;
priority_queue<pair<int,int> > q;
int dpll_status;


int readint() ;

void init() ;