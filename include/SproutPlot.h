#ifndef SPROUTPLOT_H
#define SPROUTPLOT_H

#include "TCanvas.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TStyle.h"

#include "SproutTree.h"
#include "SproutFit.h"

#include <iostream>
#include <vector>

class SproutPlot {

private:
    TCanvas* fcanvas;
    TH1F* fhist1;
    int line_color; 
    int line_style; 
    int line_width;
    int marker_color; 
    int marker_style; 
    int marker_size;

    TH1F getTH1F(){return *fhist1;}

    TString int2str(int i){TString str; str.Form("%d", i); return str;}

    void makeTH1F(TString name, int bins ,double xmin, double xmax,TString xlabel="x", TString ylabel="Counts");
    void makeTCanvas();
    void makeTCanvas(int nhist);

public:

    SproutPlot();
    ~SproutPlot(){delete fcanvas; delete fhist1;}
    SproutPlot(const SproutPlot& that) = delete;
    SproutPlot& operator=(const SproutPlot& that) = delete;

    /**
    * Returns an empty TH1F histogram with properties set to the specified parameters.
    * 
    * @param name name of the histogram
    * @param bins number of bins 
    * @param xmin lower edge of x-axis
    * @param xmax upper edge of x-axis
    * @param xlabel label displayed on x-axis. Set to "x" by default
    * @param ylabel label displayed on y-axis. Set to "counts" by default 
    *
    * @return empty TH1F histogram 
    */
    TH1F getTH1F(TString name, int bins, double xmin, double xmax, TString xlabel="x", TString ylabel="Counts");

    /**
    * Fits all branches of 'tree' to some signal and background model functions specified in the
    * given SproutFit-object and generates a canvas to display the resulting plots. 
    * The number of signal and background events as well as their uncertanties 
    * are estimated from the fitted functions and returned as output.
    * The generated canvas is saved as a .png-file with the name "big_boifit.png"
    *
    * @param tree SproutTree for which each branch is to be fitted and plotted as a histogram. 
    * @param hfit pointer to a SproutFit object, specifying the model for signal and background. 
    * 
    * @return four-branched SproutTree where row n corresponds to the fit-result of Branch n in tree. 
    * Branch 0 displays the number of signal events, 
    * Branch 1 the signal event uncertainty, 
    * Branch 2 the number of background events and
    * Branch 3 the background event uncertainty.    
    */
    SproutTree fitTree(SproutTree tree, SproutFit* hfit);

    void makeTCanvas(SproutTree tree);
    void makeTH1F(std::vector<float> data, TString name, TString xlabel="x", TString ylabel="Counts");

    void setLineStyle(int color=1, int style=1, int width=3){line_color=color; line_style=style; line_width=width;}
    void setMarkerStyle(int color, int style, int size){marker_color=color; marker_style=style; marker_size=size;}
    void setPlotText(TH1F* h1, TString text);
    void setStyle(TH1F* h);

    void writeHist(TString plot_text = "");
    void writeCanvas(TString name);
    
    ClassDef(SproutPlot, 1) // Needed for compatability with ROOT's Cling interpreter 

};
#endif // RHELPER_H
