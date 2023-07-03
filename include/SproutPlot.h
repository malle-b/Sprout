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

    void makeTCanvas();
    void makeTCanvas(int nhist);
    TString int2str(int i){TString str; str.Form("%d", i); return str;}


public:

    SproutPlot();
    ~SproutPlot(){delete fcanvas; delete fhist1;}
    SproutPlot(const SproutPlot& that) = delete;
    SproutPlot& operator=(const SproutPlot& that) = delete;

    void makeTCanvas(SproutTree tree);
    SproutTree makeTCanvas(SproutTree tree, SproutFit* hfit);

    void makeTH1F(TString name, double xmin, double xmax, 
                   TString xlabel="x", TString ylabel="Counts", int bins=100);
    void makeTH1F(std::vector<float> data, TString name, TString xlabel="x", TString ylabel="Counts");
    void makeTH2F(TString name, double xmin, double xmax, double ymin, double ymax, 
                   TString xlabel='x', TString ylabel='y', int bins=100);

    void setStyle(TH1F *h);
    void setPlotText(TH1F* h1, TString text);

    void writeHist(TString plot_text = "");
    void writeCanvas(TString name);

    TH1F getTH1F(){return *fhist1;}
    TH1F getTH1F(TString name, double xmin, double xmax, 
        TString xlabel="x", TString ylabel="Counts", int bins=100){
        
        makeTH1F(name, xmin, xmax, xlabel, ylabel, bins);
        return getTH1F();}
    

    //SproutTree binData(std::vector<float> adata, std::vector<float> bdata, int n);

    ClassDef(SproutPlot, 1) // Needed for compatability with ROOT's Cling interpreter 

};
#endif // RHELPER_H
