#include "SproutFit.h"

SproutFit::SproutFit() :
inputfile("FitParam.txt"),
savefile(nullptr),
fsig(new TF1()), fbg(new TF1()), ffit(new TF1()),
sig_name(""), bg_name(""),
xmin(0), xmax(0), xsigmin(0), xsigmax(0),
npar_sig(0), npar_bg(0), npar(0),
chi2_sig(0), chi2_bg(0), chi2(0),
cfit(new TMatrixDSym()), csig(new TMatrixDSym()), cbg(new TMatrixDSym()),
fit_line_color(7), sig_line_color(8), bg_line_color(6),
fit_line_style(1), sig_line_style(1), bg_line_style(1),
line_width(3) 
{   
gErrorIgnoreLevel = kError;   // ignore warnings, show only errors
gErrorIgnoreLevel = kFatal;   // show only fatal errors
}

SproutFit::SproutFit(std::string fitParamFile) :
inputfile("temp"),
fsig(new TF1()), fbg(new TF1()), ffit(new TF1()),
sig_name(""), bg_name(""),
xmin(0), xmax(0), xsigmin(0), xsigmax(0),
npar_sig(0), npar_bg(0), npar(0),
chi2_sig(0), chi2_bg(0), chi2(0),
cfit(new TMatrixDSym()), csig(new TMatrixDSym()), cbg(new TMatrixDSym()),
fit_line_color(7), sig_line_color(8), bg_line_color(6),
fit_line_style(1), sig_line_style(1), bg_line_style(1),
line_width(3) 
{inputfile = fitParamFile;
gErrorIgnoreLevel = kError;   // ignore warnings, show only errors
gErrorIgnoreLevel = kFatal;   // show only fatal errors
}

void SproutFit::fit(SproutPlot splot, TString save_as, SproutTree* stree, bool save_hist){
    gROOT->SetBatch(kTRUE); // Turn on batch mode to avoid pop-ups
    TCanvas can; 
    splot.setTCanvas(&can, splot.getTH1Size());

    std::fstream ob;
    ob.open(inputfile, std::ios::in);
    std::string line;

    int i=0;
    for(auto it = splot.beginTH1(); it != splot.endTH1(); it++){
        TH1F& h = it->second;

        readLine(&ob,line);

        setFitProperties();

        if(save_hist==true){fit(h,save_as+int2str(i+1));}
        else{fit(h);}
        
        can.cd(i+1);
        h.SetTitle(TString(h.GetTitle())+" fit "+int2str(i+1));
        drawResult(h);

        i++;
        writeOutput();
        if(stree!=nullptr){
            float counts[10]={0}; 
            integrate(h,counts);
            int j=0;
            for(float val : counts){
                stree->addToBranch(j,val);
                j++;
            }
        }
    }

    if(save_as.EndsWith(".png")){can.SaveAs(save_as);}
    if(savefile != nullptr){
        savefile->cd();
        can.Write(TString(save_canvas_name));
    }

    ob.close();

    const int length = inputfile.length();
    const char* char_array = inputfile.c_str();

    std::remove(char_array);
    std::rename("TestParamOutput.txt", char_array);
    gROOT->SetBatch(kFALSE); // Turn off batch mode 

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

    //Fix parameters of ffit to current values of fbg
    for(int i=0; i<npar_bg; i++){ffit->FixParameter(i,fbg->GetParameter(i));} 

    temp->Fit(ffit, "SQ0B","",xsigmin, xsigmax);

    //Release the parameters of ffit so they're no longer fixed 
    for(int i=0; i<npar_bg; i++){ffit->ReleaseParameter(i);} 

    chi2_sig = ffit->GetChisquare()/ffit->GetNDF();
    updateParam();

}

void SproutFit::fit(TH1F h, TString save_as){
    if(xmin==xmax){xmin = h.GetXaxis()->GetXmin(); xmax = h.GetXaxis()->GetXmax();}

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
    TFitResultPtr r = h.Fit(ffit, "SQ0", "",xmin, xmax);

    if(r.Get() == nullptr){return;}
    updateParam();

    chi2 = ffit->GetChisquare()/ffit->GetNDF();

    *cfit = r->GetCovarianceMatrix();
    cfit->GetSub(npar_bg,npar-1, *csig,""); 
    cfit->GetSub(0,(npar_bg-1), *cbg,"");

    

    if(save_as != ""){
        SproutPlot plt = SproutPlot();
        TCanvas can;
        plt.setTCanvas(&can);

        can.cd();
        drawResult(h);
        if(save_as.EndsWith(".png")){can.SaveAs(save_as);}
        if(savefile != nullptr){
            savefile->cd();
            can.Write(TString(save_canvas_name));
        }
    }
}

void SproutFit::drawResult(TH1F h){
    //gROOT->SetBatch(kTRUE); // Turn on batch mode to avoid pop-ups
    h.SetMinimum(0);  h.SetStats(0);
    h.DrawCopy(); //draw the histogram 
    TH1F* htemp = (TH1F*) h.Clone("htemp");
    //htemp->Add(fbg,-1); //subtract the fitted background from fhist1
    //htemp->DrawClone("same"); //draw background-subtracted histogram 
    //fsig->DrawCopy("same"); //draw the fitted signal function 
    fbg->DrawCopy("same"); //draw the fitted background function 
    ffit->DrawCopy("same");

    // Draw lines to Indicate the limits of the fit-Integral
    int height = h.GetMaximum() + 0.1 * h.GetMaximum(); 

    TGraph xmin1 = TGraph(2);
    TGraph xmax1 = TGraph(2);
    TGraph xmin2 = TGraph(2);
    TGraph xmax2 = TGraph(2);

    xmin1.SetPoint(0, xmin, 0);     xmin1.SetPoint(1, xmin, height);
    xmax1.SetPoint(0, xsigmin, 0);  xmax1.SetPoint(1, xsigmin, height);
    xmin2.SetPoint(0, xsigmax, 0);  xmin2.SetPoint(1, xsigmax, height);
    xmax2.SetPoint(0, xmax, 0);     xmax2.SetPoint(1, xmax, height);

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
    //gROOT->SetBatch(kFALSE); // Turn on batch mode to avoid pop-ups

}

