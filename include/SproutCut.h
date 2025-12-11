#pragma once 

#include "SproutPlot.h"
#include "SproutCutValue.h"
#include "TVector3.h"
#include <unordered_map>
#include<cmath>
#include "TGraph.h"
#include "TMath.h"
#include "TH1F.h"

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TLegend.h"

class SproutCut {
    public:
    SproutCut();
    
    SproutCut operator+(const SproutCut obj);

    void addCutWindow(std::string name, std::string descr, float min, float max);
    void addCutMinimum(std::string name, std::string descr, float value, float QAplotRange = 100);
    void addCutMaximum(std::string name, std::string descr, float value, float QAplotRange = 100);
    void setYieldQAPlot(std::string xTitle, int bins, float xMin, float xMax);
    
    void fillYieldQAPlot(float val, std::string name="no_cuts"){sp_yield_control.getTH1F(name).Fill(val);}
    //void setCutoff(std::string name, float min, float max);
    void print();
    int cut(std::string name, float val);
    int cut(std::string name, float val, bool isSignal);
    int cut(std::string name, float val, float controlVal);
    int cut(std::string name, float val, bool isSignal, float controlVal);
    
    void write(TFile* file, TString name); //write SproutCut to file 
    void writeQAplot(std::string title = "QAplots"); //write TCanvas to file 
    SproutPlot getQaHists(){return sp_qa;}
    int tuneCuts(std::string title);
    int tuneCuts(std::string title, double fractionOfSignal);

    private:
    void addcut(SproutCutValue& cut, std::string name, std::string descr, float min_range, float max_range);
    std::unordered_map<std::string, SproutCutValue> bcuts;
    //std::unordered_map<std::string, TH1F&> bhists;

    SproutPlot sp_qa;
    SproutPlot sp_yield_control;


    ClassDef(SproutCut, 1) // Needed for compatability with ROOT's Cling interpreter 
};