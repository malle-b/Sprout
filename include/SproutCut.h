#pragma once 

#include "SproutPlot.h"
#include "TVector3.h"
#include <unordered_map>
#include "TGraph.h"
#include "TH1F.h"

#include "TCanvas.h"

class SproutCut {
    public:
    SproutCut();

    void addCut(std::string name, std::string descr, float min, float max);
    void setCutoff(std::string name, float min, float max);
    void print();
    int cut(std::string name, float val);
    void write(std::string title = "QAplots");
    SproutPlot getQaHists(){return sp_qa;}

    private:
    std::unordered_map<std::string, std::vector<float>> bcuts;
    std::unordered_map<std::string, TH1F&> bhists;

    SproutPlot sp_qa;
};