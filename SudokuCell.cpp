#include "SudokuCell.h"

Cell::Cell(uint row, uint col, uint size):
            row_id(row), col_id(col), value(0), possibles(std::set<uint>())
{
    cout<<"Empty cell constructor created"<<endl;
    //this is an empty cell and every value is possible
    for(auto i=1; i<=size; ++i)
    {
        possibles.insert(i);
    }
}


Cell::Cell(uint row, uint col, uint val, uint size):
        row_id(row), col_id(col), value(val), possibles(std::set<uint>())
{
    if(val > size || val == 0)
    {
        cout<<"Error:How the hell did this happen, invalid value"<<endl;
        value = 0;
        for(auto i=1; i<=size; ++i)
        {
            possibles.insert(i);
        }
    }
    else
    {
        cout<<"Cell created with value"<<endl;
    }
}

Cell::~Cell()
{
    possibles.clear();
}

bool Cell::isEmpty()
{
    return value == 0;
}

uint Cell::getVal()
{
    return value;
}

void Cell::setVal(uint val)
{
    //set the value onlu if its empty, for now
    if(value)
    {
        cout<<"Error: trying to set value for a non-empty cell"<<endl;
        return;
    }
    possibles.clear();
    value = val;
}

//this method returns true if the value of cell is set after removing 
//all possible values from set.
//It means true is returned when only one possible value remains in set.
bool Cell::removePossibleValue(uint val)
{
    if(possibles.size() <= 1) return false;
    possibles.erase(val);
    auto retVal = false;
    //set the value of cell when only one possible value remains
    if(possibles.size() == 1)
    {
        if(!value)
        {
            value = *(possibles.begin());
            possibles.clear();
            retVal = true;
        }
        else {
            cout<<"Error:This case should not occur"<<endl;
            //this->printCell();
        }
    }
    return retVal;
}

uint Cell::getRowId()
{
    return row_id;
}

uint Cell::getColId()
{
    return col_id;
}

void Cell::dump()
{
    if (!isEmpty()) return;
    cout<<"["<<row_id+1<<","<<col_id+1<<"] possible values in this cell"<<endl;
    for(auto it:possibles)
    {
        cout<<it<<"\t";
    }
    cout<<endl;
}

std::vector<uint> Cell::getPossibleValues()
{
    std::vector<uint> retVect;
    for(auto &it: possibles)
    {
        retVect.push_back(it);
    }
    return retVect;
}

