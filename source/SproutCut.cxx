#include "SproutCut.h"

SproutCut::SproutCut() :
bcuts(),
sp_qa()
{}


void SproutCut::addCutWindow(std::string name, std::string descr, float min, float max){
    SproutCutValue cutVal;
    
    cutVal.min_value = min; cutVal.min_value_set=true;
    cutVal.max_value = max; cutVal.max_value_set=true;

    int range = max-min;
    int min_range = min-0.4*range;
    int max_range = max+0.4*range;

    addcut(cutVal, name, descr, min_range, max_range);
}

void SproutCut::addCutMinimum(std::string name, std::string descr, float value, float QAplotRange){
    SproutCutValue cutVal;

    cutVal.min_value = value; cutVal.min_value_set=true;

    float min_range = value - 0.3*QAplotRange;
    float max_range = value + 0.7*QAplotRange;

    addcut(cutVal, name, descr, min_range, max_range);
}

void SproutCut::addCutMaximum(std::string name, std::string descr, float value, float QAplotRange){
    SproutCutValue cutVal;
    
    cutVal.max_value = value; cutVal.max_value_set=true;

    int min_range = value-0.7*QAplotRange;
    int max_range = value+0.3*QAplotRange;

    addcut(cutVal, name, descr, min_range, max_range);
}

void SproutCut::addcut(SproutCutValue& cut, std::string name, std::string descr, float min_range, float max_range){
    int bins = 100;
    
    cut.signal_th1f = sp_qa.getTH1F(name, bins, min_range, max_range, descr, "counts");
    cut.bg_th1f = sp_qa.getTH1F(name, bins, min_range, max_range, descr, "counts");
    cut.signal_th1f.SetMinimum(0);
    cut.bg_th1f.SetMinimum(0);
    //sp_qa.clear(); NOT YET IMPLEMENTED IN SproutPlot

    bcuts.insert({name,cut});
}


void SproutCut::setYieldQAPlot(std::string xTitle, int bins, float xMin, float xMax){
    sp_yield_control.getTH1F("no_cuts", bins, xMin, xMax, xTitle, "counts");
    for(auto& p : bcuts){
        sp_yield_control.getTH1F(p.first, bins, xMin, xMax, xTitle, "counts");
    }
}

int SproutCut::cut(std::string name, float val){
    SproutCutValue& cutVal = bcuts[name];

    cutVal.signal_th1f.Fill(val); cutVal.signal_th1f_filled=true; //signal hist is filled by default

    if(cutVal.min_value_set && cutVal.max_value_set){
        if(val <= cutVal.max_value && val>=cutVal.min_value){return 1;}
        else{return 0;}
    }
    else if(cutVal.max_value_set && val <= cutVal.max_value){return 1;}
    else if(cutVal.min_value_set && val>=cutVal.min_value){return 1;}
    else{return 0;}
}

int SproutCut::cut(std::string name, float val, bool isSignal){
    SproutCutValue& cutVal = bcuts[name];

    if(isSignal){cutVal.signal_th1f.Fill(val); cutVal.signal_th1f_filled=true;}
    else{cutVal.bg_th1f.Fill(val); cutVal.bg_th1f_filled=true;}

    if(cutVal.min_value_set && cutVal.max_value_set){
        if(val <= cutVal.max_value && val>=cutVal.min_value){return 1;}
        else{return 0;}
    }
    else if(cutVal.max_value_set && val <= cutVal.max_value){return 1;}
    else if(cutVal.min_value_set && val>=cutVal.min_value){return 1;}
    else{return 0;}
}

int SproutCut::cut(std::string name, float val, float controlVal){
    int result = cut(name, val);
    if(result){sp_yield_control.getTH1F(name).Fill(controlVal);}
    return result;
}

int SproutCut::cut(std::string name, float val, bool isSignal, float controlVal){
    int result = cut(name, val, isSignal);
    if(result){sp_yield_control.getTH1F(name).Fill(controlVal);}
    return result;
}

void SproutCut::write(TFile* file, TString name){
	file->WriteObject(&(*this), name); 
}

