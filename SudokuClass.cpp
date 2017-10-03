#include "SudokuClass.h"
#include <map>
#include <algorithm>


//SudokuClass definitin starts here
std::mutex SudokuClass::printMutex;
SudokuClass::SudokuClass(uint size): 
        _size(size), _cells(std::vector<Cell>()), _sqSize(0),
        isChanged(false)
{
    if(size == 0)
    {
        cout<<"Error: valid size please. dont start with 0"<<endl;
        return;
    }
    auto sq = (uint)sqrt((double)_size);
    if(sq*sq != _size)
    {
        cout<<"Error: perfect square is not provided"<<endl;
    }
    else
    {
        _sqSize = sq;
        _cells.reserve(size*size);
    }
}

SudokuClass::SudokuClass(std::vector<std::vector<uint>> mat):
        _size(0), _sqSize(0), _cells(std::vector<Cell>())
{
    uint len = mat.size();
    if(!len)
    {
        cout<<"Error: fucking empty matrix is provided to puzzle"<<endl;
        return;
    }
    auto sq = (uint)sqrt((double)len);
    if(sq*sq != len)
    {
        cout<<"Error: matrix not of perfect square size"<<endl;
        return;
    }
    else
    {
        _size = len;
        _sqSize = sq;
        //load every element in matrix into cell object
        uint rid = 0;
        uint cid = 0;
        for(auto &row : mat)
        {            
            for(auto &col : row)
            {
                if(col)
                {
                    //if there is a valid non-zero value create cell
                    _cells.push_back(Cell(rid, cid, col, _size));
                }
                else
                {
                    //cell with possibilities should be created
                    _cells.push_back(Cell(rid, cid, _size));
                }
                ++cid;
                cid = cid % _size;
            }
            ++rid;
            rid = rid%_size;
        }
        cout<<"complete sudoku Class created "<<endl;
    }
}

SudokuClass::~SudokuClass()
{
    _cells.clear();
}

void SudokuClass::printPuzzle()
{
    for(auto it = _cells.begin(); it!=_cells.end(); ++it)
    {
        cout<< it->getVal() << "\t";
        if(it->getColId() == _size-1) cout<<endl;
    }
}

void SudokuClass::fillPossibleCells(traversal opt)
{
    //fill cells traversing by option provided
    std::vector<std::thread> threads;
    std::queue<std::pair<uint, uint>> indices;
    for(uint i=0; i<_size; ++i)
    {
        threads.push_back(std::thread(SudokuClass::threadFillPossibles, i, opt, &_cells,
                                                    &indices));
    }
    for(auto &it: threads)
    {
        it.join();
    }
    if(indices.size()) fixRemainingCells(indices);
    cout<<"possible values are fixed"<<endl;
}


//this method fixes possible values of all other cells based on fixed value
void SudokuClass::fixRemainingCells(std::queue<std::pair<uint, uint>> &inds)
{
    //iterate through all pairs in queue until it dries out
    while(inds.size())
    {
        auto indPair = inds.front();
        auto row_id = indPair.first;
        auto col_id = indPair.second;
        auto cellVal = _cells[row_id * _size + col_id].getVal();
        inds.pop();
        //fix row possibles, traverse through columns for that rid
        for(auto jj=0; jj<_size; ++jj)
        {
            if(!_cells[row_id*_size + jj].isEmpty()) continue;
            if(_cells[row_id*_size + jj].removePossibleValue(cellVal))
            {
                inds.push(std::pair<uint, uint>(row_id, jj));
                set();
            }
        }
        //fix column possibles, traverse through all rows for this column
        for(auto ii=0; ii<_size; ++ii)
        {
            if(!_cells[ii*_size + col_id].isEmpty()) continue;
            if(_cells[ii*_size + col_id].removePossibleValue(cellVal))
            {
                inds.push(std::pair<uint, uint>(ii, col_id));
                set();
            }
        }
        //fix zone for that ind pair
        uint zr = row_id/_sqSize;
        uint zc = col_id/_sqSize;
        auto r_st = zr*_sqSize;
        auto r_end = r_st + _sqSize-1;
        auto c_st = zc*_sqSize;
        auto c_end = c_st + _sqSize-1;
        for(auto ii=r_st; ii<=r_end; ++ii)
        {
            for(auto jj=c_st; jj<=c_end; ++jj)
            {
                if(!_cells[ii*_size + jj].isEmpty()) continue;
                if(_cells[ii*_size + jj].removePossibleValue(cellVal))
                {
                    inds.push(std::pair<uint, uint>(ii, jj));
                    set();
                }
            }
        }
    }
}

