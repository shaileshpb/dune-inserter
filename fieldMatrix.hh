#include <iostream>
#include <memory>
#include <vector>
#include "inserter.hh"
#include "fieldVector.hh"
template<class T,class Allocator=allocator<T> >
class FMatrix:public FVector<T,Allocator>
{
    public:
    int row,col;
    Allocator alloc;
    typedef FVector<T,Allocator> row_type;
    typedef typename Allocator::size_type size_type;
    row_type *r;

    FMatrix(size_type row,size_type col):FVector<T,Allocator>()
    {
        this->row=row;
        this->col=col;
        r=new row_type[row];
    }

    void allocMem(size_type slot)
    {
        for(int i=0;i<row;i++)
        {
            r[i].data=alloc.allocate(slot);
            r[i].indexptr=alloc.allocate(slot);
            uninitialized_fill_n(r[i].indexptr, slot,col);
            r[i].N=slot;
        }
    }

    void printComprresedMatrix()
    {
        for(int i=0;i<row;i++)
        {
            std::cout<<"row->"<<i<<endl;

            std::cout<<"Column: ";
            this->indexiterator=r[i].indexptr;
            for(int j=0;j<r[i].N;j++,this->indexiterator++)
                std::cout<<*(this->indexiterator)<<" ";
            std::cout<<endl;

            std::cout<<"Values: ";
            this->dataiterator=r[i].data;
            for(int j=0;j<r[i].N;j++,this->dataiterator++)
                std::cout<<*(this->dataiterator)<<" ";
            std::cout<<endl;
        }
    }

    FMatrix operator*(FMatrix &mat1)
    {

        FMatrix<int> res(col,mat1.row);
        {
            Inserter<FMatrix<int>,true > ins(res,2);
            for(int i=0;i<row;i++)
            {
                this->dataiterator=this->r[i].data;
                this->indexiterator=this->r[i].indexptr;
                for(int j=0;j<r[i].N;j++)
                {
                    mat1.dataiterator=mat1.r[*this->indexiterator].data;
                    mat1.indexiterator=mat1.r[*this->indexiterator].indexptr;

                    for(int k=0;k<mat1.r[*this->indexiterator].N;k++)
                    {
                        int value=(*(this->dataiterator))*(*mat1.dataiterator);
                        //cout<<i<<" "<<*(mat1.indexiterator)<<" "<<value<<endl;
                        //result[i][*(mat1.indexiterator)]+=value;
                        ins.addEntry(i,*(mat1.indexiterator),value);
                        mat1.indexiterator++;
                        mat1.dataiterator++;
                    }

                    this->indexiterator++;
                    this->dataiterator++;
                }
            }
        }
        return res;
    }
};
