#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <assert.h>
using namespace std;

template<class T,class type>
class Inserter
{
    public:
    int slotsize;
    T *matref;
    Inserter(T &mat,int slot=5)
    {
        matref=&mat;
        slotsize=slot;
    }

    //Adding values into sparse matrix and spare
    void addValue(int r,int c,type value)
    {
        pair<int,int> rowcolpair;
        rowcolpair.first=r;
        rowcolpair.second=c;
        matref->matrix[r].insert(make_pair<int,type>(c,value));

        //To check if the size is more than slot, if yes put the entry in spare
        if(matref->matrix[r].size()>slotsize)
        {
            matref->coliterator=matref->matrix[r].end();
            matref->coliterator--;
            pair<int,int> p=*matref->coliterator;
            type val=p.second;
            p.second=p.first;
            p.first=r;

            matref->spare[p]=val;   //inserting values in spare
            matref->matrix[r].erase(matref->coliterator);
        }
    }
};

template<class T>
class FieldMatrix
{
    public:
    vector<set<pair<int,T> > > matrix; //stores columns and values together in increasing order of column
    set<pair<int,int> >::iterator coliterator;
    map<pair<int,int>,T> spare;
    map<pair<int,int>,int>::iterator mapiterator;
    int row,col;

    FieldMatrix(int r,int c)
    {
        this->row=r;
        this->col=c;
        matrix.resize(r);
    }
    int rowsize()
    {
        return this->row;
    }
    int colsize()
    {
        return this->col;
    }

    //To print the columns and values of sparse matrix
    void printmat()
    {
        for(int i=0;i<row;i++)
        {
            cout<<"row "<<i<<"->\n";
            for(coliterator=matrix[i].begin();coliterator!=matrix[i].end();coliterator++)
            {
                pair<int,int> p;
                p=*coliterator;
                cout<<p.first<<" "<<p.second<<endl;
            }
        }
    }

    //To print the entries in spare
    void printspare()
    {
        cout<<"Entries in spare\nrow col value\n";
        for(mapiterator=spare.begin();mapiterator!=spare.end();mapiterator++)
            cout<<mapiterator->first.first<<" "<<mapiterator->first.second<<" "<<mapiterator->second<<endl;
    }

