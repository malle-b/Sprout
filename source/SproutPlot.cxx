#include "SproutPlot.h"

ClassImp(SproutPlot) // Needed for compatability with ROOT's Cling interpreter 

//------------------------------------------
//				  PUBLIC
//------------------------------------------

//Constructor 
SproutPlot::SproutPlot(): 
fcanvas(new TCanvas()),
fhist1(new TH1F()),
line_color(1),
line_style(1), 
line_width(3),
marker_color(1),
marker_style(1),
marker_size(1){}

TH1F SproutPlot::getTH1F(TString name, int bins, double xmin, double xmax, TString xlabel, TString ylabel){
	makeTH1F(name, bins, xmin, xmax, xlabel, ylabel); //fhist1 is cleared and adjusted
	return *fhist1;
}

TH1F SproutPlot::getTH1F(std::vector<float> data, TString name, TString xlabel, TString ylabel){
	makeTH1F(data, name, xlabel, ylabel); //fhist1 is cleared and adjusted 
	return *fhist1;
}

SproutTree SproutPlot::fitTree(SproutTree tree, SproutFit* hfit){ // Think of a way to do this without double for-loop
	// fcanvas gets cleared here and initialized to contain the same number of histograms as 'tree' has branches. 
	makeTCanvas(tree.getNumBranches()); 

	//Initialize output-SproutTree containing the number of signal/background events + errors
	SproutTree outtree(4);

	// Loop over each branch in 'tree'
	for(int i=0; i<tree.getNumBranches(); i++){
		makeTH1F(tree.getBranch(i), "hfit"+int2str(i)); //fhist1 gets cleared and filled with data in branch i in 'tree'
		setPlotText(fhist1,"bin "+int2str(i)); //fhist1 plot text set to "bin i"

		fcanvas->cd(i+1); //open the canvas for drawing. 
		hfit->fit(fhist1); //perform the fit on fhist1. The fitted function is drawn automatically. 

		//get the fitted backgorund and signal functions
		TF1* bg = hfit->getBackground();
		TF1* sig = hfit->getSignal();

		fhist1->DrawCopy(); //draw the histogram 
		fhist1->Add(bg,-1); //subtract the fitted background from fhist1
		fhist1->DrawClone("same"); //draw background-subtracted histogram 
		sig->DrawCopy("same"); //draw the fitted signal function 
		bg->DrawCopy("same"); //draw the fitted background function 

		//Calculate the lower and upper edge of the background-subtracted histogram 
		double low = fhist1->GetXaxis()->GetXmin();
		double high = fhist1->GetXaxis()->GetXmax();

		//Integrate the fitted signal and background functions over the range low-high. 
		SproutTree counts = hfit->integrate(low, high,fhist1->GetBinWidth(1)); //save results in 'counts'
		
		//Add the results saved in 'count' to the output 'outtree'
		for(int j=0; j<outtree.getNumBranches(); j++){outtree.addToBranch(j,counts.get(j,0));}
	 }
	writeCanvas("big_boifit"); //Save and write the canvas 
	return outtree; //return SproutTree with number of signal and background events + errors
}

void SproutPlot::plotTree(SproutTree tree){
	//fcanvas gets cleared and initialized to contain the same number of histograms as 'tree' has branches. 
	makeTCanvas(tree.getNumBranches()); 

	//Loop over each branch in 'tree'
	for(int i=0; i<tree.getNumBranches(); i++){
		//fhist1 gets cleared and filled with data in branch i in 'tree'
		makeTH1F(tree.getBranch(i), "h"+int2str(i));
		setPlotText(fhist1,"bin "+int2str(i)); //fhist1 plot text set to "bin i"

		fcanvas->cd(i+1); //open canvas for drawing 
		fhist1->DrawCopy(); //draw the histogram
	}
}

void SproutPlot::setPlotText(TH1F* h1, TString text){
	//Sets parameters for text style and where on the histogram it should appear. 
    TLatex latexText(h1->GetXaxis()->GetBinUpEdge(h1->GetXaxis()->GetLast())*0.8, h1->GetMaximum()*0.85, text);
	latexText.SetTextFont(42);
	latexText.SetTextSize(0.08);
	latexText.SetTextColor(15);
	latexText.DrawClone();
}

