/* Full implementation of the assignment based on your request */

#include "grid.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* init g with values from array v */
Grid_T grid_init(Grid_T g, int v[9][9]) {
    int i, j, k;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (v[i][j] != 0) {
                Choice_T choice = {i, j, v[i][j]};
                g = grid_update(g, choice);
            } else {
                g.cell[i][j].count = 9;
                for (k = 1; k <= 9; k++) {
                    g.cell[i][j].choices[k] = 1;
                }
                g.cell[i][j].choices[0] = 0;
            }
        }
    }
    g.unique = 0;
    return g;
}
/* update value of c.i,c.j to c.n and eliminate c from choices in grid */
Grid_T grid_update(Grid_T g, Choice_T c) {
    int i = c.i, j = c.j, n = c.n, k;
    int subgrid_row = i / 3 * 3;
    int subgrid_col = j / 3 * 3;
    int l, m;
    g.cell[i][j].count = 0;
    for ( k = 1; k <= 9; k++) g.cell[i][j].choices[k] = 0;
    g.cell[i][j].choices[0] = n;

    for ( k = 0; k < 9; k++) {
        g.cell[i][k].choices[n] = 0;
        g.cell[k][j].choices[n] = 0;
    }

    for ( l = subgrid_row; l < subgrid_row + 3; l++) {
        for ( m = subgrid_col; m < subgrid_col + 3; m++) {
            g.cell[l][m].choices[n] = 0;
        }
    }
    return g;
}
/* iterate over all choices in all cells starting from (t.i,t.j,t.n+1).
If no choice is left, return (0,0,0) */
Choice_T grid_iterate(Grid_T g, Choice_T t) {
    int i, j, n;
    Choice_T temp_choice0 = {0, 0, 0};

    for ( i = t.i; i < 9; i++) {
        for ( j = (i == t.i ? t.j : 0); j < 9; j++) {
            for ( n = (i == t.i && j == t.j ? t.n + 1 : 1); n <= 9; n++) {
                if (g.cell[i][j].choices[n] == 1)
                    return (Choice_T){i, j, n};
            }
        }
    }
    return temp_choice0;
}
/*helper functions*/
/*

static int find_empty(Grid_T g, int *row, int *col) {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (g.cell[i][j].choices[0] == 0) {
                *row = i;
                *col = j;
                return 1;
            }
    return 0;
}


static int is_valid(Grid_T g, int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (g.cell[row][i].choices[0] == num) return 0;
        if (g.cell[i][col].choices[0] == num) return 0;
    }

    int box_row = row / 3 * 3;
    int box_col = col / 3 * 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (g.cell[box_row + i][box_col + j].choices[0] == num)
                return 0;

    return 1;
}


static void count_solutions(Grid_T g, int *count) {
    if (*count > 1) return;

    int row, col;
    if (!find_empty(g, &row, &col)) {
        (*count)++;
        return;
    }

    for (int num = 1; num <= 9; num++) {
        if (is_valid(g, row, col, num)) {
            Grid_T g_copy = g;
            g_copy.cell[row][col].choices[0] = num;
            count_solutions(g_copy, count);
        }
    }
}


int grid_unique(Grid_T g) {
    int count = 0;
    count_solutions(g, &count);
    return (count == 1);
}
*/
int grid_unique(Grid_T g) {

    return g.unique;
}

/* clear unique flag */
Grid_T grid_clear_unique(Grid_T g) { g.unique = 0; return g; }
/* return a cell with a unique choice, if one exists, otherwise return (0,0,0) */
Choice_T grid_exist_unique(Grid_T g) {
    int i,j,k;
    Choice_T temp_choice0 = {0, 0, 0};

    for ( i = 0; i < 9; i++) {
        for ( j = 0; j < 9; j++) {
            if (g.cell[i][j].count == 1) {
                for ( k = 1; k <= 9; k++) {
                    if (g.cell[i][j].choices[k] == 1) return (Choice_T){i, j, k};
                }
            }
        }
    }
    return temp_choice0;
}
/* return value of i,j */
Choice_T grid_read_value(Grid_T g, Choice_T c) {
    return (Choice_T){c.i, c.j, g.cell[c.i][c.j].choices[0]};
}
