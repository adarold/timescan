#include "TF1.h"
#include "TH1F.h"
#include "TMath.h"
#include "Riostream.h"


//Allows to compare the pulse shapes obtained with the standard analysis with those obtained in the 
//time scan runs.

double alphabeta(double* x, double*p){

   double fitval;

   if( ( 1+(x[0]-p[1]) / (p[2]*p[3]) ) > 0. ){
      fitval = p[0] * pow(1 + (x[0]-p[1]) / (p[2]*p[3]) , p[2] ) * exp ( - (x[0]-p[1]) / p[3]);
   }else{
      fitval = 0;
   }

   return fitval;

}


int zside(int id) { return (id&0x10000)?(1):(-1); }
int ietaAbs(int id) { return (id>>9)&0x7F; }
int ieta(int id) { return zside(id)*ietaAbs(id); }
int iphi(int id) { return id&0x1FF; }

int iz(int id) { return (id&0x4000)?(1):(-1); }
int ix(int id) { return (id>>7)&0x7F; }
int iy(int id) { return id&0x7F; }


void compare() {

   //Variables
   int npt=0;
   int ix[80000]; 
   int iy[80000]; 
   int barrel[80000]; 
   int rawid[80000];
   float pulse[80000][12];

   //Open file with pulse infos and saves it

   ifstream txt_ps;
   txt_ps.open("standard_ps.txt");

   while ( kTRUE ) {
      txt_ps >> iy[npt] >> ix[npt] >> barrel[npt] >> pulse[npt][0] >> pulse[npt][1] >> pulse[npt][2] >> pulse[npt][3] >> pulse[npt][4] >> pulse[npt][5] >> pulse[npt][6] >> pulse[npt][7] >> pulse[npt][8] >> pulse[npt][9] >> pulse[npt][10] >> pulse[npt][11] >> rawid[npt];
      if ( ! txt_ps.good() ) break;
      npt++;
   }

   txt_ps.close();

   //Variables
   float b_mean_pulse[12];
   float b_mean_perror[12];
   float e_mean_pulse[12];
   float e_mean_perror[12];
   float e_mean_in_pulse[12];
   float e_mean_in_perror[12];
   float e_mean_out_pulse[12];
   float e_mean_out_perror[12];
   float time[12];
   float terror[12];
   int n_barrel=0;
   int n_endcap=0;
   int n_endcap_in=0;
   int n_endcap_out=0;
   float b_sum=0;
   float e_sum=0;


   //Initialize the variables
   for(int i=0; i<12; i++){

      b_mean_pulse[i]=0;
      b_mean_perror[i]=0;
      e_mean_pulse[i]=0;
      e_mean_perror[i]=0;

   }


   for(int i=0; i<npt; i++){

      if(barrel[i]==0){

         for(int k=0; k<12; k++){

            b_mean_pulse[k]=b_mean_pulse[k]+pulse[i][k];
            b_mean_perror[k]=b_mean_perror[k]+TMath::Power(pulse[i][k],2);

         }

         n_barrel++;

      }else{

         for(int k=0; k<12; k++){
            
            e_mean_pulse[k]=e_mean_pulse[k]+pulse[i][k];
            e_mean_perror[k]=e_mean_perror[k]+TMath::Power(pulse[i][k],2);

         }

         n_endcap++;

         if( TMath::Power(ix[i]-50,2)+TMath::Power(iy[i]-50,2) < 25*25){

            for(int k=0; k<12; k++){

               e_mean_in_pulse[k]=e_mean_in_pulse[k]+pulse[i][k];
               e_mean_in_perror[k]=e_mean_in_perror[k]+TMath::Power(pulse[i][k],2);

            }

            n_endcap_in++;

          }else{

            for(int k=0; k<12; k++){

               e_mean_out_pulse[k]=e_mean_out_pulse[k]+pulse[i][k];
               e_mean_out_perror[k]=e_mean_out_perror[k]+TMath::Power(pulse[i][k],2);

            }

            n_endcap_out++;

          }

      }

   }

   float A_b=0;
   float t_0_b=0;
   float alpha_b=0;
   float beta_b=0;
   float A_e=0;
   float t_0_e=0;
   float alpha_e=0;
   float beta_e=0;
   float b_in[4];
   float b_out[4];
   float e_in[4];
   float e_out[4];
   char name[20];

   //Open file with information on mean values of the parameters for the fitted alpha-beta functions
   ifstream txt_mean;
   txt_mean.open("meanpar.txt");

   while ( kTRUE ) {
      txt_mean >> name >> A_b >> t_0_b >> alpha_b >> beta_b;
      txt_mean >> name >> A_e >> t_0_e >> alpha_e >> beta_e;
      txt_mean >> name >> b_in[0] >> b_in[1] >> b_in[2] >> b_in[3];
      txt_mean >> name >> b_out[0] >> b_out[1] >> b_out[2] >> b_out[3];
      txt_mean >> name >> e_in[0] >> e_in[1] >> e_in[2] >> e_in[3];
      txt_mean >> name >> e_out[0] >> e_out[1] >> e_out[2] >> e_out[3];
      if ( ! txt_mean.good() ) break;
   }

   txt_mean.close();


   //Constructs average pulse shapes for barrel and endcaps
   for(int i=0; i<12; i++){

      b_mean_pulse[i]=b_mean_pulse[i]/(n_barrel*1.0);
      b_mean_perror[i]= sqrt( b_mean_perror[i]/(n_barrel*1.0) - TMath::Power(b_mean_pulse[i],2)  );

      e_mean_pulse[i]=e_mean_pulse[i]/(n_endcap*1.0);
      e_mean_perror[i]= sqrt( e_mean_perror[i]/(n_endcap*1.0) - TMath::Power(e_mean_pulse[i],2)  );

      e_mean_in_pulse[i]=e_mean_in_pulse[i]/(n_endcap_in*1.0);
      e_mean_in_perror[i]= sqrt( e_mean_in_perror[i]/(n_endcap_in*1.0) - TMath::Power(e_mean_in_pulse[i],2)  );

      e_mean_out_pulse[i]=e_mean_out_pulse[i]/(n_endcap_out*1.0);
      e_mean_out_perror[i]= sqrt( e_mean_out_perror[i]/(n_endcap_out*1.0) - TMath::Power(e_mean_out_pulse[i],2)  );

      time[i]=75+25*i;
      terror[i]=12.5;

   }


   TF1 *f_barrel = new TF1("f_barrel", alphabeta, -11, 400, 4);
   TF1 *f_endcap = new TF1("f_endcap", alphabeta, -11, 400, 4);
   TF1 *f_barrel_in = new TF1("f_barrel_in", alphabeta, -11, 400, 4);
   TF1 *f_barrel_out = new TF1("f_barrel_out", alphabeta, -11, 400, 4);
   TF1 *f_endcap_in = new TF1("f_endcap_in", alphabeta, -11, 400, 4);
   TF1 *f_endcap_out = new TF1("f_endcap_out", alphabeta, -11, 400, 4);

   f_barrel->FixParameter(0,A_b);
   f_barrel->FixParameter(1,t_0_b);
   f_barrel->FixParameter(2,alpha_b);
   f_barrel->FixParameter(3,beta_b);

   f_endcap->FixParameter(0,A_e);
   f_endcap->FixParameter(1,t_0_e);
   f_endcap->FixParameter(2,alpha_e);
   f_endcap->FixParameter(3,beta_e);

   for(int i=0; i<4; i++){

      f_barrel_in->FixParameter(i,b_in[i]);
      f_barrel_out->FixParameter(i,b_out[i]);
      f_endcap_in->FixParameter(i,e_in[i]);
      f_endcap_out->FixParameter(i,e_out[i]);
   }


   float b_max=f_barrel->GetMaximum();
   float e_max=f_endcap->GetMaximum();
   float e_max_in=f_endcap_in->GetMaximum();
   float e_max_out=f_endcap_out->GetMaximum();
   float gb_max=b_mean_pulse[2];
   float ge_max=e_mean_pulse[2];
   float ge_max_in=e_mean_in_pulse[2];
   float ge_max_out=e_mean_out_pulse[2];

   float b_ratio[12];
   float e_ratio[12];
   float e_in_ratio[12];
   float e_out_ratio[12];
   float b_ratio_err[12];
   float e_ratio_err[12];
   float e_in_ratio_err[12];
   float e_out_ratio_err[12];

   for(int i=0; i<12; i++){

      b_mean_pulse[i]=(b_mean_pulse[i]*b_max)/gb_max;
      e_mean_pulse[i]=(e_mean_pulse[i]*e_max)/ge_max;
      e_mean_in_pulse[i]=(e_mean_in_pulse[i]*e_max_in)/ge_max_in;
      e_mean_out_pulse[i]=(e_mean_out_pulse[i]*e_max_out)/ge_max_out;

      b_mean_perror[i]=(b_mean_perror[i]*b_max)/gb_max;
      e_mean_perror[i]=(e_mean_perror[i]*e_max)/ge_max;
      e_mean_in_perror[i]=(e_mean_in_perror[i]*e_max_in)/ge_max_in;
      e_mean_out_perror[i]=(e_mean_out_perror[i]*e_max_out)/ge_max_out;

      if(f_barrel->Eval(time[i])!=0){
         b_ratio[i]=b_mean_pulse[i]/f_barrel->Eval(time[i]);
         b_ratio_err[i]=b_mean_perror[i]/f_barrel->Eval(time[i]);
      }else{
         b_ratio[i]=0;
         b_ratio_err[i]=0;
      }

      if(f_endcap->Eval(time[i])!=0){
         e_ratio[i]=e_mean_pulse[i]/f_endcap->Eval(time[i]);
         e_ratio_err[i]=e_mean_perror[i]/f_endcap->Eval(time[i]);
      }else{
         e_ratio[i]=0;
         e_ratio_err[i]=0;
      }

      if(f_endcap_in->Eval(time[i])!=0){
         e_in_ratio[i]=e_mean_in_pulse[i]/f_endcap_in->Eval(time[i]);
         e_in_ratio_err[i]=e_mean_in_perror[i]/f_endcap_in->Eval(time[i]);
      }else{
         e_in_ratio[i]=0;
         e_in_ratio_err[i]=0;
      }

      if(f_endcap_out->Eval(time[i])!=0){
         e_out_ratio[i]=e_mean_out_pulse[i]/f_endcap_out->Eval(time[i]);
         e_out_ratio_err[i]=e_mean_out_perror[i]/f_endcap_out->Eval(time[i]);
      }else{
         e_out_ratio[i]=0;
         e_out_ratio_err[i]=0;
      }

   }

   TGraphErrors * gb = new TGraphErrors(12, time, b_mean_pulse, terror, b_mean_perror);
   TGraphErrors * ge = new TGraphErrors(12, time, e_mean_pulse, terror, e_mean_perror);
   TGraphErrors * ge_in = new TGraphErrors(12, time, e_mean_in_pulse, terror, e_mean_in_perror);
   TGraphErrors * ge_out = new TGraphErrors(12, time, e_mean_out_pulse, terror, e_mean_out_perror);

   TGraphErrors * gb_ratio = new TGraphErrors(12, time, b_ratio, terror, b_ratio_err);
   TGraphErrors * ge_ratio = new TGraphErrors(12, time, e_ratio, terror, e_ratio_err);
   TGraphErrors * ge_in_ratio = new TGraphErrors(12, time, e_in_ratio, terror, e_in_ratio_err);
   TGraphErrors * ge_out_ratio = new TGraphErrors(12, time, e_out_ratio, terror, e_out_ratio_err);


   //Draws the histograms
   TLine * l = new TLine(50,1.,370,1.);
      l->SetLineColor(kRed);
      l->SetLineWidth(2);

   TLegend * leg = new TLegend(0.6,0.5,0.89,0.8);
      leg->AddEntry(gb,"Standard pulseshape","el");
      leg->AddEntry(f_barrel,"Time scan pulseshape","l");


   TCanvas * cb = new TCanvas("cb","cb",0,0,1100,1200);
   TPad * pad1_cb = new TPad("pad1_cb","pad1_cb",0,0.33,1,1);
   pad1_cb->SetBottomMargin(0);
   pad1_cb->Draw();
   pad1_cb->cd();
   pad1_cb->cd();
   gb->GetYaxis()->SetLabelSize(0.04*1.2);
   gb->GetXaxis()->SetRangeUser(50,370);
   gb->SetMinimum(-0.02);
   gStyle->SetTitleFontSize(0.06);
   gb->SetTitle("Shape comparison - Barrel");
   gb->Draw("AP");
   f_barrel->SetLineWidth(2);
   f_barrel->SetLineColor(kBlue);
   f_barrel->Draw("same");

   leg->SetBorderSize(0);
   leg->Draw();

   cb->cd();
   TPad * pad2_cb = new TPad("pad2_cb","pad2_cb",0,0,1,0.33);
   pad2_cb->SetTopMargin(0); 
   pad2_cb->SetBottomMargin(0.3); 
   pad2_cb->Draw(); 
   pad2_cb->cd();
   gb_ratio->SetMinimum(0.8);
   gb_ratio->GetYaxis()->SetLabelSize(0.08*1.2);
   gb_ratio->GetXaxis()->SetLabelSize(0.08*1.2);
   gb_ratio->GetXaxis()->SetTitleOffset(1);
   gb_ratio->GetXaxis()->SetTitleSize(0.11);
   gb_ratio->GetXaxis()->SetTitle("Time");
   gb_ratio->GetXaxis()->SetRangeUser(50,370);
   gb_ratio->GetYaxis()->SetNdivisions(305);
   gb_ratio->SetTitle("");
   gb_ratio->Draw("AP");
   l->Draw("same");
   gb_ratio->Draw("P same");



   TCanvas * ce = new TCanvas("ce","ce",0,0,1100,1200);
   TPad * pad1_ce = new TPad("pad1_ce","pad1_ce",0,0.33,1,1);
   pad1_ce->SetBottomMargin(0);
   pad1_ce->Draw();
   pad1_ce->cd();
   pad1_ce->cd();
   ge->GetYaxis()->SetLabelSize(0.04*1.2);
   ge->GetXaxis()->SetRangeUser(50,370);
   ge->SetMinimum(-0.02);
   gStyle->SetTitleFontSize(0.06);
   ge->SetTitle("Shape comparison - Endcaps");
   ge->Draw("AP");
   f_endcap->SetLineWidth(2);
   f_endcap->SetLineColor(kBlue);
   f_endcap->Draw("same");

   leg->SetBorderSize(0);
   leg->Draw();

   ce->cd();
   TPad * pad2_ce = new TPad("pad2_ce","pad2_ce",0,0,1,0.33);
   pad2_ce->SetTopMargin(0); 
   pad2_ce->SetBottomMargin(0.3); 
   pad2_ce->Draw(); 
   pad2_ce->cd();
   ge_ratio->GetYaxis()->SetLabelSize(0.08*1.2);
   ge_ratio->GetXaxis()->SetLabelSize(0.08*1.2);
   ge_ratio->GetXaxis()->SetTitleOffset(1);
   ge_ratio->GetXaxis()->SetTitleSize(0.11);
   ge_ratio->GetXaxis()->SetTitle("Time");
   ge_ratio->GetXaxis()->SetRangeUser(50,370);
   ge_ratio->GetYaxis()->SetNdivisions(305);
   ge_ratio->SetTitle("");
   ge_ratio->Draw("AP");
   l->Draw("same");
   ge_ratio->Draw("P same");



   TCanvas * ce_in = new TCanvas("ce_in","ce_in",0,0,1100,1200);
   TPad * pad1_ce_in = new TPad("pad1_ce_in","pad1_ce_in",0,0.33,1,1);
   pad1_ce_in->SetBottomMargin(0);
   pad1_ce_in->Draw();
   pad1_ce_in->cd();
   pad1_ce_in->cd();
   ge_in->GetYaxis()->SetLabelSize(0.04*1.2);
   ge_in->GetXaxis()->SetRangeUser(50,370);
   ge_in->SetMinimum(-0.02);
   gStyle->SetTitleFontSize(0.06);
   ge_in->SetTitle("Shape comparison - Endcaps - IN");
   ge_in->Draw("AP");
   f_endcap_in->SetLineWidth(2);
   f_endcap_in->SetLineColor(kBlue);
   f_endcap_in->Draw("same");

   leg->SetBorderSize(0);
   leg->Draw();

   ce_in->cd();
   TPad * pad2_ce_in = new TPad("pad2_ce_in","pad2_ce_in",0,0,1,0.33);
   pad2_ce_in->SetTopMargin(0);
   pad2_ce_in->SetBottomMargin(0.3);
   pad2_ce_in->Draw();
   pad2_ce_in->cd();
   ge_in_ratio->GetYaxis()->SetLabelSize(0.08*1.2);
   ge_in_ratio->GetXaxis()->SetLabelSize(0.08*1.2);
   ge_in_ratio->GetXaxis()->SetTitleOffset(1);
   ge_in_ratio->GetXaxis()->SetTitleSize(0.11);
   ge_in_ratio->GetXaxis()->SetTitle("Time");
   ge_in_ratio->GetXaxis()->SetRangeUser(50,370);
   ge_in_ratio->GetYaxis()->SetNdivisions(305);
   ge_in_ratio->SetTitle("");
   ge_in_ratio->SetMaximum(5);
   ge_in_ratio->Draw("AP");
   l->Draw("same");
   ge_in_ratio->Draw("P same");



   TCanvas * ce_out = new TCanvas("ce_out","ce_out",0,0,1100,1200);
   TPad * pad1_ce_out = new TPad("pad1_ce_out","pad1_ce_out",0,0.33,1,1);
   pad1_ce_out->SetBottomMargin(0);
   pad1_ce_out->Draw();
   pad1_ce_out->cd();
   pad1_ce_out->cd();
   ge_out->GetYaxis()->SetLabelSize(0.04*1.2);
   ge_out->GetXaxis()->SetRangeUser(50,370);
   ge_out->SetMinimum(-0.02);
   gStyle->SetTitleFontSize(0.06);
   ge_out->SetTitle("Shape comparison - Endcaps - OUT");
   ge_out->Draw("AP");
   f_endcap_out->SetLineWidth(2);
   f_endcap_out->SetLineColor(kBlue);
   f_endcap_out->Draw("same");

   leg->SetBorderSize(0);
   leg->Draw();

   ce_out->cd();
   TPad * pad2_ce_out = new TPad("pad2_ce_out","pad2_ce_out",0,0,1,0.33);
   pad2_ce_out->SetTopMargin(0);
   pad2_ce_out->SetBottomMargin(0.3);
   pad2_ce_out->Draw();
   pad2_ce_out->cd();
   ge_out_ratio->GetYaxis()->SetLabelSize(0.08*1.2);
   ge_out_ratio->GetXaxis()->SetLabelSize(0.08*1.2);
   ge_out_ratio->GetXaxis()->SetTitleOffset(1);
   ge_out_ratio->GetXaxis()->SetTitleSize(0.11);
   ge_out_ratio->GetXaxis()->SetTitle("Time");
   ge_out_ratio->GetXaxis()->SetRangeUser(50,370);
   ge_out_ratio->GetYaxis()->SetNdivisions(305);
   ge_out_ratio->SetTitle("");
   ge_out_ratio->SetMaximum(5);
   ge_out_ratio->Draw("AP");
   l->Draw("same");
   ge_out_ratio->Draw("P same");



   TCanvas * cf = new TCanvas("cf","cf",0,0,1200,600);
   cf->Divide(2,1);
   cf->cd(1);
   f_barrel->SetTitle("Barrel");
   f_barrel->SetLineColor(kBlue);
   f_barrel->Draw();
   f_barrel_in->SetLineColor(kRed);
   f_barrel_in->Draw("same");
   f_barrel_out->SetLineColor(kGreen);
   f_barrel_out->Draw("same");
   cf->cd(2);
   f_endcap->SetTitle("Endcap");
   f_endcap->SetLineColor(kBlue);
   f_endcap->Draw();
   f_endcap_in->SetLineColor(kRed);
   f_endcap_in->Draw("same");
   f_endcap_out->SetLineColor(kGreen);
   f_endcap_out->Draw("same");


  
   //Many run comparison graphs

   TFile * f1 = new TFile("many_run.root");
    
      TGraph * gb_pre = (TGraph*)f1->Get("gb_pre");
      TGraph * ge_pre = (TGraph*)f1->Get("ge_pre");
      TGraph * ge_in_pre = (TGraph*)f1->Get("ge_in_pre");
      TGraph * ge_out_pre = (TGraph*)f1->Get("ge_out_pre");
      TGraph * gb_ratio_pre = (TGraph*)f1->Get("gb_ratio_pre");
      TGraph * ge_ratio_pre = (TGraph*)f1->Get("ge_ratio_pre");
      TGraph * ge_in_ratio_pre = (TGraph*)f1->Get("ge_in_ratio_pre");
      TGraph * ge_out_ratio_pre = (TGraph*)f1->Get("ge_out_ratio_pre");

      TGraph * gb_post = (TGraph*)f1->Get("gb_post");
      TGraph * ge_post = (TGraph*)f1->Get("ge_post");
      TGraph * ge_in_post = (TGraph*)f1->Get("ge_in_post");
      TGraph * ge_out_post = (TGraph*)f1->Get("ge_out_post");
      TGraph * gb_ratio_post = (TGraph*)f1->Get("gb_ratio_post");
      TGraph * ge_ratio_post = (TGraph*)f1->Get("ge_ratio_post");
      TGraph * ge_in_ratio_post = (TGraph*)f1->Get("ge_in_ratio_post");
      TGraph * ge_out_ratio_post = (TGraph*)f1->Get("ge_out_ratio_post");

   TLegend * leg1 = new TLegend(0.45,0.5,0.99,0.89);
   leg1->AddEntry(gb_pre,"Std pulseshape runs 304209-304910","el");
   leg1->AddEntry(gb,"Std pulseshape runs 304911-305813","el");
   leg1->AddEntry(gb_post,"Std pulseshape runs 305814-305831","el");
   leg1->AddEntry(f_barrel,"Time scan pulseshape","l");



   TCanvas * cb_run = new TCanvas("cb_run","cb_run",0,0,1100,1200);
   TPad * pad1_cb_run = new TPad("pad1_cb_run","pad1_cb_run",0,0.33,1,1);
   pad1_cb_run->SetBottomMargin(0);
   pad1_cb_run->Draw();
   pad1_cb_run->cd();
   pad1_cb_run->cd();
   gb->GetYaxis()->SetLabelSize(0.04*1.2);
   gb->GetXaxis()->SetRangeUser(50,370);
   gb->SetMinimum(-0.02);
   gStyle->SetTitleFontSize(0.06);
   gb->SetTitle("Shape comparison - Barrel");
   gb->Draw("AP");
   gb_pre->SetLineColor(kRed);
   gb_pre->Draw("P same");
   gb_post->SetLineColor(kGreen);
   gb_post->Draw("P same");
   f_barrel->SetLineWidth(2);
   f_barrel->SetLineColor(kBlue);
   f_barrel->Draw("same");

   leg1->SetBorderSize(0);
   leg1->Draw();

   cb_run->cd();
   TPad * pad2_cb_run = new TPad("pad2_cb_run","pad2_cb_run",0,0,1,0.33);
   pad2_cb_run->SetTopMargin(0); 
   pad2_cb_run->SetBottomMargin(0.3); 
   pad2_cb_run->Draw(); 
   pad2_cb_run->cd();
   gb_ratio->GetYaxis()->SetLabelSize(0.08*1.2);
   gb_ratio->GetXaxis()->SetLabelSize(0.08*1.2);
   gb_ratio->GetXaxis()->SetTitleOffset(1);
   gb_ratio->GetXaxis()->SetTitleSize(0.11);
   gb_ratio->GetXaxis()->SetTitle("Time");
   gb_ratio->GetXaxis()->SetRangeUser(50,370);
   gb_ratio->GetYaxis()->SetNdivisions(305);
   gb_ratio->SetTitle("");
   gb_ratio->Draw("AP");
   gb_ratio_pre->SetLineColor(kRed);
   gb_ratio_pre->Draw("P same");
   gb_ratio_post->SetLineColor(kGreen);
   gb_ratio_post->Draw("P same");
   l->Draw("same");
   gb_ratio->Draw("P same");



   TCanvas * ce_run = new TCanvas("ce_run","ce_run",0,0,1100,1200);
   TPad * pad1_ce_run = new TPad("pad1_ce_run","pad1_ce_run",0,0.33,1,1);
   pad1_ce_run->SetBottomMargin(0);
   pad1_ce_run->Draw();
   pad1_ce_run->cd();
   pad1_ce_run->cd();
   ge->GetYaxis()->SetLabelSize(0.04*1.2);
   ge->GetXaxis()->SetRangeUser(50,370);
   ge->SetMinimum(-0.02);
   gStyle->SetTitleFontSize(0.06);
   ge->SetTitle("Shape comparison - Endcap");
   ge->Draw("AP");
   ge_pre->SetLineColor(kRed);
   ge_pre->Draw("P same");
   ge_post->SetLineColor(kGreen);
   ge_post->Draw("P same");
   f_endcap->SetLineWidth(2);
   f_endcap->SetLineColor(kBlue);
   f_endcap->Draw("same");

   leg1->SetBorderSize(0);
   leg1->Draw();

   ce_run->cd();
   TPad * pad2_ce_run = new TPad("pad2_ce_run","pad2_ce_run",0,0,1,0.33);
   pad2_ce_run->SetTopMargin(0); 
   pad2_ce_run->SetBottomMargin(0.3); 
   pad2_ce_run->Draw(); 
   pad2_ce_run->cd();
   ge_ratio->GetYaxis()->SetLabelSize(0.08*1.2);
   ge_ratio->GetXaxis()->SetLabelSize(0.08*1.2);
   ge_ratio->GetXaxis()->SetTitleOffset(1);
   ge_ratio->GetXaxis()->SetTitleSize(0.11);
   ge_ratio->GetXaxis()->SetTitle("Time");
   ge_ratio->GetXaxis()->SetRangeUser(50,370);
   ge_ratio->GetYaxis()->SetNdivisions(305);
   ge_ratio->SetTitle("");
   ge_ratio->Draw("AP");
   ge_ratio_pre->SetLineColor(kRed);
   ge_ratio_pre->Draw("P same");
   ge_ratio_post->SetLineColor(kGreen);
   ge_ratio_post->Draw("P same");
   l->Draw("same");
   ge_ratio->Draw("P same");


   //Saves histograms
   cb->SaveAs("plots/barrel_compare.pdf");
   ce->SaveAs("plots/endcap_compare.pdf");
   ce_in->SaveAs("plots/endcap_compare_in.pdf");
   ce_out->SaveAs("plots/endcap_compare_out.pdf");
   cb_run->SaveAs("plots/barrel_compare_manyrun.pdf");
   ce_run->SaveAs("plots/endcap_compare_manyrun.pdf");
   cf->SaveAs("plots/in_out.pdf");

}
