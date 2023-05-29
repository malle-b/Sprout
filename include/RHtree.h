#include "TTree.h"
#include "TString.h"
#include "TFile.h"

#include <vector>
#include <iostream>
#include <iomanip>

#pragma once

class RHtree{

private:
    TTree* t;
    std::vector<std::vector<float>> fvec;
    
    TString int2str(int i){TString str; str.Form("%d", i); return str;}

public:
    RHtree();
    RHtree(int num_branches);

    void addBranch(){std::vector<float> vec; fvec.push_back(vec);}
    void addToBranch(int branch_num, float a){fvec[branch_num].push_back(a);}
    
    
    float get(int branch_num, int i){return fvec[branch_num][i];}
    std::vector<float> getBranch(int branch_num){return fvec[branch_num];}
    int getNumEntries(int branch_num){return fvec[branch_num].size();}
    int getNumBranches(){return fvec.size();}
    TTree* getTTree(){return t;}

    void fillTTree();

    void printBranch(int branch_num);
    void print();

    void write(TFile* file, TString name){file->WriteObject(&(*this), name);};

    ClassDef(RHtree, 1)
};