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

int iz(int id) { return (id&0x4000)?(1):(-1); }
int ix(int id) { return (id>>7)&0x7F; }
int iy(int id) { return id&0x7F; }


void ringpar(string path, int year){

   int rawid_value[80000];
   float A[80000];
   float t_0[80000];
   float alpha[80000];
   float beta[80000];
   int npt=0;


   //Reads fitted parameters for all the crystals
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


   //Initialise variables
   float A_b_mean[171];
   float t_0_b_mean[171];
   float alpha_b_mean[171];
   float beta_b_mean[171];

   for(int i=0; i<171; i++){

      A_b_mean[i]=0;
      t_0_b_mean[i]=0;
      alpha_b_mean[i]=0;
      beta_b_mean[i]=0;

   }

   float A_ep_mean[40];
   float t_0_ep_mean[40];
   float alpha_ep_mean[40];
   float beta_ep_mean[40];
   float A_em_mean[40];
   float t_0_em_mean[40];
   float alpha_em_mean[40];
   float beta_em_mean[40];

   for(int i=0; i<40; i++){

      A_ep_mean[i]=0;
      t_0_ep_mean[i]=0;
      alpha_ep_mean[i]=0;
      beta_ep_mean[i]=0;
      A_em_mean[i]=0;
      t_0_em_mean[i]=0;
      alpha_em_mean[i]=0;
      beta_em_mean[i]=0;

   }

   float cry_x[npt];
   float cry_y[npt];
   float cry_z[npt];
   int nevents_b[171];
   int nevents_ep[40];
   int nevents_em[40];
   int index=0;
   float r=0.;


   //Evaluates mean values for fit parameters in eta rings
   for(int i=0; i<npt; i++){

      if(rawid_value[i]<839000000){

         cry_x[i]=iphi(rawid_value[i]);
         cry_y[i]=ieta(rawid_value[i]);
         cry_z[i]=zside(rawid_value[i]);

         index=cry_y[i]+85;
         nevents_b[index]=nevents_b[index]+1;
         A_b_mean[index]=A_b_mean[index]+A[i];
         t_0_b_mean[index]=t_0_b_mean[index]+t_0[i];
         alpha_b_mean[index]=alpha_b_mean[index]+alpha[i];
         beta_b_mean[index]=beta_b_mean[index]+beta[i];

      }else{
         cry_x[i]=ix(rawid_value[i]);
         cry_y[i]=iy(rawid_value[i]);
         cry_z[i]=iz(rawid_value[i]);

         r=sqrt(TMath::Power(cry_x[i]-50.5,2)+TMath::Power(cry_y[i]-50.5,2));

         for(int j=0; j<40; j++){

            if(r>11+j && r<12+j){

               if(cry_z[i]==+1){

                  A_ep_mean[j]=A_ep_mean[j]+A[i];
                  t_0_ep_mean[j]=t_0_ep_mean[j]+t_0[i];
                  alpha_ep_mean[j]=alpha_ep_mean[j]+alpha[i];
                  beta_ep_mean[j]=beta_ep_mean[j]+beta[i];
                  nevents_ep[j]=nevents_ep[j]+1;

               }else{

                  A_em_mean[j]=A_em_mean[j]+A[i];
                  t_0_em_mean[j]=t_0_em_mean[j]+t_0[i];
                  alpha_em_mean[j]=alpha_em_mean[j]+alpha[i];
                  beta_em_mean[j]=beta_em_mean[j]+beta[i];
                  nevents_em[j]=nevents_em[j]+1;

               }

            }  

         }

      }

   }


   //Saves parameters in three .txt files (EB, EE+, EE-)
   char file_etaring_barrel[100];
   sprintf (file_etaring_barrel, "%d_etaring_barrel.txt", year);

   ofstream txt_barrel;
   txt_barrel.open (file_etaring_barrel);

   for(int i=0; i<171; i++){
      if(i!=85) txt_barrel << i-85 << "\t" << A_b_mean[i]/(1.*nevents_b[i]) << "\t" << t_0_b_mean[i]/(1.*nevents_b[i]) << "\t" << alpha_b_mean[i]/(1.*nevents_b[i]) << "\t" << beta_b_mean[i]/(1.*nevents_b[i])<< endl;
      if(i==85) txt_barrel << i-85 << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0<< endl;
   }

   txt_barrel.close();

   char file_etaring_endcapp[100];
   sprintf (file_etaring_endcapp, "%d_etaring_endcapp.txt", year);

   ofstream txt_endcapp;
   txt_endcapp.open (file_etaring_endcapp);

   for(int i=0; i<40; i++){
      txt_endcapp << 11+i << "\t" << A_ep_mean[i]/(1.*nevents_ep[i]) << "\t" << t_0_ep_mean[i]/(1.*nevents_ep[i]) << "\t" << alpha_ep_mean[i]/(1.*nevents_ep[i]) << "\t" << beta_ep_mean[i]/(1.*nevents_ep[i])<< endl;
   }

   txt_endcapp.close();

   char file_etaring_endcapm[100];
   sprintf (file_etaring_endcapm, "%d_etaring_endcapm.txt", year);

   ofstream txt_endcapm;
   txt_endcapm.open (file_etaring_endcapm);

   for(int i=0; i<40; i++){
      txt_endcapm << 11+i << "\t" << A_em_mean[i]/(1.*nevents_em[i]) << "\t" << t_0_em_mean[i]/(1.*nevents_em[i]) << "\t" << alpha_em_mean[i]/(1.*nevents_em[i]) << "\t" << beta_em_mean[i]/(1.*nevents_em[i])<< endl;
   }

   txt_endcapm.close();

}
