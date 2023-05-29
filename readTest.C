#include "TFile.h"
#include "include/RHtree.h"

void readTest(){
    TFile file = TFile("testFile.root", "read");
    RHtree* t;
    file.GetObject("my_tree", t);

    t->print();
}