void SudokuClass::threadFillPossibles(uint id, traversal opt, std::vector<Cell> *cells,
                                    std::queue<std::pair<uint, uint>> *inds)
{
    if(!cells || !inds) 
    {
        //error checks please
        printMutex.lock();
        cout<<"@ thread: "<<id<<" cell vector or indices vector is passed empty"<<endl;
        printMutex.unlock();
        return;
    }
    uint row_st = 0;
    uint row_end = 0;
    uint col_st = 0;
    uint col_end = 0;
    uint  size = (uint)sqrt((double)cells->size());
    uint sqSize = (uint)sqrt((double)size);
    if(opt == ROW)
    {
        row_st = id;
        row_end = id;
        col_st = 0;
        col_end = size-1;
    }
    else if(opt == COL)
    {
        row_st = 0;
        row_end = size-1;
        col_st = id;
        col_end = id;
    }
    else if(opt == ZONE)
    {
        uint rq = id/sqSize;
        uint cm = id%sqSize;
        row_st = sqSize*rq;
        row_end = row_st + sqSize-1;
        col_st = sqSize*cm;
        col_end = col_st + sqSize-1;
    }

    std::vector<uint> cellVals;
    for(auto ii=row_st; ii<= row_end; ++ii)
    {
        for(auto jj = col_st; jj<=col_end; ++jj)
        {
            if ((*cells)[ii*size+jj].isEmpty()) continue;
            else
            {
                cellVals.push_back((*cells)[ii*size + jj].getVal());
            }
        }
    }

    if(cellVals.empty())
    {
        printMutex.lock();
        cout<<"@ thread: "<<id+1<<" cells are empty"<<endl;
        printMutex.unlock();
        return;
    }
    for(auto ii=row_st; ii<= row_end; ++ii)
    {
        for(auto jj=col_st; jj<=col_end; ++jj)
        {
            if(!((*cells)[ii*size + jj].isEmpty())) continue;
            else
            {
                //dealing with empty cell in this iteration.
                
                for(auto it = cellVals.begin(); it!= cellVals.end();
                        ++it)
                {
                    if((*cells)[ii*size + jj].removePossibleValue(*it))
                    {
                        //If removingpossiblevalues ends with only one value,
                    //cell might ve got set with value. So update row, col and zone with
                    //new value.
                        printMutex.lock();
                        (*inds).push(std::pair<uint, uint>(ii, jj)); 
                        printMutex.unlock();
                    }
                }
            }
        }
    }
}

bool SudokuClass::validate()
{
    //checks if any repititions happen in row, col, zone
    //verified by adding to set and checking the return value to be false.
    //if return value is true, that means we tried to add a value that already
    //exists in set.
    //validate rows
    if(!validateRows()) return false;
    if(!validateColumns()) return false;
    if(!validateZones()) return false;
    return true;
}

bool SudokuClass::complete()
{
    //call same methods with true for check ompletion.
    if(!validateRows(true)) return false;
    if(!validateColumns(true)) return false;
    if(!validateZones(true)) return false;
    return true;
}

bool SudokuClass::validateRows(bool checkComplete, int rowInd)
{
    uint r_st = rowInd == -1? 0 : rowInd;
    uint r_end = rowInd == -1? _size : rowInd+1;
    for(auto r=r_st; r<_size; ++r)
    {
        std::set<uint> tmpRow;
        for(auto c=0; c<_size; ++c)
        {
            auto ind = r*_size + c;
            auto cellVal = _cells[ind].getVal();
            if(!cellVal && checkComplete) return false;
            else if(!cellVal) continue;
            auto ret = tmpRow.insert(cellVal);
            if(!ret.second)
            {
                cout<< "Error: "<<*(ret.first) <<" exists twice in row "<<r+1<<" and column: "<<c+1<<endl;
                return ret.second;
            }
        }
        tmpRow.clear();
    }
    return true;
}

bool SudokuClass::validateColumns(bool checkComplete, int colInd)
{
    uint col_st = colInd == -1? 0 : colInd;
    uint col_end = colInd == -1? _size : colInd+1;
    //validate columns
    for(auto c=col_st; c<col_end; ++c)
    {
        std::set<uint> tmpCol;
        for(auto r=0; r<_size; ++r)
        {
            auto ind = r*_size + c;
            auto cellVal = _cells[ind].getVal();
            if(!cellVal && checkComplete) return false;
            else if(!cellVal) continue;
            auto ret = tmpCol.insert(cellVal);
            if(!ret.second)
            {
                cout<< "Error: "<<*(ret.first) <<" exists twice in column: "<<c+1<<" and row: "<<r+1<<endl;
                return false;
            }
        }
        tmpCol.clear();
    }
    return true;
}

