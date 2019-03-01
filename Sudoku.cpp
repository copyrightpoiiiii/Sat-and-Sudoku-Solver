#include"Sudoku.h"

void Sudoku::random_shuffle (int *shuffle) {//生成一个9个数的随机乱排
	for (int i = 0; i < 9; i++)
		shuffle[i] = i + 1;
	for (int i = 0; i < 9; i++) {
		int j = rand () % 9;
		swap (shuffle[i], shuffle[j]);
	}
}

void Sudoku::swap_row (int x, int y) {
	for (int i = 0; i < 9; i++)
		swap (mp[x][i], mp[y][i]);
}

void Sudoku::swap_col (int x, int y) {
	for (int i = 0; i < 9; i++)
		swap (mp[i][x], mp[i][y]);
}

void Sudoku::trans () {
	for (int i = 0; i < 9; i++)
		for (int j = i; j < 9; j++)
			swap (mp[i][j], mp[j][i]);
}

bool Sudoku::check () {
	//逐行检查
	for (int row = 0; row < 9; row++) {
		clear_num_rec ();
		for (int col = 0; col < 9; col++)
			book[mp[row][col]]++;
		for (int i = 1; i <= 9; i++)
			if (book[i] != 1)
				return false;
	}
	//逐列检查
	for (int col = 0; col < 9; col++) {
		clear_num_rec ();
		for (int row = 0; row < 9; row++)
			book[mp[row][col]]++;
		for (int i = 1; i <= 9; i++)
			if (book[i] != 1)
				return false;
	}
	//逐块检查
	for (int st_row = 0; st_row < 9; st_row += 3)
		for (int st_col = 0; st_col < 9; st_col += 3) {
			clear_num_rec ();
			for (int row = st_row; row < 3 + st_row; row++)
				for (int col = st_col; col < 3 + st_col; col++)
					book[mp[row][col]]++;
			for (int i = 1; i <= 9; i++)
				if (book[i] != 1)
					return false;
		}
	return true;
}

void Sudoku::clear () {//清空操作
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			mp[i][j] = 0;
}

void Sudoku::clear_num_rec () {//清空book数组
	for (int i = 1; i <= 9; i++)
		book[i] = 0;
}

status Sudoku::init_SudokuMap (int hint_num) {//初始化数独地图
	int shuffle[9];
	random_shuffle (shuffle);
	int row = 3, column = 3;
	//先填最中间的方格
	for (int i = 0; i < 9; i++) {
		mp[row][column] = shuffle[i];
		column++;
		if (column % 3 == 0) {
			row++;
			column = 3;
		}
	}
	//处理方格左右的小方格
	row = 3;
	column = 0;
	for (int i = 3; row < 6; i = (i + 1) % 9) {
		mp[row][column] = shuffle[i];
		column++;
		if (column % 3 == 0) {
			row++;
			column = 0;
		}
	}
	row = 3;
	column = 6;
	for (int i = 6; row < 6; i = (i + 1) % 9) {
		mp[row][column] = shuffle[i];
		column++;
		if (column % 3 == 0) {
			row++;
			column = 6;
		}
	}
	//将三个方格的情况推广到其它两列
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 3; j++)
			mp[j][i] = mp[j + 3][(i + 1) % 9];
	for (int i = 0; i < 9; i++)
		for (int j = 6; j < 9; j++)
			mp[j][i] = mp[j - 3][(i + 2) % 9];
	//为了增加随机性，再进行多次行列变化以及转置操作
	int opt_num = 100;
	while (opt_num--) {
		int opt_size = rand () % 3;
		switch (opt_size) {
			case 0: {//行交换
				row = rand () % 3;//选择是三行中的哪一行进行交换
				int x = rand () % 3, y = ((rand () % 2 + 1) + x) % 3;//选择其中两个
				swap_row (x, y);
				break;
			}
			case 1: {//列交换
				column = rand () % 3;//选择是三列中的哪一列进行交换
				int x = rand () % 3, y = ((rand () % 2 + 1) + x) % 3;//选择其中两个
				swap_col (x, y);
				break;
			}
			case 2://转置
				trans ();
				break;
		}
	}
	//检测数独是否合法
	if (!check ()) {
		clear ();
		return ERROR;
	} else {//挖空
		int tmp = 17 > hint_num ? 17 : hint_num;
		int blank_num = 81 - tmp;
		note_size = hint_num;
		while (blank_num--) {
			int row = rand () % 9, col = rand () % 9;
			while (!mp[row][col]) {
				row = rand () % 9;
				col = rand () % 9;
			}
			mp[row][col] = 0;
		}
		return OK;
	}
}