void SproutFit::updateParam(){
    Double_t p[npar];
    ffit->GetParameters(p);
    fsig->SetParameters(&p[npar_bg]);
    fbg->SetParameters(&p[0]);
}

void SproutFit::setFitProperties(){
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

    TMatrixDSym cfit_temp = TMatrixDSym(npar);
    TMatrixDSym csig_temp = TMatrixDSym(npar_sig);
    TMatrixDSym cbg_temp = TMatrixDSym(npar_bg);

    cfit = (TMatrixDSym*) cfit_temp.Clone();
    csig = (TMatrixDSym*) csig_temp.Clone();
    cbg = (TMatrixDSym*) cbg_temp.Clone();


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
}

void SproutFit::writeOutput(){
    std::fstream ob;

    ob.open("TestParamOutput.txt", std::ios::app);
    ob << bg_name << " " << sig_name << " " << xmin << " " << xmax << " " << xsigmin << " " << xsigmax << " ";
    for(int i=0; i<npar; i++){ob << ffit->GetParameter(i) << "  ";}
    ob << "\n";
    ob.close();
}

void SproutFit::integrate(TH1F h, float vals[10]){

    if(h.GetEntries()==0){return;}

    float sig_integral = fsig->Integral(xmin,xmax)/(h.GetXaxis()->GetBinWidth(1)); 
    float sig_error = fsig->IntegralError(xmin,xmax, fsig->GetParameters(),csig->GetMatrixArray(),1.E-6)/(h.GetXaxis()->GetBinWidth(1)); 

    float bg_integral = fbg->Integral(xmin,xmax)/(h.GetXaxis()->GetBinWidth(1));
    float bg_error = fbg->IntegralError(xmin,xmax, fbg->GetParameters(),cbg->GetMatrixArray(),1.E-6)/(h.GetXaxis()->GetBinWidth(1)); 

    h.Sumw2();
    //h.Add(fbg,-1,"I"); //Should I realy be specified? might be wrong 
    h.Add(fbg,-1); // might be wrong
    double h_error;

    float hsig_integral = h.IntegralAndError(h.GetXaxis()->FindBin(xsigmin),h.GetXaxis()->FindBin(xsigmax),h_error);
    float hsig_error = sqrt(h_error*h_error + bg_error*bg_error);

    float bg_N_sig_region = fbg->Integral(xsigmin,xsigmax)/(h.GetXaxis()->GetBinWidth(1));
    float bg_error_N_sig_region = fbg->IntegralError(xsigmin,xsigmax, fbg->GetParameters(),cbg->GetMatrixArray(),1.E-6)/(h.GetXaxis()->GetBinWidth(1)); 

    float bg_N_bg_region1 = fbg->Integral(xmin,xsigmin)/(h.GetXaxis()->GetBinWidth(1));
    float bg_error_N_bg_region1 = fbg->IntegralError(xmin,xsigmin, fbg->GetParameters(),cbg->GetMatrixArray(),1.E-6)/(h.GetXaxis()->GetBinWidth(1)); 

    float bg_N_bg_region2 = fbg->Integral(xsigmax,xmax)/(h.GetXaxis()->GetBinWidth(1));
    float bg_error_N_bg_region2 = fbg->IntegralError(xsigmax,xmax, fbg->GetParameters(),cbg->GetMatrixArray(),1.E-6)/(h.GetXaxis()->GetBinWidth(1)); 

    float bg_N_bg_region = bg_N_bg_region1 + bg_N_bg_region2;
    float bg_error_N_bg_region = sqrt(bg_error_N_bg_region1*bg_error_N_bg_region1 + bg_error_N_bg_region2*bg_error_N_bg_region2);

    float alpha = bg_N_sig_region/bg_N_bg_region;
    float alpha_error = sqrt(std::pow(bg_error_N_sig_region/bg_N_bg_region,2) + std::pow(bg_error_N_bg_region*bg_N_sig_region/(bg_N_bg_region*bg_N_bg_region),2));

    vals[0] = sig_integral;
    vals[1] = sig_error;
    vals[2] = hsig_integral;
    vals[3] = hsig_error;
    vals[4] = bg_N_sig_region;
    vals[5] = bg_error_N_sig_region;
    vals[6] = bg_N_bg_region;
    vals[7] = bg_error_N_bg_region;
    vals[8] = alpha;
    vals[9] = alpha_error;
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

void SproutFit::setFit(std::string bf_func, std::string sig_func, double xlow, double xup, double siglow, double sigup, std::vector<double> pars){
    bg_name = bf_func;
    sig_name = sig_func;
    xmin = xlow;
    xmax = xup;
    xsigmin = siglow;
    xsigmax = sigup;
    param = pars;

    setFitProperties();
}
