#include "SproutPlot.h"

ClassImp(SproutPlot) // Needed for compatability with ROOT's Cling interpreter 

//------------------------------------------
//				  PUBLIC
//------------------------------------------

//Constructor 
SproutPlot::SproutPlot(): 
fhist1(TH1F()),
fhist2(TH2F()),
line_color(1),
line_style(1), 
line_width(3),
marker_color(1),
marker_style(1),
marker_size(1),
draw_opt1(""),
draw_opt2(""){}

void SproutPlot::add(TString filename, TString h_identify){
	TFile f(filename);
	TList *list = f.GetListOfKeys();

	TKey *key;
	TIter iter(list);
	static TString classname("TH1F");

	while((key = (TKey*)iter())) {
		if(key->GetClassName() == classname) {
		TH1F *hist = (TH1F*)key->ReadObj();
		if(hist){
			TString histname = hist->GetName();
			if(histname.Contains(h_identify)){fvec.push_back(*hist);}
			delete hist;
		}
		}
	}
}

TH1F& SproutPlot::getTH1F(TString name, int bins, double xmin, double xmax, TString xlabel, TString ylabel){
	makeTH1F(name, bins, xmin, xmax, xlabel, ylabel); //fhist1 is cleared and adjusted
	return fvec.back();
}

TH1F& SproutPlot::getTH1F(std::vector<float> data,TString name, int bins, TString xlabel, TString ylabel){
	makeTH1F(data, name, bins, xlabel, ylabel); //fhist1 is cleared and adjusted 
	return fvec.back();
}

TH1F& SproutPlot::getTH1F(int i){
	std::list<TH1F>::iterator it = fvec.begin();
	int j=0;
	while(j<i){it++; j++;}
	return *it;
}

TH2F& SproutPlot::getTH2F(TString name, int binsx, double xmin, double xmax, int binsy, double ymin, double ymax, TString xlabel, TString ylabel){
	fhist2.Reset("ICESM"); //Resets fhist2
	setStyle(&fhist2); //Sets the default style
	// Sets the specified properties to fhist1
	fhist2.SetName(name);
	fhist2.SetBins(binsx, xmin, xmax, binsy, ymin, ymax);
    fhist2.SetXTitle(xlabel);
    fhist2.SetYTitle(ylabel);

	fvec2.push_back(fhist2);
	
	return fvec2.back();
}

void SproutPlot::plotTree(SproutTree tree, int bins, TString xlabel, TString ylabel){
	//Loop over each branch in 'tree'
	for(int i=0; i<tree.getNumBranches(); i++){
		TH1F& temp = getTH1F(tree.getBranch(i), "h"+int2str(i), bins, xlabel, ylabel);
		temp.SetTitle("B"+int2str(i+1));
	}
}

SproutPlot SproutPlot::setPlotText(TString text){
	if(text != ""){
		int latex_modifyer = text.CountChar('#')*4 + text.CountChar('^')*3;

		double text_box_x1;
		double text_box_x2;
		double text_box_y1 = 0.867575;
		double text_box_y2 = 0.940639;
		
		if(text.Length() - latex_modifyer <= 4){
			text_box_x1 = 0.7207207;
			text_box_x2 = 0.8678678;
		}
		else if(text.Length() - latex_modifyer <= 4){
			text_box_x1 = 0.7207207;
			text_box_x2 = 0.8678678;
		}
		else if(text.Length() - latex_modifyer <= 11){
			text_box_x1 = 0.5765765;
			text_box_x2 = 0.873873;			
		}
		else if(text.Length() - latex_modifyer <= 14){
			text_box_x1 = 0.561561;
			text_box_x2 = 0.9129129;
		}
		else if(text.Length() - latex_modifyer <= 17){
			text_box_x1 = 0.561561;
			text_box_x2 = 1;
		}
		else{
			text_box_x1 = 0.525525;
			text_box_x2 = 1;
		}

		TPaveText pt = TPaveText(text_box_x1, text_box_y1, text_box_x2, text_box_y2,"NDC");
		pt.SetBorderSize(1);
		pt.SetLineColor(kBlack);
		pt.SetLineWidth(3);
		pt.AddText(text); 
		text_box = pt;
		//text_box.DrawClone("same");
	}
	return *this;
}

