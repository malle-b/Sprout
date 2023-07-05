#include "SproutFit.h"

SproutFit::SproutFit(TString sig_func, TString bg_func, int num) 
    : fsig(new TF1("fsig", sig_func, 0,1)), fbg(new TF1("fbg", bg_func, 0,1)), ffit(new TF1("ffit", "fsig+fbg",0,1)),
    npar(getNumParam(*ffit)), npar_sig(getNumParam(*fsig)), npar_bg(getNumParam(*fbg)),
    ftree(npar), count(0), sig_func_name(sig_func), ecount(num), par_range(100),inputfile("FitParam.txt"),
    cfit(TMatrixDSym(npar)), csig(TMatrixDSym(npar_sig)), cbg(TMatrixDSym(npar_bg)),
    fit_line_color(1), sig_line_color(1), bg_line_color(1),
    fit_line_style(2), sig_line_style(2), bg_line_style(2),
    line_width(3){

    readInput();
    static std::fstream ob;
    ob.open(inputfile, std::ios::out | std::ios::trunc);
    ob.close();
}

SproutFit::SproutFit(TString sig_func, TString bg_func, std::string infile, int num) 
    : fsig(new TF1("fsig", sig_func, 0,1)), fbg(new TF1("fbg", bg_func, 0,1)), ffit(new TF1("ffit", "fsig+fbg",0,1)),
    npar(getNumParam(*ffit)), npar_sig(getNumParam(*fsig)), npar_bg(getNumParam(*fbg)),
    ftree(npar), count(0), sig_func_name(sig_func), ecount(num), par_range(100), inputfile(infile),
    cfit(TMatrixDSym(npar)), csig(TMatrixDSym(npar_sig)), cbg(TMatrixDSym(npar_bg)),
    fit_line_color(1), sig_line_color(1), bg_line_color(1),
    fit_line_style(2), sig_line_style(2), bg_line_style(2),
    line_width(3){

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
        for(int i=0; i<npar; i++){
            ffit->SetParameter(i,ftree.get(i,count));
            ffit->SetParLimits(i,ftree.get(i,count)-par_range, ftree.get(i,count)+par_range);
        }
    }

    setStyle();    
    TFitResultPtr r = h->Fit("ffit","SQB");

    Double_t param[npar];
    ffit->GetParameters(param);
    fsig->SetParameters(&param[npar_bg]);
    fbg->SetParameters(&param[0]);

    cfit = r->GetCovarianceMatrix();
    cfit.GetSub(npar_bg,npar-1, csig,""); 
    cfit.GetSub(0,(npar_bg-1), cbg,"");

    writeOutput();
    count++;
}

SproutTree SproutFit::integrate(double low, double high, double bin_width){
    SproutTree outtree(4);

    int sig_integral = fsig->Integral(low,high)/bin_width; 
    int bg_integral = fbg->Integral(low,high)/bin_width;

    float sig_error = fsig->IntegralError(low,high, fsig->GetParameters(),csig.GetMatrixArray());
    float bg_error = fbg->IntegralError(low,high, fbg->GetParameters(),cbg.GetMatrixArray());

    outtree.addToBranch(0, sig_integral);
    outtree.addToBranch(1, sig_error);
    outtree.addToBranch(2, bg_integral);
    outtree.addToBranch(3, bg_error);

    return outtree;
}

void SproutFit::writeOutput(){
    std::fstream ob;
    ob.open("FitParam.txt", std::ios::app);
    for(int i=0; i<npar; i++){ob << ffit->GetParameter(i) << "  ";}
    ob << "\n";
    ob.close();
}

void SproutFit::setStyle(){
    ffit->SetLineColor(fit_line_color);
    fsig->SetLineColor(sig_line_color);
    fbg->SetLineColor(bg_line_color);

    ffit->SetLineStyle(bg_line_style);
    fsig->SetLineStyle(sig_line_style);
    fbg->SetLineStyle(bg_line_style);

    ffit->SetLineWidth(line_width);
    fsig->SetLineWidth(line_width);
    fbg->SetLineWidth(line_width);
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