#include <sstream>
#include <fstream>
#include <stdlib.h>
#include "SudokuClass.h"

typedef unsigned int uint;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout<<"Enter puzzle name to solve"<<endl;
        exit(-1);
    }
    /*file contents should be 
        first line - size of puzzle 9 (for 9 X 9), 4(for 4X4 )
    */
    std::string filename(argv[1]);
    cout<<"filename processing: "<<filename<<endl;
    std::fstream filePtr;
    filePtr.open(filename.c_str(), std::fstream::in);
    if(!filePtr.is_open())
    {
        cout<<"error opening filename. Check if file exists"<<endl;
        filePtr.close();
        exit(-2);
    }
    std::string tmpStr;
    std::stringstream sstream;
    uint puzzlesize, line=0;
    //get size of puzzle
    if(getline(filePtr, tmpStr))
    {
        sstream<<tmpStr;
        sstream>>puzzlesize;
        uint val = (uint)sqrt((double)puzzlesize);
        if(val*val != puzzlesize)
        {
            cout<<"invlalid puzzle size. size should be perfect square 4,9,16 etc"<<endl;
            filePtr.close();
            exit(-3);
        }
    }
    else
    {
        cout<<"wrong format of input file"<<endl;
        filePtr.close();
        exit(-3);
    }
    sstream.clear();
    int tmp;
    std::vector<std::vector<uint>> tmpPuzzle;
    tmpPuzzle.reserve(puzzlesize);
    while(getline(filePtr, tmpStr))
    {
        ++line;
        sstream << tmpStr;
        auto ind = 0;
        std::vector<uint> tmpRow;
        tmpRow.reserve(puzzlesize);
        while(ind < puzzlesize)
        {
            sstream>>tmp;
            tmpRow.push_back(tmp);
//            cout<<"tmp value : "<<tmp<<endl;
            ++ind;
        }
        if(tmpRow.size() != puzzlesize)
        {
            cout<<"incomplete puzzle row at line: "<<line<<endl;
            break;
        }
        //add row vector to tmpPuzzle
        tmpPuzzle.push_back(tmpRow);
        if(line == puzzlesize)
        {
            //if the file has more content, just ignore that.
            break;
        }
        sstream.clear();
        sstream.flush();
    }
    if(line < puzzlesize)
    {
        cout<<"incomplete puzzle content in file"<<endl;
        exit(-4);
    }
    filePtr.close();
    auto printRow = [](std::vector<uint> &row){
        for(auto &it:row)
        {
            cout<< it<<"\t"; 
        }
        cout<<endl;
        };
    cout<<"printing tmpPuzzle"<<endl;
#if 0
    for(auto &it:tmpPuzzle)
    {
        printRow(it);
    }
#endif
    SudokuClass mySudoku(tmpPuzzle);
#if 0
    cout<<"printing sudoku puzzle from class"<<endl;
    mySudoku.printPuzzle();
#endif
    if(!mySudoku.validate()) 
    {
        cout<<"entered puzzle cannot be solved."<<endl;
        exit(-4);
    }
#if 0
    uint iter = 0;
    while(!mySudoku.complete())
    {
        ++iter;
        if(iter == 25) break;
    }
#endif
//this step is to blindly fill all possible values of an empty cell
    mySudoku.fillPossibleCells(ROW);
    mySudoku.fillPossibleCells(COL);
    mySudoku.fillPossibleCells(ZONE);
    if(mySudoku.complete())
    {
        cout<<"puzzle is solved"<<endl;
        mySudoku.dump();
        exit(0);
    }
    else
        cout<<"puzzle is not solved"<<endl;

    auto iter = 0;
    while(mySudoku.isPuzzleChanged())
    {
        mySudoku.reset();
        ++iter;        
//this step is to find identical possible pairs within row, col, zone and 
//eliminate them as possible values in neighboring empty cells
        mySudoku.pairPossibles(ROW);
        mySudoku.pairPossibles(COL);
        mySudoku.pairPossibles(ZONE);
        if(mySudoku.complete())
        {
            cout<<"puzzle is solved"<<endl;
            //mySudoku.dump();
            break;
        }
        else
        {
            cout<<"puzzle is not solved"<<endl;
        }

//next step is to find numbers which only appear in one of the possible cell.
        mySudoku.fillUniqueValues(ROW);
        mySudoku.fillUniqueValues(COL);
        mySudoku.fillUniqueValues(ZONE);

        if(mySudoku.complete())
        {
            cout<<"puzzle is solved"<<endl;
            //mySudoku.dump();
            break;
        } else
        {
            cout<<"puzzle is not solved yet"<<endl;
        }
    }
    cout<<"Total iterations: "<<iter<<endl;
    mySudoku.dump();
    return 0;
}

