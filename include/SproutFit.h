#pragma once 

#include "TF1.h"
#include "TString.h"
#include "TH1F.h"
#include "SproutTree.h"

#include <fstream>


class SproutFit{
    private:
    TF1* fsig;
    TF1* fbg;
    TF1* ffit;

    int npar_sig;
    int npar_bg;
    int npar;
    int count;
    int ecount;

    TString sig_func_name;
    std::string inputfile;

    SproutTree ftree;
    
    void readInput();
    int getNumParam(TF1 f){int n=0; for(Int_t i=0; i<f.GetNpar();i++){n++;} return n;}

    public:
    SproutFit(TString sig_func, TString bg_func,int num=0);
    SproutFit(TString sig_func, TString bg_func, std::string infile,int num=0);
    ~SproutFit(){delete fsig; delete fbg; delete ffit;}
    SproutFit(const SproutFit& that) = delete;
    SproutFit& operator=(const SproutFit& that) = delete;

    void fit(TH1F* h);

    TF1* getSignal(){return fsig;}
    TF1* getBackground(){return fbg;}
    TString getSigFuncName(){return sig_func_name;}

    void writeInput();
    void printInput();

};
