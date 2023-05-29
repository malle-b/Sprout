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
	canvas->SetRightMargin(0.03);
	canvas->SetLeftMargin(0.10);

	return canvas;
}

TH1F* HistogramHelper::makeTH1F(TString name, double xmin, double xmax, TString xlabel, TString ylabel, int bins){
    TH1F* h = new TH1F(name, "", bins, xmin, xmax);
    h->SetXTitle(xlabel);
    h->SetYTitle(ylabel);
    setStyle(h);
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

//Make this return a ttree with n branches and the elements of evtnum and bdata sorted into the correct ones 
//Make sure each branch only contains one vector
//std::vector<std::vector<float>> binning(std::vector<float> evtnum, std::vector<float> adata, std::vector<float> bdata, int n){



	// float min = *std::min_element(adata.begin(), adata.end());
	// float max = *std::max_element(adata.begin(), adata.end());
	// float range = max-min;

	// for(int i=0; i<adata.size(); i++){
	// 	for(int j=0; j<n; j++){
	// 		if(adata[i]==max){

	// 		}
	// 		else if(adata[i]<range/n*(j+1)){
				
	// 		}
	// 	}
	// }
//}
