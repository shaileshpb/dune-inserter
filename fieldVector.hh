#include <iostream>
using namespace std;
template<class T,class Allocator=allocator<T> >
class FVector
{
    public:
    Allocator alloc;
    T* data;
    T* dataiterator;
    typedef typename Allocator::size_type size_type;
    size_type N;
    int* indexptr;
    int* indexiterator;

    FVector()
    {
        this->N=0;
    }
    FVector(T *_data,int *_indexptr,size_type _N):data(_data),indexptr(_indexptr),N(_N)
    {}

    void setsize(size_type _N)
    {
        this->N=_N;
    }

    void setptr(T* _data)
    {
        this->data=_data;
    }

    void setindexptr(int* _indexptr)
    {
        this->indexptr=_indexptr;
    }

    T* getptr()
    {
        return this->data;
    }

    int* getindexptr()
    {
        return this->indexptr;
    }

    size_type getsize()
    {
        return this->N;
    }
};

