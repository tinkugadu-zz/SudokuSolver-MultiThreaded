#include "../SudokuCell.h"

int main()
{
    Cell cell1(2,3,5);
    Cell cell2(2,3,4);
    if(cell1 == cell2)
    {
        cout<<"cell1 and cell2 are equal"<<endl;
    }
    else
    {
        cout<<"cell1 and cell2 are not equal"<<endl;
    }

    Cell cell3(2,2,3, 5);
    Cell cell4(4,4,4, 5);
    if(cell3 == cell4)
    {
        cout<<"cell3 and cell4 are equal"<<endl;
    }
    else
    {
        cout<<"cell3 and cell4 are not equal"<<endl;
    }
    return 0;
}
