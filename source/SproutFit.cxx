#include "SproutFit.h"

SproutFit::SproutFit(std::string filename) :
inputfile(filename),
fsig(new TF1()), fbg(new TF1()), ffit(new TF1()),
npar_sig(0), npar_bg(0), npar(0),
cfit(TMatrixDSym()), csig(TMatrixDSym()), cbg(TMatrixDSym()),
fit_line_color(1), sig_line_color(1), bg_line_color(1),
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
        h.Fit(ffit,"SQ0");
        updateParam();

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

    

}

void SproutFit::drawResult(TH1F h){
    h.DrawCopy(); //draw the histogram 
    TH1F* htemp = (TH1F*) h.Clone("htemp");
    htemp->Add(fbg,-1); //subtract the fitted background from fhist1
    htemp->DrawClone("same"); //draw background-subtracted histogram 
    fsig->DrawCopy("same"); //draw the fitted signal function 
    fbg->DrawCopy("same"); //draw the fitted background function 
    ffit->DrawCopy("same");
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
    TF1 tfit = TF1("tfit", "tsig+tbg", xmin, xmax);

    fsig = (TF1*) tsig.Clone("fsig");
    fbg = (TF1*) tbg.Clone("fbg");
    ffit = (TF1*) tfit.Clone("ffit");

    npar_sig = getNumParam(*fsig);
    npar_bg = getNumParam(*fbg);
    npar = getNumParam(*ffit);

    for(int i=0; i<param.size(); i++){ffit->SetParameter(i,param[i]);}

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

    std::cout << "read line: " << bg_name << " " << sig_name << " " << xmin << " " << xmax << " " << xsigmin << " " << xsigmax << " ";
    for(float val : param){std::cout << val << " ";}
    std::cout << "\n";

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