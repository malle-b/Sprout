#include "HistogramHelper.h"

ClassImp(HistogramHelper) // Needed for compatability with ROOT's Cling interpreter 

HistogramHelper::HistogramHelper(){
	
}

TCanvas* HistogramHelper::makeTCanvas(){
	TCanvas *canvas = new TCanvas();
	canvas->SetCanvasSize(800, 500);
	canvas->SetWindowSize(801, 501);
	canvas->SetTopMargin(1.);
	canvas->SetBottomMargin(0.15);
	canvas->SetRightMargin(0.10);
	canvas->SetLeftMargin(0.10);

	return canvas;
}

TCanvas* HistogramHelper::makeTCanvas(int nhist){
	TCanvas* c = new TCanvas();

	/*Finds suitible canvas splitting into 
	rows and columns depending on nhist*/
	int rows,cols;
	int i=1; int res=1;
	if(nhist==0 || nhist==1){rows=nhist; rows = nhist;}
	else{ while(res <= nhist){i++; res=i*i;} rows=i-1;}	
	if(nhist%rows==0){cols=nhist/rows;}
	else{cols = nhist/rows + 1;}


	c->SetCanvasSize(800*cols, 500*rows);
	c->SetWindowSize(810*cols, 510*rows);
	c->SetTopMargin(1.);
	c->SetBottomMargin(0.01);
	c->SetRightMargin(0.50);
	c->SetLeftMargin(0.10);

	c->Divide(cols,rows);

	return c;
}

TCanvas* HistogramHelper::makeTCanvas(RHtree* tree){
	TCanvas* c = makeTCanvas(tree->getNumBranches());

	for(int i=0; i<tree->getNumBranches(); i++){
		TH1F* h = makeTH1F(tree->getBranch(i), "h"+int2str(i));
		setPlotText(h,"bin "+int2str(i));
		c->cd(i+1);
		h->Draw();
	}
	return c;
}


TH1F* HistogramHelper::makeTH1F(TString name, double xmin, double xmax, TString xlabel, TString ylabel, int bins){
    TH1F* h = new TH1F(name, "", bins, xmin, xmax);
    h->SetXTitle(xlabel);
    h->SetYTitle(ylabel);
    setStyle(h);
    return h;
}

TH1F* HistogramHelper::makeTH1F(std::vector<float> data, TString name, TString xlabel, TString ylabel){
	float min = *std::min_element(data.begin(), data.end());
	float max = *std::max_element(data.begin(), data.end());

	int count = 0;
	int number = data.size();
    while(number != 0){number = number / 10; count++;}

	TH1F* h = new TH1F(name, "", 10^(count/2), min, max);
	h->SetXTitle(xlabel);
    h->SetYTitle(ylabel);
    setStyle(h);
	for(int i=0; i<data.size(); i++) h->Fill(data[i]);
    return h;
}

TH1F* HistogramHelper::makeInvMassTH1F(TString name, double mass, double range, int bins){
	TH1F* h = makeTH1F(name, mass-range, mass+range, "MeV/c^{2}", "counts", bins);
	return h;
}

std::vector<TH1F*> HistogramHelper::makeInvMassTH1FVector(int num, TString name, double mass, double range, int bins){
	std::vector<TH1F*> hvec;
	hvec.clear();
	for(int i=0; i<num; i++){
		TString index_str; index_str.Form("%d", i);
		hvec.push_back(makeInvMassTH1F(name+index_str, mass, range, bins)); 
	}

	return hvec;
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


void HistogramHelper::writeHist(TH1F *h1, TString plot_text){

	//h1->ClearUnderflowAndOverflow();
	
	TCanvas *can = makeTCanvas();
	can->cd();
    h1->Draw();

    setPlotText(h1, plot_text);

	can->Write();

    TString title = h1->GetName();
	can->SaveAs(title+".png");
}

void HistogramHelper::writeCanvas(TCanvas* can, TString name){
	can->Write();
	can->SaveAs(name+".png");
}

RHtree* HistogramHelper::binData(std::vector<float> adata, std::vector<float> bdata, int n){

	float min = *std::min_element(adata.begin(), adata.end());
	float max = *std::max_element(adata.begin(), adata.end());
	float bin_width = (max-min)/n;

	RHtree* tree = new RHtree(n);

	for(int i=0; i<adata.size(); i++){
		for(int j=1; j<n+1; j++){
			if(adata[i]>=bin_width*(j-1) && adata[i]<bin_width*j){tree->addToBranch(j-1,bdata[i]);} // skips last element
		}
	}
	return tree;
}
