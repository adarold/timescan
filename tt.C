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
#include "TLine.h"


int zside(int id) { return (id&0x10000)?(1):(-1); }
int ietaAbs(int id) { return (id>>9)&0x7F; }
int ieta(int id) { return zside(id)*ietaAbs(id); }
int iphi(int id) { return id&0x1FF; }

int izz(int id) { return (id&0x4000)?(1):(-1); }
int ixx(int id) { return (id>>7)&0x7F; }
int iyy(int id) { return id&0x7F; }

double alphabeta(double* x, double*p){

   double fitval;

   if( ( 1+(x[0]-p[1]) / (p[2]*p[3]) ) > 0. ){
      fitval = p[0] * pow(1 + (x[0]-p[1]) / (p[2]*p[3]) , p[2] ) * exp ( - (x[0]-p[1]) / p[3]);
   }else{
      fitval = 0;
   }

   return fitval;

}

void tt::Loop(string path, int year)
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

   char file_everycrystalpar[100];
   sprintf (file_everycrystalpar, "%s/standard_%d_everycrystal_par.txt", (path).c_str(), year);

   ifstream txt_par;
   txt_par.open(file_everycrystalpar);

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

   int drawmax = 0;
   
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

            if(fabs( t_0[i] - m_f[key_ret].valf[2]/m_f[key_ret].valf[0]) > 5) {

               //rawid abs(delta_t) delta_t average_t_in_triggertower	
               cout << rawid_value[i] << "   " << fabs( t_0[i] - m_f[key_ret].valf[2]/m_f[key_ret].valf[0]) << "   " << t_0[i] - m_f[key_ret].valf[2]/m_f[key_ret].valf[0] << "   " << m_f[key_ret].valf[2]/m_f[key_ret].valf[0] << endl;

               if (drawmax<2){
 
                  drawmax++;
 
                  TF1 *function_alphabeta = new TF1("function_alphabeta", alphabeta, -11, 236, 4);
                  function_alphabeta->FixParameter(0, A[i]);
                  function_alphabeta->FixParameter(1, t_0[i]);
                  function_alphabeta->FixParameter(2, alpha[i]);
                  function_alphabeta->FixParameter(3, beta[i]);

                  char plot_title[300];
                  sprintf (plot_title, "Crystal %d", rawid_value[i]);

                  TLine * l1 = new TLine (t_0[i], -0.05, t_0[i], 0.3);
                  TLine * l2 = new TLine (m_f[key_ret].valf[2]/m_f[key_ret].valf[0], -0.05, m_f[key_ret].valf[2]/m_f[key_ret].valf[0], 0.3);

                  TCanvas * ct1 = new TCanvas("ct1", "ct1", 0, 0, 500, 500);
                  ct1->cd();
                  function_alphabeta->SetMinimum(-0.05); 
                  function_alphabeta->SetMaximum(0.3); 
                  function_alphabeta->SetTitle(plot_title); 
                  function_alphabeta->Draw();
                  l1->SetLineColor(kBlue);
                  l1->SetLineStyle(2);
                  l1->SetLineWidth(4);
                  l1->Draw("same");
                  l2->SetLineColor(kRed);
                  l2->SetLineStyle(2);
                  l2->SetLineWidth(4);
                  l2->Draw("same");

                  char pathct1[1000];
                  sprintf (pathct1, "/home/darold/html/shape/%d/shifted_crystal_%d.pdf", year, rawid_value[i]);
                  ct1->SaveAs(pathct1);

               }

            }

h_delay->Fill(t_0[i] - m_f[key_ret].valf[2]/m_f[key_ret].valf[0]);

         }

      }

      key_ret=0;

   }

   //Output file with TT average of the parameters
   char file_triggertower[100];
   sprintf (file_triggertower, "%d_triggertower_info.txt", year);

   ofstream txt_ttinfo;
   txt_ttinfo.open (file_triggertower);

   for(int i=0; i<npt; i++){

      if(rawid_value[i]<839000000){

      }else{

         key_ret=m_e[rawid_value[i]].vale[0];

            txt_ttinfo << rawid_value[i] << "\t" << m_f[key_ret].valf[1]/m_f[key_ret].valf[0] << "\t" << m_f[key_ret].valf[2]/m_f[key_ret].valf[0] << "\t" << m_f[key_ret].valf[3]/m_f[key_ret].valf[0] << "\t" << m_f[key_ret].valf[4]/m_f[key_ret].valf[0] << endl;

      }

   }

   txt_ttinfo.close();

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
   gStyle->SetPalette(1);

 
   TCanvas * c = new TCanvas("c","c",0,0,1000,500);
   c->Divide(2,1);

   c->cd(1);
   hp->GetXaxis()->SetTitleSize(0.06);
   hp->GetXaxis()->SetTitleOffset(0.7);
   hp->GetYaxis()->SetTitleSize(0.06);
   hp->GetYaxis()->SetTitleOffset(0.7);
   hp->GetXaxis()->SetTitle("ix");
   hp->GetYaxis()->SetTitle("iy");
   hp->SetMaximum(17.);
   hp->SetTitle("Max time displacement from average for TT +side");
   hp->Draw("colz");

   c->cd(2);
   hm->GetXaxis()->SetTitleSize(0.06);
   hm->GetXaxis()->SetTitleOffset(0.7);
   hm->GetYaxis()->SetTitleSize(0.06);
   hm->GetYaxis()->SetTitleOffset(0.7);
   hm->GetXaxis()->SetTitle("ix");
   hm->GetYaxis()->SetTitle("iy");
   hm->SetMaximum(17.);
   hm->SetTitle("Max time displacement from average for TT -side");
   hm->Draw("colz");

   char histo_title[300];
   sprintf (histo_title, "Time displacement from average for TT - %d", year);

   TCanvas * c1 = new TCanvas("c1","c1",0,0,500,500);
   c1->cd();
   h_delay->GetXaxis()->SetTitleSize(0.06);
   h_delay->GetXaxis()->SetTitleOffset(0.7);
   h_delay->GetXaxis()->SetTitle("#Delta t (ns)");
   h_delay->SetTitle(histo_title);
   h_delay->SetLineColor(kBlue);
   h_delay->SetFillColorAlpha(kBlue,0.35);
   h_delay->Draw();

   char pathc[1000];
   char pathc1[1000];
   sprintf (pathc, "/home/darold/html/shape/%d/timedisplacement.pdf", year);
   sprintf (pathc1, "/home/darold/html/shape/%d/timedisplacement_histo.pdf", year);

   c->SaveAs(pathc);
   c1->SaveAs(pathc1);

}
