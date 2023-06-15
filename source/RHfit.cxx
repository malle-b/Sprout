#include "RHfit.h"

RHfit::RHfit(TString sig_func, TString bg_func) 
    : fsig(new TF1("fsig", sig_func, 0,1)), fbg(new TF1("fbg", bg_func, 0,1)), ffit(new TF1("ffit", "fsig+fbg",0,1)),
    npar(getNumParam(*ffit)), npar_sig(getNumParam(*fsig)), npar_bg(getNumParam(*fbg)),
    ftree(npar), count(0){

    readInput();
    static std::fstream ob;
    ob.open("FitParam.txt", std::ios::out | std::ios::trunc);
    ob.close();
}


void RHfit::readInput(){
    std::fstream ob;
    ob.open("FitParam.txt", std::ios::in);
    float val;
    int i=0;
    while(ob >> val){
        ftree.addToBranch(i%npar, val);
        i++;
    }
    ob.close();
}

void RHfit::fit(TH1F* h){
    fsig->SetRange(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
    fbg->SetRange(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
    ffit->SetRange(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());


    if(ftree.getNumEntries(0)!=0){
        for(int i=0; i<npar; i++){ffit->SetParameter(i,ftree.get(i,count));}
    }

    
    h->Fit("ffit", "Q");

    std::fstream ob;
    ob.open("FitParam.txt", std::ios::app);
    for(int i=0; i<npar; i++){ob << ffit->GetParameter(i) << "  ";}
    ob << "\n";
    ob.close();
    count++;
}


// RHtree RHfit::fit(RHtree intree, HistogramHelper h){
//     RHtree outtree(intree.getNumBranches());
//     for(int i=0; i<int.getNumBranches();i++){
//         for(int j=0; j<intree.getMaxEntries();j++){
            

//         }

//     }
// }


void RHfit::writeInput(){
    std::fstream ob;
    if(ftree.getNumEntries(0)!=0){
        ob.open("FitParamInput.txt", std::ios::app);
        for(int i=0; i<count; i++){
            for(int j=0; j<npar; j++){
                ob << ftree.get(j,i) << "  ";
            }
            ob << "\n";
        }
        ob.close();
    }
    else{std::cout << "Error in RHFit: No specified fit input parameters. File not written" << std::endl;}
}

void RHfit::printInput(){
    ftree.print();
}