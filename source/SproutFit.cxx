#include "SproutFit.h"

SproutFit::SproutFit(std::string filename) :
inputfile(filename),
fsig(new TF1()), fbg(new TF1()), ffit(new TF1()),
npar_sig(0), npar_bg(0), npar(0),
chi2_sig(0), chi2_bg(0), chi2(0),
cfit(TMatrixDSym()), csig(TMatrixDSym()), cbg(TMatrixDSym()),
fit_line_color(1), sig_line_color(2), bg_line_color(3),
fit_line_style(2), sig_line_style(2), bg_line_style(2),
line_width(3) 
{}

void SproutFit::fit(SproutPlot splot){
    TCanvas can = TCanvas(); 
    splot.setTCanvas(&can, splot.getSize());

    std::fstream ob;
    ob.open(inputfile, std::ios::in);
    std::string line;

    int i=0;
    for(TH1F& h : splot){
        readLine(&ob,line);

        setFitFunctions();
        fit(h);

        can.cd(i+1);
        drawResult(h);

        i++;
        writeOutput();
    }
    can.SaveAs("testCanFit.png");
    ob.close();

    const int length = inputfile.length();
    const char* char_array = inputfile.c_str();

    std::remove(char_array);
    std::rename("TestParamOutput.txt", char_array);
}

void SproutFit::fitBackground(TH1F h){
    TH1F* temp = (TH1F*) h.Clone("temp");
    int low_bin = temp->GetXaxis()->FindBin(xsigmin);
    int up_bin = temp->GetXaxis()->FindBin(xsigmax);

    while(low_bin <= up_bin){
        temp->SetBinError(low_bin,0);
        low_bin++;
    }

    temp->Fit(fbg, "SQ0", "", xmin, xmax);

    chi2_bg = fbg->GetChisquare()/fbg->GetNDF();

    for(int i=0; i<npar_bg; i++){
        ffit->SetParameter(i, fbg->GetParameter(i));
    }
}

void SproutFit::fitSignal(TH1F h){
    TH1F* temp = (TH1F*) h.Clone("temp");

    // //Fix parameters of ffit to current values of fbg
    // for(int i=0; i<npar_bg; i++){ffit->FixParameter(i,fbg->GetParameter(i));} 

    // temp->Fit(ffit, "S0B","",xsigmin, xsigmax);

    // //Release the parameters of ffit so they're no longer fixed 
    // for(int i=0; i<npar_bg; i++){ffit->ReleaseParameter(i);} 

    // chi2_sig = ffit->GetNDF();
    // updateParam();

    temp->Add(fbg, -1);
    temp->Fit(fsig, "SQ0", "", xsigmin, xsigmax);
    chi2_sig = fsig->GetChisquare()/fsig->GetNDF();

    for(int i=npar_bg; i<npar; i++){
        ffit->SetParameter(i, fsig->GetParameter(i-npar_bg));
    }  
}

void SproutFit::fit(TH1F h){
    fitBackground(h);
    fitSignal(h);
    h.Fit(ffit, "SQ0", "",xmin, xmax);
    updateParam();

    fitBackground(h);
    fitSignal(h);
    h.Fit(ffit, "SQ0", "",xmin, xmax);
    updateParam();

    fitBackground(h);
    fitSignal(h);
    h.Fit(ffit, "SQ0", "",xmin, xmax);
    updateParam();

    chi2 = ffit->GetChisquare()/ffit->GetNDF();
}