unsigned long Sudoku::hint_num () {
	return note_size;
}

void Sudoku::enum_row () {
	for (int row = 0; row < 9; row++) {//枚举每一行
		clear_num_rec ();
		for (int col = 0; col < 9; col++)
			if (mp[row][col])
				rec[row][col].push_back (mp[row][col]), book[mp[row][col]] = 1;
		for (int col = 0; col < 9; col++)
			if (!mp[row][col]) {
				for (int i = 1; i <= 9; i++)
					if (!book[i])
						rec[row][col].push_back (i);
			}
	}
}

void Sudoku::enum_col () {
	for (int col = 0; col < 9; col++) {//枚举每一列，做减法
		clear_num_rec ();
		for (int row = 0; row < 9; row++)
			if (mp[row][col])
				book[mp[row][col]] = 1;
		for (int row = 0; row < 9; row++)
			if (!mp[row][col]) {
				int now = 0;
				for (int i = 1; i <= 9; i++)
					if (book[i]) {
						while (now < rec[row][col].size () && rec[row][col][now] < i)
							now++;
						if (now >= rec[row][col].size ())
							break;
						if (rec[row][col][now] == i)
							rec[row][col].erase (now);
					}
			}
	}
}

void Sudoku::enum_grid () {
	for (int st_row = 0; st_row < 9; st_row += 3)//枚举每个小方格，做减法
		for (int st_col = 0; st_col < 9; st_col += 3) {
			clear_num_rec ();
			for (int row = st_row; row < 3 + st_row; row++)
				for (int col = st_col; col < 3 + st_col; col++)
					if (mp[row][col])
						book[mp[row][col]]++;
			for (int row = st_row; row < 3 + st_row; row++)
				for (int col = st_col; col < 3 + st_col; col++)
					if (!mp[row][col]) {
						int now = 0;
						for (int i = 1; i <= 9; i++)
							if (book[i]) {
								while (now < rec[row][col].size () && rec[row][col][now] < i)
									now++;
								if (now >= rec[row][col].size ())
									break;
								if (rec[row][col][now] == i)
									rec[row][col].erase (now);
							}
					}
		}
}