    //Matrix multiplication
    FieldMatrix operator*(FieldMatrix &B)
    {
        FieldMatrix<int> C(this->row,B.col);
        if(this->col!=B.row)
        {
            cout<<"Not compatible matrices to multiply\n";
            return C;
        }

        pair<int,int> acol,bcol;
        vector<vector<int> > result;
        result.resize(this->row);
        {
            for(int i=0;i<this->row;i++)
                result[i].resize(B.col,0);
        }

        //Iterating through A's sparse matrix
        for(int i=0;i<this->row;i++)
        {
            //This is for traversing through column entries in row i
            for(coliterator=this->matrix[i].begin();coliterator!=this->matrix[i].end();coliterator++)
            {
                acol=*coliterator;
                //Finding appropriate value in B's sparse matrix to multiply with
                for(B.coliterator=B.matrix[acol.first].begin();B.coliterator!=B.matrix[acol.first].end();B.coliterator++)
                {
                    bcol=*B.coliterator;
                    result[i][bcol.first]+=((acol.second)*(bcol.second));
                }
            }
        }
        //This is for iterating through A's spare and performing multiplication with B's spare as well as with B's sparse matrix
        for(mapiterator=this->spare.begin();mapiterator!=this->spare.end();mapiterator++)
        {
            int index=mapiterator->first.second;
            for(B.coliterator=B.matrix[index].begin();B.coliterator!=B.matrix[index].end();B.coliterator++)
            {
                bcol=*B.coliterator;
                result[mapiterator->first.first][bcol.first]+=((mapiterator->second)*(bcol.second));
            }
            pair<int,int> check;
            check.first=mapiterator->first.second;

            for(int i=0;i<B.col;i++)
            {
                check.second=i;
                result[mapiterator->first.first][i]+=((mapiterator->second)*(B.spare[check]));
            }
        }

        //This is for iterating through B's spare and performing multiplication with A's sparse matrix (Not with A's spare because we have already done it above)
        for(B.mapiterator=B.spare.begin();B.mapiterator!=B.spare.end();B.mapiterator++)
        {
            for(int i=0;i<this->row;i++)
            {
                for(coliterator=this->matrix[i].begin();coliterator!=this->matrix[i].end();coliterator++)
                {
                    acol=*coliterator;
                    if(acol.first==B.mapiterator->first.first)
                        result[i][B.mapiterator->first.second]+=((B.mapiterator->second)*(acol.second));

                }
            }
        }

        //Storing the result in FieldMatrix C using inserter
        Inserter<FieldMatrix<int>,int> ins(C,2);
        for(int i=0;i<this->row;i++)
            for(int j=0;j<B.col;j++)
                if(result[i][j]!=0)
                    ins.addValue(i,j,result[i][j]);

        //This is to check the result of multiplication in n*m format
        cout<<"Result\n";
        for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<B.col;j++)
                cout<<result[i][j]<<" ";
            cout<<endl;
        }
        return C;
    }


    //This is for matrix addition
    FieldMatrix operator+(FieldMatrix &B)
    {
        FieldMatrix<int> C(this->row,B.col);
        if(this->row!=B.row || this->col!=B.col)
        {
            cout<<"Not compatible matrices\n";
            return C;
        }

        vector<vector<int> > result;
        result.resize(this->row);
        for(int i=0;i<row;i++)
            result[i].resize(this->col,0);

        pair<int,int> acol,bcol;
        //Adding the values of sparse matrix A
        for(int i=0;i<this->row;i++)
        {
            for(coliterator=this->matrix[i].begin();coliterator!=this->matrix[i].end();coliterator++)
            {
                acol=*coliterator;
                result[i][acol.first]+=acol.second;
            }
        }

        //Adding the values of sparse matrix B
        for(int i=0;i<this->row;i++)
        {
            for(B.coliterator=B.matrix[i].begin();B.coliterator!=B.matrix[i].end();B.coliterator++)
            {
                bcol=*B.coliterator;
                result[i][bcol.first]+=bcol.second;
            }
        }

        //Adding the values of A's spare
        for(mapiterator=this->spare.begin();mapiterator!=this->spare.end();mapiterator++)
            result[mapiterator->first.first][mapiterator->first.second]+=mapiterator->second;

        //Adding the values of B's spare
        for(B.mapiterator=B.spare.begin();B.mapiterator!=B.spare.end();B.mapiterator++)
            result[B.mapiterator->first.first][B.mapiterator->first.second]+=B.mapiterator->second;


        Inserter<FieldMatrix<int>,int> ins(C,2);
        for(int i=0;i<this->row;i++)
            for(int j=0;j<B.col;j++)
                if(result[i][j]!=0)
                    ins.addValue(i,j,result[i][j]);


        //This is to check the result of addition
        /*for(int i=0;i<this->row;i++)
        {
            for(int j=0;j<B.col;j++)
                cout<<result[i][j]<<" ";
            cout<<endl;
        }*/
        return C;
    }


    //This is to perform matrix substraction
    FieldMatrix operator-(FieldMatrix &B)
    {
        FieldMatrix<int> C(this->row,B.col);
        if(this->row!=B.row || this->col!=B.col)
        {
            cout<<"Not compatible matrices\n";
            return C;
        }

        vector<vector<int> > result;
        result.resize(this->row);
        for(int i=0;i<row;i++)
            result[i].resize(this->col,0);

        pair<int,int> acol,bcol;
        for(int i=0;i<this->row;i++)
        {
            for(coliterator=this->matrix[i].begin();coliterator!=this->matrix[i].end();coliterator++)
            {
                acol=*coliterator;
                result[i][acol.first]+=acol.second;
            }
        }

        for(mapiterator=this->spare.begin();mapiterator!=this->spare.end();mapiterator++)
            result[mapiterator->first.first][mapiterator->first.second]+=mapiterator->second;


        //substractin B from A
        for(int i=0;i<this->row;i++)
        {
            for(B.coliterator=B.matrix[i].begin();B.coliterator!=B.matrix[i].end();B.coliterator++)
            {
                bcol=*B.coliterator;
                result[i][bcol.first]-=bcol.second;
            }
        }


        for(B.mapiterator=B.spare.begin();B.mapiterator!=B.spare.end();B.mapiterator++)
            result[B.mapiterator->first.first][B.mapiterator->first.second]-=B.mapiterator->second;

        Inserter<FieldMatrix<int>,int> ins(C,2);
        for(int i=0;i<this->row;i++)
            for(int j=0;j<B.col;j++)
                if(result[i][j]!=0)
                    ins.addValue(i,j,result[i][j]);

        //This is to check the result of substraction
        /*for(int i=0;i<this->row;i++)
        {
             for(int j=0;j<B.col;j++)
                cout<<result[i][j]<<" ";
            cout<<endl;
        }*/
        return C;
    }
};

int main()
{
    FieldMatrix<int> A(4,4);
    {
        //Creating inserter for matrix A

        Inserter<FieldMatrix<int>,int> ins(A,2);//slot is of 2
        //row, col and value entries
		ins.addValue(0,2,6);

        ins.addValue(1,0,4);
        ins.addValue(1,1,3);


        ins.addValue(2,1,5);
        ins.addValue(2,2,1);

        ins.addValue(3,0,2);
        ins.addValue(3,2,9);


        /*A.printmat();
        cout<<endl;
        A.printspare();*/

    }
    FieldMatrix<int> B(4,4);
    {
        //Creating inserter for B
        Inserter<FieldMatrix<int>,int> ins(B,1);  //slot is of 1
        ins.addValue(0,1,9);
        ins.addValue(0,3,2);

        ins.addValue(1,0,7);
        ins.addValue(1,1,6);

        ins.addValue(2,0,5);
        ins.addValue(2,2,2);
        ins.addValue(2,3,1);

        ins.addValue(3,1,3);
        ins.addValue(3,3,8);

        /*B.printmat();
        cout<<endl;
        B.printspare();*/

    }

    FieldMatrix<int> C(4,4);
    //performing multiplication
    C=A*B;
    cout<<endl;
    C.printmat();
    cout<<endl;
    C.printspare();

    C=A+B;

    C=A-B;

    return 0;
}