bool SudokuClass::validateZones(bool checkComplete, int zoneInd)
{
    uint z_st = zoneInd == -1? 0 : zoneInd;
    uint z_end = zoneInd == -1? _size : zoneInd;

    for(auto zz = z_st; zz<z_end; ++zz)
    {
        auto z_quo = zz/_sqSize;
        auto z_rem = zz%_sqSize;
        auto r_st = z_quo*_sqSize;
        auto r_end = r_st + _sqSize-1;
        auto c_st = z_rem*_sqSize;
        auto c_end = c_st + _sqSize-1;
        std::set<uint> tmpZone;
        for(auto r=r_st; r<=r_end; ++r)
        {
            for(auto c=c_st; c<=c_end; ++c)
            {
                auto ind = r*_size + c;
                auto cellVal = _cells[ind].getVal();
                if(!cellVal && checkComplete) return false;
                else if(!cellVal) continue;
                auto ret = tmpZone.insert(cellVal);
                if(!ret.second)
                {
                    cout<< "Error: "<< *(ret.first) <<" exists twice in zone: "<<zz;
                    cout<<" with row: "<<r<<" col: "<<c<<endl;
                    return ret.second;
                }
            }
        }
        tmpZone.clear();
    }
    //if you reached here, all cases are true
    return true;
}

void SudokuClass::dump()
{
    //prints the entire information of puzzle with possibilities and everything.
    cout<<"size of puzzle: "<<_size<<endl;
    printPuzzle();
    for(auto &it: _cells)
    {
        it.dump();
    }
}

void SudokuClass::fillUniqueValues(traversal opt)
{
    std::vector<std::thread> threads;
    std::queue<std::pair<uint, uint>> indices;

    for(auto i=0; i<_size; ++i)
    {
        threads.push_back(std::thread(SudokuClass::threadFillUniqueValues, i, opt,
                        &_cells, &indices));
    }

    for(auto &it:threads)
    {
        it.join();
        cout<<"unique values thread is complete"<<endl;
    }

    if(indices.size()) fixRemainingCells(indices);
    cout<<"unique values filled"<<endl;
}

void SudokuClass::threadFillUniqueValues(uint thr_id, traversal opt, std::vector<Cell> *cells, 
            std::queue<std::pair<uint, uint>> *inds)
{
    if(!cells || !inds)
    {
        printMutex.lock();
        cout<<"@ thread: "<<thr_id<<" cell vector or indices vector is passed empty"<<endl;
        printMutex.unlock();
        return;
    }
    uint size = (uint)sqrt((double)cells->size());
    uint sqSize = (uint)sqrt((double)size);
    uint r_st, r_end, c_st, c_end;
    if(opt == ROW)
    {
        r_st = thr_id;
        r_end = r_st;
        c_st = 0;
        c_end = size-1;
    }else if(opt == COL)
    {
        r_st = 0;
        r_end = size-1;
        c_st = thr_id;
        c_end = c_st;
    }else if(opt == ZONE)
    {
        uint zq = thr_id/sqSize;
        uint zr = thr_id % sqSize;
        r_st = zq*sqSize;
        r_end = r_st + sqSize-1;
        c_st = zr*sqSize;
        c_end = c_st + sqSize-1;
    }

    //found all the boundaries of rows and cols
    std::map<uint, std::vector<uint>> valueCellMap;
    for(auto ii=r_st; ii<= r_end; ++ii)
    {
        for(auto jj=c_st; jj<=c_end; ++jj)
        {
            auto ind = ii*size+jj;
            if(!(*cells)[ind].isEmpty()) continue;
            //handling only empty cells. We dont care about filled cells
            auto cellVect = (*cells)[ind].getPossibleValues();
            for(auto &it: cellVect)
            {
                valueCellMap[it].push_back(ind);
            }
        }
    }

    for(auto mit = valueCellMap.begin(); mit != valueCellMap.end(); ++mit)
    {
        if(mit->second.size() == 1)
        {
            //if possible value has only one cell associated then set the cell with that value
            (*cells)[(mit->second)[0]].setVal(mit->first);
            //other threads might also be adding to this queue. just be aware.
            printMutex.lock();
            inds->push(std::pair<uint, uint>((mit->second)[0]/size, (mit->second)[0]%size));
            printMutex.unlock();
        }
    }
}