void SproutFit::drawResult(TH1F h){
    h.DrawCopy(); //draw the histogram 
    TH1F* htemp = (TH1F*) h.Clone("htemp");
    htemp->Add(fbg,-1); //subtract the fitted background from fhist1
    htemp->DrawClone("same"); //draw background-subtracted histogram 
    fsig->DrawCopy("same"); //draw the fitted signal function 
    fbg->DrawCopy("same"); //draw the fitted background function 
    ffit->DrawCopy("same");

    // Draw lines to Indicate the limits of the fit-Integral
    int height = h.GetMaximum() + 0.1 * h.GetMaximum(); 

    TGraph xmin1 = TGraph(2);
    TGraph xmax1 = TGraph(2);
    TGraph xmin2 = TGraph(2);
    TGraph xmax2 = TGraph(2);

    xmin1.SetPoint(0, xmin, 0);
    xmin1.SetPoint(1, xmin, height);

    xmax1.SetPoint(0, xsigmin, 0);
    xmax1.SetPoint(1, xsigmin, height);

    xmin2.SetPoint(0, xsigmax, 0);
    xmin2.SetPoint(1, xsigmax, height);

    xmax2.SetPoint(0, xmax, 0);
    xmax2.SetPoint(1, xmax, height);

    xmin1.SetLineColor(13); xmin1.SetLineWidth(3); xmin1.SetLineStyle(2);
    xmax1.SetLineColor(15); xmax1.SetLineWidth(3); xmax1.SetLineStyle(2);
    xmin2.SetLineColor(15); xmin2.SetLineWidth(3); xmin2.SetLineStyle(2);
    xmax2.SetLineColor(13); xmax2.SetLineWidth(3); xmax2.SetLineStyle(2);

    xmin1.DrawClone("same");
    xmax1.DrawClone("same");
    xmin2.DrawClone("same");
    xmax2.DrawClone("same");

    TPaveText pt = TPaveText(0.7,0.7,0.99,0.99,"brNDC");
    pt.SetBorderSize(0);
    pt.SetFillColor(0);
    pt.SetFillStyle(0);
    pt.AddText("#chi^{2}_{#nu}(bg) = "+double2str(chi2_bg));
    pt.AddText("#chi^{2}_{#nu}(sig) = "+double2str(chi2_sig));
    pt.AddText("#chi^{2}_{#nu}(tot) = "+double2str(chi2));
    pt.DrawClone("same");
}

void SproutFit::updateParam(){
    Double_t p[npar];
    ffit->GetParameters(p);
    fsig->SetParameters(&p[npar_bg]);
    fbg->SetParameters(&p[0]);
}

void SproutFit::setFitFunctions(){
    TF1 tsig = TF1("tsig", (TString)sig_name, xmin, xmax);
    TF1 tbg = TF1("tbg", (TString) bg_name, xmin, xmax);

    fsig = (TF1*) tsig.Clone("fsig");
    fbg = (TF1*) tbg.Clone("fbg");

    npar_sig = getNumParam(*fsig);
    npar_bg = getNumParam(*fbg);
    
    TF1 tfit = TF1("tfit", "tsig+tbg", xmin, xmax);

    ffit = (TF1*) tfit.Clone("ffit");
    npar = getNumParam(*ffit);

    if(param.size()<npar){
        for(int i=0; i<npar; i++){ffit->SetParameter(i,1);}
    }
    else{
        for(int i=0; i<param.size(); i++){ffit->SetParameter(i,param[i]);}
    }

    updateParam();

    setStyle();
}


void SproutFit::readLine(std::fstream* ob, std::string line){
    param.clear();
    std::getline(*ob, line);

    std::istringstream iss(line);

    iss >> bg_name >> sig_name;
    iss >> xmin >> xmax;
    iss >> xsigmin >> xsigmax;

    float val;
    while(iss >> val){
        param.push_back(val);
    }

    // std::cout << "read line: " << bg_name << " " << sig_name << " " << xmin << " " << xmax << " " << xsigmin << " " << xsigmax << " ";
    // for(float val : param){std::cout << val << " ";}
    // std::cout << "\n";

}

void SproutFit::writeOutput(){
    std::fstream ob;

    ob.open("TestParamOutput.txt", std::ios::app);
    ob << bg_name << " " << sig_name << " " << xmin << " " << xmax << " " << xsigmin << " " << xsigmax << " ";
    for(int i=0; i<npar; i++){ob << ffit->GetParameter(i) << "  ";}
    ob << "\n";
    ob.close();
}


// SproutFit::SproutFit(TString sig_func, TString bg_func, int num) 
//     : fsig(new TF1("fsig", sig_func, 0,1)), fbg(new TF1("fbg", bg_func, 0,1)), ffit(new TF1("ffit", "fsig+fbg",0,1)),
//     npar(getNumParam(*ffit)), npar_sig(getNumParam(*fsig)), npar_bg(getNumParam(*fbg)),
//     ftree(npar), count(0), sig_func_name(sig_func), ecount(num), par_range(100),inputfile("FitParam.txt"),
//     cfit(TMatrixDSym(npar)), csig(TMatrixDSym(npar_sig)), cbg(TMatrixDSym(npar_bg)),
//     fit_line_color(1), sig_line_color(1), bg_line_color(1),
//     fit_line_style(2), sig_line_style(2), bg_line_style(2),
//     line_width(3){

