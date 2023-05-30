#ifndef HISTOGRAMHELPER_H
#define HISTOGRAMHELPER_H

#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TTree.h"
#include "RHtree.h"

#include <iostream>
#include <vector>

class HistogramHelper {

private:
    TCanvas* makeTCanvas();
    TCanvas* makeTCanvas(int nhist);
    TString int2str(int i){TString str; str.Form("%d", i); return str;}

public:

    HistogramHelper();

    TCanvas* makeTCanvas(RHtree* tree);
    TH1F* makeTH1F(TString name, double xmin, double xmax, 
                   TString xlabel="x", TString ylabel="Counts", int bins=100);
    TH1F* makeTH1F(std::vector<float> data, TString name, TString xlabel="x", TString ylabel="Counts");
    TH1F* makeInvMassTH1F(TString name, double mass, double range=200, int bins=100);
    std::vector<TH1F*> makeInvMassTH1FVector(int num, TString name, double mass, double range=200, int bins=100);

    TH2F* makeTH2F(TString name, double xmin, double xmax, double ymin, double ymax, 
                   TString xlabel='x', TString ylabel='y', int bins=100);

    void setStyle(TH1F *h);
    void setStyle(TH2F *h);
    void setPlotText(TH1F* h1, TString text);
    void setPlotText(TH2F* h1, TString text);

    void writeHist(TH1F *h1, TString plot_text = "");
    void writeHist(TH2F *h1, TString drawopt="",TString plot_text = "");
    void writeCanvas(TCanvas* c, TString name);

    RHtree* binData(std::vector<float> adata, std::vector<float> bdata, int n);

    ClassDef(HistogramHelper, 1) // Needed for compatability with ROOT's Cling interpreter 

};
#endif // RHELPER_H
