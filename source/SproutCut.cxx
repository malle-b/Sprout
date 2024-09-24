#include "SproutCut.h"

SproutCut::SproutCut() :
bcuts(),
bhists(),
sp_qa()
{}


void SproutCut::addCut(std::string name, std::string descr, float min, float max){
    bcuts.insert({name,{min,max}});
    float range = max-min;
    TH1F& h = sp_qa.getTH1F(name, 100, min-range*0.4, max+range*0.4, descr, "counts");
    bhists.insert({name,h});
}

int SproutCut::cut(std::string name, float val){
    std::vector<float> cut_vec = bcuts[name];
    
    (bhists.find(name)->second).Fill(val);

    if(val>cut_vec[0] && val<cut_vec[1]){return 1;}
    else{return 0;}
}

void SproutCut::write(std::string title){
    TString can_name(title);
    TCanvas can(can_name); sp_qa.setTCanvas(&can, bhists.size());
    int i=0;
    for(auto& p : bhists){
        std::string name = p.first;
        TH1F& h = p.second;
        
        std::vector<float> cut_vec = bcuts[name];

        can.cd(i+1);
        h.Draw();

        int height = h.GetMaximum() + 0.1 * h.GetMaximum(); 

        TGraph xmin = TGraph(2);
        TGraph xmax = TGraph(2);

        xmin.SetPoint(0,cut_vec[0], 0);     xmin.SetPoint(1, cut_vec[0], height);
        xmax.SetPoint(0, cut_vec[1], 0);  xmax.SetPoint(1, cut_vec[1], height);

        xmin.SetLineColor(15); xmin.SetLineWidth(3); xmin.SetLineStyle(2);
        xmax.SetLineColor(15); xmax.SetLineWidth(3); xmax.SetLineStyle(2);

        xmin.DrawClone("same");
        xmax.DrawClone("same");
        i++;
    }
    can.Write();
}