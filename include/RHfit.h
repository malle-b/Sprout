#pragma once 

#include "TF1.h"
#include "TString.h"
#include "TH1F.h"
#include "RHtree.h"

#include <fstream>


class RHfit{
    private:
    TF1* fsig;
    TF1* fbg;
    TF1* ffit;

    int npar_sig;
    int npar_bg;
    int npar;
    int count;

    RHtree ftree;
    
    void readInput();
    int getNumParam(TF1 f){int n=0; for(Int_t i=0; i<f.GetNpar();i++){n++;} return n;}


    public:
    RHfit(TString sig_func, TString bg_func);
    ~RHfit(){delete fsig; delete fbg; delete ffit;}
    RHfit(const RHfit& that) = delete;
    RHfit& operator=(const RHfit& that) = delete;

    void fit(TH1F* h);
    //RHtree fit(RHtree t, HistogramHelper h); 

    void writeInput();
    void printInput();

};
