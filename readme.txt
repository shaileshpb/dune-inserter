These are the files for implementing inseter concept for DUNE' sparse matrix.
1)fieldVector.hh class is compressed_base_array_unmanged class.
2)fieldMatrix.hh is our BCRSMatrix class.
3)inserter.hh is the inserter class which allows the BCRSMatrix to get initialized.
4)testInserter.cpp file is for testing the work of Inserter class

->indexprt and data in fieldVector.hh files are the array to store columns and values respt. in increasing order of columns.

->Allocating memory to slots will be done by method allocMem difined in fieldMatrix.hh file, but it will get called by inserter object during its construction.

->During inserter's destruction, the entries from map will be entered back to slots by dynamically allocating more memory and the unused entries in indexptr and data will be removed.

->fieldMatrix.hh file also has a function to perform two compressed Matrix multipliaction. Here the update_plus(the second template argument while creating inserter object) will be needed. The function also stores result in compressed matrix form.
