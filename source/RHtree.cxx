#include "RHtree.h"

ClassImp(RHtree) // Needed for compatability with ROOT's Cling interpreter 


RHtree::RHtree() : t(), fvec(){}

RHtree::RHtree(int num_branches){
    t = new TTree("t","");
    fvec.reserve(num_branches);
    for(int i=0; i<num_branches;i++){std::vector<float> val; fvec.push_back(val);}    
}


void RHtree::fillTTree(){
    for(int i=0; i<fvec.size(); i++){
        t->Branch("b"+int2str(i), &fvec);
    }
    t->Fill(); 
}

void RHtree::printBranch(int branch_num){
    int w = 10;
    int count = 0;
    int number = fvec.size();

    while(number != 0){number = number / 10; count++;}

    std::cout << "|";
    std::cout << std::left << std::setw(w) << "Branch" << std::left << std::setw(count) << branch_num;
    std::cout << "|";

    std::cout << "\n";

    w=w+count;

    std::cout << std::setw(w+1) << std::setfill('-') << "|" << "|";
    std::cout << "\n";

    for(int i=0; i<fvec[branch_num].size(); i++){
        std::cout << "|";
        std::cout << std::setw(w) << std::setfill(' ') << get(branch_num,i) << std::setfill(' ');
        std::cout << "|";
        std::cout << "\n";
    }
}

void RHtree::print(){
    int w = 10;
    int count = 0;
    int number = fvec.size();

    while(number != 0){number = number / 10; count++;}

    int max = 0;
    for(int i=0; i<fvec.size();i++){
        std::cout << "|";
        std::cout << std::left << std::setw(w) << "Branch" << std::left << std::setw(count) << i;
        std::cout << "|";
        for(int j=0; j<fvec.size();j++){
            if(fvec[j].size() >= fvec[i].size() && fvec[j].size() >= max) max = fvec[j].size();
        } 
    }
    std::cout << "\n";

    w=w+count;

    for(int i=0; i<fvec.size(); i++) std::cout << std::setw(w+1) << std::setfill('-') << "|" << "|";
    std::cout << "\n";

    for(int j=0; j<max;j++){
        for(int i=0;i<fvec.size();i++){
            std::cout << "|";
            if(j>=fvec[i].size()) std::cout << std::setw(w) << std::setfill(' ') <<"-";
            else std::cout << std::setw(w) << std::setfill(' ') << get(i,j) << std::setfill(' ');
            std::cout << "|";
        }
        std::cout << "\n";
    }
}

