#ifndef GENERAL
#define GENERAL
#include"General.h"
#endif
#include"Cnf.h"

#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
typedef int status;			//状态返回值
class Sudoku{
    private:
        int note_size;//提示数个数

        inline void random_shuffle(int* shuffle);
        
        inline void swapRow(int x,int y);

        inline void swapCol(int x,int y);

        inline void trans();
        
    public:
        int mp[9][9];//9*9的数独局面
        inline Sudoku(){
            memset(mp,0,sizeof(mp));
            note_size=20;
        }

        inline bool check();

        inline void clear();

        status initSudokuMap(int hint_num);

        inline unsigned long hint_num();

        Cnf transform();

};