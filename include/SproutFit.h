#pragma once 

#include "TF1.h"
#include "TString.h"
#include "TH1F.h"
#include "SproutTree.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TMatrixD.h"

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
    float par_range;

    TMatrixDSym cfit;
    TMatrixDSym csig;
    TMatrixDSym cbg;

    int fit_line_color;
    int sig_line_color;
    int bg_line_color;
    int fit_line_style;
    int sig_line_style;
    int bg_line_style;
    int line_width;
    

    TString sig_func_name;
    std::string inputfile;

    SproutTree ftree;
    
    void readInput();
    void writeOutput();
    int getNumParam(TF1 f){int n=0; for(Int_t i=0; i<f.GetNpar();i++){n++;} return n;}

    public:
    SproutFit(TString sig_func, TString bg_func,int num=0);
    SproutFit(TString sig_func, TString bg_func, std::string infile,int num=0);
    ~SproutFit(){delete fsig; delete fbg; delete ffit;}
    SproutFit(const SproutFit& that) = delete;
    SproutFit& operator=(const SproutFit& that) = delete;

    void fit(TH1F* h);

    SproutTree integrate(double low, double high, double bin_width);
    

    TF1* getFit(){return ffit;}
    TF1* getSignal(){return fsig;}
    TF1* getBackground(){return fbg;}
    TString getSigFuncName(){return sig_func_name;}
    TMatrixDSym getFitCovariance(){return cfit;}
    TMatrixDSym getSignalCovariance(){return csig;}
    TMatrixDSym getBackgroundCovariance(){return cbg;}

    void setStyle();
    void setLineStyle(int fitcolor=15, int sigcolor=42, int bgcolor=35,
        int fitstyle=5, int sigstyle=2, int bgstyle=3,
        int width=3){fit_line_color=fitcolor; sig_line_color=sigcolor; bg_line_color=bgcolor;
        fit_line_style=fitstyle; sig_line_style=sigstyle; bg_line_style=bgstyle; 
        line_width=width;}
    void setParRange(float range){par_range=range;}

    void writeInput();
    void printInput();

};
