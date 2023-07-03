#include "SproutPlot.h"

ClassImp(SproutPlot) // Needed for compatability with ROOT's Cling interpreter 

SproutPlot::SproutPlot(){
	fcanvas = new TCanvas();
	fhist1 = new TH1F();

	setStyle(fhist1);
}

void SproutPlot::makeTCanvas(){
	fcanvas->Clear();
	fcanvas->SetCanvasSize(800, 500);
	fcanvas->SetWindowSize(801, 501);
	fcanvas->SetTopMargin(1.);
	fcanvas->SetBottomMargin(0.15);
	fcanvas->SetRightMargin(0.10);
	fcanvas->SetLeftMargin(0.10);
}

void SproutPlot::makeTCanvas(int nhist){
	/*Finds suitible canvas splitting into 
	rows and columns depending on nhist*/
	int rows,cols;
	int i=1; int res=1;
	if(nhist==0 || nhist==1){rows=nhist; rows = nhist;}
	else{ while(res <= nhist){i++; res=i*i;} rows=i-1;}	
	if(nhist%rows==0){cols=nhist/rows;}
	else{cols = nhist/rows + 1;}

	fcanvas->Clear();
	fcanvas->SetCanvasSize(800*cols, 500*rows);
	fcanvas->SetWindowSize(810*cols, 510*rows);
	fcanvas->SetTopMargin(1.);
	fcanvas->SetBottomMargin(0.01);
	fcanvas->SetRightMargin(0.50);
	fcanvas->SetLeftMargin(0.10);

	fcanvas->Divide(cols,rows);
}

void SproutPlot::makeTCanvas(SproutTree tree){
	makeTCanvas(tree.getNumBranches()); // fcanvas gets cleared and adjusted 
	for(int i=0; i<tree.getNumBranches(); i++){
		makeTH1F(tree.getBranch(i), "h"+int2str(i)); //fhist1 gets cleared and adjusted
		setPlotText(fhist1,"bin "+int2str(i));
		fcanvas->cd(i+1);
		fhist1->DrawCopy();
		
	}
}

SproutTree SproutPlot::makeTCanvas(SproutTree tree, SproutFit* hfit){ // Think of a way to do this without double for-loop
	makeTCanvas(tree.getNumBranches()); // fcanvas gets cleared here 

	SproutTree outtree(1);
	for(int i=0; i<tree.getNumBranches(); i++){
		makeTH1F(tree.getBranch(i), "hfit"+int2str(i)); //fhist1 gets cleared and adjusted
		setPlotText(fhist1,"bin "+int2str(i));

		fcanvas->cd(i+1);
		hfit->fit(fhist1);
		TF1* bg = hfit->getBackground();
		TF1* sig = hfit->getSignal();
		fhist1->DrawCopy();
		fhist1->Add(bg,-1);
		fhist1->DrawClone("same");
		sig->Draw("same");
		bg->Draw("same");

		if(hfit->getSigFuncName() == "gaus"){
			double low = (double) sig->GetParameter(1)-3*sig->GetParameter(2);
			double high = (double) sig->GetParameter(1)+3*sig->GetParameter(2);

			double blow = (double) fhist1->FindFixBin(low);
			double bhigh = (double) fhist1->FindFixBin(high);

			outtree.addToBranch(0,(float)fhist1->Integral(blow,bhigh));
		}
		else{outtree.addToBranch(0,(float)fhist1->Integral(0,fhist1->GetNbinsX()));}
	}
	if(hfit->getSigFuncName() != "gaus"){std::cout << "Signal not Gaussian, no cut-off on histogram entries." << std::endl;}
	return outtree;
}


void SproutPlot::makeTH1F(TString name, double xmin, double xmax, TString xlabel, TString ylabel, int bins){
	fhist1->Reset("ICESM");

	fhist1->SetName(name);
	fhist1->SetBins(bins, xmin, xmax);
    fhist1->SetXTitle(xlabel);
    fhist1->SetYTitle(ylabel);
}

void SproutPlot::makeTH1F(std::vector<float> data, TString name, TString xlabel, TString ylabel){

	float min=0; float max=1; int count = 0;
	long number = data.size();
	if(number>0){
		min = *std::min_element(data.begin(), data.end());
		max = *std::max_element(data.begin(), data.end());
		while(number != 0){number = number / 10; count++;}
	}

	fhist1->Reset("ICESM");

	if(count < 2){fhist1->SetBins(2, min, max);}
	else if(count < 4){fhist1->SetBins(10^(count), min, max);}
	else{fhist1->SetBins(10^3, min,max);}


	fhist1->SetName(name);
    fhist1->SetXTitle(xlabel);
    fhist1->SetYTitle(ylabel);

	for(int i=0; i<data.size(); i++){fhist1->Fill(data[i]);}
}

void SproutPlot::setPlotText(TH1F* h1, TString text){
    TLatex latexText(h1->GetXaxis()->GetBinUpEdge(h1->GetXaxis()->GetLast())*0.8, h1->GetMaximum()*0.85, text);
	latexText.SetTextFont(42);
	latexText.SetTextSize(0.08);
	latexText.SetTextColor(15);
	latexText.DrawClone();
}

void SproutPlot::setStyle(TH1F *h){

	gStyle->SetOptStat(0);
	h->GetXaxis()->SetLabelSize(0.06);
	h->GetXaxis()->SetTitleSize(0.06);
	h->GetXaxis()->SetTitleOffset(0.7);
	h->GetXaxis()->CenterTitle();

	h->GetYaxis()->SetLabelSize(0.06);
	h->GetYaxis()->SetTitleSize(0.06);
	h->GetYaxis()->SetTitleOffset(0.7);
	h->GetYaxis()->CenterTitle();
	if(h->GetMaximum()/1000 >1) h->GetYaxis()->SetMaxDigits(3);
	if(h->GetMaximum()/10000 >1) h->GetYaxis()->SetMaxDigits(4);
	
	h->SetLineWidth(1);
	h->SetLineColor(kBlack);
}


void SproutPlot::writeHist(TString plot_text){
	
	makeTCanvas(); //fcanvas gets cleared and set to default 
	fcanvas->cd();
    fhist1->Draw();

    setPlotText(fhist1, plot_text);

	fcanvas->Write();

    TString title = fhist1->GetName();
	fcanvas->SaveAs(title+".png");
}

void SproutPlot::writeCanvas(TString name){
	fcanvas->Write();
	fcanvas->SaveAs(name+".png");
}