void SudokuClass::pairPossibles(traversal opt)
{
    std::vector<std::thread> threads;
    std::queue<std::pair<uint, uint>> indices;

    for(auto i=0; i<_size; ++i)
    {
        threads.push_back(std::thread(SudokuClass::threadPairPossibles, i, opt,
                        &_cells, &indices));
    }

    for(auto &it:threads)
    {
        it.join();
        cout<<"pair possibles thread is complete"<<endl;
    }

    if(indices.size()) fixRemainingCells(indices);
    cout<<"paired possibles"<<endl;
}

void SudokuClass::threadPairPossibles(uint id, traversal opt, std::vector<Cell> *cells,
                                    std::queue<std::pair<uint, uint>> *inds)
{
    if(!cells || !inds) 
    {
        //error checks please
        printMutex.lock();
        cout<<"@ thread: "<<id<<" cell vector or indices vector is passed empty"<<endl;
        printMutex.unlock();
        return;
    }
    uint row_st = 0;
    uint row_end = 0;
    uint col_st = 0;
    uint col_end = 0;
    uint  size = (uint)sqrt((double)cells->size());
    uint sqSize = (uint)sqrt((double)size);
    if(opt == ROW)
    {
        row_st = id;
        row_end = id;
        col_st = 0;
        col_end = size-1;
    }
    else if(opt == COL)
    {
        row_st = 0;
        row_end = size-1;
        col_st = id;
        col_end = id;
    }
    else if(opt == ZONE)
    {
        uint rq = id/sqSize;
        uint cm = id%sqSize;
        row_st = sqSize*rq;
        row_end = row_st + sqSize-1;
        col_st = sqSize*cm;
        col_end = col_st + sqSize-1;
    }

    //create a map of possible values to indices that has same possible value vector
    //for now, we are going to create a vector of that pair and inefficiently traverse through every
    //to actually fill map
    std::vector< std::pair<std::vector<uint>, std::vector<uint> > > pairMap;
    auto insertFunc = [](vector<pair<vector<uint>, vector<uint> > > &myMap, 
                        vector<uint> &possib, uint ind){

            //comparator function for 2 vectors
                auto compFunc = [](vector<uint> &vec1, vector<uint> &vec2)
                {
                    if (vec1.size() != vec2.size()) return false;
                    for(auto i=0; i<vec1.size(); ++i)
                    {
                        if(vec1[i] != vec2[i]) return false;
                    }
                    return true;
                }; //nested lambda function end

                for(auto &it: myMap)
                {
                    if(compFunc(it.first, possib))
                    {
                        it.second.push_back(ind);
                        return;
                    }
                }
            //if reached her, then possib vector is not compared to any other pair 
        //in pairMap. So insert new one
                std::vector<uint> vecInd;
                vecInd.push_back(ind);
                myMap.push_back(pair<vector<uint>, vector<uint>>(possib, vecInd));
                //end of lambdda function                                                
                }; //lambda function end
    for(auto ii=row_st; ii<= row_end; ++ii)
    {
        for(auto jj = col_st; jj <= col_end; ++jj)
        {
            //if pairMap is empty just enter the possible pair with indices
            uint cellInd = ii*size+jj;
            if(!(*cells)[cellInd].isEmpty()) continue;
            auto cellPossibles = (*cells)[cellInd].getPossibleValues();
            if(cellPossibles.size() > sqSize) continue;
            //if reached here, update pairMap with possibles and indices
            insertFunc(pairMap, cellPossibles, cellInd);
        }
    }


    //once pairMap is built, check if any of the pair has same multiple indices and remove
        //them ffrom other cells
    for(auto &it: pairMap)
    {
        if(it.first.size() != it.second.size()) continue;
            //remove those possibles from all indices
        for(auto ii=row_st; ii<=row_end; ++ii)
        {
            for(auto jj=col_st; jj<=col_end; ++jj)
            {
                uint cellInd = ii*size + jj;
                if (!(*cells)[cellInd].isEmpty()) continue;
                if(std::find(it.second.begin(), it.second.end(), cellInd) == 
                            it.second.end())
                {
                 //didnt find this index in indicws vector, so remove possibles from 
                 //this cellInd
                    for(auto &possibIt:it.first)
                    {
                        if((*cells)[cellInd].removePossibleValue(possibIt))
                        {
                            inds->push(std::pair<uint, uint>(ii, jj));
                        }
                    }
                }
            }
        }
    }

}

void SudokuClass::set()
{
    isChanged = true;
}

void SudokuClass::reset()
{
    isChanged = false;
}

bool SudokuClass::isPuzzleChanged()
{
    return isChanged;
}
