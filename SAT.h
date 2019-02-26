#include"Sudoku.h"
#include"data_structure/vector.hpp"
#include"data_structure/set.hpp"
#include"data_structure/queue.hpp"
#include"data_structure/heap.hpp"
#include"data_structure/sort.hpp"
#include"solve/solve.h"
#ifndef GENERAL
#define GENERAL
#include"General.hpp"
#endif

#define inf 1e9

Cnf clauses;
//<pair<int,int> > q;
int dpll_status;
Solver S;


int readint() ;

void init() ;