#include "SAT.h"

int readint(){
    int x=0,f=1;char ch=getchar();
    while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
    while(ch>='0'&&ch<='9'){x=10*x+ch-'0';ch=getchar();}
    return x*f;
}

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