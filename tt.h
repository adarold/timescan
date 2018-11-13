//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jul  5 15:34:44 2018 by ROOT version 6.02/05
// from TChain t/
//////////////////////////////////////////////////////////

#ifndef tt_h
#define tt_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class tt {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           fed;
   Int_t           tcc;
   Int_t           tower;
   Int_t           stripInTower;
   Int_t           xtalInStrip;
   Int_t           CCU;
   Int_t           VFE;
   Int_t           xtalInVFE;
   Int_t           xtalInCCU;
   Int_t           ieta;
   Int_t           iphi;
   Int_t           ix;
   Int_t           iy;
   Int_t           iz;
   Int_t           hashedId;
   Int_t           ic;
   Int_t           cmsswId;
   Int_t           dbId;
   Int_t           ietaTT;
   Int_t           iphiTT;
   Int_t           TCCch;
   Int_t           TCCslot;
   Int_t           SLBch;
   Int_t           SLBslot;
   Int_t           ietaGCT;
   Int_t           iphiGCT;
   Char_t          det[6];
   Char_t          crate[7];

   TString         filepath;
   Int_t           year;

   // List of branches
   TBranch        *b_fed;   //!
   TBranch        *b_tcc;   //!
   TBranch        *b_tower;   //!
   TBranch        *b_stripInTower;   //!
   TBranch        *b_xtalInStrip;   //!
   TBranch        *b_CCU;   //!
   TBranch        *b_VFE;   //!
   TBranch        *b_xtalInVFE;   //!
   TBranch        *b_xtalInCCU;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_ix;   //!
   TBranch        *b_iy;   //!
   TBranch        *b_iz;   //!
   TBranch        *b_hashedId;   //!
   TBranch        *b_ic;   //!
   TBranch        *b_cmsswId;   //!
   TBranch        *b_dbId;   //!
   TBranch        *b_ietaTT;   //!
   TBranch        *b_iphiTT;   //!
   TBranch        *b_TCCch;   //!
   TBranch        *b_TCCslot;   //!
   TBranch        *b_SLBch;   //!
   TBranch        *b_SLBslot;   //!
   TBranch        *b_ietaGCT;   //!
   TBranch        *b_iphiGCT;   //!
   TBranch        *b_det;   //!
   TBranch        *b_crate;   //!

   tt(string path="");
   virtual ~tt();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(string path, Int_t year);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tt_cxx
tt::tt(string path) : fChain(0) 
{
   TTree *tree=0;

   char file_ecal[100];
   sprintf (file_ecal, "%s/EcalTPGParam.root/tpgmap", (path).c_str());

// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("t",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("t","");
      chain->Add(file_ecal);
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

tt::~tt()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tt::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tt::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void tt::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fed", &fed, &b_fed);
   fChain->SetBranchAddress("tcc", &tcc, &b_tcc);
   fChain->SetBranchAddress("tower", &tower, &b_tower);
   fChain->SetBranchAddress("stripInTower", &stripInTower, &b_stripInTower);
   fChain->SetBranchAddress("xtalInStrip", &xtalInStrip, &b_xtalInStrip);
   fChain->SetBranchAddress("CCU", &CCU, &b_CCU);
   fChain->SetBranchAddress("VFE", &VFE, &b_VFE);
   fChain->SetBranchAddress("xtalInVFE", &xtalInVFE, &b_xtalInVFE);
   fChain->SetBranchAddress("xtalInCCU", &xtalInCCU, &b_xtalInCCU);
   fChain->SetBranchAddress("ieta", &ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", &iphi, &b_iphi);
   fChain->SetBranchAddress("ix", &ix, &b_ix);
   fChain->SetBranchAddress("iy", &iy, &b_iy);
   fChain->SetBranchAddress("iz", &iz, &b_iz);
   fChain->SetBranchAddress("hashedId", &hashedId, &b_hashedId);
   fChain->SetBranchAddress("ic", &ic, &b_ic);
   fChain->SetBranchAddress("cmsswId", &cmsswId, &b_cmsswId);
   fChain->SetBranchAddress("dbId", &dbId, &b_dbId);
   fChain->SetBranchAddress("ietaTT", &ietaTT, &b_ietaTT);
   fChain->SetBranchAddress("iphiTT", &iphiTT, &b_iphiTT);
   fChain->SetBranchAddress("TCCch", &TCCch, &b_TCCch);
   fChain->SetBranchAddress("TCCslot", &TCCslot, &b_TCCslot);
   fChain->SetBranchAddress("SLBch", &SLBch, &b_SLBch);
   fChain->SetBranchAddress("SLBslot", &SLBslot, &b_SLBslot);
   fChain->SetBranchAddress("ietaGCT", &ietaGCT, &b_ietaGCT);
   fChain->SetBranchAddress("iphiGCT", &iphiGCT, &b_iphiGCT);
   fChain->SetBranchAddress("det", det, &b_det);
   fChain->SetBranchAddress("crate", crate, &b_crate);
   Notify();
}

Bool_t tt::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tt::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tt::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tt_cxx
