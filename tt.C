#define tt_cxx
#include "tt.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "Riostream.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TMath.h"


int zside(int id) { return (id&0x10000)?(1):(-1); }
int ietaAbs(int id) { return (id>>9)&0x7F; }
int ieta(int id) { return zside(id)*ietaAbs(id); }
int iphi(int id) { return id&0x1FF; }

int izz(int id) { return (id&0x4000)?(1):(-1); }
int ixx(int id) { return (id>>7)&0x7F; }
int iyy(int id) { return id&0x7F; }

void tt::Loop()
{
//   In a ROOT session, you can do:
//      root> .L tt.C
//      root> tt t
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

   Long64_t nentries = fChain->GetEntriesFast();

   int rawid_value[80000];
   float A[80000];
   float t_0[80000];
   float alpha[80000];
   float beta[80000];
   int npt=0;
   int cry_x=0;
   int cry_y=0;
   int cry_z=0;

   ifstream txt_par;
   txt_par.open("everycrystal_par.txt");

   while ( kTRUE ) {
      txt_par >> rawid_value[npt] >> A[npt] >> t_0[npt] >> alpha[npt] >> beta[npt];
      if ( ! txt_par.good() ) break;
      npt++;
   }

   txt_par.close();


   struct valuese {float vale[1];};
   map<int, valuese> m_e;
   valuese v_vale;

   int key_e=0;


   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      key_e=tcc*10000+tower;

      v_vale.vale[0]=key_e;
      m_e[cmsswId]=v_vale;

   }


   struct valuesf {float valf[5];};
   map<int, valuesf> m_f;
   valuesf v_valf;

   int key_ret=0;

   for(int i=0; i<npt; i++){

      if(rawid_value[i]<839000000){

      }else{

         key_ret=m_e[rawid_value[i]].vale[0];

         if(A[i]!=0){

            v_valf.valf[0]=m_f[key_ret].valf[0]+1;
            v_valf.valf[1]=m_f[key_ret].valf[1]+A[i];
            v_valf.valf[2]=m_f[key_ret].valf[2]+t_0[i];
            v_valf.valf[3]=m_f[key_ret].valf[3]+alpha[i];
            v_valf.valf[4]=m_f[key_ret].valf[4]+beta[i];

         }

         m_f[key_ret]=v_valf;

      }

      key_ret=0;

   }

   //Creates map for the maximum difference between the mean value and a fitted value of t_0 for a certain TT

   TH1F * h_delay = new TH1F("h_delay","h_delay", 100, -7, 7);
   
   struct valuesm {float valm[1];};
   map<int, valuesm> m_m;
   valuesm v_valm;

   for(int i=0; i<npt; i++){

      if(rawid_value[i]<839000000){

      }else{

         key_ret=m_e[rawid_value[i]].vale[0];

         if(A[i]!=0){

            if(fabs( t_0[i] - m_f[key_ret].valf[2]/m_f[key_ret].valf[0]) > m_m[key_ret].valm[0]){

               v_valm.valm[0]=fabs( t_0[i] - m_f[key_ret].valf[2]/m_f[key_ret].valf[0]);

               m_m[key_ret]=v_valm;

            }
if(fabs( t_0[i] - m_f[key_ret].valf[2]/m_f[key_ret].valf[0]) > 4) cout << rawid_value[i] << "   " << fabs( t_0[i] - m_f[key_ret].valf[2]/m_f[key_ret].valf[0]) << "   " << t_0[i] - m_f[key_ret].valf[2]/m_f[key_ret].valf[0] << "   " << m_f[key_ret].valf[2]/m_f[key_ret].valf[0] << endl;

h_delay->Fill(t_0[i] - m_f[key_ret].valf[2]/m_f[key_ret].valf[0]);

         }

      }

      key_ret=0;

   }


   TH2F * hp = new TH2F("hp","hp",100,0,100,100,0,100);
   TH2F * hm = new TH2F("hm","hm",100,0,100,100,0,100);

   for(int i=0; i<npt; i++){

      if(rawid_value[i]<839000000){

      }else{

         cry_x=ixx(rawid_value[i]);
         cry_y=iyy(rawid_value[i]);
         cry_z=izz(rawid_value[i]);

         key_ret=m_e[rawid_value[i]].vale[0];


         if(cry_z>0) hp->Fill(cry_x,cry_y,m_m[key_ret].valm[0]);
         if(cry_z<0) hm->Fill(cry_x,cry_y,m_m[key_ret].valm[0]);

      }

      key_ret=0;

   }

gStyle->SetOptFit(0);
gStyle->SetOptStat(0);
  
   TCanvas * c = new TCanvas("c","c",0,0,1000,500);
   c->Divide(2,1);

   c->cd(1);
   hp->GetXaxis()->SetTitleSize(0.06);
   hp->GetXaxis()->SetTitleOffset(0.7);
   hp->GetYaxis()->SetTitleSize(0.06);
   hp->GetYaxis()->SetTitleOffset(0.7);
   hp->GetXaxis()->SetTitle("ix");
   hp->GetYaxis()->SetTitle("iy");
   hp->SetTitle("Max time displacement from average for TT +side");
   hp->Draw("colz");

   c->cd(2);
   hm->GetXaxis()->SetTitleSize(0.06);
   hm->GetXaxis()->SetTitleOffset(0.7);
   hm->GetYaxis()->SetTitleSize(0.06);
   hm->GetYaxis()->SetTitleOffset(0.7);
   hm->GetXaxis()->SetTitle("ix");
   hm->GetYaxis()->SetTitle("iy");
   hm->SetTitle("Max time displacement from average for TT -side");
   hm->Draw("colz");


   TCanvas * c1 = new TCanvas("c1","c1",0,0,500,500);
   c1->cd();
   h_delay->GetXaxis()->SetTitleSize(0.06);
   h_delay->GetXaxis()->SetTitleOffset(0.7);
   h_delay->GetXaxis()->SetTitle("#Delta t (ns)");
   h_delay->SetTitle("Time displacement from average for TT - 2017");
   h_delay->SetLineColor(kBlue);
   h_delay->SetFillColorAlpha(kBlue,0.35);
   h_delay->Draw();


   c->SaveAs("plots/timedisplacement.pdf");
   c1->SaveAs("plots/timedisplacement_histo.pdf");

}
