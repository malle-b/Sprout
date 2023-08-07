#pragma once 

#include "TF1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1F.h"
#include "SproutTree.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TMatrixD.h"
#include "TFormula.h"
#include "TGraph.h"
#include "TPaveText.h"

#include <fstream>
#include <sstream>
#include <string>

#include "SproutPlot.h"

class SproutFit{
public:

    SproutFit(std::string filename);

    /**
    * Constructor of a SproutFit object with a specified signal and background model 
    * function as well as the number of histograms that will be fitted. 
    * 
    * @param sig_func name of the signal function, eg. "gaus"
    * @param bg_func name of the background function, eg. "pol2"
    * @param num number of objects that will be fitted 
    */
    SproutFit(TString sig_func, TString bg_func,int num=0);

    /**
    * Constructor of a SproutFit object with a specified signal and background model 
    * function, input file with start parameters as well as the number of histograms 
    * that will be fitted. 
    * 
    * @param sig_func name of the signal function, eg. "gaus"
    * @param bg_func name of the background function, eg. "pol2"
    * @param infile name of file containing the input parameters
    * @param num number of objects that will be fitted 
    */
    SproutFit(TString sig_func, TString bg_func, std::string infile,int num=0);

    ~SproutFit(){delete fsig; delete fbg; delete ffit;} // Destructor 
    SproutFit(const SproutFit& that) = delete; //Copy constructor set to delete. Object cannot be copied
    SproutFit& operator=(const SproutFit& that) = delete; // assignment operator set to delete. Object cannot be assigned 

    /**
    * Fits the sum of the specified signal and background functions to the specified histogram 
    * 
    * @param h pointer to TH1F histogram to be fitted 
    */
    void fit(TH1F* h);   

    void fit(SproutPlot splot); 

    /**
    * Get the fitted function, i.e. the sum of the specified signal and background functions, 
    * 
    * @return pointer to the TF1 object corresponding to the fitted function 
    */
    //TF1* getFit(){return ffit;}

    /**
    * Get the fitted signal function 
    *
    * @return pointer to the TF1 object corresponding to the fitted signal function 
    */
    //TF1* getSignal(){return fsig;}

    /**
    * Get the fitted signal funciton 
    * 
    * @return pointer to the TF1 object corresponding to the fitted background function 
    */
    //TF1* getBackground(){return fbg;}

    /**
    * Get the name of the signal funciton that was specified in the constructor 
    * 
    * @return TString of the signal function name specified in the constructor 
    */
    TString getSigFuncName(){return sig_name;}

    /**
    * Get the covariance matrix of the fit 
    * 
    * @return TMatrixDSym object representing the covariance matrix of the fitted function 
    */
    TMatrixDSym getFitCovariance(){return cfit;}

    /**
    * Get the covariance matrix of the fitted signal function. 
    * 
    * @return TMatrixDSym object representing the covariance matrix of the fitted signal function 
    */
    TMatrixDSym getSignalCovariance(){return csig;}

    /**
    * Get the covariance matrix of the fitted background function. 
    * 
    * @return TMatrixDSym object representing the covariance matrix of the fitted background function 
    */
    TMatrixDSym getBackgroundCovariance(){return cbg;}

    /**
    * 
    */
    SproutTree integrate(double low, double high, double bin_width);


    void setStyle();
    void setLineStyle(int fitcolor=15, int sigcolor=42, int bgcolor=35,
        int fitstyle=5, int sigstyle=2, int bgstyle=3,
        int width=3){fit_line_color=fitcolor; sig_line_color=sigcolor; bg_line_color=bgcolor;
        fit_line_style=fitstyle; sig_line_style=sigstyle; bg_line_style=bgstyle; 
        line_width=width;}
    //void setParRange(float range){par_range=range;}

    void writeInput();
    void printInput();

    void fitBackground(TH1F h);
    void fitSignal(TH1F h);
    void fit(TH1F h);


private:
    TF1* fsig; // signal function 
    TF1* fbg; // background function 
    TF1* ffit; // fit-function, i.e. sum of fsig and fbg

    int npar_sig; // number of parameters in fsig
    int npar_bg; // number of parameters in fbg
    int npar; // number of parameters in ffit 
    // int count; // number of times the fit has been applied 
    // int ecount; // number of histogram to be fitted 
    // float par_range; // range of fit parameters.

    double chi2_sig;
    double chi2_bg;
    double chi2;

    std::string sig_name;
    std::string bg_name;
    double xmin;
    double xmax;
    double xsigmin;
    double xsigmax;
    std::vector<double> param;

    TMatrixDSym cfit; // Covariance matrix of ffit after fit 
    TMatrixDSym csig; // covariance matrix of fsig after fit 
    TMatrixDSym cbg; // Covariance matrix of fbg after fit 

    int fit_line_color; // ffit line color displayed when drawn 
    int sig_line_color; // fsig line color displayed when drawn
    int bg_line_color; // fbg line color displayed when drawn 
    int fit_line_style; // ffit line style displayed when drawn 
    int sig_line_style; // fsig line style displayed when drawn 
    int bg_line_style; // fbg line color displayed when drawn 
    int line_width; // line width displayed when drawn 
    

    std::string inputfile; // name of the input file 

    void readLine(std::fstream* ob, std::string line);
    
    TString int2str(int i){TString str; str.Form("%d", i); return str;}
    TString double2str(double d){TString str; str.Form("%g", d); return str;}
    void readInput();
    void writeOutput();
    int getNumParam(TF1 f){int n=0; for(Int_t i=0; i<f.GetNpar();i++){n++;} return n;}
    void setFitFunctions();
    void updateParam();
    void drawResult(TH1F h);

};
