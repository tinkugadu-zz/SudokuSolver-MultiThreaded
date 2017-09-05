#include <iostream>
#include <stdio.h>
#include <vector>
#include <set>

using namespace std;

int main()
{
    std::set<int> mySet;
    std::vector<int> myVect;
    for (auto i=0; i<10; ++i)
    {
        mySet.insert(i);
    }

    for(auto &it:mySet)
    {
        cout<<it<<"\t";
        myVect.push_back(it);
    }
    cout<<endl;

    for(auto &it:myVect)
    {
        cout<<it<<"\t";
    }
    cout<<endl;
    return 0;
}
