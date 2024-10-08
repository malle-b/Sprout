void HistDemoMacro(){
    
    SproutPlot splot;

    /*
    Add 1d histograms to splot
    */
    TH1F& h1 = splot.getTH1F("h1", 100, -5, 5, "x", "y");
    TH1F& h2 = splot.getTH1F("h2", 100, -5, 5, "x", "y");
    TH1F& h3 = splot.getTH1F("h3", 100, -5, 5, "x", "y");
    
    /*
    Add 2d histograms to splot 
    */
    TH2F& g1 = splot.getTH2F("g1", 50, -5, 5, 50, -5, 5, "x", "y");
    TH2F& g2 = splot.getTH2F("g2", 50, -5, 5, 50, -5, 5, "x", "y");
    TH2F& g3 = splot.getTH2F("g3", 100, -5, 5, 100, -5, 5, "x", "y");
    
    /*
    Fill 1d histograms from a Gaussian distrubution 
    */
    h1.FillRandom("gaus",10000);
    h2.FillRandom("gaus",10000);
    h3.FillRandom("gaus",10000);

    /*
    Define a 2d Gaussian distribution and use it to
    fill the 2d histograms 
    */
    TF2 *xyg = new TF2("xyg","xygaus",-5,5,-5,5);  xyg->SetParameters(1,0,2,0,2);

    g1.FillRandom("xyg",100000);
    g2.FillRandom("xyg",100000);
    g3.FillRandom("xyg",100000);


    /*
    Set title to all histograms 
    */
    splot.setTitle("My Title");

    /*
    Set plot text to all histograms. Latex syntax also works! 
    */
    splot.setPlotText("My Text");

    TFile outFile("myFile.root", "recreate");
    outFile.cd();

    /*
    Write all histograms to an open .root file as a single canvas, named myCanvas
    */
    splot.writeCanvas("myCanvas");

    /*
    Write all histograms to an open .root file separately separately 
    */
    splot.writeHist();

    outFile.Close();

    /*
    Specify a specific draw options for your 1d and 2d histograms. 
    Utilises the same syntax as THistPainter (https://root.cern/doc/master/classTHistPainter.html)
    */
    splot.setTH1FDrawOpt("E");
    splot.setTH2FDrawOpt("LEGO");

    /*
    Save all histograms as a single canvas to a .png file in MyPlots
    */
    splot.saveCanvasAs("MyPlots/myCanvas.png");

    /*
    Save all histograms separately as .png files in MyPlots
    */
    splot.saveHistAs("MyPlots/",".png");

    delete xyg;
}