void SproutPlot::setStyle(TH1F* h){
	//Sets default style of the given histogram 
	gStyle->SetOptStat(0);
	h->GetXaxis()->SetLabelSize(0.06);
	h->GetXaxis()->SetTitleSize(0.06);
	h->GetXaxis()->SetTitleOffset(0.75);
	h->GetXaxis()->CenterTitle();

	h->GetYaxis()->SetLabelSize(0.06);
	h->GetYaxis()->SetTitleSize(0.06);
	h->GetYaxis()->SetTitleOffset(0.75);
	h->GetYaxis()->CenterTitle();
	if(h->GetMaximum()/1000 >1) h->GetYaxis()->SetMaxDigits(3);
	if(h->GetMaximum()/10000 >1) h->GetYaxis()->SetMaxDigits(4);
	
	h->SetLineStyle(line_style);
	h->SetLineWidth(line_width);
	h->SetLineColor(line_color);
}

void SproutPlot::setStyle(TH2F* h){
	//Sets default style of the given histogram 
	gStyle->SetOptStat(0);
	h->GetXaxis()->SetLabelSize(0.06);
	h->GetXaxis()->SetTitleSize(0.06);
	h->GetXaxis()->SetTitleOffset(0.75);
	h->GetXaxis()->CenterTitle();

	h->GetYaxis()->SetLabelSize(0.06);
	h->GetYaxis()->SetTitleSize(0.06);
	h->GetYaxis()->SetTitleOffset(0.75);
	h->GetYaxis()->CenterTitle();
	if(h->GetMaximum()/1000 >1) h->GetYaxis()->SetMaxDigits(3);
	if(h->GetMaximum()/10000 >1) h->GetYaxis()->SetMaxDigits(4);
	
	h->SetMarkerStyle(marker_style);
	h->SetMarkerSize(marker_size);
	h->SetMarkerColor(marker_color);
}

void SproutPlot::write(TFile* file, TString name){
	file->WriteObject(&(*this), name); //write tree to file
}

void SproutPlot::writeBasic(){
	for(TH1F h : fvec){
		h.Write();
	}

	for(TH2F h : fvec2){
		h.Write();
	}
}

void SproutPlot::writeHist(){
	gROOT->SetBatch(kTRUE); // Turn on batch mode to avoid pop-ups
	for(TH1F h : fvec){
		h.SetStats(0);
		TCanvas can(h.GetName()); 
		setTCanvas(&can);

		can.cd(); //Open fcanvas for drawing
		if(draw_opt1 != ""){h.Draw(draw_opt1);}
		else{h.Draw();}

    	text_box.DrawClone("same");
		can.Write();
	}

	for(TH2F h : fvec2){
		h.SetStats(0);
		TCanvas can(h.GetName()); 
		setTCanvas(&can);
		can.cd(); //Open fcanvas for drawing 
    	if(draw_opt2 != ""){h.Draw(draw_opt2);}
		else{h.Draw("colz");}
		text_box.DrawClone("same");
		can.Write();
	}
	gROOT->SetBatch(kFALSE); // Turn on Batch-mode again.
}

void SproutPlot::writeCanvas(TString name){
	gROOT->SetBatch(kTRUE); // Needed for the Draw() to work properly for some reason...
	TCanvas can(name); 
	setTCanvas(&can, fvec.size()+fvec2.size());
	
	int i=0;
	for(TH1F h : fvec){
		h.SetStats(0);
		can.cd(i+1);
		if(draw_opt1 != ""){h.DrawClone(draw_opt1);}
		else{h.DrawClone();}
		text_box.DrawClone("same");
		i++;
	}

	for(TH2F h : fvec2){
		h.SetStats(0);
		can.cd(i+1);
		if(draw_opt2 != ""){h.DrawClone(draw_opt2);}
		else{h.DrawClone("colz");}
		text_box.DrawClone("same");
		i++;
	}

	can.SetName(name);
	can.Write(); //Writies fcanvas to file, provided one is open
	//if(save_as != ""){can.SaveAs(save_as);} //saves fcanvas as a .png-file with the specified name.
	gROOT->SetBatch(kFALSE); // Turn on Batch-mode again. 
}

