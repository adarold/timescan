//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jan 18 10:08:43 2018 by ROOT version 6.02/05
// from TChain pulse_tree/
//////////////////////////////////////////////////////////

#ifndef make_rawid_h
#define make_rawid_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class make_rawid {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           run;
   Int_t           lumi;
   Bool_t          barrel;
   UInt_t          gain;
   Double_t        pedrms;
   Double_t        pedval;
   Int_t           ietaix;
   Int_t           iphiiy;
   Int_t           iz;
   Double_t        pulse[10];
   UInt_t          rawid;
   Double_t        ene;
   Double_t        time;
   Double_t        chi2;
   Int_t           flag_kweird;
   Int_t           flag_kdiweird;
   Int_t           lumisec;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_barrel;   //!
   TBranch        *b_gain;   //!
   TBranch        *b_pedrms;   //!
   TBranch        *b_pedval;   //!
   TBranch        *b_ietaix;   //!
   TBranch        *b_iphiiy;   //!
   TBranch        *b_iz;   //!
   TBranch        *b_pulse;   //!
   TBranch        *b_rawid;   //!
   TBranch        *b_ene;   //!
   TBranch        *b_time;   //!
   TBranch        *b_chi2;   //!
   TBranch        *b_flag_kweird;   //!
   TBranch        *b_flag_kdiweird;   //!
   TBranch        *b_lumisec;   //!

   make_rawid(TTree *tree=0);
   virtual ~make_rawid();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef make_rawid_cxx
make_rawid::make_rawid(TTree *tree) : fChain(0) 
{
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
      f->GetObject("pulse_tree",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("pulse_tree","");
      chain->Add("root://eosinfnts.ts.infn.it//eos/infnts/cms/store/user/adarold/ECAL/TimeScan/AlCaPhiSym/crab_TimeScan/180116_164610/0000/templates_1.root/pulseDump/pulse_tree");
      chain->Add("root://eosinfnts.ts.infn.it//eos/infnts/cms/store/user/adarold/ECAL/TimeScan/AlCaPhiSym/crab_TimeScan/180116_164610/0000/templates_2.root/pulseDump/pulse_tree");
      chain->Add("root://eosinfnts.ts.infn.it//eos/infnts/cms/store/user/adarold/ECAL/TimeScan/AlCaPhiSym/crab_TimeScan/180116_164610/0000/templates_3.root/pulseDump/pulse_tree");
      chain->Add("root://eosinfnts.ts.infn.it//eos/infnts/cms/store/user/adarold/ECAL/TimeScan/AlCaPhiSym/crab_TimeScan/180116_164610/0000/templates_4.root/pulseDump/pulse_tree");
      chain->Add("root://eosinfnts.ts.infn.it//eos/infnts/cms/store/user/adarold/ECAL/TimeScan/AlCaPhiSym/crab_TimeScan/180116_164610/0000/templates_5.root/pulseDump/pulse_tree");
      chain->Add("root://eosinfnts.ts.infn.it//eos/infnts/cms/store/user/adarold/ECAL/TimeScan/AlCaPhiSym/crab_TimeScan/180116_164610/0000/templates_6.root/pulseDump/pulse_tree");
      chain->Add("root://eosinfnts.ts.infn.it//eos/infnts/cms/store/user/adarold/ECAL/TimeScan/AlCaPhiSym/crab_TimeScan/180116_164610/0000/templates_7.root/pulseDump/pulse_tree");
      chain->Add("root://eosinfnts.ts.infn.it//eos/infnts/cms/store/user/adarold/ECAL/TimeScan/AlCaPhiSym/crab_TimeScan/180116_164610/0000/templates_8.root/pulseDump/pulse_tree");
      chain->Add("root://eosinfnts.ts.infn.it//eos/infnts/cms/store/user/adarold/ECAL/TimeScan/AlCaPhiSym/crab_TimeScan/180116_164610/0000/templates_9.root/pulseDump/pulse_tree");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

make_rawid::~make_rawid()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t make_rawid::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t make_rawid::LoadTree(Long64_t entry)
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

void make_rawid::Init(TTree *tree)
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

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("barrel", &barrel, &b_barrel);
   fChain->SetBranchAddress("gain", &gain, &b_gain);
   fChain->SetBranchAddress("pedrms", &pedrms, &b_pedrms);
   fChain->SetBranchAddress("pedval", &pedval, &b_pedval);
   fChain->SetBranchAddress("ietaix", &ietaix, &b_ietaix);
   fChain->SetBranchAddress("iphiiy", &iphiiy, &b_iphiiy);
   fChain->SetBranchAddress("iz", &iz, &b_iz);
   fChain->SetBranchAddress("pulse", pulse, &b_pulse);
   fChain->SetBranchAddress("rawid", &rawid, &b_rawid);
   fChain->SetBranchAddress("ene", &ene, &b_ene);
   fChain->SetBranchAddress("time", &time, &b_time);
   fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
   fChain->SetBranchAddress("flag_kweird", &flag_kweird, &b_flag_kweird);
   fChain->SetBranchAddress("flag_kdiweird", &flag_kdiweird, &b_flag_kdiweird);
   fChain->SetBranchAddress("lumisec", &lumisec, &b_lumisec);
   Notify();
}

Bool_t make_rawid::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void make_rawid::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t make_rawid::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef make_rawid_cxx