void SproutCut::writeQAplot(std::string title){
    gROOT->SetBatch(kTRUE); // Needed for the Draw() to work properly for some reason...

    TString can_name(title);
    TCanvas can(can_name);
    
    if(sp_yield_control.sizeTH1F()!=0){sp_qa.setTCanvas(&can, bcuts.size()+1);}
    else{sp_qa.setTCanvas(&can, bcuts.size());}
     
    int i=0;
    gStyle->SetOptStat(0);

    for(auto& p : bcuts){
        
        SproutCutValue& cutVal = p.second;

        can.cd(i+1);

        int height;

        cutVal.signal_th1f.SetStats(0);
        //if(!cutVal.signal_th1f_filled){continue;}
        if(cutVal.bg_th1f_filled){
            cutVal.signal_th1f.SetLineColor(8); 
            cutVal.bg_th1f.SetLineColor(46); cutVal.bg_th1f.SetStats(0);
        }

        if(cutVal.signal_th1f.GetMaximum() > cutVal.bg_th1f.GetMaximum()){
            cutVal.signal_th1f.Draw();
            cutVal.bg_th1f.Draw("same");
            height = cutVal.signal_th1f.GetMaximum() + 0.1 * cutVal.signal_th1f.GetMaximum();
        }
        else{
            cutVal.bg_th1f.Draw();
            cutVal.signal_th1f.Draw("same");
            height = cutVal.bg_th1f.GetMaximum() + 0.1 * cutVal.bg_th1f.GetMaximum();
        }

        if(cutVal.bg_th1f_filled){
            
            TLegend legend = TLegend(0.7391304347826086,0.8163992869875223,1,0.9358288770053476);
            legend.AddEntry(&cutVal.signal_th1f,"Signal");
            legend.AddEntry(&cutVal.bg_th1f,"Background");
            legend.DrawClone("same");

        }


        if(cutVal.min_value_set){
            TGraph xmin = TGraph(2);
            xmin.SetPoint(0,cutVal.min_value, 0);     xmin.SetPoint(1, cutVal.min_value, height);
            xmin.SetLineColor(15); xmin.SetLineWidth(3); xmin.SetLineStyle(2);
            xmin.DrawClone("same");
        }
        if(cutVal.max_value_set){
            TGraph xmax = TGraph(2);
            xmax.SetPoint(0, cutVal.max_value, 0);  xmax.SetPoint(1, cutVal.max_value, height);
            xmax.SetLineColor(15); xmax.SetLineWidth(3); xmax.SetLineStyle(2);
            xmax.DrawClone("same");
        }
        i++;
    }

    // IMPLEMENT DRAW OF YIELD PLOT
    if(sp_yield_control.sizeTH1F() != 0){
        TLegend yield_legend = TLegend(0.640373197625106,0.7232761518972219,0.9677692960135709,0.9513046554601674);


        can.cd(i+1);
        sp_yield_control.getTH1F("no_cuts").SetStats(0);
        sp_yield_control.getTH1F("no_cuts").DrawClone();
        yield_legend.AddEntry(&sp_yield_control.getTH1F("no_cuts"),"no_cuts");

        int i=0;
        for(auto& p : bcuts){
            sp_yield_control.getTH1F(p.first).SetLineColor(40+i%10);
            sp_yield_control.getTH1F(p.first).SetStats(0);
            sp_yield_control.getTH1F(p.first).Draw("same");
            yield_legend.AddEntry(&sp_yield_control.getTH1F(p.first),TString (p.first));
            i++;
        }

        yield_legend.DrawClone("same");
    }
    

    can.Write();

    gROOT->SetBatch(kFALSE); // Turn on Batch-mode again. 
}

