#ifndef SPROUTPLOT_H
#define SPROUTPLOT_H

#include "TCanvas.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TStyle.h"

#include "SproutTree.h"

#include <iostream>
#include <list>

class SproutPlot {
public:

    SproutPlot(); // Constructor 
    // ~SproutPlot(){delete fcanvas; delete fhist1;} // Destructor 
    // SproutPlot(const SproutPlot& that) = delete; // Copy constructor set to delete, object cannot be copied
    // SproutPlot& operator=(const SproutPlot& that) = delete; // Assignmet operator set to delete, object cannot be assigned

    /** 
    * Returns an empty TH1F histogram with properties set to the specified parameters.
    * 
    * @param name name of the histogram
    * @param bins number of bins 
    * @param xmin lower edge of x-axis
    * @param xmax upper edge of x-axis
    * @param xlabel label displayed on x-axis. Set to "x" by default
    * @param ylabel label displayed on y-axis. Set to "counts" by default 
    *
    * @return empty TH1F histogram 
    */
    TH1F& getTH1F(TString name, int bins, double xmin, double xmax, TString xlabel="x", TString ylabel="Counts");


    /**
    * Returns a histogram filled with the entries of 'data' with properties set to the specified parameters. 
    * The binning is set automatically based on the number of entries in 'data'. 
    *
    * @param data vector containing floats that will be used ot fill the returned histogram
    * @param name name of the histogram 
    * @param xlabel label displayed on x-axis. Set to "x" by default
    * @param ylabel label displayed on y-axis. Set to "counts" by default 
    *
    * @return filled TH1F histogram 
    */
    TH1F& getTH1F(std::vector<float> data, TString name, TString xlabel="x", TString ylabel="Counts");

    /**
    * Fits all branches of 'tree' to some signal and background model functions specified in 'hfit'
    * and generates a canvas to display the resulting plots. 
    * The number of signal and background events as well as their uncertanties 
    * are estimated from the fitted functions and returned as output.
    * The generated canvas is saved as a .png-file with the name "big_boifit.png"
    *
    * @param tree SproutTree for which each branch is to be fitted and plotted as a histogram. 
    * @param hfit pointer to a SproutFit object, specifying the model for signal and background. 
    * 
    * @return four-branched SproutTree where row n corresponds to the fit-result of Branch n in 'tree'. 
    * Branch 0 displays the number of signal events, 
    * Branch 1 the signal event uncertainty, 
    * Branch 2 the number of background events and
    * Branch 3 the background event uncertainty.    
    */
    //SproutTree fitTree(SproutTree tree, SproutFit* hfit);


    /**
    * Creates a histogram of each branch in 'tree' and generates a canvas where each 
    * histogram is drawn. The generated canvas is saved as a .png-file with the name 
    * "big_poi.png". The canvas is also saved to a .root-file if one is open. 
    *
    * The produced histograms are binned automatically based on the number of entries in each 
    * branch. 
    * 
    * @param tree SproutTree for which a histogram is to be drawn for each branch. 
    */
    void plotTree(SproutTree tree);

    /**
    * Sets the default line appearance for TH1F histograms. The specified style will be used for all 
    * produced histogram until this function is called again. 
    * The user may specify the line color, style and width by an integer value
    * that corresponds to a given setting. These are the same as those in 
    * ROOT's TAttLine class and the integer corresponding to a given color/style/width can be found at
    * https://root.cern.ch/doc/master/classTAttLine.html    
    * 
    * @param color specifies the color of the TH1F line. By default set to 1 = black. 
    * @param style specifies the style of the TH1F line. By default set to 1 = solid line
    * @param width specifies the width or thickness of the TH1F line. By default set to 3. 
    */
    void setLineStyle(int color=1, int style=1, int width=3){line_color=color; line_style=style; line_width=width;}


    /**
    * Sets the default marker appearance for histograms. The specified style will be used for all 
    * produced histogram until this function is called again. 
    * The user may specify the marker color, style and size by an integer value
    * that corresponds to a given setting. These are the same as those in 
    * ROOT's TAttMarker class and the integer corresponding to a given color/style/size can be found at
    * https://root.cern.ch/doc/master/classTAttMarker.html    
    * 
    * @param color specifies the color of the marker. 
    * @param style specifies the style of the marker.
    * @param size specifies the size of the marker.
    */
    void setMarkerStyle(int color, int style, int size){marker_color=color; marker_style=style; marker_size=size;}

    /**
    * Sets plot text to be displayed on the given TH1F histogram when drawn. 
    * The position of the text is automatically set depending on the contents of the histogram. 
    * 
    * @param h1 TH1F histogram pointer for which a plot text should be added. 
    * @param text text to be displayed on the plot. 
    */
    void setPlotText(TH1F* h1, TString text);

    /**
    * Sets the style of a given TH1F histogram to the default. Can be used to update 
    * histogram linestyle if a new one has been set by a call to setLineStyle(). 
    *
    * @param h TH1F histogram pointer for which the default style is to be set. 
    */
    void setStyle(TH1F* h);

    /**
    * Saves the given TH1F histogram as a .png-file with the same name as the histogram 
    * and saves it to a .root-file, provided that one is opened. 
    * 
    * @param h pointer to the TH1F histogram that is to be saved. 
    * @param plot_text specifies a plot text to be displayed on the drawn histogram. 
    */
    void writeHist(TString plot_text = "");

    void writeCanvas(TString name);

    void setTCanvas(TCanvas* fcanvas);
    void setTCanvas(TCanvas* fcanvas, int nhist);

    int getSize(){return fvec.size();}

    std::list<TH1F>::iterator begin() {return fvec.begin();}
    std::list<TH1F>::iterator end() {return fvec.end();}

    private:
    TH1F fhist1;
    std::list<TH1F> fvec;
    int line_color; 
    int line_style; 
    int line_width;
    int marker_color; 
    int marker_style; 
    int marker_size;

    TString int2str(int i){TString str; str.Form("%d", i); return str;}

    void makeTH1F(TString name, int bins ,double xmin, double xmax,TString xlabel="x", TString ylabel="Counts");
    void makeTH1F(std::vector<float> data, TString name, TString xlabel="x", TString ylabel="Counts");



    
    
    ClassDef(SproutPlot, 1) // Needed for compatability with ROOT's Cling interpreter 

};
#endif // RHELPER_H
