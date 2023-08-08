#include "SproutPlot.h"

ClassImp(SproutPlot) // Needed for compatability with ROOT's Cling interpreter 

//------------------------------------------
//				  PUBLIC
//------------------------------------------

//Constructor 
SproutPlot::SproutPlot(): 
fhist1(TH1F()),
line_color(1),
line_style(1), 
line_width(3),
marker_color(1),
marker_style(1),
marker_size(1){}

TH1F& SproutPlot::getTH1F(TString name, int bins, double xmin, double xmax, TString xlabel, TString ylabel){
	makeTH1F(name, bins, xmin, xmax, xlabel, ylabel); //fhist1 is cleared and adjusted
	std::cout << "fvec size " << fvec.size() << std::endl;
	return fvec.back();
}

TH1F& SproutPlot::getTH1F(std::vector<float> data,TString name, int bins, TString xlabel, TString ylabel){
	makeTH1F(data, name, bins, xlabel, ylabel); //fhist1 is cleared and adjusted 
	return fvec.back();
}

TH1F& SproutPlot::getTH1F(int i){
	std::list<TH1F>::iterator it = begin();
	int j=0;
	while(j<i){it++; j++;}
	return *it;
}

void SproutPlot::plotTree(SproutTree tree, int bins, TString xlabel, TString ylabel){
	//Loop over each branch in 'tree'
	for(int i=0; i<tree.getNumBranches(); i++){
		TH1F& temp = getTH1F(tree.getBranch(i), "h"+int2str(i), bins, xlabel, ylabel);
		temp.SetTitle("B"+int2str(i+1));
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

void SproutPlot::writeHist(TString plot_text){
	for(TH1F h : fvec){
		TCanvas can = TCanvas(); 
		setTCanvas(&can);

		can.cd(); //Open fcanvas for drawing 
    	h.Draw();

    	setPlotText(&h, plot_text);
		writeCanvas(h.GetName()); //write and save fcanvas with the same name as the histogram. 
	}
}

void SproutPlot::writeCanvas(TString name){
	TCanvas can = TCanvas(); 
	setTCanvas(&can, fvec.size());
	std::cout << "get here " << std::endl;

	int i=0;
	for(TH1F h : fvec){
		can.cd(i+1);
		h.DrawClone();
		i++;
	}

	can.Write(); //Writies fcanvas to file, provided one is open
	can.SaveAs(name+".png"); //saves fcanvas as a .png-file with the specified name. 
}

//------------------------------------------
//				  PRIVATE
//------------------------------------------

void SproutPlot::setTCanvas(TCanvas* fcanvas){
	fcanvas->Clear(); //Resets canvas
	//Sets default window size 
	fcanvas->SetCanvasSize(800, 500);
	fcanvas->SetWindowSize(801, 501);
	fcanvas->SetTopMargin(1.);
	fcanvas->SetBottomMargin(0.15);
	fcanvas->SetRightMargin(0.10);
	fcanvas->SetLeftMargin(0.10);
}

void SproutPlot::setTCanvas(TCanvas* fcanvas, int nhist){
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
	fhist1.Reset("ICESM"); //Resets fhist1
	setStyle(&fhist1); //Sets the default style

	// Sets the specified properties to fhist1
	fhist1.SetName(name);
	fhist1.SetBins(bins, xmin, xmax);
    fhist1.SetXTitle(xlabel);
    fhist1.SetYTitle(ylabel);

	fvec.push_back(fhist1);

}

void SproutPlot::makeTH1F(std::vector<float> data, TString name, int bins, TString xlabel, TString ylabel){
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

	fhist1.Reset("ICESM"); //Resets fhist1
	setStyle(&fhist1); //sets the default style 


	if(bins==0){
		/*Sets the number of bins and histogram range 
		depending the number of digits in 'number', i.e. the size of data.*/
		if(count < 2){fhist1.SetBins(2, min, max);} //number < 10, data is divided into 2 bins 
		else if(count < 4){fhist1.SetBins(10^(count), min, max);} //number < 10 000, data is divided into 10^(count) bins
		else{fhist1.SetBins(10^3, min,max);} // number >= 10 000, data is divided into 10^3 bins
	}
	else{
		fhist1.SetBins(bins,min,max);
	}


	//Sets the specified histogram properties. 
	fhist1.SetName(name);
    fhist1.SetXTitle(xlabel);
    fhist1.SetYTitle(ylabel);

	//Fills fhist1 with the contents in 'data'. 
	for(int i=0; i<data.size(); i++){fhist1.Fill(data[i]);}
	fvec.push_back(fhist1);
}




