#include <iostream>
#include "fieldMatrix.hh"
using namespace std;
int main()
{
    FMatrix<int> f1(3,3);
    {
        Inserter<FMatrix<int> > ins(f1,2);
        ins.addEntry(0,0,2);
        ins.addEntry(0,1,2);
        ins.addEntry(0,2,3);
        ins.addEntry(1,0,2);
        ins.addEntry(2,1,1);
        ins.addEntry(2,2,9);
    }
    f1.printComprresedMatrix();

    FMatrix<int> f2(3,3);
    {
        Inserter<FMatrix<int> > ins(f2,1);
        ins.addEntry(0,1,5);
        ins.addEntry(1,0,4);
        ins.addEntry(1,2,6);
        ins.addEntry(2,1,7);
    }
    cout<<endl;
    f2.printComprresedMatrix();
    cout<<endl;

    FMatrix<int> result(3,3);
    result=f1*f2;
    result.printComprresedMatrix();

    return 0;
}
