#ifndef HISTOGRAMHELPER_H
#define HISTOGRAMHELPER_H

#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TTree.h"

#include "RHtree.h"
#include "RHfit.h"

#include <iostream>
#include <vector>

class HistogramHelper {

private:
    TCanvas* fcanvas;
    TH1F* fhist1;

    void makeTCanvas();
    void makeTCanvas(int nhist);
    TString int2str(int i){TString str; str.Form("%d", i); return str;}

public:

    HistogramHelper();
    ~HistogramHelper(){delete fcanvas; delete fhist1;}
    HistogramHelper(const HistogramHelper& that) = delete;
    HistogramHelper& operator=(const HistogramHelper& that) = delete;

    void makeTCanvas(RHtree tree);
    void makeTCanvas(RHtree tree, RHfit* hfit);

    void makeTH1F(TString name, double xmin, double xmax, 
                   TString xlabel="x", TString ylabel="Counts", int bins=100);
    void makeTH1F(std::vector<float> data, TString name, TString xlabel="x", TString ylabel="Counts");
    void makeTH2F(TString name, double xmin, double xmax, double ymin, double ymax, 
                   TString xlabel='x', TString ylabel='y', int bins=100);

    void setStyle(TH1F *h);
    void setPlotText(TH1F* h1, TString text);

    void writeHist(TString plot_text = "");
    void writeCanvas(TString name);

    RHtree binData(std::vector<float> adata, std::vector<float> bdata, int n);

    ClassDef(HistogramHelper, 1) // Needed for compatability with ROOT's Cling interpreter 

};
#endif // RHELPER_H
