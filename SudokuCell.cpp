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
    //check if cell is not solved, if you are solving it.
    if(!isEmpty())
    {        
        cout<<"cell is already filled. this should not happen"<<endl;
        return false;
    }
    //check if there are nay possibles remaining
    if(possibles.size() <= 1)
    {
        cout<<"invalid state of possible values. This should not occur"<<endl;
        return false;
    }
    possibles.erase(val);
    return autoFillCell();
}

bool Cell::removePossibleValue(std::vector<uint> &vals)
{
    if(!isEmpty())
    {
        cout<<"trying to fill a solved cell."<<endl;
        return false;
    }
    if(possibles.size() <= 1)
    {
        cout<<"possible values are not of operating size."<<endl;
        return false;
    }

    for(auto &it:vals)
    {
        possibles.erase(it);
    }
    return autoFillCell();
}

//This method checks if cell is ready to be solved. 
//returns true if the cell is resolved.
//false if nothing changed.
bool Cell::autoFillCell()
{
    if(!possibles.size())
    {
        cout<<"Unexpected behavior"<<endl;
        return false;
    }
    if(possibles.size() > 1)
    {
        //still more solving needs for this cell so pass.
        return false;
    }
    //if you are here, then cell is about to be filled.
    value = *(possibles.begin());
    possibles.clear();
    return true;
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

bool Cell::operator ==(Cell &rhs)
{
    //if non-empty values match, return true.
    if(!this->isEmpty() && !rhs.isEmpty() && this->value == rhs.getVal())
    {
        return true;
    }

    //if reached here, then possiblities should match.
    auto possibVect = rhs.getPossibleValues();
    cout<<"this length: "<<(this->possibles).size()<<endl;
    cout<<"rhs legth: "<<possibVect.size()<<endl;
    if((this->possibles).size() != possibVect.size())
    {
        return false;
    }
    //seems like both lengths are same
    auto ind = 0;
    for(auto &it: this->possibles)
    {
        if(it != possibVect[ind]) return false;
        ++ind;
    }
    //if reached here, then both arrays seems to be same
    return true;
}
