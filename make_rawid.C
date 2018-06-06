#define make_rawid_cxx
#include "make_rawid.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "Riostream.h"
#include "TMath.h"

void make_rawid::Loop()
{
//   In a ROOT session, you can do:
//      root> .L make_rawid.C
//      root> make_rawid t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   fChain->SetBranchStatus("*",0);
   fChain->SetBranchStatus("rawid",1);

   int id_list[80000];
   int sorted_index[80000];
   int index=0;
   int n=0;

   bool control=0;   

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      for(int i=0; i<=index; i++){
         if(rawid==id_list[i]) control=0;
         if(control==0) break;
      }      

      if(control){
         id_list[index]=rawid;
         index++;
      }
      
      control=1;
      
   }
  
   index=index-1;

   TMath::Sort(index+1,id_list,sorted_index,0);

   int final_list[index+1]; 
   for(int i=0; i<index; i++){
      final_list[i]=id_list[sorted_index[i]];
   }

   ofstream txt_file;
   txt_file.open ("rawid_list.txt");

   for(int i=0; i<index; i++){
      txt_file << final_list[i] << endl;
   }

   txt_file.close();


}