void Sudoku::transform () {
	for (int i = 1; i <= 8829; i++)
		Sudoku_trans.push_back ();
	int tot = 0;
	for (int row = 0; row < 9; row++)
		for (int col = 0; col < 9; col++)
			if (mp[row][col]) {
				myVector<int> clause;
				clause.clear (true);
				clause.push_back (binary_conversion (row, col, mp[row][col]));
				for (int i = 1; i <= 9; i++) {
					for (int j = i + 1; j <= 9; j++) {
						myVector<int> clause_lit;
						clause_lit.clear (true);
						clause_lit.push_back (-binary_conversion (row, col, i));
						clause_lit.push_back (-binary_conversion (row, col, j));
						for (int k = 0; k < clause_lit.size (); k++)
							Sudoku_trans[tot].push_back (clause_lit[k]);
						tot++;
					}
				}
				for (int k = 0; k < clause.size (); k++)
					Sudoku_trans[tot].push_back (clause[k]);
				tot++;
			} else {
				myVector<int> clause;
				clause.clear (true);
				for (int i = 1; i <= 9; i++) {
					clause.push_back (binary_conversion (row, col, i));
					for (int j = i + 1; j <= 9; j++) {
						myVector<int> clause_lit;
						clause_lit.clear (true);
						clause_lit.push_back (-binary_conversion (row, col, i));
						clause_lit.push_back (-binary_conversion (row, col, j));
						for (int k = 0; k < clause_lit.size (); k++)
							Sudoku_trans[tot].push_back (clause_lit[k]);
						tot++;
					}
				}
				for (int k = 0; k < clause.size (); k++)
					Sudoku_trans[tot].push_back (clause[k]);
				tot++;
			}
	for (int row = 0; row < 9; row++)
		for (int num = 1; num <= 9; num++)
			for (int col = 0; col < 9; col++)
				for (int col_eum = col + 1; col_eum < 9; col_eum++) {
					myVector<int> clause;
					clause.clear (true);
					clause.push_back (-binary_conversion (row, col, num));
					clause.push_back (-binary_conversion (row, col_eum, num));
					for (int k = 0; k < clause.size (); k++)
						Sudoku_trans[tot].push_back (clause[k]);
					tot++;
				}
	for (int col = 0; col < 9; col++)
		for (int num = 1; num <= 9; num++)
			for (int row = 0; row < 9; row++)
				for (int row_eum = row + 1; row_eum < 9; row_eum++) {
					myVector<int> clause;
					clause.clear (true);
					clause.push_back (-binary_conversion (row, col, num));
					clause.push_back (-binary_conversion (row_eum, col, num));
					for (int k = 0; k < clause.size (); k++)
						Sudoku_trans[tot].push_back (clause[k]);
					tot++;
				}
	for (int num = 1; num <= 9; num++)
		for (int st_row = 0; st_row < 9; st_row += 3)//枚举每个小方格
			for (int st_col = 0; st_col < 9; st_col += 3)
				for (int row = st_row; row < 3 + st_row; row++)
					for (int col = st_col; col < 3 + st_col; col++) {
						int row_eum = row, col_eum = col + 1;
						if (col_eum >= st_col + 3) {
							row_eum++;
							col_eum = st_col;
						}
						while (row_eum < 3 + st_row) {
							myVector<int> clause;
							clause.clear (true);
							clause.push_back (-binary_conversion (row, col, num));
							clause.push_back (-binary_conversion (row_eum, col_eum, num));
							for (int k = 0; k < clause.size (); k++)
								Sudoku_trans[tot].push_back (clause[k]);
							tot++;
							col_eum++;
							if (col_eum >= st_col + 3) {
								row_eum++;
								col_eum = st_col;
							}
						}
					}
	for (int row = 0; row < 9; row++)
		for (int num = 1; num <= 9; num++) {
			myVector<int> clause;
			clause.clear (true);
			for (int col = 0; col < 9; col++)
				clause.push_back (binary_conversion (row, col, num));
			for (int k = 0; k < clause.size (); k++)
				Sudoku_trans[tot].push_back (clause[k]);
			tot++;
		}
	for (int col = 0; col < 9; col++)
		for (int num = 1; num <= 9; num++) {
			myVector<int> clause;
			clause.clear (true);
			for (int row = 0; row < 9; row++)
				clause.push_back (binary_conversion (row, col, num));
			for (int k = 0; k < clause.size (); k++)
				Sudoku_trans[tot].push_back (clause[k]);
			tot++;
		}
	for (int num = 1; num <= 9; num++)
		for (int st_row = 0; st_row < 9; st_row += 3)//枚举每个小方格
			for (int st_col = 0; st_col < 9; st_col += 3) {
				myVector<int> clause;
				clause.clear (true);
				for (int row = st_row; row < 3 + st_row; row++)
					for (int col = st_col; col < 3 + st_col; col++)
						clause.push_back (binary_conversion (row, col, num));
				for (int k = 0; k < clause.size (); k++)
					Sudoku_trans[tot].push_back (clause[k]);
				tot++;
			}
	int book_var[1000];
	memset (book_var, 0, sizeof (book_var));
	for (int i = 0; i < Sudoku_trans.size (); i++) {
		for (int j = 0; j < Sudoku_trans[i].size (); j++) {
			book_var[abs (Sudoku_trans[i][j])]++;
		}
	}
	for (int i = 1; i < 1000; i++)
		if (book_var[i])
			literal_size++;
}

void Sudoku::print () {
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++)
			printf ("%d", mp[row][col]);
		printf ("\n");
	}
}

void Sudoku::output () {
	FILE *fp;
	char filename[30];
	printf ("请输入输出的文件名: \n");
	scanf ("%s", filename);
	fp = fopen (filename, "w");
	for (int row = 0; row < 9; row++) {
		fprintf (fp, "c     ");
		for (int col = 0; col < 9; col++)
			if (mp[row][col])
				fprintf (fp, "%d", mp[row][col]);
			else fprintf (fp, ".");
		fprintf (fp, "\n");
	}
	transform ();
	fprintf (fp, "p    %d    %d\n", literal_size, Sudoku_trans.size ());
	for (int i = 0; i < Sudoku_trans.size (); i++) {
		fprintf (fp, "p ");
		for (int j = 0; j < Sudoku_trans[i].size (); j++)
			fprintf (fp, "%d ", Sudoku_trans[i][j]);
		fprintf (fp, "0\n");
	}
	fclose (fp);
}