//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jan 21 15:55:42 2018 by ROOT version 6.02/05
// from TTree tree/tree
// found on file: event.root
//////////////////////////////////////////////////////////

#ifndef fitter_h
#define fitter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class fitter {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           rawid;
   Int_t           delay;
   Int_t           nevents;
   Double_t        pulse[10];
   Double_t        square[10];

   TString 	   filepath;

   // List of branches
   TBranch        *b_t_rawid;   //!
   TBranch        *b_t_delay;   //!
   TBranch        *b_t_nevents;   //!
   TBranch        *b_t_pulse;   //!
   TBranch        *b_t_square;   //!

   fitter(string path="");
   virtual ~fitter();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef fitter_cxx
fitter::fitter(string path) : fChain(0), filepath(path) 
{
   TTree *tree=0;
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

//##########################################################################################//
//                    PUT THE PATH TO YOUR event.root FILE BELOW
//#########################################################################################//                    
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject((path+"/event.root").c_str());
      if (!f || !f->IsOpen()) {
         f = new TFile((path+"/event.root").c_str());
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

fitter::~fitter()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t fitter::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t fitter::LoadTree(Long64_t entry)
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

void fitter::Init(TTree *tree)
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

   fChain->SetBranchAddress("rawid", &rawid, &b_t_rawid);
   fChain->SetBranchAddress("delay", &delay, &b_t_delay);
   fChain->SetBranchAddress("nevents", &nevents, &b_t_nevents);
   fChain->SetBranchAddress("pulse", pulse, &b_t_pulse);
   fChain->SetBranchAddress("square", square, &b_t_square);
   Notify();
}

Bool_t fitter::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void fitter::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t fitter::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef fitter_cxx
