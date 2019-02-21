#include "SAT.h"

int main () {
	int op = 1;
	while (op) {
		//system("cls");
		printf ("\n\n");
		printf ("      Menu for Sudoku and Sat Solver \n");
		printf ("------------------------------------------------------------\n");
		printf ("    	  1. Sudoku Solver             2. Sat Solver\n");
		printf ("    	  0. Exit\n");
		printf ("------------------------------------------------------------\n");
		printf ("    请选择你的操作[0~14]:\n");
		op = readint ();
		switch (op) {
			case 1: //数独求解器

				break;
			case 2: //Sat求解器

				break;
			case 0: //退出
				break;
			default: //如果输入了非法的操作编号
				printf ("请输入正确的操作编号！\n");
				break;
		} //end of switch
		getchar ();
		if (op != 0)
			system ("clear");
	} //end of while
	printf ("即将退出系统！\n");
	return 0;
}

bool solve () {
	for (int i = 0; i < clauses.cnf_set.size (); i++)
		q.push (make_pair ((int) clauses.cnf_set[i].size (), i));
	while (!q.empty ()) {
		for (pair<int, int> i = q.top (); clauses.clause_info[i.first].size != i.second; q.pop ());
		pair<int, int> cho_cla = q.top ();
		q.pop ();
	}
}

pair<int, int> find_mini_clause () {
	/*
	pair<int,int> info;
	info.first=inf;info.second=-1;
	for(int i=0;i<clauses.cnf_set.size();i++)
		if(clauses.clause_info[i].exist){
			if(clauses.clause_info[i].size<info.first)
				info=make_pair(clauses.clause_info[i].size,i);
		}
	return info;*/
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

void update () {
	for (int i = 0; i < clauses.cnf_set.size (); i++)
		for (int j = 0, lit; j < clauses.cnf_set[i].size (); j++, lit = clauses.cnf_set[i][j])
			if (lit > 0 && clauses.lit_set[lit] == TRUE) {
				clauses.clause_info[i].exist = false;
				break;
			} else if (lit < 0 && clauses.lit_set[-lit] == FALSE) {
				clasues.clause_info[i].exist = false;
				break;
			} else clauses.clause_info[i].exist = true;
}

bool origin_dpll () {
	while (1) {
		pair<int, int> min_clause = find_mini_clause ();
		if (min_clause.second == -1)return true;
		else if (min_clause.first == 0)return false;
		else if (min_clause.first == 1) {
			int choose_lit;
			for (int i = 0; i < clauses.cnf_set[min_clause.second].size (); i++)
				if (clauses.lit_set[clauses.cnf_set[min_clause.second][i]] == UNDEFINE) {
					choose_lit = clauses.lit_set[clauses.cnf_set[min_clause.second][i]];
					break;
				}
			clauses.clause_info[min_clause.second].exist = false;
			clauses.lit_set[abs (choose_lit)] = choose_lit > 0 ? TRUE : FALSE;
			//clauses.clause_size--;
			//change
			if (clauses.lit_set[abs (choose_lit)] == TRUE) {
				for (int i = 0; i < clauses.rec_pos_lit[choose_lit].size (); i++)
					clauses.clause_info[clauses.rec_pos_lit[choose_lit][i]].exist = false;
			} else {
				for (int i = 0; i < clauses.rec_neg_lit[-choose_lit].size (); i++)
					clauses.clause_info[clauses.rec_neg_lit[-choose_lit][i]].exist = false;
			}
		} else break;
	}
	pair<int, int> min_clause = find_mini_clause ();
	for (int i = 0; i < clauses.cnf_set[min_clause.second].size (); i++)
		if (clauses.lit_set[abs (clauses.cnf_set[min_clause.second][i])] == UNDEFINE) {
			int choose_lit = clauses.cnf_set[min_clause.second][i];
			clauses.lit_set[abs (choose_lit)] = TRUE;
			update ();
			if (origin_dpll ())
				return true;
			clauses.lit_set[abs (choose_lit)] = FALSE;
			update ();
			if (origin_dpll ())
				return true;
			clauses.lit_set[abs (choose_lit)] = UNDEFINE;
			update ();
		}
	return false;
}

void init () {
	int n = readint (), m = readint ();
	clauses.literal_size = n;
	clauses.clause_size = m;
	clauses.rec_pos_lit.capacity (n);
	clauses.rec_neg_lit.capacity (n);
	for (int i = 0; i < m; i++) {
		myVector<int> new_clause;
		new_clause.clear ();
		int x = readint ();
		while (x) {
			new_clause.push_back (x);
			if (x > 0)clauses.rec_pos_lit[x].push_back (i);
			else clauses.rec_neg_lit[-x].push_back (i);
			x = readint ();
		}
		new_clause.qsort (0, new_clause.size ());
		clauses.cnf_set.push_back (new_clause);
	}
	clauses.init ();
}