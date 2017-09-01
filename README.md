# SudokuSolver-MultiThreaded
Tool that solves sudoku puzzles. 
Currently it solves easy and medium puzzles. traversing through each empty cell and analyzing possible values to fill based on neighboring row, column and zones.

Designed a multi-threaded approach to parallelize processing every row independently on each thread
Every row process, column process or zone process will be processed in a different thread

This tool is designed to solve a sudoku puzzle of any size. 4, 9, 16, 25. Thats right, a perfect square board.

Release Version 1:
This is the very first version. for now, run make.sh script. script creates executable sudoku
call sudoku with file containing puzzle as argument.
Eg: ./sudoku puzzle1


If the puzzle is solved, puzzle solved message is displayed and displays a completely solved puzzle
If the puzzle is not solved yet, incomplete puzzle with possible answers for every cell is displayed.

Feel free to modify and share this tool. Happy solving!!
