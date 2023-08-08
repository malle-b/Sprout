#pragma once 

//ROOT includes
#include "TF1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1F.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TGraph.h"
#include "TPaveText.h"

//C++ includes
#include <fstream>
#include <sstream>
#include <string>
#include <array>

//Sprout includes
#include "SproutPlot.h"
#include "SproutTree.h"

class SproutFit{
public:

    //Constructor 
    SproutFit();

    ~SproutFit(){delete fsig; delete fbg; delete ffit; delete cfit; delete csig; delete cbg;} // Destructor 
    SproutFit(const SproutFit& that) = delete; //Copy constructor set to delete. Object cannot be copied
    SproutFit& operator=(const SproutFit& that) = delete; // assignment operator set to delete. Object cannot be assigned 

    /**
    * Fits the sum of the specified signal and background functions to the given histogram. 
    * Note that the nessecary parameters to perform the fit must be given by calling SproutFit::setFit
    * before this function. 
    *
    * The fitting proccedure is as follows:
    * 1. The histogram is split into three regions based on the ranges specified in SproutFit::setFit: 
    *    -The signal region, between siglow and sigup. 
    *    -The lower region (below signal), between xlow and siglow.
    *    -The upper region (above signal), between sigup and xup. 
    *
    * 2. The specified background function, bg_func, is fitted simultaneously to the lower and upper region.
    *
    * 3. The sum of the signal and background function, bf_func+sig_func, is fitted to the the signal 
    *    region with fixed values of bf_func obtained in step 2. 
    *
    * 4. The sum of the signal and background is fitted to the full region between xlow and xup with 
    *    starting values as those obtained in the previous step. 
    *
    * 5. Steps 2-4 are repeated two more times.
    * 
    * @param h TH1F histogram to be fitted 
    * @param save_as if specified, the histogram and fit results are drawn and saved as 
    * .png file with the specified path/name. 
    */
    void fit(TH1F h, TString save_as = "");   


    /**
    * :) 
    */
    void fit(SproutPlot splot, TString save_as = " ", SproutTree* stree = nullptr, bool save_hist=false); 

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
    * Get the covariance matrix of the fit 
    * 
    * @return TMatrixDSym object representing the covariance matrix of the fitted function 
    */
    TMatrixDSym getFitCovariance(){return *cfit;}

    /**
    * Get the covariance matrix of the fitted signal function. 
    * 
    * @return TMatrixDSym object representing the covariance matrix of the fitted signal function 
    */
    TMatrixDSym getSignalCovariance(){return *csig;}

    /**
    * Get the covariance matrix of the fitted background function. 
    * 
    * @return TMatrixDSym object representing the covariance matrix of the fitted background function 
    */
    TMatrixDSym getBackgroundCovariance(){return *cbg;}

    /**
    * 
    */
    void integrate(TH1F h, float vals[4]);

    void setFileName(std::string filename){inputfile = filename;}


    void setStyle();
    void setLineStyle(int fitcolor=15, int sigcolor=42, int bgcolor=35,
        int fitstyle=5, int sigstyle=2, int bgstyle=3,
        int width=3){fit_line_color=fitcolor; sig_line_color=sigcolor; bg_line_color=bgcolor;
        fit_line_style=fitstyle; sig_line_style=sigstyle; bg_line_style=bgstyle; 
        line_width=width;}


    void setFit(std::string bf_func, std::string sig_func, double xlow, double xup, double siglow, double sigup, std::vector<double> pars);


private:
    std::string sig_name; //user-specified name of signal function 
    std::string bg_name; //user-specified name of background function 
    double xmin; //lower edge of fit region
    double xmax; //upper edge of fit region 
    double xsigmin; //lower edge of signal region 
    double xsigmax; //upper edge of signal region 
    std::vector<double> param; //vector to store fit parameters

    TF1* fsig; // signal function 
    TF1* fbg; // background function 
    TF1* ffit; // fit-function, i.e. sum of fsig and fbg

    int npar_sig; // number of parameters in fsig
    int npar_bg; // number of parameters in fbg
    int npar; // number of parameters in ffit 

    double chi2_sig; //reduced chi2 of signal fit 
    double chi2_bg; //reduced chi2 of background fit 
    double chi2; //reduced chi2 of total fit 

    TMatrixDSym* cfit; // Covariance matrix of ffit after fit 
    TMatrixDSym* csig; // covariance matrix of fsig after fit 
    TMatrixDSym* cbg; // Covariance matrix of fbg after fit 

    int fit_line_color; // ffit line color displayed when drawn 
    int sig_line_color; // fsig line color displayed when drawn
    int bg_line_color; // fbg line color displayed when drawn 
    int fit_line_style; // ffit line style displayed when drawn 
    int sig_line_style; // fsig line style displayed when drawn 
    int bg_line_style; // fbg line color displayed when drawn 
    int line_width; // line width displayed when drawn 
    
    std::string inputfile; // name of the input file 

    TString int2str(int i){TString str; str.Form("%d", i); return str;}
    TString double2str(double d){TString str; str.Form("%g", d); return str;}
    void readLine(std::fstream* ob, std::string line);
    void writeOutput();
    void setFitProperties();
    void updateParam();
    void drawResult(TH1F h);
    void fitBackground(TH1F h);
    void fitSignal(TH1F h);
    int getNumParam(TF1 f){int n=0; for(Int_t i=0; i<f.GetNpar();i++){n++;} return n;}
};
