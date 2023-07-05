#include "TTree.h"
#include "TString.h"
#include "TFile.h"
#include "TVectorD.h"

#include <vector>
#include <iostream>
#include <iomanip>

#pragma once

class SproutTree{

private:

    std::vector<std::vector<float>> fvec; //[fsize]
    int fsize;
    int nmax;
    
    TString int2str(int i){TString str; str.Form("%d", i); return str;}

public:
    SproutTree(): fvec(), fsize(), nmax(){};
    SproutTree(int num_branches, int entries=0);
    SproutTree(TTree* t, std::vector<TString> bstr={});
    
    void addBranch(){std::vector<float> vec; fvec.push_back(vec);}
    void addToBranch(int branch_num, float a){
        fvec[branch_num].push_back(a);
        if((int) fvec[branch_num].size()> nmax){nmax = fvec[branch_num].size();}
        }
    
    SproutTree binData(std::vector<float> adata, std::vector<float> bdata, int n);
    
    float get(int branch_num, int i){return fvec[branch_num][i];}
    std::vector<float> getBranch(int branch_num){return fvec[branch_num];}
    int getNumEntries(int branch_num){return fvec[branch_num].size();}
    int getNumBranches(){return fvec.size();}
    int getMaxEntries(){return nmax;}

    void set(int branch_num, int i, float val){fvec[branch_num][i]=val;} //Add error handling if i is outside the scope

    void writeTTree(TString name = "sproutTree");
    void readTTree(TTree t); // Not added 

    void printBranch(int branch_num);
    void print();
    void write(TFile* file);
    void read(TFile* file);

    void clear();


    ClassDef(SproutTree, 4)
};