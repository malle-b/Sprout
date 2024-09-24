
void fitDemoMacro(){
    
    /* 
    Defines a gauss + pol2 distribution used as pdf
    to fill some histograms     
    */
    TFormula* form1 = new TFormula("form1","x*x");
    TF1 rndFunc("rndFunc","x*gaus(0) + [3]*form1*0.03",0,10);
    rndFunc.SetParameters(10,4,1,20);
    
    SproutPlot plots;

    /* 
    Add 9 histograms to plots and fill randomly from
    the previously created distribution 
    */
    for(int i=0; i<9; i++){
        TH1F& h = plots.getTH1F("name", 70, 0, 10);
        h.FillRandom("rndFunc",10000);
    }

    /*
    Initialize SproutFit and specify your param file.
    Try experimenting by running with the other param files
    provided in this folder. 
    */
    SproutFit fitter("myParamFile.txt");
    fitter.fit(plots, "myFits");

/*
    * Required format of the input .txt file where row i corresponds to histogram i in SproutPlot
    *   - Parameter 1: Name of the background function. This class uses the same name convention as ROOT::TFormula.
    *     See documentation (https://root.cern.ch/doc/master/classTFormula.html) for valid expressions. 
    *   - Parameter 2: Name of the signal function (Any valid expression of ROOT::TFormula)
    *   - Parameters 3 and 4: lower and upper range of the fit. If both are set to the same value, the fit is performed
    *     over the entire histogram range. 
    *   - Parameters 5 and 6: lower and upper range of the signal region. 
    *   - Additional parameters (optional): Starting values of the background function followed by starting values of the 
    *     signal function. 
    * Note that the input file doesn't need to contain the same number of lines as the number of histograms held in (SproutPlot).
    * If there are fewer of the former, the last read line will be used to set the fit properties of all subsequent histograms. 
    *
    * After the fit, the input file is overwritten to contain the fitted parameters of the signal and background functions.
    * If a fit has failed, simply alter the parameters of the corresponding line in your input param file and run again. 
*/
}