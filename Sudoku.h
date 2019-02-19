#ifndef GENERAL
#define GENERAL
#include"General.hpp"
#endif
#include"Cnf.hpp"
#define binary_conversion(x,y,z) ((x*9*9)+(y*9)+z)

#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
typedef int status;			//状态返回值
class Sudoku{
    private:
        myVector<int> rec[9][9];
        
        int book[10];

        int note_size;//提示数个数

        inline void random_shuffle(int* shuffle);
        
        inline void swap_row(int x,int y);

        inline void swap_col(int x,int y);

        inline void trans();

        inline void enum_row();

        inline void enum_col();

        inline void enum_grid();

        inline void clear_num_rec();

        inline void naked_single();

        inline void hidden_single();

        inline void intersection_removal();

        inline void clear_other_num(int row,int col,int num);

        inline void find_single_number(int* que,int& r);

        //inline void update(int row,int col,int num,int* que,int& r);

    public: 
        int mp[9][9];//9*9的数独局面

        Cnf Sudoku_trans;
        
        inline Sudoku(){
            memset(mp,0,sizeof(mp));
            note_size=20;
        }

        inline bool check();

        inline void clear();

        status init_SudokuMap(int hint_num);

        inline unsigned long hint_num();

        void transform();

};