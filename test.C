//#include "TH1F.h"

#include "include/HistogramHelper.h"
#include "include/RHtree.h"
#include "TTree.h"
#include "TFile.h"


#include <iostream>
#include <vector>
#include <chrono>
using namespace std::chrono;

void test(){

    // HistogramHelper rh;

    // TH1F* hist = rh.makeTH1F("test", 0, 11000);
    // TH1F* invhist = rh.makeInvMassTH1F("test_inv_mass", 9900);

    // for(int i=0; i<10000; i++){
    //     hist->Fill(i);
    //     invhist->Fill(i);


    // }

    // rh.writeHist(invhist, "Invariant mass");
    // rh.writeHist(hist, "test boi");

    int test = 15;

    RHtree mytree = RHtree(3);

    float a,b,c;
    float d,e,f;

    TTree tree = TTree("tree", "");
    tree.Branch("rb0", &a);
    tree.Branch("rb1", &b);
    tree.Branch("rb2", &c);

    auto start1 = high_resolution_clock::now();
    for(int i=0; i<test; i++){
        a = 11;
        b = 22;
        c = 33;
        tree.Fill();
    }
    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);

    auto start2 = high_resolution_clock::now();
    for(int i=0; i<test; i++){
        mytree.addToBranch(0,11);
        mytree.addToBranch(1,22);
        mytree.addToBranch(2,33);
    }
    mytree.fillTTree();
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);

    std::cout << "TTree Fill Time: " << duration1.count() << " us" << std::endl;
    std::cout << "RHtree Fill Time " << duration2.count() << " us" << std::endl;

    float ra, rb, rc;
    tree.SetBranchAddress("rb0", &ra);
    tree.SetBranchAddress("rb1", &rb);
    tree.SetBranchAddress("rb2", &rc);

    float rsum = 0;

    auto start3 = high_resolution_clock::now();
    for (int i=0; tree.LoadTree(i) >=0; i++){
        tree.GetEntry(i);
        rsum = rsum + ra + rb + rc;

    }
    auto stop3 = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(stop3 - start3);

    float sum = 0;

    auto start4 = high_resolution_clock::now();
    for(int i=0; i<3; i++){
        for(int j=0; j<test; j++){
            sum = sum + mytree.get(i,j);
        }
    }
    auto stop4 = high_resolution_clock::now();
    auto duration4 = duration_cast<microseconds>(stop4 - start4);
    

    std::cout << "TTree Read Time: " << duration3.count() << " us" << std::endl;
    std::cout << "RHtree Read Time " << duration4.count() << " us" << std::endl;

    TFile* file = new TFile("testFile.root", "recreate");
    //mytree.write(file, "myRHtree");

    TTree* t = mytree.getTTree();

    file->cd(0);
    file->WriteObject(&(*t), "root tree");
    mytree.write(file, "my_tree");
    file->Close();

}