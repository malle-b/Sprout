#include "SproutTree.h"

ClassImp(SproutTree) // Needed for compatability with ROOT's Cling interpreter 

SproutTree::SproutTree(int num_branches, int entries){
    //Reserves memory in the outer vector to hold the inner ones, i.e. the "branches"
    fvec.reserve(num_branches); 

    //Loop over the number of branches to fill the outer vector with inner vectors 
    for(int i=0; i<num_branches;i++){
        std::vector<float> val; 
        val.reserve(entries); // reserve memory to hold the data points. 
        fvec.push_back(val);
    }

    // Set the current value of the other member variables. 
    fsize = fvec.size(); nmax = 0;
}

SproutTree::SproutTree(TTree* t, std::vector<TString> bstr){ // coult add try and catch for bstr input
    // Set starting value of the member variables. 
    nmax=0;
    fsize=0;

    //Sets the number of branches from TTree that will be copied. 
    int nbranch;
    if(bstr.size()==0){nbranch = (int) t->GetNbranches();} // No input - all branches are copied 
    else{nbranch = bstr.size();} // Only the specified branches are copied 

    // Loop over the brances    
    for(int i=0; i<nbranch; i++){
        //Initialize the inner vector / SproutTree branch 
        std::vector<float> val; val.reserve(t->GetEntries());
        float bval; // variable to which the TTree's branch value will be passed

        // Sets the bval address to the specified or default/assumed branch name 
        if(bstr.size()==0){t->SetBranchAddress("b"+int2str(i),&bval);}
        else{t->SetBranchAddress(bstr[i],&bval);}

        // Loop over each entry / row in TTree
        for(int j=0; j<t->GetEntries(); j++){ 
            t->GetEntry(j); //Assigns the value of position i in the current TTree branch to bval
            val.push_back(bval); // Append the value to the inner vector
        }

        fvec.push_back(val); // add the filled inner vector to the outer vector 
        
        // Update the other member variables 
        if(val.size()>nmax){nmax = val.size();} // checks if the size of the new branch is the largest one yet
        fsize++;
    }
}

void SproutTree::addBranch(){
    std::vector<float> vec; fvec.push_back(vec); //Adds new inner vector to the outer vector 
}

void SproutTree::addToBranch(int branch_num, float a){
    fvec[branch_num].push_back(a); //Adds a to the specified branch / inner vector 

    //Checks if the current branch has the largest number of entries and adjusts nmax accoardingly
    if((int) fvec[branch_num].size()> nmax){nmax = fvec[branch_num].size();}
}

void SproutTree::clear(){
    for(int i=0; i<fvec.size(); i++){fvec[i].clear();} // Clear inner vectors 
    fvec.clear(); // clear the outer vector

    // reset the other member variables  
    nmax=0; 
    fsize=0;
}

SproutTree SproutTree::binData(std::vector<float> adata, std::vector<float> bdata, int n){ 

    // Finds the minimum and maximum value stored in adata 
	float min = *std::min_element(adata.begin(), adata.end());
	float max = *std::max_element(adata.begin(), adata.end());

	float bin_width = (max-min)/n; //Determine the bin-width

	SproutTree tree(n); //Initialize the returned SproutTree

    //Loop over each entry in adata 
	for(int i=0; i<adata.size(); i++){
        //Loop over the number of desired bins
		for(int j=1; j<n+1; j++){
            //Check if the adata enttry falls within the bin range
			if(adata[i]>=bin_width*(j-1) && adata[i]<bin_width*j){
                tree.addToBranch(j-1,bdata[i]); //if so, add bdata to the corresponding branch.
            } // skips last element
		}
	}
	return tree;
}

void SproutTree::read(TFile* file){

    //Clear the SproutTree
    if(nmax != 0){std::cout << "Overwriting SproutTree" << std::endl;}
    for(int i=0; i<fvec.size(); i++){fvec[i].clear();} fvec.clear(); nmax=0; fsize=0; // make into clear() function

    //Get TVectorD containing the number of SproutTree objects saved
    TVectorD *v;
    file->GetObject("n_sprouts",v);
    int n = (int) (*v)[0];
    
    //Loop over the saved SproutTrees
    for(int i=1; i<n+1; i++){
        //Retrieve from file
        SproutTree* temp = (SproutTree*) file->Get("sproutTree"+int2str(i));

        //loop over the number of branches in temp
        for(int j=0; j<temp->getNumBranches(); j++){
            //if temp has more branches than fvec, add new branch (new inner vector)
            if(j+1>fvec.size()){
                std::vector<float> val; val.reserve(n*1000);
                fvec.push_back(val); fsize = fvec.size(); 
            }

            //If the fvec branch is empty, assign it to the corresponding temp branch
            if(fvec[j].size()==0){fvec[j]= temp->getBranch(j); if(nmax < fvec[j].size()){nmax = fvec[j].size();}}
            else{ //Otherwise, append the temp branch to the end of corresponding fvec branch

                // This may seem inefficient but doing it any other way unleashes dark magic. Be warned. 
                std::vector<float> v2 = temp->getBranch(j);
                fvec[j].insert(fvec[j].end(), v2.begin(), v2.end());
                if(nmax < fvec[j].size()){nmax = fvec[j].size();}
            }    
        }
    }
}

void SproutTree::write(TFile* file){
    size_t mem = 0; //To keep track of memory
    fsize = fvec.size(); //make sure fsize is updated (maybe not nessecary anymore)

    //Estimate the number of bytes fvec occupies
    for(int i=0; i<fsize; i++){
        mem = mem + sizeof(float)*fvec[i].size();
    }
    std::cout << "SproutTree memory: " << mem << std::endl;

    int split = 1; //how many times fvec should be split before writing to file
    int c = 0; //counter for how many objects are written to file. Unessecary?? remove??

    //If the memory is less than 0.8 GB, simply write to file
    if(mem<800000000){file->WriteObject(&(*this), "sproutTree1");}
    else{
        //if not, calculate how many splits are required to make each tree smaller than 0.8 GB
        while(mem/split>800000000){split++;}
        std::cout << "split " << split << std::endl;
        
        int i=0; //too keep track of entry positions 

        //Loop over each split 
        for(int k=1; k<split+1; k++){
            SproutTree tree = SproutTree(fsize); //initialize tree to be written 

            //while i is within the split range
            while(i<(nmax/split+1)*k){
                //Loop over each fvec branch and add the entry at position i to the new tree
                for(int j=0; j<fsize; j++){ // do this with insert maybe?
                    if(i<fvec[j].size()){tree.addToBranch(j,fvec[j][i]); c++;}
                }
                i++;
            }
            file->WriteObject(&tree, "sproutTree"+int2str(k)); //write tree to file
        }
    }
    //Save the number of tree's saved. Perhaps I can do this with a list of keys obtained from file?
    //This is a silly work-around for root not letting you save a primitive to file. 
    TVectorD v(1); v[0] = (double) split; file->WriteObject(&v, "n_sprouts"); //Need number to trees saved to read
    std::cout << "objects written " << c << std::endl;
}

void SproutTree::writeTTree(TString name){
    TTree t(name,""); // Initialize the TTree 

    // Loop over each branch in fvec
    for(int i=0; i<fvec.size(); i++){
        // link the TTree with the inner std::vector 
        t.Branch("b"+int2str(i), &fvec[i]);
    }
    // Fill and write the TTree 
    t.Fill(); 
    t.Write();
}

void SproutTree::print(){
    //Some parameters to format the output
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

void SproutTree::printBranch(int branch_num){
    //Some parameters to format the output
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




