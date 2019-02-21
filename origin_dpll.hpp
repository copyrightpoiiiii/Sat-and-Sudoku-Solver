#ifndef SAT
#define  SAT
#include"SAT.h"
#endif
pair<int, int> find_mini_clause () {
	pair<int, int> info;
	info.first = inf;
	info.second = -1;
	for (int i = 0; i < clauses.cnf_set.size (); i++)
		if (clauses.clause_info[i].exist) {
			int size = 0;
			for (int j = 0; j < clauses.cnf_set[i].size (); j++)
				if (clauses.lit_set[clauses.cnf_set[i][j]] == UNDEFINE)
					size++;
			if (size < info.first) {
				info.first = size;
				info.second = i;
			}
		}
	return info;
}

void update (int lit_id,int status) {
    clauses.lit_set[lit_id] = status;
    int clause_size=clauses.cnf_set.size();
	for (int i = 0 ; i < clause_size; i++)
		for (int j = 0, lit; j < clauses.cnf_set[i].size (); j++){
			lit=clauses.cnf_set[i][j];
            if (lit > 0 && clauses.lit_set[lit] == TRUE) {
				clauses.clause_info[i].exist = false;
				break;
			} else if (lit < 0 && clauses.lit_set[-lit] == FALSE) {
				clauses.clause_info[i].exist = false;
				break;
			} else clauses.clause_info[i].exist = true;
        }
}

void update_clause_statu(int lit_id,bool lit_statu){
    if (lit_statu == TRUE) {
		int num_lit_inclu = clauses.rec_pos_lit[lit_id].size ();
		for (int i = 0; i < num_lit_inclu; i++)
			clauses.clause_info[clauses.rec_pos_lit[lit_id][i]].exist = false;
	} else {
		int num_lit_inclu = clauses.rec_pos_lit[lit_id].size ();
		for (int i = 0; i < num_lit_inclu; i++)
			clauses.clause_info[clauses.rec_neg_lit[lit_id][i]].exist = false;
	}
}

void update_aft_assign (pair<int, int> lit_info) {
	int choose_lit, clause_id = lit_info.second;
	int lit_size = clauses.cnf_set[clause_id].size ();
	for (int i = 0; i < lit_size; i++) {
		int eum_lit = abs (clauses.cnf_set[clause_id][i]);
		if (clauses.lit_set[eum_lit] == UNDEFINE) {
			choose_lit = clauses.cnf_set[clause_id][i];
			break;
		}
	}
	clauses.clause_info[clause_id].exist = false;
	clauses.lit_set[abs (choose_lit)] = choose_lit > 0 ? TRUE : FALSE;
    choose_lit=abs(choose_lit);
    update_clause_statu(choose_lit,clauses.lit_set[choose_lit]);
}

bool origin_dpll () {
    pair<int, int> min_cla;
	while (1) {
		min_cla = find_mini_clause ();
		if (min_cla.second == -1)return true;
		else if (min_cla.first == 0)return false;
		else if (min_cla.first == 1)update_aft_assign(min_cla);
        else break;
	}
    int lit_size=clauses.cnf_set[min_cla.second].size ();
	for (int i = 0; i < lit_size; i++){
        int eum_lit=clauses.cnf_set[min_cla.second][i];
		if (clauses.lit_set[abs (eum_lit)] == UNDEFINE) {
			int choose_lit = abs(clauses.cnf_set[min_cla.second][i]);
			update (choose_lit,TRUE);
			if (origin_dpll ())return true;
			update (choose_lit,FALSE);
			if (origin_dpll ())return true;
			update (choose_lit,UNDEFINE);
		}
    }
	return false;
}