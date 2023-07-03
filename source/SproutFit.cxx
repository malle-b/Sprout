#include "SproutFit.h"

SproutFit::SproutFit(TString sig_func, TString bg_func, int num) 
    : fsig(new TF1("fsig", sig_func, 0,1)), fbg(new TF1("fbg", bg_func, 0,1)), ffit(new TF1("ffit", "fsig+fbg",0,1)),
    npar(getNumParam(*ffit)), npar_sig(getNumParam(*fsig)), npar_bg(getNumParam(*fbg)),
    ftree(npar), count(0), sig_func_name(sig_func), ecount(num), inputfile("FitParam.txt"){

    readInput();
    static std::fstream ob;
    ob.open(inputfile, std::ios::out | std::ios::trunc);
    ob.close();
}

SproutFit::SproutFit(TString sig_func, TString bg_func, std::string infile, int num) 
    : fsig(new TF1("fsig", sig_func, 0,1)), fbg(new TF1("fbg", bg_func, 0,1)), ffit(new TF1("ffit", "fsig+fbg",0,1)),
    npar(getNumParam(*ffit)), npar_sig(getNumParam(*fsig)), npar_bg(getNumParam(*fbg)),
    ftree(npar), count(0), sig_func_name(sig_func), ecount(num), inputfile(infile){

    readInput();
    static std::fstream ob;
    ob.open("FitParam.txt", std::ios::out | std::ios::trunc);
    ob.close();
}


void SproutFit::readInput(){
    std::fstream ob;
    ob.open(inputfile, std::ios::in);
    float val;
    int i=0;
    while(ob >> val){
        ftree.addToBranch(i%npar, val);
        i++;
    }
    ob.close();

    if(ftree.getNumEntries(0)==1){
        for(int i=1; i<ecount*npar+1; i++){ftree.addToBranch((i-1)%npar, ftree.get((i-1)%npar,0));}
    }
}

void SproutFit::fit(TH1F* h){
    fsig->SetRange(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
    fbg->SetRange(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
    ffit->SetRange(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());


    if(ftree.getNumEntries(0)!=0){
        for(int i=0; i<npar; i++){ffit->SetParameter(i,ftree.get(i,count));}
    }    
    h->Fit("ffit", "Q");

    Double_t param[npar];
    ffit->GetParameters(param);
    fsig->SetParameters(&param[npar_bg]);
    fbg->SetParameters(&param[0]);

    std::fstream ob;
    ob.open("FitParam.txt", std::ios::app);
    for(int i=0; i<npar; i++){ob << ffit->GetParameter(i) << "  ";}
    ob << "\n";
    ob.close();
    count++;
}


void SproutFit::writeInput(){
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
    else{std::cout << "Error in SproutFit: No specified fit input parameters. File not written" << std::endl;}
}

void SproutFit::printInput(){
    ftree.print();
}