#ifndef SAT
#define  SAT
#include"../SAT.h"
#endif

pair<int, int> find_mini_clause () ;

void update (int lit_id,int status) ;

void update_clause_statu(int lit_id,bool lit_statu) ;

void update_aft_assign (pair<int, int> lit_info) ;

bool origin_dpll () ;