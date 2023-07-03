#include "SproutTree.h"

ClassImp(SproutTree) // Needed for compatability with ROOT's Cling interpreter 


SproutTree::SproutTree(int num_branches){
    fvec.reserve(num_branches);
    for(int i=0; i<num_branches;i++){std::vector<float> val; fvec.push_back(val);}
    fsize = fvec.size(); 
}

SproutTree::SproutTree(TTree* t, std::vector<TString> bstr){ // coult add try and catch for bstr input
    nmax=0;
    fsize=0;

    int nbranch;
    if(bstr.size()==0){nbranch = (int) t->GetNbranches();}
    else{nbranch = bstr.size();}

    std::cout << "nbranch " << nbranch << std::endl;
    
    for(int i=0; i<nbranch; i++){
        std::vector<float> val;
        float bval;

        if(bstr.size()==0){t->SetBranchAddress("b"+int2str(i),&bval);}
        else{t->SetBranchAddress(bstr[i],&bval);}

        for(int j=0; j<t->GetEntries(); j++){ 
            t->GetEntry(j); 
            val.push_back(bval); 
        }

        fvec.push_back(val);
        if(val.size()>nmax){nmax = val.size();}
        fsize++;
    }
}

void SproutTree::writeTTree(TString name){
    TTree t(name,"");
    for(int i=0; i<fvec.size(); i++){
        t.Branch("b"+int2str(i), &fvec[i]);
    }
    t.Fill(); 
    t.Write();
}


void SproutTree::printBranch(int branch_num){
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

void SproutTree::print(){
    int w = 10;
    int count = 0;
    int number = fvec.size();

    while(number != 0){number = number / 10; count++;}

    for(int i=0; i<fvec.size();i++){
        std::cout << "|";
        std::cout << std::left << std::setw(w) << "Branch" << std::left << std::setw(count) << i;
        std::cout << "|";
    }
    std::cout << "\n";

    w=w+count;

    for(int i=0; i<fvec.size(); i++) std::cout << std::setw(w+1) << std::setfill('-') << "|" << "|";
    std::cout << "\n";

    for(int j=0; j<nmax;j++){
        for(int i=0;i<fvec.size();i++){
            std::cout << "|";
            if(j>=fvec[i].size()) std::cout << std::setw(w) << std::setfill(' ') <<"-";
            else std::cout << std::setw(w) << std::setfill(' ') << get(i,j) << std::setfill(' ');
            std::cout << "|";
        }
        std::cout << "\n";
    }
}

//Remove mentries arguments and replace it with mmax
//But first implement a dynamic check for the maximum... 
void SproutTree::write(TFile* file){
    size_t mem = 0;
    fsize = fvec.size();
    for(int i=0; i<fsize; i++){
        mem = mem + sizeof(float)*fvec[i].size();
    }
    std::cout << "SproutTree memory: " << mem << std::endl;
    //mem = mem/1000000000;
    int split = 1;
    int c = 0;
    if(mem<800000000){file->WriteObject(&(*this), "sproutTree1");}
    else{
        while(mem/split>800000000){split++;}
        std::cout << "split " << split << std::endl;
        
        int i=0;
        for(int k=1; k<split+1; k++){
            SproutTree tree = SproutTree(fsize);
            //while(i<nmax/split*k+1){ // this is sus. 
            while(i<(nmax/split+1)*k){
                for(int j=0; j<fsize; j++){ // do this with insert maybe?
                    if(i<fvec[j].size()){tree.addToBranch(j,fvec[j][i]); c++;}
                }
                i++;
            }
            file->WriteObject(&tree, "sproutTree"+int2str(k));
        }
    }
    //Save the number of tree's saved. Perhaps I can do this with a list of keys obtained from file?
    TVectorD v(1); v[0] = (double) split; file->WriteObject(&v, "n_sprouts");
    std::cout << "objects written " << c << std::endl;
}

void SproutTree::read(TFile* file){
    if(nmax != 0){std::cout << "Overwriting SproutTree" << std::endl;}
    for(int i=0; i<fvec.size(); i++){fvec[i].clear();} fvec.clear(); nmax=0; fsize=0; // make into clear() function

    TVectorD *v;
    file->GetObject("n_sprouts",v);

    int n = (int) (*v)[0];
    
    for(int i=1; i<n+1; i++){
        SproutTree* temp = (SproutTree*) file->Get("sproutTree"+int2str(i));
        for(int j=0; j<temp->getNumBranches(); j++){

            if(j+1>fvec.size()){
                std::vector<float> val; val.reserve(n*1000);
                fvec.push_back(val); fsize = fvec.size(); 
            }

            if(fvec[j].size()==0){fvec[j]= temp->getBranch(j); if(nmax < fvec[j].size()){nmax = fvec[j].size();}}
            else{
                // This may seem inefficient but doing it any other way unleashes dark magic. Be warned. 
                std::vector<float> v2 = temp->getBranch(j);
                fvec[j].insert(fvec[j].end(), v2.begin(), v2.end());
                

                if(nmax < fvec[j].size()){nmax = fvec[j].size();}
            }    
        }
    }
}

SproutTree SproutTree::binData(std::vector<float> adata, std::vector<float> bdata, int n){ // write copy constructor so I can call this without returning a pointer. 

	float min = *std::min_element(adata.begin(), adata.end());
	float max = *std::max_element(adata.begin(), adata.end());
	float bin_width = (max-min)/n;

	SproutTree tree(n);

	for(int i=0; i<adata.size(); i++){
		for(int j=1; j<n+1; j++){
			if(adata[i]>=bin_width*(j-1) && adata[i]<bin_width*j){tree.addToBranch(j-1,bdata[i]);} // skips last element
		}
	}
	return tree;
}

void SproutTree::clear(){
    for(int i=0; i<fvec.size(); i++){fvec[i].clear();} fvec.clear(); nmax=0; fsize=0; // make into clear() function

}