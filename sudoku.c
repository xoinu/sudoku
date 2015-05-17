/* Sudoku Solver */
#include <stdio.h>

static int num_empty_cells(int mtx[9][9])
{
  int n = 0;

  for (int i = 0; i < 9; ++i) {
    int* cols = mtx[i];
    for (int j = 0; j < 9; ++j) {
      if (!cols[j]) ++n;
    }
  }

  return n;
}

static void print_mtx(int mtx[9][9])
{
  for (int i = 0; i < 9; ++i) {
    int* row = mtx[i];

    if (0 == (i%3))
      printf("+---+---+---+\n");

    printf("|%d%d%d|%d%d%d|%d%d%d|\n",
	   row[0],row[1],row[2],
	   row[3],row[4],row[5],
	   row[6],row[7],row[8]);
  }
  printf("+---+---+---+\n");
}

static void calc_candidates_at(
			       int mtx[9][9],
			       int row,
			       int col,
			       int cand[9],
			       int* num
			       )
{
  *num = 0;

  if (mtx[row][col])
    return;

  int flags[9] = {0};
  int* cols = mtx[row];

  for (int i = 0; i < 9; ++i) {
    int v = cols[i];
    if (v) flags[v-1] = 1;

    v = mtx[i][col];
    if (v) flags[v-1] = 1;
  }

  int row_block = (row/3) * 3;
  int col_block = (col/3) * 3;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      int v = mtx[i+row_block][j+col_block];
      if (v) flags[v-1] = 1;
    }
  }

  for (int i  = 0; i < 9; ++i) {
    if (!flags[i]) {
      cand[(*num)++] = i+1;
    }
  }
}

static void calc_candidates(int mtx[9][9],
			    int min_cand[9],
			    int* num_min_cand,
			    int* min_cand_row,
			    int* min_cand_col
			    )
{
  int cand[9] = {0};
  int num = 0;
  *num_min_cand = 10;

  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      calc_candidates_at(mtx, i, j, cand, &num);

      if (!num)
        continue;

      printf("[%d,%d] ...", i+1, j+1);
      for (int k = 0; k < num; ++k)
        printf(" %d", cand[k]);
      printf("\n");

      if (num <= *num_min_cand) {
        *num_min_cand = num;
        *min_cand_row = i;
        *min_cand_col = j;

        for (int k = 0; k < num; ++k)
          min_cand[k] = cand[k];

        if (1 == num)
          return;
      }
    }
  }
}

static int solve_one(int mtx[9][9], int n)
{
  if (!n)
    return 0;

  int cand[9] = {0};
  int num_cand = 0;
  int i=0, j=0;

  calc_candidates(mtx, cand, &num_cand, &i, &j);

  if (num_cand == 10)
    return 1;

  for (int k = 0; k < num_cand; ++k) {
    mtx[i][j] = cand[k];

    printf("n=%d\n", n);
    print_mtx(mtx);

    if (!solve_one(mtx, n-1))
      return 0;
    printf("[%d,%d] = %d was a bad guess...\n", i,j, cand[k]);
    mtx[i][j] = 0;
  }

  return 1;
}

int main(int argc, char* argv[])
{
  /*   int mtx[9][9] = { */
  /*     { 0,0,2,6,4,0,0,0,8 }, */
  /*     { 3,0,0,0,0,7,0,4,0 }, */
  /*     { 0,0,0,0,0,0,0,5,0 }, */
  /*     { 5,1,0,4,0,0,3,0,0 }, */
  /*     { 0,0,7,0,0,0,4,0,0 }, */
  /*     { 0,0,9,0,0,1,0,7,2 }, */
  /*     { 0,7,0,0,0,0,0,0,0 }, */
  /*     { 0,9,0,7,0,0,0,0,4 }, */
  /*     { 2,0,0,0,3,6,1,0,0 } */
  /*   }; */
  int mtx[9][9] = {
    { 0,0,0,0,8,0,0,6,0 },
    { 0,0,0,1,6,0,5,0,8 },
    { 8,9,0,0,4,3,0,0,2 },
    { 0,5,9,0,0,0,0,0,6 },
    { 0,0,0,0,0,0,0,0,0 },
    { 4,0,0,0,0,0,8,5,0 },
    { 2,0,0,9,5,0,0,3,7 },
    { 6,0,5,0,2,1,0,0,0 },
    { 0,4,0,0,3,0,0,0,0 }
  };

  print_mtx(mtx);
  int n = num_empty_cells(mtx);

  if (!solve_one(mtx, n)) {
    printf("Solved.\n");
  }
  else {
    printf("Gee!!!\n");
  }

  return 0;
}
