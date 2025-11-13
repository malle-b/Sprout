#pragma once 

#include "TH1F.h"

class SproutCutValue{
    public:
    SproutCutValue(){min_value_set=false; max_value_set=false; signal_th1f_filled=false; bg_th1f_filled=false;}
    
    bool min_value_set;
    bool max_value_set;
    bool signal_th1f_filled;
    bool bg_th1f_filled;
    
    TH1F signal_th1f;
    TH1F bg_th1f;
    float min_value;
    float max_value;

    ClassDef(SproutCutValue, 1) // Needed for compatability with ROOT's Cling interpreter 
};

inline bool operator==(const SproutCutValue& a, const SproutCutValue& b){
    if(a.min_value_set==b.min_value_set && a.max_value_set==b.max_value_set &&
       a.signal_th1f.GetNbinsX() == b.signal_th1f.GetNbinsX() &&
       a.signal_th1f.GetXaxis()->GetXmin() == b.signal_th1f.GetXaxis()->GetXmin() &&
       a.signal_th1f.GetXaxis()->GetXmax() == b.signal_th1f.GetXaxis()->GetXmax()){

        if(a.min_value_set && a.min_value != b.min_value){return false;}
        if(a.max_value_set && a.max_value != b.max_value){return false;}
        else{return true;}
    }
    else{return false;}
}

inline bool operator!=(const SproutCutValue& a, const SproutCutValue& b){
    if(a.min_value_set==b.min_value_set && a.max_value_set==b.max_value_set &&
       a.signal_th1f.GetNbinsX() == b.signal_th1f.GetNbinsX() &&
       a.signal_th1f.GetXaxis()->GetXmin() == b.signal_th1f.GetXaxis()->GetXmin() &&
       a.signal_th1f.GetXaxis()->GetXmax() == b.signal_th1f.GetXaxis()->GetXmax()){

        if(a.min_value_set && a.min_value != b.min_value){return true;}
        if(a.max_value_set && a.max_value != b.max_value){return true;}
        else{return false;}    
    }
    else{return true;}
}

//// original 
// inline bool operator==(const SproutCutValue& a, const SproutCutValue& b){
//     if(a.min_value_set==b.min_value_set && a.max_value_set==b.max_value_set &&
//        a.signal_th1f_filled==b.signal_th1f_filled && a.bg_th1f_filled == b.bg_th1f_filled &&
//        a.min_value==b.min_value && a.max_value==b.max_value &&
//        a.signal_th1f.GetNbinsX() == b.signal_th1f.GetNbinsX() &&
//        a.signal_th1f.GetXaxis()->GetXmin() == b.signal_th1f.GetXaxis()->GetXmin() &&
//        a.signal_th1f.GetXaxis()->GetXmax() == b.signal_th1f.GetXaxis()->GetXmax()){return true;}
//     else{return false;}
// }

//// original 
// inline bool operator!=(const SproutCutValue& a, const SproutCutValue& b){
//     if(a.min_value_set==b.min_value_set && a.max_value_set==b.max_value_set &&
//        a.signal_th1f_filled==b.signal_th1f_filled && a.bg_th1f_filled == b.bg_th1f_filled &&
//        a.min_value==b.min_value && a.max_value==b.max_value &&
//        a.signal_th1f.GetNbinsX() == b.signal_th1f.GetNbinsX() &&
//        a.signal_th1f.GetXaxis()->GetXmin() == b.signal_th1f.GetXaxis()->GetXmin() &&
//        a.signal_th1f.GetXaxis()->GetXmax() == b.signal_th1f.GetXaxis()->GetXmax()){return false;}
//     else{return true;}
// }