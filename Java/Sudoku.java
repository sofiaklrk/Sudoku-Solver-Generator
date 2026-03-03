package A13;//package com.baeldung.algorithms.sudoku;
import java.util.Arrays;
import java.util.Random;
import java.util.stream.IntStream;

public class Sudoku {

    private static final int BOARD_SIZE = 9;
    private static final int SUBSECTION_SIZE = 3;
    private static final int BOARD_START_INDEX = 0;

    private static final int NO_VALUE = 0;
    private static final int MIN_VALUE = 1;
    private static final int MAX_VALUE = 9;

    private static int[][] board = {
            {8, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 3, 6, 0, 0, 0, 0, 0},
            {0, 7, 0, 0, 9, 0, 2, 0, 0},
            {0, 5, 0, 0, 0, 7, 0, 0, 0},
            {0, 0, 0, 0, 4, 5, 7, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 3, 0},
            {0, 0, 1, 0, 0, 0, 0, 6, 8},
            {0, 0, 8, 5, 0, 0, 0, 1, 0},
            {0, 9, 0, 0, 0, 0, 4, 0, 0}
    };
    public static long startTime;
    public static void main(String[] args) {
        Sudoku solver = new Sudoku();
        int n=10;
        int p=0,p1=0,p2=0,p3=0,p4=0;
        boolean flag=true,flag1=true,flag2=true;

        for (int i=0;i<n;i++){
            do {



                board = CreateBoard(75);
                flag1 = solver.isValidBoard(board);
                if (flag1) {
                    p3++;
                    System.out.println("Board #" + p3);
                    for (int i1 = 0; i1 < 9; i1++) {
                        for (int j = 0; j < 9; j++) {
                            System.out.print(board[i1][j] + " ");
                        }
                        System.out.println("\r");
                    }
                    flag2= solver.isSolvableBoard(board);
                    if (!flag2){
                        p1++;
                    }
                }else {
                    //p1++;
                    p2++;
                }
                flag = solver.isSolvableBoard(board);
                if (flag) {
                    p++;
                    //System.out.println("Board #"+p);
                    //solver.printBoard();

                    //System.out.println(Arrays.deepToString(board));
                    //System.out.print(board);
                    //p4=p+1;
                    System.out.println("Solution of Board #" + p);
                    solver.solve(board);
                    solver.printBoard();
                }/*} else {
                    p1++;
                }*/
            }while (!flag1);
        }
        System.out.println("Empty cells per Board:       75");
        System.out.println("Valid Boards Created:        "+p);
        System.out.println("Invalid Boards Created:      "+p2);
        System.out.println("Unsolvable Boards Created:   "+p1);
        long endTime = System.currentTimeMillis();
        float sec=(endTime - startTime) / 1000F;
        System.out.println("Χρόνος εκτέλεσης:" + sec + "seconds");
        //solver.solve(board);
        //solver.printBoard();
    }
    public static int[][] CreateBoard(int x){
        //int[][] b={new int[]{0}};
        int[][] B= new int[9][9];
        for (int[] row : B)
            Arrays.fill(row, 0);
        //Arrays.fill(B,0);
        /*for (int i=0;i<9;i++){
            for (int j=0;j<9;j++){
               B[i][j]=0;
            }
        }*/
        Random randomGenerator = new Random();
        int number,rndmi,rndmj;
        boolean flag;
        //int randomlnt = randomGenerator.nextInt(MAX_VALUE-MIN_VALUE+1)+MIN_VALUE;
        int n=81-x;
        for (int i=0;i<n;i++){
            do {
                flag = false;
                number = randomGenerator.nextInt(9)+1;
                rndmi = randomGenerator.nextInt(9);
                rndmj = randomGenerator.nextInt(9);
                if (B[rndmi][rndmj] == 0) {
                    B[rndmi][rndmj] = number;
                } else {
                    flag = true;
                }
            }while (flag);
        }
        return B;
        //return randomlnt;
    }
    public  boolean isSolvableBoard(int[][] board){

        boolean flag= isValidBoard(board);

        return flag && solve(board);
    }
    boolean isValidBoard(int[][]brd){

        boolean flag=true;
        boolean flag1=true;
        int n=0;
        for (int i=0;i<9;i++){
            for (int j=0;j<9;j++){
                if (!(brd[i][j]>=0&&brd[i][j]<10)){
                    flag=false;
                }
            }
        }
        if (flag) {
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                        if (!isValid(brd,i,j)){
                            flag1=false;
                        }
                }
            }
        }

        return flag && flag1;
    }

    private void printBoard() {
        for (int row = BOARD_START_INDEX; row < BOARD_SIZE; row++) {
            for (int column = BOARD_START_INDEX; column < BOARD_SIZE; column++) {
                System.out.print(board[row][column] + " ");
            }
            System.out.println();
        }
    }

    private boolean solve(int[][] board) {
        for (int row = BOARD_START_INDEX; row < BOARD_SIZE; row++) {
            for (int column = BOARD_START_INDEX; column < BOARD_SIZE; column++) {
                if (board[row][column] == NO_VALUE) {
                    for (int k = MIN_VALUE; k <= MAX_VALUE; k++) {
                        board[row][column] = k;
                        if (isValid(board, row, column) && solve(board)) {
                            return true;
                        }
                        board[row][column] = NO_VALUE;
                    }
                    return false;
                }
            }
        }
        return true;
    }

    private boolean isValid(int[][] board, int row, int column) {
        return rowConstraint(board, row) &&
                columnConstraint(board, column) &&
                subsectionConstraint(board, row, column);
    }

    private boolean subsectionConstraint(int[][] board, int row, int column) {
        boolean[] constraint = new boolean[BOARD_SIZE];
        int subsectionRowStart = (row / SUBSECTION_SIZE) * SUBSECTION_SIZE;
        int subsectionRowEnd = subsectionRowStart + SUBSECTION_SIZE;

        int subsectionColumnStart = (column / SUBSECTION_SIZE) * SUBSECTION_SIZE;
        int subsectionColumnEnd = subsectionColumnStart + SUBSECTION_SIZE;

        for (int r = subsectionRowStart; r < subsectionRowEnd; r++) {
            for (int c = subsectionColumnStart; c < subsectionColumnEnd; c++) {
                if (!checkConstraint(board, r, constraint, c)) return false;
            }
        }
        return true;
    }

    private boolean columnConstraint(int[][] board, int column) {
        boolean[] constraint = new boolean[BOARD_SIZE];
        return IntStream.range(BOARD_START_INDEX, BOARD_SIZE)
                .allMatch(row -> checkConstraint(board, row, constraint, column));
    }

    private boolean rowConstraint(int[][] board, int row) {
        boolean[] constraint = new boolean[BOARD_SIZE];
        return IntStream.range(BOARD_START_INDEX, BOARD_SIZE)
                .allMatch(column -> checkConstraint(board, row, constraint, column));
    }

    private boolean checkConstraint(int[][] board, int row, boolean[] constraint, int column) {
        if (board[row][column] != NO_VALUE) {
            if (!constraint[board[row][column] - 1]) {
                constraint[board[row][column] - 1] = true;
            } else {
                return false;
            }
        }
        return true;
    }
}