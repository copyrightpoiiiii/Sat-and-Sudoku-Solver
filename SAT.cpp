#ifndef SAT
#define  SAT
#include"SAT.h"
#endif
#include<ctime>

Solver S;

int readint(FILE *fp){
    int x=0,f=1;char ch;
    fscanf(fp,"%c",&ch);
    while(ch<'0'||ch>'9'){if(ch=='-')f=-1;fscanf(fp,"%c",&ch);}
    while(ch>='0'&&ch<='9'){x=10*x+ch-'0';fscanf(fp,"%c",&ch);}
    return x*f;
}

int readint(){
    int x=0,f=1;char ch=getchar();
    while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
    while(ch>='0'&&ch<='9'){x=10*x+ch-'0';ch=getchar();}
    return x*f;
}

void init () {
    FILE *fp;
    char filename[30];
    printf("请输入读入的文件名: \n");
    scanf("%s", filename);
    if ((fp = fopen(filename, "r")) == NULL)
    {
        printf("文件不存在！");
        return ;
    }
	int n = readint (fp), m = readint (fp);
	clauses.literal_size = n;
	clauses.clause_size = m;
	clauses.rec_pos_lit.capacity (n);
	clauses.rec_neg_lit.capacity (n);
    myVector<Lit> sat_clause;
	for (int i = 0; i < m; i++) {
		myVector<int> new_clause;
        sat_clause.clear();
		new_clause.clear ();
		int x = readint (fp);
        Lit tmp;
		while (x) {
			new_clause.push_back (x);
			if (x > 0)clauses.rec_pos_lit[x].push_back (i);
			else clauses.rec_neg_lit[-x].push_back (i);
            tmp.x=x;
            sat_clause.push_back(tmp);
			x = readint (fp);
		}
		new_clause.qsort (0, new_clause.size ());
		clauses.cnf_set.push_back (new_clause);
        
        S.addClause(sat_clause);
	}
	clauses.init ();
    fclose(fp);
}

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

void minisat_solve(){
    clock_t start=clock();
    S.solve();
    clock_t finish=clock();
    printf("time used:%lf\n",(double)finish-start);
}

void naive_solve(){
    clock_t start=clock();
    origin_dpll();
    clock_t finish=clock();
    printf("time used:%lf\n",(double)finish-start);
}

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
            case 3://MiniSat

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