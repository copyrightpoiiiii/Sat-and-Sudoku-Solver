#pragma once
#ifndef GENERAL
#define GENERAL

#include"General.hpp"

#endif
#define binary_conversion(x, y, z) (((x+1)*100)+((y+1)*10)+z)

#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
typedef int status;            //状态返回值

class Sudoku {
private:
	myVector<int> rec[9][9];

	int book[10];

	int note_size;//提示数个数

	void random_shuffle (int *shuffle);

	void swap_row (int x, int y);

	void swap_col (int x, int y);

	void trans ();

	void enum_row ();

	void enum_col ();

	void enum_grid ();

	void clear_num_rec ();

	void naked_single ();

	void hidden_single ();

	void intersection_removal ();

	void clear_other_num (int row, int col, int num);

	void find_single_number (int *que, int &r);

	//void update(int row,int col,int num,int* que,int& r);

public:
	int mp[9][9];//9*9的数独局面

	int literal_size;

	myVector<myVector<int> > Sudoku_trans;

	Sudoku () {
		memset (mp, 0, sizeof (mp));
		note_size = 20;
	}

	bool check ();

	void clear ();

	status init_SudokuMap (int hint_num = 20);

	unsigned long hint_num ();

	void transform ();

	void output ();

	void print ();

};