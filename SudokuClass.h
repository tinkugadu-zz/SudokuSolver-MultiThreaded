#ifndef SUDOKUCLASS_H
#define SUDOKUCLASS_H

#include "SudokuCell.h"

typedef unsigned int uint;

enum traversal{
    ROW,
    COL,
    ZONE
};

class Cell;
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
    static void threadPairPossibles(uint thr_id, traversal opt, std::vector<Cell> *cells,
                std::queue<std::pair<uint, uint>> *inds);
    SudokuClass() = delete;
    void fixRemainingCells(std::queue<std::pair<uint, uint>> &indices);
    bool isChanged;
    uint _fixedCells;
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
    void pairPossibles(traversal opt);
    //sets to true if a cell value is solved
    void set();
    //resets Sudoku puzzle to false
    void reset();
    bool isPuzzleChanged();
};

#endif //SUDOKUCLASS_H