int SproutCut::tuneCuts(std::string title){
    try{
        if(bcuts.size()==0){throw 1;}

        gROOT->SetBatch(kTRUE); // Needed for the Draw() to work properly for some reason...

        TString can_name(title);
        TCanvas can(can_name); sp_qa.setTCanvas(&can, bcuts.size());

        //make canvas
        int cut_index=0;
        for(auto& p : bcuts){
            SproutCutValue& cutVal = p.second;
            int nbins = cutVal.signal_th1f.GetXaxis()->GetNbins();
        
            //Double_t xarr[nbins], yarr[nbins], yerr[nbins];
            TGraphErrors plot_min(nbins);
            TGraphErrors plot_max(nbins);

            double high_plot_min_y=0; double high_plot_min_x=0;
            double high_plot_max_y=0; double high_plot_max_x=0;
    

            if(!cutVal.signal_th1f_filled || !cutVal.bg_th1f_filled){throw 2;}

            if(cutVal.min_value_set){
                
                for(int i=1; i<=nbins; i++){
                    double sig_error, bg_error;

                    double sig_integral = cutVal.signal_th1f.IntegralAndError(i,nbins, sig_error);
                    double bg_integral = cutVal.bg_th1f.IntegralAndError(i,nbins, bg_error);

                    double x = cutVal.signal_th1f.GetXaxis()->GetBinLowEdge(i);      
                    double y = sig_integral/(sqrt(sig_integral+bg_integral));

                    double dydS = (bg_integral+sig_integral/2)/(pow(bg_integral+sig_integral,3/2)); 
                    double dydB = sig_integral/(2*pow(bg_integral+sig_integral,3/2));

                    double yerr = sqrt(pow(sig_error,2)*pow(dydS,2) + pow(bg_error,2)*pow(dydB,2)); 
                    
                    //if(!isnan(y)){
                    plot_min.SetPoint(i-1,x,y);
                    if(y>high_plot_min_y){high_plot_min_y=y; high_plot_min_x=x;}
                    //plot.SetPointError(i-1,0,yerr);
                    //}
                }

            }
            if(cutVal.max_value_set){
                for(int i=1; i<=nbins; i++){
                    double sig_error, bg_error;

                    double sig_integral = cutVal.signal_th1f.IntegralAndError(1,i,sig_error);
                    double bg_integral = cutVal.bg_th1f.IntegralAndError(1,i,bg_error);

                    

                    double x = cutVal.signal_th1f.GetXaxis()->GetBinLowEdge(i);                    
                    double y = sig_integral/(sqrt(sig_integral+bg_integral));

                    //std::cout << "y " << y <<std::endl;

                    double dydS = (bg_integral+sig_integral/2)/(pow(bg_integral+sig_integral,3/2)); 
                    double dydB = sig_integral/(2*pow(bg_integral+sig_integral,3/2));

                    double yerr = sqrt(pow(sig_error,2)*pow(dydS,2) + pow(bg_error,2)*pow(dydB,2)); 
                    
                    if(!isnan(y)){
                    plot_max.SetPoint(i-1,x,y);
                    if(y>high_plot_max_y){high_plot_max_y=y; high_plot_max_x=x;}
                    //plot.SetPointError(i-1,0,yerr);
                    }
                }
            }

            can.cd(cut_index+1);
            plot_min.SetTitle("");
            plot_min.SetMarkerStyle(8);
	        plot_min.SetMarkerSize(1);
            plot_min.SetMarkerColor(1);
            plot_min.GetXaxis()->SetTitle(cutVal.signal_th1f.GetXaxis()->GetTitle());
            plot_min.GetYaxis()->SetTitle("S/#sqrt{S+B}");

            plot_max.SetTitle("");
            plot_max.SetMarkerStyle(8);
	        plot_max.SetMarkerSize(1);
            plot_max.SetMarkerColor(1);
            plot_max.GetXaxis()->SetTitle(cutVal.signal_th1f.GetXaxis()->GetTitle());
            plot_max.GetYaxis()->SetTitle("S/#sqrt{S+B}");
            
            if(cutVal.min_value_set && cutVal.max_value_set){
                plot_min.DrawClone();
                plot_max.DrawClone("same");
                
                TGraph xmin = TGraph(2);
                xmin.SetPoint(0,high_plot_min_x, 0);  xmin.SetPoint(1, high_plot_min_x, high_plot_min_y*1.1);
                xmin.SetLineColor(15); xmin.SetLineWidth(3); xmin.SetLineStyle(2);
                xmin.DrawClone("same");

                TGraph xmax = TGraph(2);
                xmax.SetPoint(0, high_plot_max_x, 0);  xmax.SetPoint(1, high_plot_max_x, high_plot_max_y*1.1);
                xmax.SetLineColor(15); xmax.SetLineWidth(3); xmax.SetLineStyle(2);
                xmax.DrawClone("same");

                float height;
                if(cutVal.signal_th1f.GetMaximum() > cutVal.bg_th1f.GetMaximum()){height = 1.1*cutVal.signal_th1f.GetMaximum();}
                else{height = 1.1*cutVal.bg_th1f.GetMaximum();}

                float plot_height;
                if(high_plot_max_y>high_plot_min_y){plot_height=high_plot_max_y;}
                else{plot_height = high_plot_min_y;}
                
                float scale = plot_height/height;

                cutVal.signal_th1f.Scale(scale);
                cutVal.bg_th1f.Scale(scale);

                cutVal.signal_th1f.Draw("HIST same");
                cutVal.bg_th1f.Draw("HIST same");
            }
            else if(cutVal.min_value_set){
                plot_min.DrawClone();

                TGraph xmin = TGraph(2);
                xmin.SetPoint(0,high_plot_min_x, 0);  xmin.SetPoint(1, high_plot_min_x, high_plot_min_y*1.1);
                xmin.SetLineColor(15); xmin.SetLineWidth(3); xmin.SetLineStyle(2);
                xmin.DrawClone("same");

                float height;
                if(cutVal.signal_th1f.GetMaximum() > cutVal.bg_th1f.GetMaximum()){height = 1.1*cutVal.signal_th1f.GetMaximum();}
                else{height = 1.1*cutVal.bg_th1f.GetMaximum();}

                float plot_height;
                if(high_plot_max_y>high_plot_min_y){plot_height=high_plot_max_y;}
                else{plot_height = high_plot_min_y;}
                
                float scale = plot_height/height;

                cutVal.signal_th1f.Scale(scale);
                cutVal.bg_th1f.Scale(scale);

                cutVal.signal_th1f.Draw("HIST same");
                cutVal.bg_th1f.Draw("HIST same");
            }
            else if(cutVal.max_value_set){
                plot_max.DrawClone();
                TGraph xmax = TGraph(2);
                xmax.SetPoint(0, high_plot_max_x, 0);  xmax.SetPoint(1, high_plot_max_x, high_plot_max_y*1.1);
                xmax.SetLineColor(15); xmax.SetLineWidth(3); xmax.SetLineStyle(2);
                xmax.DrawClone("same");


                float height;
                if(cutVal.signal_th1f.GetMaximum() > cutVal.bg_th1f.GetMaximum()){height = 1.1*cutVal.signal_th1f.GetMaximum();}
                else{height = 1.1*cutVal.bg_th1f.GetMaximum();}

                float plot_height;
                if(high_plot_max_y>high_plot_min_y){plot_height=high_plot_max_y;}
                else{plot_height = high_plot_min_y;}
                
                float scale = plot_height/height;

                cutVal.signal_th1f.Scale(scale);
                cutVal.bg_th1f.Scale(scale);

                cutVal.signal_th1f.Draw("HIST same");
                cutVal.bg_th1f.Draw("HIST same");
            }

            if(cutVal.min_value_set) std::cout<<title +"_" << p.first << "_min   "<<high_plot_min_x<<std::endl;
            if(cutVal.max_value_set) std::cout<<title +"_" <<p.first << "_max   "<<high_plot_max_x<<std::endl;
            

            cut_index++;
        }
        can.Write();
        gROOT->SetBatch(kFALSE); // Needed for the Draw() to work properly for some reason...
        return 1;

    }
    catch(int n){
        if(n==1){std::cout<<"\n ERROR in SproutCut::tuneCuts: no cuts added! \n"; exit(0);}
        if(n==2){std::cout<<"\n WARNING in SproutCut::tuneCuts: Cut Tuning not possible! \n No signal/background distributions found."<< 
                            "\n Call SproutCut::cut(std::string name, float val, bool isSignal) before tuning \n";}
        
    }
    return 0;
}


