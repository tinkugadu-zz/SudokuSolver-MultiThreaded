#ifndef SUDOKU_H
#define SUDOKU_H

#include<set>
#include<vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
#include  <thread>
#include <mutex>
#include <queue>
#include <utility>

using namespace std;

typedef unsigned int uint;

enum traversal{
    ROW,
    COL,
    ZONE
};

class Cell
{
    uint row_id;
    uint col_id;
    uint value;
    std::set<uint> possibles;
public:
    Cell(uint row, uint col, uint size);
    Cell(uint row, uint col, uint val, uint size);
    ~Cell();
    bool isEmpty();
    uint getVal();
    void setVal(uint val);
    bool removePossibleValue(uint val);
    uint getRowId();
    uint getColId();
    void dump();
    std::vector<uint> getPossibleValues();
};

class SudokuClass
{
    static std::mutex printMutex;
    uint _size;
    uint _sqSize;
    std::vector<Cell> _cells;
    static void threadFillPossibles(uint thr_id, traversal opt, std::vector<Cell> *cells,
                std::queue<std::pair<uint, uint>> *inds);
    static void threadFillUniqueValues(uint thr_id, traversal opt, std::vector<Cell> *cells,
                std::queue<std::pair<uint, uint>> *inds);
    SudokuClass() = delete;
    void fixRemainingCells(std::queue<std::pair<uint, uint>> &indices);
public:
    SudokuClass(uint size);
    SudokuClass(std::vector<std::vector<uint>> mat);
    ~SudokuClass();
    void printPuzzle();
    void dump();
    void fillPossibleCells(traversal opt);
    bool validate();
    bool complete();
    bool validateRows(bool checkComplete=false, int row = -1);
    bool validateColumns(bool checkComplete=false, int col= -1);
    bool validateZones(bool checkComplete=false, int zone= -1);
    void fillUniqueValues(traversal opt); 
};

#endif //SUDOKU_H