void SproutPlot::saveHistAs(TString filePrefix,TString fileSuffix){
	gROOT->SetBatch(kTRUE); // Turn on batch mode to avoid pop-ups
	for(TH1F h : fvec){
		h.SetStats(0);
		TCanvas can(h.GetName()); 
		setTCanvas(&can);

		can.cd(); //Open fcanvas for drawing
		if(draw_opt1 != ""){h.Draw(draw_opt1);}
		else{h.Draw();}

    	text_box.DrawClone("same");
		can.Print(filePrefix+h.GetName()+fileSuffix);
	}

	for(TH2F h : fvec2){
		h.SetStats(0);
		TCanvas can(h.GetName()); 
		setTCanvas(&can);
		can.cd(); //Open fcanvas for drawing 
    	if(draw_opt2 != ""){h.Draw(draw_opt2);}
		else{h.Draw("colz");}
		text_box.DrawClone("same");
		can.Print(filePrefix+h.GetName()+fileSuffix);
	}
	gROOT->SetBatch(kFALSE); // Turn on Batch-mode again.	
}
void SproutPlot::saveCanvasAs(TString fileName){
	gROOT->SetBatch(kTRUE); // Needed for the Draw() to work properly for some reason...
	TCanvas can(fileName); 
	setTCanvas(&can, fvec.size()+fvec2.size());
	
	int i=0;
	for(TH1F h : fvec){
		h.SetStats(0);
		can.cd(i+1);
		if(draw_opt1 != ""){h.DrawClone(draw_opt1);}
		else{h.DrawClone();}
		text_box.DrawClone("same");
		i++;
	}

	for(TH2F h : fvec2){
		h.SetStats(0);
		can.cd(i+1);
		if(draw_opt2 != ""){h.DrawClone(draw_opt2);}
		else{h.DrawClone("colz");}
		text_box.DrawClone("same");
		i++;
	}

	can.SetName(fileName);
	can.Print(fileName); //saves fcanvas as a .png-file with the specified name.
	gROOT->SetBatch(kFALSE); // Turn on Batch-mode again. 
}

SproutTree SproutPlot::getBinEdges(TH1F h){
	SproutTree stree(2);
	int bins = h.GetXaxis()->GetNbins();

	for(int i=1; i<bins+1; i++){
		stree.addToBranch(0,h.GetXaxis()->GetBinLowEdge(i));
		stree.addToBranch(1,h.GetXaxis()->GetBinUpEdge(i));
	}
	return stree;
}

SproutPlot SproutPlot::setTitle(TString title){
	for(TH1F& h1: fvec){
		h1.SetTitle(title);
		h1.SetTitleSize(0.06);
	}
	for(TH2F& h2: fvec2){
		h2.SetTitle(title);
		h2.SetTitleSize(0.06);
	}
	return *this;
}

SproutPlot SproutPlot::operator+(SproutPlot obj){
	try{
		if(obj.fvec.size() != fvec.size() || obj.fvec2.size() != fvec2.size()){
			if((fvec.size() != 0 && fvec2.size() !=0) && (obj.fvec.size() != 0 && obj.fvec2.size() !=0)){
				throw(1);
			}
		}
		
		if(fvec.size() != 0 || fvec2.size() != 0){
			std::list<TH1F>::iterator it1 = obj.fvec.begin();
			for(TH1F& h : fvec){
				if(it1 != obj.fvec.end()){
					h.Add(&*it1);
					it1++;
				}
			} 
			std::list<TH2F>::iterator it2 = obj.fvec2.begin();
			for(TH2F& h : fvec2){
				if(it2 != obj.fvec2.end()){
					h.Add(&*it2);
					it2++;
				}
			} 
			return *this;
		}
		else{
			return obj;
		}
	}
	catch(int a){
		std::cout<< "SproutPlot::operator+ addition failed. Unequal number of histograms" << std::endl;
		exit(0);
	}
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
		else{fhist1.SetBins(10e3, min,max);} // number >= 10 000, data is divided into 10^3 bins
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




