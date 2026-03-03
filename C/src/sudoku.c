
#include "sudoku.h"


#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*helper functions*/

int is_valid(Grid_T g, int row, int col, int num) {
    int i;
    for ( i = 0; i < 9; i++) {
        /* Check if num is not already placed in current row, current column and current 3x3 grid*/
        if (g.cell[row][i].choices[0] == num || g.cell[i][col].choices[0] == num ||
            g.cell[row - row % 3 + i / 3][col - col % 3 + i % 3].choices[0] == num)
            return 0;
    }
    return 1;
}

int find_empty_location(Grid_T g, int *row, int *col) {
    for (*row = 0; *row < 9; (*row)++)
        for (*col = 0; *col < 9; (*col)++)
            if (g.cell[*row][*col].choices[0] == 0)
                return 1;/*found an empty cell*/
    return 0;/*no empty cell found*/
}
/* Solve g and return the solution; if g has multiple solutions, return one of them.
c can help you iterate over all cells in g */
Grid_T sudoku_solve(Grid_T g, Choice_T c) {
    int row, col;
    int n;
    Grid_T g_copy;
    Grid_T result;
    while (c.n != 0) { /*while cell is not empty*/
        g = grid_update(g, c); /* update value of c.i,c.j to c.n and eliminate c from choices in grid */
        g.cell[c.i][c.j].count = 0;
        c = grid_exist_unique(g);
    }

    /*find an empty location in the grid*/
    if (!find_empty_location(g, &row, &col)) {
        g.unique = 1; /*puzzle has unique-choice solution*/
        return g; /* Puzzle solved*/
    }
    /*generate possible choices for the empty cell*/

    for ( n = 1; n <= 9; n++) {
        if (is_valid(g, row, col, n)) {
            Choice_T temp_choice0 = {0, 0, 0};
            Choice_T temp_choice = {row, col, n};
            g_copy = grid_update(g, temp_choice);

            /*g_copy = grid_update(g, (Choice_T){row, col, n});
             */
            result = sudoku_solve(g_copy, temp_choice0);
            if (grid_unique(result)) return result;
        }
    }
    return g;
}
/* Generate and return a sudoku puzzle with "approximately" nelts
elements having non-0 value. The smaller nelts the harder may be to
generate/solve the puzzle. For instance, nelts=81 should return a
completed and correct puzzle. If unique=1 the puzzle must have a
unique choice at each step of finding the solution. */
Grid_T sudoku_generate(int nelts, int unique) {
    Grid_T g = {0};
    int values[9][9] = {0};
    Choice_T temp_choice = {0, 0, 0};
    int filled = 81;

    g = grid_init(g, values);
    g = sudoku_solve(g, temp_choice);

   /* g = sudoku_solve(g, (Choice_T){0, 0, 0});
*/
    srand(getpid());

    while (filled > nelts) {
        int i = rand() % 9;
        int j = rand() % 9;

        if (g.cell[i][j].choices[0] != 0) {
            int backup = g.cell[i][j].choices[0];
            int temp[9][9];
            int x,y;
            Grid_T candidate;
            Grid_T solved;
            g.cell[i][j].choices[0] = 0;
            for ( x = 0; x < 9; x++)
                for ( y = 0; y < 9; y++)
                    temp[x][y] = g.cell[x][y].choices[0];

             candidate = grid_init((Grid_T){0}, temp);
             solved = sudoku_solve(candidate, temp_choice);

            if (!unique || grid_unique(solved)) {
                filled--;
            } else {
                g.cell[i][j].choices[0] = backup;
            }
        }
    }
    return g;
}
/* Read a sudoku puzzle from stdin and return an object Grid_T
initialized to the values read. The input has the format:
1 2 3 4 5 6 7 8 9
4 5 6 7 8 9 1 2 3
7 8 9 1 2 3 4 5 6
2 3 4 5 6 7 8 9 1
5 6 7 8 9 1 2 3 4
8 9 1 2 3 4 5 6 7
3 4 5 6 7 8 9 1 2
6 7 8 9 1 2 3 4 5
9 1 2 3 4 5 6 7 8
Each number is followed by a space. Each line is terminated with '\n'.
Values of 0 indicate empty puzzle cells. */
Grid_T sudoku_read(void) {
    int puzzle[9][9];
    int i,j;
    Grid_T g = {0};
    for ( i = 0; i < 9; i++)
        for ( j = 0; j < 9; j++)
            scanf("%d", &puzzle[i][j]);
    return grid_init(g, puzzle);
}
/* Print g to stream s in the same format as expected by sudoku_read(). */
void sudoku_print(FILE *s, Grid_T g) {
    int i,j;
    for ( i = 0; i < 9; i++) {
        for ( j = 0; j < 9; j++) {
            fprintf(s, "%d ", g.cell[i][j].choices[0]);
        }
        fprintf(s, "\n");
    }
}
/* Return true iff g is complete and filled in correctly. */
int sudoku_is_correct(Grid_T g) {
    int i,j;
    int box;
    for ( i = 0; i < 9; i++) {
        int row[10] = {0}, col[10] = {0};
        for ( j = 0; j < 9; j++) {
            int rv = g.cell[i][j].choices[0];
            int cv = g.cell[j][i].choices[0];
            if (rv == 0 || cv == 0 || row[rv]++ || col[cv]++) return 0;
        }
    }
    for ( box = 0; box < 9; box++) {
        int box_vals[10] = {0};
        for ( i = 0; i < 3; i++) {
            for ( j = 0; j < 3; j++) {
                int val = g.cell[(box/3)*3+i][(box%3)*3+j].choices[0];
                if (val == 0 || box_vals[val]++) return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        Choice_T temp_choice0 = {0, 0, 0};

        Grid_T g = sudoku_read(); /*read sudoku puzzle from stdin*/
        fprintf(stderr, "Input puzzle:\n");
        sudoku_print(stderr, g); /*print input puzzle to stderr*/

        g = sudoku_solve(g, temp_choice0); /*start from cell (0,0)*/

        if (grid_unique(g) && sudoku_is_correct(g)) fprintf(stderr, "Unique solution found.\n");
        else fprintf(stderr, "No unique solution.\n");

        if (sudoku_is_correct(g)) fprintf(stderr, "Correct puzzle.\n");
        else fprintf(stderr, "Incorrect puzzle.\n");

        sudoku_print(stdout, g); /*print solution to stdout*/

    } else if (argc == 2 && strcmp(argv[1], "-c") == 0) {
        Choice_T temp_choice0 = {0, 0, 0};

        Grid_T g = sudoku_read();
        fprintf(stderr, "Input puzzle:\n");
        sudoku_print(stderr, g);
        g = sudoku_solve(g, temp_choice0); /*start from cell (0,0)*/


        if (sudoku_is_correct(g)) fprintf(stderr, "Correct puzzle.\n");
        else fprintf(stderr, "Incorrect puzzle.\n");

    } else if ((argc == 4 || argc == 3) && strcmp(argv[1], "-g") == 0) {
        int nelts = atoi(argv[2]);
        int unique = (argc == 4 && strcmp(argv[3], "-u") == 0) ? 1 : 0;
        Grid_T g = sudoku_generate(nelts, unique);
        sudoku_print(stdout, g);

    } else {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  sudoku: Solve Sudoku puzzle\n");
        fprintf(stderr, "  sudoku -c: Check correctness of Sudoku puzzle\n");
        fprintf(stderr, "  sudoku -g nelts: Generate Sudoku puzzle\n");
        fprintf(stderr, "  sudoku -g nelts -u: Generate Sudoku puzzle with unique solution\n");
        return 1;
    }
    return 0;
}
