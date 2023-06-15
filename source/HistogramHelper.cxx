#include "HistogramHelper.h"

ClassImp(HistogramHelper) // Needed for compatability with ROOT's Cling interpreter 

HistogramHelper::HistogramHelper(){
	fcanvas = new TCanvas();
	fhist1 = new TH1F();

	setStyle(fhist1);
}

void HistogramHelper::makeTCanvas(){
	fcanvas->Clear();
	fcanvas->SetCanvasSize(800, 500);
	fcanvas->SetWindowSize(801, 501);
	fcanvas->SetTopMargin(1.);
	fcanvas->SetBottomMargin(0.15);
	fcanvas->SetRightMargin(0.10);
	fcanvas->SetLeftMargin(0.10);
}

void HistogramHelper::makeTCanvas(int nhist){
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

void HistogramHelper::makeTCanvas(RHtree tree){
	makeTCanvas(tree.getNumBranches()); // fcanvas gets cleared and adjusted 

	for(int i=0; i<tree.getNumBranches(); i++){
		makeTH1F(tree.getBranch(i), "h"+int2str(i)); //fhist1 gets cleared and adjusted
		setPlotText(fhist1,"bin "+int2str(i));
		fcanvas->cd(i+1);
		fhist1->DrawCopy();
	}
}

void HistogramHelper::makeTCanvas(RHtree tree, RHfit* hfit){
	makeTCanvas(tree.getNumBranches()); // fcanvas gets cleared here 

	for(int i=0; i<tree.getNumBranches(); i++){
		makeTH1F(tree.getBranch(i), "hfit"+int2str(i)); //fhist1 gets cleared and adjusted
		setPlotText(fhist1,"bin "+int2str(i));
		fcanvas->cd(i+1);
		hfit->fit(fhist1);
		fhist1->DrawCopy();
	}
}


void HistogramHelper::makeTH1F(TString name, double xmin, double xmax, TString xlabel, TString ylabel, int bins){
	fhist1->Reset("ICESM");

	fhist1->SetName(name);
	fhist1->SetBins(bins, xmin, xmax);
    fhist1->SetXTitle(xlabel);
    fhist1->SetYTitle(ylabel);
}

void HistogramHelper::makeTH1F(std::vector<float> data, TString name, TString xlabel, TString ylabel){
	float min = *std::min_element(data.begin(), data.end());
	float max = *std::max_element(data.begin(), data.end());

	int count = 0;
	int number = data.size();
    while(number != 0){number = number / 10; count++;}

	fhist1->Reset("ICESM");

	fhist1->SetName(name);
	fhist1->SetBins(10^(count), min, max);
    fhist1->SetXTitle(xlabel);
    fhist1->SetYTitle(ylabel);

	for(int i=0; i<data.size(); i++) fhist1->Fill(data[i]);
}

void HistogramHelper::setPlotText(TH1F* h1, TString text){
    TLatex latexText(h1->GetXaxis()->GetBinUpEdge(h1->GetXaxis()->GetLast())*0.8, h1->GetMaximum()*0.85, text);
	latexText.SetTextFont(42);
	latexText.SetTextSize(0.08);
	latexText.SetTextColor(15);
	latexText.DrawClone();
}

void HistogramHelper::setStyle(TH1F *h){

	gStyle->SetOptStat(0);
	h->GetXaxis()->SetLabelSize(0.06);
	h->GetXaxis()->SetTitleSize(0.06);
	h->GetXaxis()->SetTitleOffset(1.1);
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


void HistogramHelper::writeHist(TString plot_text){
	
	makeTCanvas(); //fcanvas gets cleared and set to default 
	fcanvas->cd();
    fhist1->Draw();

    setPlotText(fhist1, plot_text);

	fcanvas->Write();

    TString title = fhist1->GetName();
	fcanvas->SaveAs(title+".png");
}

void HistogramHelper::writeCanvas(TString name){
	fcanvas->Write();
	fcanvas->SaveAs(name+".png");
}

RHtree HistogramHelper::binData(std::vector<float> adata, std::vector<float> bdata, int n){ // write copy constructor so I can call this without returning a pointer. 

	float min = *std::min_element(adata.begin(), adata.end());
	float max = *std::max_element(adata.begin(), adata.end());
	float bin_width = (max-min)/n;

	RHtree tree(n);

	for(int i=0; i<adata.size(); i++){
		for(int j=1; j<n+1; j++){
			if(adata[i]>=bin_width*(j-1) && adata[i]<bin_width*j){tree.addToBranch(j-1,bdata[i]);} // skips last element
		}
	}
	return tree;
}
