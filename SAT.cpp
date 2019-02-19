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