void SproutPlot::setStyle(TH1F* h){
	//Sets default style of the given histogram 
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
	
	h->SetLineStyle(line_style);
	h->SetLineWidth(line_width);
	h->SetLineColor(line_color);
}

void SproutPlot::writeHist(TH1F* h, TString plot_text){
	makeTCanvas(); //fcanvas gets cleared and set to default 
	fcanvas->cd(); //Opeb fcanvas for drawing 
    h->Draw();

    setPlotText(fhist1, plot_text);
	writeCanvas(h->GetName()); //write and save fcanvas with the same name as the histogram. 
}

//------------------------------------------
//				  PRIVATE
//------------------------------------------

void SproutPlot::makeTCanvas(){
	fcanvas->Clear(); //Resets canvas
	//Sets default window size 
	fcanvas->SetCanvasSize(800, 500);
	fcanvas->SetWindowSize(801, 501);
	fcanvas->SetTopMargin(1.);
	fcanvas->SetBottomMargin(0.15);
	fcanvas->SetRightMargin(0.10);
	fcanvas->SetLeftMargin(0.10);
}

void SproutPlot::makeTCanvas(int nhist){
	/*Finds suitible canvas splitting into 
	rows and columns such that the window is as square as possible*/
	int rows,cols;
	int i=1;

	//Finds the floor of the square root of nhist and sets it to rows 
	if(nhist==0 || nhist==1){rows = nhist;}
	else{ 
		while(i*i <= nhist){i++;} 
		rows=i-1;
	}

	//Assigns a value to cols such that the window is as square as possible 
	if(nhist%rows==0){cols=nhist/rows;}
	else{cols = nhist/rows + 1;}

	fcanvas->Clear(); // Resets fcanvas

	//Sets the window dimensions based on rows and cols 
	fcanvas->SetCanvasSize(800*cols, 500*rows);
	fcanvas->SetWindowSize(810*cols, 510*rows);
	fcanvas->SetTopMargin(1.);
	fcanvas->SetBottomMargin(0.01);
	fcanvas->SetRightMargin(0.50);
	fcanvas->SetLeftMargin(0.10);

	//Divides the canvas into pads for which histograms can be drawn. 
	fcanvas->Divide(cols,rows);
}

void SproutPlot::makeTH1F(TString name, int bins, double xmin, double xmax, TString xlabel, TString ylabel){
	fhist1->Reset("ICESM"); //Resets fhist1
	setStyle(fhist1); //Sets the default style

	// Sets the specified properties to fhist1
	fhist1->SetName(name);
	fhist1->SetBins(bins, xmin, xmax);
    fhist1->SetXTitle(xlabel);
    fhist1->SetYTitle(ylabel);
}

void SproutPlot::makeTH1F(std::vector<float> data, TString name, TString xlabel, TString ylabel){
	//Finds suitable values for binning and histogram range 
	float min=0; float max=1; int count = 0;
	long number = data.size();
	if(number>0){
		//Finds the smallest and largest entries of data. These are later set as the range of fhist1 
		min = *std::min_element(data.begin(), data.end());
		max = *std::max_element(data.begin(), data.end());

		//Calculates the number of digits in 'number' and assigns this to 'count' 
		while(number != 0){number = number / 10; count++;}
	}

	fhist1->Reset("ICESM"); //Resets fhist1
	setStyle(fhist1); //sets the default style 

	/*Sets the number of bins and histogram range 
	depending the number of digits in 'number', i.e. the size of data.*/
	if(count < 2){fhist1->SetBins(2, min, max);} //number < 10, data is divided into 2 bins 
	else if(count < 4){fhist1->SetBins(10^(count), min, max);} //number < 10 000, data is divided into 10^(count) bins
	else{fhist1->SetBins(10^3, min,max);} // number >= 10 000, data is divided into 10^3 bins

	//Sets the specified histogram properties. 
	fhist1->SetName(name);
    fhist1->SetXTitle(xlabel);
    fhist1->SetYTitle(ylabel);

	//Fills fhist1 with the contents in 'data'. 
	for(int i=0; i<data.size(); i++){fhist1->Fill(data[i]);}
}

void SproutPlot::writeCanvas(TString name){
	fcanvas->Write(); //Writies fcanvas to file, provided one is open
	fcanvas->SaveAs(name+".png"); //saves fcanvas as a .png-file with the specified name. 
}


