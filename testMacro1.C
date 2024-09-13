
void testMacro1(){
    SproutPlot splot;

    TH1F& h1 = splot.getTH1F("h1", 100, -5, 5, "x", "y");
    TH1F& h2 = splot.getTH1F("h2", 100, -5, 5, "x", "y");
    TH1F& h3 = splot.getTH1F("h3", 100, -5, 5, "x", "y");
    

    TH2F& g1 = splot.getTH2F("g1", 100, -5, 5, 100, -5, 5, "x", "y");
    TH2F& g2 = splot.getTH2F("g2", 100, -5, 5, 100, -5, 5, "x", "y");
    TH2F& g3 = splot.getTH2F("g3", 100, -5, 5, 100, -5, 5, "x", "y");
    TH2F& g4 = splot.getTH2F("g4", 100, -5, 5, 100, -5, 5, "x", "y");

    
    TF2 *xyg = new TF2("xyg","xygaus",-5,5,-5,5);  xyg->SetParameters(1,0,2,0,2);

    h1.FillRandom("gaus",10000);
    h2.FillRandom("gaus",10000);
    h3.FillRandom("gaus",10000);

    g1.FillRandom("xyg",100000);
    g2.FillRandom("xyg",100000);
    g3.FillRandom("xyg",100000);
    g4.FillRandom("xyg",100000);

    TFile outFile("test.root", "recreate");
    outFile.cd();
    splot.setTH1FDrawOpt("E");
    splot.setTH2FDrawOpt("LEGO");
    splot.setTitle("Title").writeHist("sim");
    outFile.Close();

    delete xyg;
}