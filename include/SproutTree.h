#include "TTree.h"
#include "TString.h"
#include "TFile.h"
#include "TVectorD.h"

#include <vector>
#include <iostream>
#include <iomanip>

#pragma once

class SproutTree{
public:
    // Default constructor 
    SproutTree(): fvec(), fsize(), nmax(){};

    /**
    * Constructor of an empty SproutTree with the specified number of branches. 
    * The optional argument 'entries' is used to reserve memory to store the 
    * specified number of entries. This is not nessecary but may increase performance. 
    * 
    * @param num_branches number of branches the SproutTree should contian 
    * @param entries parameter used to reserve memory for entries to be stored in the branches
    */
    SproutTree(int num_branches, int entries=0);

    /**
    * Constructor for creating a SproutTree out of a TTree already filled with floats. 
    * The entries of the specified TTree's branches are copied into the created SproutTree. 
    * 
    * @param t pointer to a TTree filled with floats 
    * @param bstr vector of TStrings specifying the names of the TTree's branches for which the entries 
    * will be copied to SproutTree. By default, if no branch-names are specified, it is assumed that the 
    * TTree's branches are named "b0", "b1", "b2"... and all of the TTree's contents are copied
    * to the new SproutTree. 
    */
    SproutTree(TTree* t, std::vector<TString> bstr={});
    
    /**
    * Adds a new, filled branch, or column, to the SproutTree
    */
    void addBranch();

    /**
    * Adds a new entry to the end of the specified branch
    * 
    * @param branch_num integer specifying the branch 
    * @param a float to be added at the end of the branch 
    */
    void addToBranch(int branch_num, float a);
    
    /* Clears the tree */
    void clear();

    /**
    * Get the value stored at the specified position
    *
    * @param branch_num integer specifying the branch 
    * @param i integer specifying the position of the retrieved entry
    *
    * @return value stored in branch branch_num at posiiton i
    */
    float get(int branch_num, int i){return fvec[branch_num][i];}

    /**
    * Returns the specified branch as an std::vector of floats. 
    * 
    * @param branch_num integer specifying the branch 
    * @return std::vector of floats containing all entries in the branch. 
    */
    std::vector<float> getBranch(int branch_num){return fvec[branch_num];}

    /**
    * Get the number of entries in the specified branch
    * 
    * @param branch_num integer specifying the branch 
    * @return number of entries in the specified branch
    */
    int getNumEntries(int branch_num){return fvec[branch_num].size();}

    /**
    * Get the number of branches in the SproutTree
    * 
    * @return the number of branches in the tree. 
    */
    int getNumBranches(){return fvec.size();}

    /**
    * Get the largest numer of entries stored in the tree's branches
    * 
    * @return maximum number of entries stored in the branches. 
    */
    int getMaxEntries(){return nmax;}

    /**
    * Overwrites the entry stored at the specified branch and position with a new value. 
    *
    * @param branch_num integer specifying the branch 
    * @param i integer specifying the position on the branch 
    * @param val new value to be set at the specified position
    */
    void set(int branch_num, int i, float val){fvec[branch_num][i]=val;} //Add error handling if i is outside the scope

    /**
    * Requires two vectors of equal size where the respective entry of each is assumed to 
    * to originate from the same event. 'adata' is then divided into 'n' bins for which 
    * the corresponding entries of bdata are copied to a new branch of the output SproutTree. 
    * The entries of branch i in the returned SproutTree thus contain the correponding bdata entires
    * for bin i of adata.
    *
    * @param adata vector for which the binning is performed 
    * @param bdata vector with different data points
    * @param n number of bins 
    * 
    * @return SproutTree where branch i contains the entries of bdata corresponding
    * to bin i of adata  
    */
    SproutTree binData(std::vector<float> adata, std::vector<float> bdata, int n);

    /**
    * Reads all SproutTrees saved to the specified .root-file. 
    *
    * If the SproutTree to be saved is larger than 1 GB it is split up into multiple objects 
    * and saved to the file as SproutTree1, SproutTree 2 ... . This read function will iterate
    * over these and append their contents to the current object. 
    *
    * Note that any previous data stored in this SproutTree is cleared upon calling this funciton.
    *
    * @param file .root-file from which one or several SproutTrees are to be read
    */
    void read(TFile* file);

    /**
    * Writes the SproutTree to the specified .root-file. 
    *
    * If the SproutTree to be saved is larger than 1 GB it is split up into multiple objects 
    * and saved to the file as SproutTree1, SproutTree 2 ... 
    * These can be read from file and reassembeled into a single object again using SproutTree::read() 
    *
    * @param file pointer to the TFile object for which the SproutTree is to be written 
    */
    void write(TFile* file);

    /**
    * Converts the SproutTree into ROOT's TTree and saves it to an opened 
    * .root-file with the specified name. Each SproutTree branch is saved 
    * as an std::vector in the corresponding TTree branch. 
    * The TTree branches are named "b0", "b1", "b2" ...
    * 
    * @param name name of the TTree that is to be written 
    */
    void writeTTree(TString name = "sproutTree");

    // Prints the contents of the SproutTree. 
    void print();

    /**
    * Prints the contents of the specified branch. 
    * @param branch_num intefer specifying the branch 
    */
    void printBranch(int branch_num);

private:

    std::vector<std::vector<float>> fvec; //vector of a vector to store the data points (floats)
    int fsize; //size of the "outer" vector, i.e. the number of branches. 
    int nmax; // maximum number of entries stored in a branch
    
    // Function to convert integers to TString. Useful for naming. 
    TString int2str(int i){TString str; str.Form("%d", i); return str;}


    ClassDef(SproutTree, 4) // needed for compatability with ROOT's interpreter. 
};