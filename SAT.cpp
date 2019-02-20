#include "SAT.h"

int main()
{
    int op = 1;
    while (op)
    {
        //system("cls");
        printf("\n\n");
        printf("      Menu for Sudoku and Sat Solver \n");
        printf("------------------------------------------------------------\n");
        printf("    	  1. Sudoku Solver             2. Sat Solver\n");
        printf("    	  0. Exit\n");
        printf("------------------------------------------------------------\n");
        printf("    请选择你的操作[0~14]:\n");
        op = readint();
        switch (op)
        {
        case 1: //数独求解器
            
            break;
        case 2: //Sat求解器
            
            break;
        case 0: //退出
            break;
        default: //如果输入了非法的操作编号
            printf("请输入正确的操作编号！\n");
            break;
        } //end of switch
        getchar();
        if (op != 0)
            system("clear");
    } //end of while
    printf("即将退出系统！\n");
    return 0;
}

bool solve(){
    for(int i=0;i<clauses.cnf_set.size();i++)
        q.push(make_pair((int)clauses.cnf_set[i].size(),i));
    while(!q.empty()){
        for(pair<int,int>i=q.top();clauses.clause_info[i.first].size!=i.second;q.pop());
        pair<int,int> cho_cla=q.top();q.pop();
    }
}

pair<int,int> find_mini_clause(){
    pair<int,int> info;
    info.first=inf;info.second=-1;
    for(int i=0;i<clauses.cnf_set.size();i++)
        if(clauses.clause_info[i].exist){
            if(clauses.clause_info[i].size<info.first)
                info=make_pair(clauses.clause_info[i].size,i);
        }
    return info;
}

bool origin_dpll(){
    while(1){
        pair<int,int> min_clause=find_mini_clause();
        if(min_clause.second==-1)return true;
        else if(min_clause.first==0)return false;
        else if(min_clause.second==1&&(!dpll_status)){
            //clauses.lit_set[abs(clauses.cnf_set[min_clause.second][0])]=(clauses.cnf_set[min_clause.second][0]>0);
            //clauses.[min_clause.second].
            int lit_id=clauses.clause_info[min_clause.second].l1;
            int choose_lit=clauses.cnf_set[min_clause.second][lit_id];
            clauses.lit_set[abs(choose_lit)]=choose_lit>0;
            clauses.clause_info[min_clause.second]=(info){false,0,lit_id,lit_id};
            for(int i=0;i<clauses.cnf_set.size();i++)
                if(clauses.clause_info[i].exist){
                    clauses.clause_info[i].size--;
                    int left=clauses.clause_info[i].l1,right=clauses.clause_info[i].l2;
                    while(left<=right){
                        while(clauses.lit_set[abs(clauses.cnf_set[i][left])]!=UNDEFINE)left++;
                        while(clauses.lit_set[abs(clauses.cnf_set[i][right])]!=UNDEFINE)right--;
                    }
                    clauses.clause_info[i].l1=left;
                    clauses.clause_info[i].l2=right;
                    if(left>right)
                        clauses.clause_info[i].exist=false;
                }
        }
    }
    
}

void init(){
    int n=readint(),m=readint();
    clauses.literal_size=n;
    while(m--){
        myVector<int> new_clause;
        new_clause.clear();
        int x=readint();
        while(x){
            new_clause.push_back(x);
            x=readint();
        }
        new_clause.qsort(0,new_clause.size());
        clauses.cnf_set.push_back(new_clause);
    }   
    clauses.init();
}