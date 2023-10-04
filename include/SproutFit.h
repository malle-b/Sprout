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

    //Default constructor 
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
    * 
    * @param h TH1F histogram to be fitted 
    * @param save_as if specified, the histogram and fit results are drawn and saved as 
    * .png file with the specified path/name. 
    */
    void fit(TH1F h, TString save_as = "");   


    /**
    * Fits the sum of a specified signal and background function to each histogram contained in the SproutPlot
    * object passed as the first argument. The type of fit, its ranges and start parameters (optional) is specified 
    * in a .txt file (by default named FitParam.txt). Such an input file must be created and filled by the user prior 
    * to calling this function. 
    *
    * Required format of the input .txt file where row i corresponds to histogram i in SproutPlot
    *   - Parameter 1: Name of the background function. This class uses the same name convention as ROOT::TFormula.
    *     See documentation (https://root.cern.ch/doc/master/classTFormula.html) for valid expressions. 
    *   - Parameter 2: Name of the signal function (Any valid expression of ROOT::TFormula)
    *   - Parameters 3 and 4: lower and upper range of the fit. If both are set to the same value, the fit is performed
    *     over the entire histogram range. 
    *   - Parameters 5 and 6: lower and upper range of the signal region. 
    *   - Additional parameters (optional): Starting values of the background function followed by starting values of the 
    *     signal function. 
    * Note that the input file doesn't need to contain the same number of lines as the number of histograms held in (SproutPlot).
    * If there are fewer of the former, the last read line will be used to set the fit properties of all subsequent histograms. 
    *
    * After the fit, the input file is overwritten to contain the fitted parameters of the signal and background functions. 
    *
    * The fitting procedure of each histogram in SproutPlot is as follows: 
    * 1. The histogram is split into three regions based on the ranges specified in SproutFit::setFit: 
    *    -The signal region, between siglow and sigup. 
    *    -The lower region (below signal), between xlow and siglow.
    *    -The upper region (above signal), between sigup and xup. 
    * 2. The specified background function, bg_func, is fitted simultaneously to the lower and upper region.
    * 3. The sum of the signal and background function, bf_func+sig_func, is fitted to the the signal 
    *    region with fixed values of bf_func obtained in step 2. 
    * 4. The sum of the signal and background is fitted to the full region between xlow and xup with 
    *    starting values as those obtained in the previous step. 
    * 5. Steps 2-4 are repeated two more times.
    *
    * @param splot SproutPlot containing a collection of histograms, each of which is fitted. 
    * @param save_as (optional). If specified, a .png file showing all fitted histograms will be saved with the
    * specified path/name. 
    * @param stree (optional). If a pointer to a sproutTree object containing at least 4 branches is passed it is 
    * filled with the results of SproutPlot::integrate for each fitted histogram. 
    * @param save_hist (optinal). If set to true, a separate figure of each fitted histogram is generated and 
    * saved as a .png file with path/name specified in save_as. 
    * 
    */
    void fit(SproutPlot splot, TString save_as = " ", SproutTree* stree = nullptr, bool save_hist=false); 

    /**
    * Get the fitted function, i.e. the sum of the specified signal and background functions, 
    * 
    * @return pointer to the TF1 object corresponding to the fitted function 
    */
    TF1* getFit(){return ffit;}

    /**
    * Get the fitted signal function 
    *
    * @return pointer to the TF1 object corresponding to the fitted signal function 
    */
    TF1* getSignal(){return fsig;}

    /**
    * Get the fitted signal funciton 
    * 
    * @return pointer to the TF1 object corresponding to the fitted background function 
    */
    TF1* getBackground(){return fbg;}


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
    * Calculates the number of events in the signal peak of a fitted histogram (note that SproutFit::fit)
    * must have been called before this function. 
    *
    * The integral is perfomred over the total fitting range in two different ways for comparison. If the reuslts
    * differ signigicantly, the user should take notice and identify the cause of the discrepancy.  
    * The events in the signal peak is estimated as follows: 
    * 
    * First method: The fitted signal function is integrated over the fitting range and divided by the bin width. 
    * The uncertainty is obtained through error propagation of the fit parameter covariance matrix. 
    *
    * Second method: The integral of the fitted background function is subtracted from each histogram bin and 
    * the remaining bin contents are summed. The uncertainty is obtained through error propagation of the 
    * background integral uncertainty (propagated from the covariance matrix) and the histogram uncertainties. 
    * 
    * The results of these two methods are written to the array "vals" passed as an argument to this function. 
    * val[0] is set to the number of peak events obatained through method 1. 
    * val[1] is the uncertainty of the former. 
    * val[2] is set to the number of peak events obtained through method 2. 
    * val[3] is set to the uncertainty of the former. 
    *
    * @param h fitted histogram for which the number of signal counts is to be estimated. 
    * @param vals array into which the result is written. 
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
