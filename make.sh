#!/bin/sh

rm -rf *.o
rm -rf sudoku
#compile SudokuClass.o
g++ --std=c++11 -pthread -c SudokuClass.cpp
#compile SudokuMain.o
g++ --std=c++11 -pthread -c SudokuMain.cpp
#build sudoku
g++ -pthread -o sudoku SudokuClass.o SudokuMain.o


