#include <iostream>
#include <algorithm>
#include <map>
#include <memory>
using namespace std;
template<class T,bool update=false>
class Inserter
{
    private:
    int slotsize;
    map<pair<int,int>,int> spare;
    T *matrix;
    bool isupdate;//for update_plus operations

    public:
    Inserter(T &refmatrix,int slot=5)
    {
        slotsize=slot;
        matrix=&refmatrix;
        matrix->allocMem(slotsize);
        isupdate=update;
    }

    void addEntry(int row,int col,int value)
    {
        int *indexPtrFirst=matrix->r[row].indexptr;
        int *indexPtrLast=indexPtrFirst+slotsize;
        int *indexPtrPos=std::lower_bound(indexPtrFirst,indexPtrLast,col);

        int index=indexPtrPos-indexPtrFirst;

        if(index==slotsize)
        {
            pair<int,int> p;
            p.first=row;
            p.second=col;
            if(isupdate)
                spare[p]+=value;
            else
                spare[p]=value;

            return;
        }
        int *indexValueFirst=matrix->r[row].data;
        int *indexValueLast=indexValueFirst+slotsize;
        int *indexValuePos=indexValueFirst+index;

        if(*indexPtrPos==col)
        {
            //for update_plus operation
            if(isupdate)
                *indexValuePos+=value;

            return;
        }


        std::copy_backward(indexPtrPos,indexPtrLast,indexPtrLast+1);
        std::copy_backward(indexValuePos,indexValueLast,indexValueLast+1);

        if(*(indexPtrLast)!=matrix->col)
        {
            pair<int,int> p;
            p.first=row;
            p.second=*(indexPtrLast);
            if(isupdate)
                spare[p]+=*(indexValueLast);
            else
                spare[p]=*(indexValueLast);

        }

        *indexPtrPos=col;
        *indexValuePos=value;

    }

    ~Inserter()
    {
        //For adding the entries from spare back to slots
        map<pair<int,int>,int>::iterator itr;
        for(itr=spare.begin();itr!=spare.end();itr++)
        {
            int size=matrix->r[itr->first.first].N;
            matrix->alloc.construct((matrix->r[itr->first.first].indexptr+size),itr->first.second);
            matrix->alloc.construct((matrix->r[itr->first.first].data+size),itr->second);
            size++;
            matrix->r[itr->first.first].N=size;
        }

        //For removing the unused entries from slots
        for(int i=0;i<matrix->row;i++)
        {
            int *indexPtrFirst=matrix->r[i].indexptr;
            int *indexPtrLast=indexPtrFirst+matrix->r[i].N;

            int *pos=std::find(indexPtrFirst,indexPtrLast,matrix->col);
            int index=pos-indexPtrFirst;

            for(int *itr=pos;itr<=indexPtrLast;itr++)
            {
                matrix->alloc.destroy(itr);
            }

            matrix->r[i].N=index;
        }

    }
};