SproutCut SproutCut::operator+(const SproutCut obj){
    try{

        SproutCut scut;

        if(this->bcuts.size()==0){
            return obj;
        }

        for(auto& p : this->bcuts){

            std::string name = p.first;
            std::unordered_map<std::string, SproutCutValue> objbcuts = obj.bcuts;

            if(objbcuts != this->bcuts){throw 2;}  

            SproutCutValue scutVal;

            TH1F hsig = p.second.signal_th1f;
            hsig.Add(&((objbcuts[name]).signal_th1f));
            scutVal.signal_th1f = hsig;
            if(p.second.signal_th1f_filled || (objbcuts[name]).signal_th1f_filled){scutVal.signal_th1f_filled = true;}
            else{scutVal.signal_th1f_filled = false;}

            TH1F hbg = p.second.bg_th1f;
            hbg.Add(&((objbcuts[name]).bg_th1f));
            scutVal.bg_th1f=hbg;
            if(p.second.bg_th1f_filled || (objbcuts[name]).bg_th1f_filled){scutVal.bg_th1f_filled = true;}
            else{scutVal.bg_th1f_filled = false;}

            scutVal.min_value_set = p.second.min_value_set;
            scutVal.max_value_set = p.second.max_value_set;

            if(scutVal.min_value_set){scutVal.min_value = p.second.min_value;}
            if(scutVal.max_value_set){scutVal.max_value = p.second.max_value;}

            scut.bcuts.insert({name,scutVal});
        }

        scut.sp_yield_control = this->sp_yield_control+obj.sp_yield_control;

        return scut;

    }
    catch(int n){
        if(n==2){std::cout<<"\n SproutCut::operator+ ERROR: SproutCut content mismatch! Must contain identical cuts. \n";}
        exit(0);

    }  
}