//     readInput();
//     static std::fstream ob;
//     ob.open(inputfile, std::ios::out | std::ios::trunc);
//     ob.close();
// }

// SproutFit::SproutFit(TString sig_func, TString bg_func, std::string infile, int num) 
//     : fsig(new TF1("fsig", sig_func, 0,1)), fbg(new TF1("fbg", bg_func, 0,1)), ffit(new TF1("ffit", "fsig+fbg",0,1)),
//     npar(getNumParam(*ffit)), npar_sig(getNumParam(*fsig)), npar_bg(getNumParam(*fbg)),
//     ftree(npar), count(0), sig_func_name(sig_func), ecount(num), par_range(100), inputfile(infile),
//     cfit(TMatrixDSym(npar)), csig(TMatrixDSym(npar_sig)), cbg(TMatrixDSym(npar_bg)),
//     fit_line_color(1), sig_line_color(1), bg_line_color(1),
//     fit_line_style(2), sig_line_style(2), bg_line_style(2),
//     line_width(3){

//     readInput();
//     static std::fstream ob;
//     ob.open("FitParam.txt", std::ios::out | std::ios::trunc);
//     ob.close();
// }


// void SproutFit::readInput(){
//     std::fstream ob;
//     ob.open(inputfile, std::ios::in);
//     float val;
//     int i=0;
//     while(ob >> val){
//         ftree.addToBranch(i%npar, val);
//         i++;
//     }
//     ob.close();

//     if(ftree.getNumEntries(0)==1){
//         for(int i=1; i<ecount*npar+1; i++){ftree.addToBranch((i-1)%npar, ftree.get((i-1)%npar,0));}
//     }
// }

// void SproutFit::fit(TH1F* h){
//     fsig->SetRange(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
//     fbg->SetRange(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
//     ffit->SetRange(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());


//     if(ftree.getNumEntries(0)!=0){
//         for(int i=0; i<npar; i++){
//             ffit->SetParameter(i,ftree.get(i,count));
//             ffit->SetParLimits(i,ftree.get(i,count)-par_range, ftree.get(i,count)+par_range);
//         }
//     }

//     setStyle();    
//     TFitResultPtr r = h->Fit("ffit","SQB");

//     Double_t param[npar];
//     ffit->GetParameters(param);
//     fsig->SetParameters(&param[npar_bg]);
//     fbg->SetParameters(&param[0]);

//     cfit = r->GetCovarianceMatrix();
//     cfit.GetSub(npar_bg,npar-1, csig,""); 
//     cfit.GetSub(0,(npar_bg-1), cbg,"");

//     writeOutput();
//     count++;
// }

// SproutTree SproutFit::integrate(double low, double high, double bin_width){
//     SproutTree outtree(4);

//     int sig_integral = fsig->Integral(low,high)/bin_width; 
//     int bg_integral = fbg->Integral(low,high)/bin_width;

//     float sig_error = fsig->IntegralError(low,high, fsig->GetParameters(),csig.GetMatrixArray());
//     float bg_error = fbg->IntegralError(low,high, fbg->GetParameters(),cbg.GetMatrixArray());

//     outtree.addToBranch(0, sig_integral);
//     outtree.addToBranch(1, sig_error);
//     outtree.addToBranch(2, bg_integral);
//     outtree.addToBranch(3, bg_error);

//     return outtree;
// }



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

// void SproutFit::writeInput(){
//     std::fstream ob;
//     if(ftree.getNumEntries(0)!=0){
//         ob.open("FitParamInput.txt", std::ios::app);
//         for(int i=0; i<count; i++){
//             for(int j=0; j<npar; j++){
//                 ob << ftree.get(j,i) << "  ";
//             }
//             ob << "\n";
//         }
//         ob.close();
//     }
//     else{std::cout << "Error in SproutFit: No specified fit input parameters. File not written" << std::endl;}
// }

// void SproutFit::printInput(){
//     ftree.print();
// }