#define fitter_cxx
#include "fitter.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "Riostream.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TMath.h"
#include "TLine.h"

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

void fitter::Loop(int year, int run_condition)
{
   if (fChain == 0) return;

   //###############################################################################################################################
   //#                                                                                                                            ##
   //#                                                     RUNNING OPTIONS                                                        ##
   //#                                                                                                                            ##
   //# Fitting can be done in different ways:                                                                                     ##
   //# standard     = starting fit parameters are set manually at a certain value                                                 ##
   //# etaring      = starting fit parameters are set exploting external files with etaring parameters                            ##
   //# etaring_fix  = alpha and beta parameters are FIXED according to external files with etaring parameters                     ##
   //# producer     = starting fit parameters are set exploting external files with parameters divided by crystal producer        ##
   //# producer_fix = alpha and beta parameters are FIXED exploting external files with parameters divided by crystal producer    ##
   //# triggertower_fix = alpha and beta parameters are FIXED exploting external files with parameters divided by trigger towers  ## 
   //#                                                                                                                            ##
   //###############################################################################################################################

   string setting = "standard";
   if (run_condition == 1) setting = "etaring";
   if (run_condition == 2) setting = "etaring_fix";
   if (run_condition == 3) setting = "supermodule_fix";
   if (run_condition == 4) setting = "triggertower_fix";

cout << year << " " << setting << endl;

   //Variables
   int  n_delays=23;
   int  rawid_value[80000];
   int  npt=0;
   int  nchi=0;
   char path[100];
   char title[100];

   //Preliminary operations

   //Creates the map and the struct
   
   struct values {float val_x[230]; float val_y[230]; float val_ex[230]; float val_ey[230];};
   
   map<int, values> overall;
   

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      if(nevents>1){ //If the condition is not satisfied, the histogram would be filled with points where there is not
                     //enough statistics and therefore the fitting procedure is biassed
      values v_val;

      for(int i=0; i<230; i++){

         v_val.val_x[i]=0.;
         v_val.val_ex[i]=0.;
         v_val.val_y[i]=0.;
         v_val.val_ey[i]=0.;

      }

      for(int i=0; i<230; i++){

         v_val.val_x[i]=overall[rawid].val_x[i];
         v_val.val_ex[i]=overall[rawid].val_ex[i];
         v_val.val_y[i]=overall[rawid].val_y[i];
         v_val.val_ey[i]=overall[rawid].val_ey[i];

      }

      overall.erase(rawid);

      for(int i=0; i<10; i++){

         v_val.val_x[i*n_delays+(delay+11)]=25*i-delay;
         v_val.val_ex[i*n_delays+(delay+11)]=0;
         v_val.val_y[i*n_delays+(delay+11)]=pulse[i];
         v_val.val_ey[i*n_delays+(delay+11)]=square[i];

      }

      overall[rawid] = v_val;

      }//QUESTA
 
   }

   gStyle->SetOptFit(0);

   //Open txt file with crystals rawid and saves it into a vector
   ifstream txt_rawid;
   
   txt_rawid.open("rawid_list.txt");

   while ( kTRUE ) {
      txt_rawid >> rawid_value[npt];
      if ( ! txt_rawid.good() ) break;
      npt++;
   }

   txt_rawid.close();


   //Open txt file with mean fit parameters for eta rings
   int eta[171];
   float A_b_etaring[171];
   float t_0_b_etaring[171];
   float alpha_b_etaring[171];
   float beta_b_etaring[171];
   int eta_npt=0;

   char file_etaring_barrel[100];
   sprintf (file_etaring_barrel, "%d_etaring_barrel.txt", year);

   ifstream txt_etabarrel;
   txt_etabarrel.open(file_etaring_barrel);

   while ( kTRUE ) {
      txt_etabarrel >> eta[eta_npt] >> A_b_etaring[eta_npt] >> t_0_b_etaring[eta_npt] >> alpha_b_etaring[eta_npt] >> beta_b_etaring[eta_npt];
      if ( ! txt_etabarrel.good() ) break;
      eta_npt++;
   }

   txt_etabarrel.close();


   int radius_p[40];
   float A_ep_etaring[40];
   float t_0_ep_etaring[40];
   float alpha_ep_etaring[40];
   float beta_ep_etaring[40];
   int etaep_npt=0;

   char file_etaring_endcapp[100];
   sprintf (file_etaring_endcapp, "%d_etaring_endcapp.txt", year);

   ifstream txt_etaendcapp;
   txt_etaendcapp.open(file_etaring_endcapp);

   while ( kTRUE ) {
      txt_etaendcapp >> radius_p[etaep_npt] >> A_ep_etaring[etaep_npt] >> t_0_ep_etaring[etaep_npt] >> alpha_ep_etaring[etaep_npt] >> beta_ep_etaring[etaep_npt];
      if ( ! txt_etaendcapp.good() ) break;
      etaep_npt++;
   }

   txt_etaendcapp.close();


   int radius_m[40];
   float A_em_etaring[40];
   float t_0_em_etaring[40];
   float alpha_em_etaring[40];
   float beta_em_etaring[40];
   int etaem_npt=0;

   char file_etaring_endcapm[100];
   sprintf (file_etaring_endcapm, "%d_etaring_endcapm.txt", year);

   ifstream txt_etaendcapm;
   txt_etaendcapm.open(file_etaring_endcapm);

   while ( kTRUE ) {
      txt_etaendcapm >> radius_m[etaem_npt] >> A_em_etaring[etaem_npt] >> t_0_em_etaring[etaem_npt] >> alpha_em_etaring[etaem_npt] >> beta_em_etaring[etaem_npt];
      if ( ! txt_etaendcapm.good() ) break;
      etaem_npt++;
   }

   txt_etaendcapm.close();


   //Open the file with the parameters for crystal producers
   int prod_npt=0;
   int prod_rawid[npt];
   float prod_A[npt];
   float prod_t_0[npt];
   float prod_alpha[npt];
   float prod_beta[npt];

   char file_producer[100];
   sprintf (file_producer, "%d_producer_info.txt", year);

   ifstream txt_producer;
   txt_producer.open(file_producer);

   while ( kTRUE ) {
      txt_producer >> prod_rawid[prod_npt] >> prod_A[prod_npt] >> prod_t_0[prod_npt] >> prod_alpha[prod_npt] >> prod_beta[prod_npt];
      if ( ! txt_producer.good() ) break;
      prod_npt++;
   }

   txt_producer.close();

   struct valuesp {float valp[4];};
   map<int, valuesp> m_p;
   valuesp v_valp;

   for(int i=0; i<prod_npt; i++){

      v_valp.valp[0]=prod_A[i];
      v_valp.valp[1]=prod_t_0[i];
      v_valp.valp[2]=prod_alpha[i];
      v_valp.valp[3]=prod_beta[i];

      m_p[prod_rawid[i]]=v_valp;

   }


   //Open the file with the parameters for the supermodules
   int super_npt=0;
   int super_rawid[npt];
   float super_A[npt];
   float super_t_0[npt];
   float super_alpha[npt];
   float super_beta[npt];

   char file_supermodule[100];
   sprintf (file_supermodule, "%d_supermodule_info.txt", year);

   ifstream txt_supermod;
   txt_supermod.open(file_supermodule);

   while ( kTRUE ) {
      txt_supermod >> super_rawid[super_npt] >> super_A[super_npt] >> super_t_0[super_npt] >> super_alpha[super_npt] >> super_beta[super_npt];
      if ( ! txt_supermod.good() ) break;
      super_npt++;
   }

   txt_supermod.close();

   struct valuess {float vals[4];};
   map<int, valuess> m_s;
   valuess v_vals;

   for(int i=0; i<super_npt; i++){

      v_vals.vals[0]=super_A[i];
      v_vals.vals[1]=super_t_0[i];
      v_vals.vals[2]=super_alpha[i];
      v_vals.vals[3]=super_beta[i];

      m_s[super_rawid[i]]=v_vals;

   }
   

   //Open the file with the parameters for the trigger towers
   int tt_npt=0;
   int tt_rawid[npt];
   float tt_A[npt];
   float tt_t_0[npt];
   float tt_alpha[npt];
   float tt_beta[npt];

   char file_triggertower[100];
   sprintf (file_triggertower, "%d_triggertower_info.txt", year);

   ifstream txt_tt;
   txt_tt.open(file_triggertower);

   while ( kTRUE ) {
      txt_tt >> tt_rawid[tt_npt] >> tt_A[tt_npt] >> tt_t_0[tt_npt] >> tt_alpha[tt_npt] >> tt_beta[tt_npt];
      if ( ! txt_tt.good() ) break;
      tt_npt++;
   }

   txt_tt.close();

   struct valuest {float valt[4];};
   map<int, valuest> m_t;
   valuest v_valt;

   for(int i=0; i<tt_npt; i++){

      v_valt.valt[0]=tt_A[i];
      v_valt.valt[1]=tt_t_0[i];
      v_valt.valt[2]=tt_alpha[i];
      v_valt.valt[3]=tt_beta[i];

      m_t[tt_rawid[i]]=v_valt;

   }

   //Establishes the number of barrel and endcaps crystals
   int n_barrel=0;
   int n_endcap=0;
   for(int i=0; i<npt; i++){
      
      if(rawid_value[i]<839000000){
         n_barrel++;
      }else{
         n_endcap++;
      }

   }


   //Define variables into which save the fit parameter values
   float A[npt];
   float t_0[npt];
   float alpha[npt];
   float beta[npt];
   float A_error[npt];
   float t_0_error[npt];
   float alpha_error[npt];
   float beta_error[npt];
   float chi[npt];
   int cry_x[npt]; //In the barrel corresponds to the crystal iphi and in the endcap to the crystal ix
   int cry_y[npt]; //In the barrel corresponds to the crystal ieta and in the endcap to the crystal iy
   int cry_z[npt];
   float chi_tomean[npt];
   float chi_tomean_inout[npt];
   int fitStatus=0;
   float tot_events=0.;
   int bi=0;
   int black_list[npt];
   double xx;
   double yy;
   double A_mean=0;
   double t_0_mean=0;
   double t_0_err=0;
   double alpha_mean=0;
   double alpha_err=0;
   double beta_mean=0;
   double beta_err=0;
   int nstep=40;
   double t_0_step=0;
   double alpha_step=0;
   double beta_step=0;
   int nu=0;
   int index=0;
   float calc_radius=0;


   TH2F * h_nevents_b = new TH2F("h_nevents_b","h_nevents_b",360,0,360,86*2,-86,86);
   TH2F * h_nevents_em = new TH2F("h_nevents_em","h_nevents_em",100,0,100,100,0,100);
   TH2F * h_nevents_ep = new TH2F("h_nevents_ep","h_nevents_ep",100,0,100,100,0,100);

int nerrors=0;

   //Loop on all crystals to fit all the distributions and get the parameters
   for(int i=0; i<npt; i++){
   //for(int i=55000; i<npt; i++){

      //Retrieves crystal coordinates
      if(rawid_value[i]<839000000){
         cry_x[i]=iphi(rawid_value[i]);
         cry_y[i]=ieta(rawid_value[i]);
         cry_z[i]=zside(rawid_value[i]);
      }else{
         cry_x[i]=ix(rawid_value[i]);
         cry_y[i]=iy(rawid_value[i]);
         cry_z[i]=iz(rawid_value[i]);
      }

      //Define TGraph with all the shape points
      TGraphErrors * g = new TGraphErrors(10*n_delays, overall[rawid_value[i]].val_x, overall[rawid_value[i]].val_y, overall[rawid_value[i]].val_ex, overall[rawid_value[i]].val_ey);

      //Check how many empty points (scan delay for which no data are recorded) are there for each crystal
      nevents=0; //reset variable

      for(int l=114; l<=136; l++){
         g->GetPoint(l, xx, yy);
         if(yy>0.01) nevents++;
      }


      if(rawid_value[i]<839000000){
         h_nevents_b->Fill(cry_x[i], cry_y[i], nevents); 
      }else{
         if(cry_z[i]==-1){
         h_nevents_em->Fill(cry_x[i], cry_y[i], nevents); 
         }else{
         h_nevents_ep->Fill(cry_x[i], cry_y[i], nevents); 
         }
      }

      //Request at least two different time scans (so at least 20 points) to fit the shape. This prevents the situation in which
      //there are just 10 points which could be profoundly different from the nominal situation.
      if(nevents<2) continue;  

      //Construct and inizialise alpha-beta function and its parameters
      TF1 *function_alphabeta = new TF1("function_alphabeta", alphabeta, -11, 236, 4);

         function_alphabeta->SetParName(0, "A");
         function_alphabeta->SetParName(1, "t_0");
         function_alphabeta->SetParName(2, "alpha");
         function_alphabeta->SetParName(3, "beta");
   
         if(rawid_value[i]<839000000){

            if(setting=="standard"){

               function_alphabeta->SetParameter (0, 0.242);    // A
               function_alphabeta->SetParLimits (0, 0, 1);    // A
               function_alphabeta->SetParameter (1, 124.3);   // t_0    
               function_alphabeta->SetParameter (2, 1.66);   // alpha
               function_alphabeta->SetParameter (3, 40.6);   // beta

            }

            index=cry_y[i]+85;

            if(setting=="etaring"){

               function_alphabeta->SetParameter (0, A_b_etaring[index]);
               function_alphabeta->SetParLimits (0, 0, 1);
               function_alphabeta->SetParameter (1, t_0_b_etaring[index]);
               function_alphabeta->SetParameter (2, alpha_b_etaring[index]);
               function_alphabeta->SetParameter (3, beta_b_etaring[index]);

            }

            if(setting=="etaring_fix"){

               function_alphabeta->SetParameter (0, A_b_etaring[index]);
               function_alphabeta->SetParLimits (0, 0, 1);
               function_alphabeta->SetParameter (1, t_0_b_etaring[index]);
               function_alphabeta->FixParameter (2, alpha_b_etaring[index]);
               function_alphabeta->FixParameter (3, beta_b_etaring[index]);

            }

         }else{

            if(setting=="standard"){

               function_alphabeta->SetParameter (0, 0.232);    // A
               function_alphabeta->SetParLimits (0, 0, 1);    // A
               function_alphabeta->SetParameter (1, 121.6);   // t_0    
               function_alphabeta->SetParameter (2, 1.45);   // alpha
               function_alphabeta->SetParameter (3, 37.8);   // beta

            }

            calc_radius=sqrt(TMath::Power(cry_x[i]-50.5,2)+TMath::Power(cry_y[i]-50.5,2));

            for(int kk=0; kk<40; kk++){

               if(calc_radius>radius_p[kk] && calc_radius<radius_p[kk]+1){

                  if(cry_z[i]==+1){

                     if(setting=="etaring"){
                  
                        function_alphabeta->SetParameter (0, A_ep_etaring[kk]);
                        function_alphabeta->SetParLimits (0, 0, 1);
                        function_alphabeta->SetParameter (1, t_0_ep_etaring[kk]);
                        function_alphabeta->SetParameter (2, alpha_ep_etaring[kk]);
                        function_alphabeta->SetParameter (3, beta_ep_etaring[kk]);

                     }

                     if(setting=="etaring_fix"){
                  
                        function_alphabeta->SetParameter (0, A_ep_etaring[kk]);
                        function_alphabeta->SetParLimits (0, 0, 1);
                        function_alphabeta->FixParameter (1, t_0_ep_etaring[kk]);
                        function_alphabeta->FixParameter (2, alpha_ep_etaring[kk]);
                        function_alphabeta->FixParameter (3, beta_ep_etaring[kk]);

                     }

                  }else{

                     if(setting=="etaring"){

                        function_alphabeta->SetParameter (0, A_em_etaring[kk]);
                        function_alphabeta->SetParLimits (0, 0, 1);
                        function_alphabeta->SetParameter (1, t_0_em_etaring[kk]);
                        function_alphabeta->SetParameter (2, alpha_em_etaring[kk]);
                        function_alphabeta->SetParameter (3, beta_em_etaring[kk]);

                     }

                     if(setting=="etaring_fix"){

                        function_alphabeta->SetParameter (0, A_em_etaring[kk]);
                        function_alphabeta->SetParLimits (0, 0, 1);
                        function_alphabeta->FixParameter (1, t_0_em_etaring[kk]);
                        function_alphabeta->FixParameter (2, alpha_em_etaring[kk]);
                        function_alphabeta->FixParameter (3, beta_em_etaring[kk]);

                     }

                  }

               }

            }

         }

         if(setting=="producer"){

            function_alphabeta->SetParameter (0, m_p[rawid_value[i]].valp[0]);
            function_alphabeta->SetParLimits (0, 0, 1);
            function_alphabeta->SetParameter (1, m_p[rawid_value[i]].valp[1]);
            function_alphabeta->SetParameter (2, m_p[rawid_value[i]].valp[2]);
            function_alphabeta->SetParameter (3, m_p[rawid_value[i]].valp[3]);

         }

         if(setting=="producer_fix"){

            function_alphabeta->SetParameter (0, m_p[rawid_value[i]].valp[0]);
            function_alphabeta->SetParLimits (0, 0, 1);
            function_alphabeta->FixParameter (1, m_p[rawid_value[i]].valp[1]);
            function_alphabeta->FixParameter (2, m_p[rawid_value[i]].valp[2]);
            function_alphabeta->FixParameter (3, m_p[rawid_value[i]].valp[3]);

         }

         if(setting=="supermodule_fix"){

            function_alphabeta->SetParameter (0, m_s[rawid_value[i]].vals[0]);
            function_alphabeta->FixParameter (1, m_s[rawid_value[i]].vals[1]);
            function_alphabeta->FixParameter (2, m_s[rawid_value[i]].vals[2]);
            function_alphabeta->FixParameter (3, m_s[rawid_value[i]].vals[3]);

         }

         if(setting=="triggertower_fix"){

            function_alphabeta->SetParameter (0, m_t[rawid_value[i]].valt[0]);
            function_alphabeta->FixParameter (1, m_t[rawid_value[i]].valt[1]);
            function_alphabeta->FixParameter (2, m_t[rawid_value[i]].valt[2]);
            function_alphabeta->FixParameter (3, m_t[rawid_value[i]].valt[3]);

         }



      g->Fit("function_alphabeta","RQM","",-11,236);
      fitStatus =  g->Fit("function_alphabeta","RQM","",-11,236); //Checks the result of the fit

      //2D parameter plots construction
      if(i == 7000 || i == 23456 || i == 70000) {

      nu++;
      A_mean=function_alphabeta->GetParameter(0);
      t_0_mean=function_alphabeta->GetParameter(1);
      t_0_err=function_alphabeta->GetParError(1);
      alpha_mean=function_alphabeta->GetParameter(2);
      alpha_err=function_alphabeta->GetParError(2);
      beta_mean=function_alphabeta->GetParameter(3);
      beta_err=function_alphabeta->GetParError(3);

      nstep=40;
      t_0_step=t_0_err/(0.5*nstep);
      alpha_step=alpha_err/(0.5*nstep);
      beta_step=beta_err/(0.5*nstep);

      TH2F * h_alpha_beta = new TH2F("h_alpha_beta","h_alpha_beta",nstep,alpha_mean-alpha_err,alpha_mean+alpha_err,nstep,beta_mean-beta_err,beta_mean+beta_err);
      TH2F * h_alpha_t_0 = new TH2F("h_alpha_t_0","h_alpha_t_0",nstep,alpha_mean-alpha_err,alpha_mean+alpha_err,nstep,t_0_mean-t_0_err,t_0_mean+t_0_err);
      TH2F * h_t_0_beta = new TH2F("h_t_0_beta","h_t_0_beta",nstep,t_0_mean-t_0_err,t_0_mean+t_0_err,nstep,beta_mean-beta_err,beta_mean+beta_err);

      for(int ii=0; ii<nstep; ii++){

         for(int jj=0; jj<nstep; jj++){

            TF1 *function_alpha_beta_temp = new TF1("function_alpha_beta_temp", alphabeta, -11, 236, 4);
            TF1 *function_alpha_t_0_temp = new TF1("function_alpha_t_0_temp", alphabeta, -11, 236, 4);
            TF1 *function_t_0_beta_temp = new TF1("function_t_0_beta_temp", alphabeta, -11, 236, 4);

            function_alpha_beta_temp->FixParameter(0,A_mean);
            function_alpha_beta_temp->FixParameter(1,t_0_mean);
            function_alpha_beta_temp->FixParameter(2,(alpha_mean-alpha_err)+alpha_step*0.5+ii*alpha_step);
            function_alpha_beta_temp->FixParameter(3,(beta_mean-beta_err)+beta_step*0.5+jj*beta_step);

            function_alpha_t_0_temp->FixParameter(0,A_mean);
            function_alpha_t_0_temp->FixParameter(1,(t_0_mean-t_0_err)+t_0_step*0.5+jj*t_0_step);
            function_alpha_t_0_temp->FixParameter(2,(alpha_mean-alpha_err)+alpha_step*0.5+ii*alpha_step);
            function_alpha_t_0_temp->FixParameter(3,beta_mean);

            function_t_0_beta_temp->FixParameter(0,A_mean);
            function_t_0_beta_temp->FixParameter(1,(t_0_mean-t_0_err)+t_0_step*0.5+ii*t_0_step);
            function_t_0_beta_temp->FixParameter(2,alpha_mean);
            function_t_0_beta_temp->FixParameter(3,(beta_mean-beta_err)+beta_step*0.5+jj*beta_step);

            g->Fit("function_alpha_beta_temp","RQM","",-11,236);
            h_alpha_beta->Fill(function_alpha_beta_temp->GetParameter(2),function_alpha_beta_temp->GetParameter(3),function_alpha_beta_temp->GetChisquare());
            delete function_alpha_beta_temp;

            g->Fit("function_alpha_t_0_temp","RQM","",-11,236);
            h_alpha_t_0->Fill(function_alpha_t_0_temp->GetParameter(2),function_alpha_t_0_temp->GetParameter(1),function_alpha_t_0_temp->GetChisquare());
            delete function_alpha_t_0_temp;

            g->Fit("function_t_0_beta_temp","RQM","",-11,236);
            h_t_0_beta->Fill(function_t_0_beta_temp->GetParameter(1),function_t_0_beta_temp->GetParameter(3),function_t_0_beta_temp->GetChisquare());
            delete function_t_0_beta_temp;
 
         }
      }


      //Saves the graphs
      //gStyle->SetOptFit(0);

      TCanvas * c2 = new TCanvas("c2","c2",0,0,1500,500);
      c2->Divide(3,1);
      c2->cd(1);
      h_alpha_beta->SetTitle("Alpha vs Beta");
      h_alpha_beta->GetXaxis()->SetTitle("Alpha");
      h_alpha_beta->GetYaxis()->SetTitle("Beta");
      h_alpha_beta->Draw("COLZ");
      c2->cd(2);
      h_alpha_t_0->SetTitle("Alpha vs t_0");
      h_alpha_t_0->GetXaxis()->SetTitle("Alpha");
      h_alpha_t_0->GetYaxis()->SetTitle("t_{0}");
      h_alpha_t_0->Draw("COLZ");
      c2->cd(3);
      h_t_0_beta->SetTitle("t_0 vs Beta");
      h_t_0_beta->GetXaxis()->SetTitle("t_{0}");
      h_t_0_beta->GetYaxis()->SetTitle("Beta");
      h_t_0_beta->Draw("COLZ");
 
      sprintf (path, "/home/darold/html/shape/%d/%s_maps_%d.pdf", year, (setting).c_str(), rawid_value[i]);
      c2->SaveAs(path);

      delete h_alpha_beta;
      delete h_alpha_t_0;
      delete h_t_0_beta;
      delete c2;

      }

      //Check fit convergence and saves parameters. If the fit failed, an error message is displayed.
      if(fitStatus==4000){

         A[i] = function_alphabeta->GetParameter(0);
         t_0[i] = function_alphabeta->GetParameter(1);
         alpha[i] = function_alphabeta->GetParameter(2);
         beta[i] = function_alphabeta->GetParameter(3);
         chi[i] = (function_alphabeta->GetChisquare() / (1.0*function_alphabeta->GetNDF()) );

         A_error[i] = function_alphabeta->GetParError(0);
         t_0_error[i] = function_alphabeta->GetParError(1);
         alpha_error[i] = function_alphabeta->GetParError(2);
         beta_error[i] = function_alphabeta->GetParError(3);

      }else{ //Fit not converged, check if it happens because the tgraph is void

         for(int l=0; l<10*n_delays; l++){

            tot_events=tot_events + overall[rawid_value[i]].val_y[l];

         }
       
         //Save the rawid of the crystal where the fitting procedure failed into a black list
         black_list[bi]=rawid_value[i];

         tot_events=0;
         bi++;

      }

      delete g;
      delete function_alphabeta;

   }

   //Saves parameters for each crystal
   char file_everycrystal[100];
   sprintf (file_everycrystal, "%s_%d_everycrystal_par.txt", (setting).c_str(), year);

   ofstream txt_par;
   txt_par.open (file_everycrystal);

     // txt_par << "crystal rawid" << "\t" << " A " << "\t" << " t_0 " << "\t" << " alpha " << "\t" << " beta " << endl;

      for(int i=0; i<npt; i++){
         
         txt_par << rawid_value[i] << "\t" << A[i] << "\t" << t_0[i] << "\t" << alpha[i] << "\t" << beta[i] << endl;

      }

   txt_par.close();

   char file_everycrystal_err[100];
   sprintf (file_everycrystal_err, "%s_%d_everycrystal_par_err.txt", (setting).c_str(), year);

   ofstream txt_par2;
   txt_par2.open (file_everycrystal_err);

      for(int i=0; i<npt; i++){

         txt_par2 << rawid_value[i] << "\t" << A[i] << "\t" << A_error[i] << "\t" << t_0[i] << "\t" << t_0_error[i] << "\t" << alpha[i] << "\t" << alpha_error[i] << "\t" << beta[i] << "\t" << beta_error[i] << endl;

      }

   txt_par2.close();

   //Fills the 3d maps and the one dimensional graphs for barrel and endcaps parameters
   TH2F * h_b_chi = new TH2F("h_b_chi","h_b_chi",360,0,360,86*2,-86,86);
   TH2F * h_b_A = new TH2F("h_b_A","h_b_A",360,0,360,86*2,-86,86);
   TH2F * h_b_t_0 = new TH2F("h_b_t_0","h_b_t_0",360,0,360,86*2,-86,86);
   TH2F * h_b_alpha = new TH2F("h_b_alpha","h_b_alpha",360,0,360,86*2,-86,86);
   TH2F * h_b_beta = new TH2F("h_b_beta","h_b_beta",360,0,360,86*2,-86,86);

   TH2F * h_ep_chi = new TH2F("h_ep_chi","h_ep_chi",100,0,100,100,0,100);
   TH2F * h_ep_A = new TH2F("h_ep_A","h_ep_A",100,0,100,100,0,100);
   TH2F * h_ep_t_0 = new TH2F("h_ep_t_0","h_ep_t_0",100,0,100,100,0,100);
   TH2F * h_ep_alpha = new TH2F("h_ep_alpha","h_ep_alpha",100,0,100,100,0,100);
   TH2F * h_ep_beta = new TH2F("h_ep_beta","h_ep_beta",100,0,100,100,0,100);

   TH2F * h_em_chi = new TH2F("h_em_chi","h_em_chi",100,0,100,100,0,100);
   TH2F * h_em_A = new TH2F("h_em_A","h_em_A",100,0,100,100,0,100);
   TH2F * h_em_t_0 = new TH2F("h_em_t_0","h_em_t_0",100,0,100,100,0,100);
   TH2F * h_em_alpha = new TH2F("h_em_alpha","h_em_alpha",100,0,100,100,0,100);
   TH2F * h_em_beta = new TH2F("h_em_beta","h_em_beta",100,0,100,100,0,100);

   TH1F * h1_b_A = new TH1F("h1_b_A","h1_b_A",50,0.23,0.25);
   TH1F * h1_b_t_0 = new TH1F("h1_b_t_0","h1_b_t_0",50,115,135);
   TH1F * h1_b_alpha = new TH1F("h1_b_alpha","h1_b_alpha",50,1.,1.4);
   TH1F * h1_b_beta = new TH1F("h1_b_beta","h1_b_beta",50,38,44);
   TH1F * h1_b_chi = new TH1F("h1_b_chi","h1_b_chi",100,0,500);

   TH1F * h1_e_A = new TH1F("h1_e_A","h1_e_A",50,0.20,0.25);
   TH1F * h1_e_t_0 = new TH1F("h1_e_t_0","h1_e_t_0",50,110,130);
   TH1F * h1_e_alpha = new TH1F("h1_e_alpha","h1_e_alpha",50,1.1,2.);
   TH1F * h1_e_beta = new TH1F("h1_e_beta","h1_e_beta",50,34,45);
   TH1F * h1_e_chi = new TH1F("h1_e_chi","h1_e_chi",100,0,500);

   int  last_barrel_crystal=0; //Necessary for the endcap histograms
   bool black_list_test=1;

   //Variables for the determination of inner and outer pulse shapes in eta
   float b_in[4]={0};
   float b_out[4]={0};
   float e_in[4]={0};
   float e_out[4]={0};

   int n_b_in=0;
   int n_b_out=0;
   int n_e_in=0;
   int n_e_out=0;

   for(int i=0; i<npt; i++){

      //Do not include in the analysis the crystals that have not been fitted
      for(int l=0; l<bi; l++){
         
         if(rawid_value[i]==black_list[l]) black_list_test=0;

      }

      if(black_list_test==1){

         if(rawid_value[i]>839000000) break; //When the barrel crystals are finished, exits the cycle

         h_b_chi->Fill(cry_x[i], cry_y[i], chi[i]);
         h_b_A->Fill(cry_x[i], cry_y[i], A[i]);
         h_b_t_0->Fill(cry_x[i], cry_y[i], t_0[i]);
         h_b_alpha->Fill(cry_x[i], cry_y[i], alpha[i]);
         h_b_beta->Fill(cry_x[i], cry_y[i], beta[i]);

         //1D graphs of the fitted variables to obtain the values for the mean fit
         h1_b_A->Fill(A[i]); 
         h1_b_t_0->Fill(t_0[i]); 
         h1_b_alpha->Fill(alpha[i]); 
         h1_b_beta->Fill(beta[i]); 
         h1_b_chi->Fill(chi[i]);

         //Parameters for inner and outer eta fit
         if(cry_y[i]<40){
            b_in[0]=b_in[0]+A[i];
            b_in[1]=b_in[1]+t_0[i];
            b_in[2]=b_in[2]+alpha[i];
            b_in[3]=b_in[3]+beta[i];
            n_b_in++;
         }else{
            b_out[0]=b_out[0]+A[i];
            b_out[1]=b_out[1]+t_0[i];
            b_out[2]=b_out[2]+alpha[i];
            b_out[3]=b_out[3]+beta[i];
            n_b_out++;
         }

         last_barrel_crystal=i;

      }
 
      black_list_test=1;

   }

   for(int i=last_barrel_crystal+1; i<npt; i++){

      for(int l=0; l<bi; l++){

         if(rawid_value[i]==black_list[l]) black_list_test=0;

      }

      if(black_list_test==1){

         //1D graphs of the fitted variables to obtain the values for the mean fit
         h1_e_A->Fill(A[i]); 
         h1_e_t_0->Fill(t_0[i]); 
         h1_e_alpha->Fill(alpha[i]); 
         h1_e_beta->Fill(beta[i]); 
         h1_e_chi->Fill(chi[i]);
 
         //Parameters for inner and outer eta fit
         if( TMath::Power(cry_x[i]-50,2)+TMath::Power(cry_y[i]-50,2) < 25*25){
            e_in[0]=e_in[0]+A[i];
            e_in[1]=e_in[1]+t_0[i];
            e_in[2]=e_in[2]+alpha[i];
            e_in[3]=e_in[3]+beta[i];
            n_e_in++;
         }else{
            e_out[0]=e_out[0]+A[i];
            e_out[1]=e_out[1]+t_0[i];
            e_out[2]=e_out[2]+alpha[i];
            e_out[3]=e_out[3]+beta[i];
            n_e_out++;
         }

         if(cry_z[i]==-1){

            h_em_chi->Fill(cry_x[i], cry_y[i], chi[i]);
            h_em_A->Fill(cry_x[i], cry_y[i], A[i]);
            h_em_t_0->Fill(cry_x[i], cry_y[i], t_0[i]);
            h_em_alpha->Fill(cry_x[i], cry_y[i], alpha[i]);
            h_em_beta->Fill(cry_x[i], cry_y[i], beta[i]);         

         }else{

            h_ep_chi->Fill(cry_x[i], cry_y[i], chi[i]);
            h_ep_A->Fill(cry_x[i], cry_y[i], A[i]);
            h_ep_t_0->Fill(cry_x[i], cry_y[i], t_0[i]);
            h_ep_alpha->Fill(cry_x[i], cry_y[i], alpha[i]);
            h_ep_beta->Fill(cry_x[i], cry_y[i], beta[i]);

         }

      }

      black_list_test=1;

   }


   //Mean fit determination for barrel and endcaps and residual computation

   float A_b_mean=h1_b_A->GetMean();
   float t_0_b_mean=h1_b_t_0->GetMean();
   float alpha_b_mean=h1_b_alpha->GetMean();
   float beta_b_mean=h1_b_beta->GetMean();

   float A_e_mean=h1_e_A->GetMean();
   float t_0_e_mean=h1_e_t_0->GetMean();
   float alpha_e_mean=h1_e_alpha->GetMean();
   float beta_e_mean=h1_e_beta->GetMean();

   //Mean fit parameters determination for inner and outer eta values
   for(int i=0; i<4; i++){
   
      b_in[i]=b_in[i]/(n_b_in*1.+1.);
      b_out[i]=b_out[i]/(n_b_out*1.+1.);
      e_in[i]=e_in[i]/(n_e_in*1.);
      e_out[i]=e_out[i]/(n_e_out*1.);

   }
   
   //Saves the average fitting parameter to an external file
   char file_meanpar[100];
   sprintf (file_meanpar, "%s_%d_meanpar.txt", (setting).c_str(), year);

   ofstream txt_meanpar;
   txt_meanpar.open (file_meanpar);

      //Barrel
      txt_meanpar << A_b_mean << "\t" << t_0_b_mean << "\t" << alpha_b_mean << "\t" << beta_b_mean << endl;
      //Endcap
      txt_meanpar << A_e_mean << "\t" << t_0_e_mean << "\t" << alpha_e_mean << "\t" << beta_e_mean << endl;

   txt_meanpar.close();


   TF1 *f_barrel = new TF1("f_barrel", alphabeta, -11, 236, 4);
   TF1 *f_endcap = new TF1("f_endcap", alphabeta, -11, 236, 4);
   TF1 *f_endcap_in = new TF1("f_endcap_in", alphabeta, -11, 236, 4);
   TF1 *f_endcap_out = new TF1("f_endcap_out", alphabeta, -11, 236, 4);

   f_barrel->FixParameter(0,A_b_mean);
   f_barrel->FixParameter(1,t_0_b_mean);
   f_barrel->FixParameter(2,alpha_b_mean);
   f_barrel->FixParameter(3,beta_b_mean);

   f_endcap->FixParameter(0,A_e_mean);
   f_endcap->FixParameter(1,t_0_e_mean);
   f_endcap->FixParameter(2,alpha_e_mean);
   f_endcap->FixParameter(3,beta_e_mean);

   f_endcap_in->FixParameter(0,e_in[0]);
   f_endcap_in->FixParameter(1,e_in[1]);
   f_endcap_in->FixParameter(2,e_in[2]);
   f_endcap_in->FixParameter(3,e_in[3]);

   f_endcap_out->FixParameter(0,e_out[0]);
   f_endcap_out->FixParameter(1,e_out[1]);
   f_endcap_out->FixParameter(2,e_out[2]);
   f_endcap_out->FixParameter(3,e_out[3]);

   //To get the chi^2 for each crystal from the mean value fit, the exact same procedure as before is implemented. However,
   //this time the parameters of the fitting function are fixed and hence no real fit is performed. 
   for(int i=0; i<npt; i++){

      for(int l=0; l<bi; l++){

         if(rawid_value[i]==black_list[l]) black_list_test=0;

      }

      if(black_list_test==1){


         TGraphErrors * g = new TGraphErrors(10*n_delays, overall[rawid_value[i]].val_x, overall[rawid_value[i]].val_y, overall[rawid_value[i]].val_ex, overall[rawid_value[i]].val_ey);

         if(rawid_value[i]<839000000){

            g->Fit("f_barrel","RQM","",-11,236);
            chi_tomean[i] = (f_barrel->GetChisquare() / (1.0*f_barrel->GetNDF()-4.) );

         }else{

            g->Fit("f_endcap","RQM","",-11,236);
            chi_tomean[i] = (f_endcap->GetChisquare() / (1.0*f_endcap->GetNDF()-4.) );

            if( TMath::Power(cry_x[i]-50,2)+TMath::Power(cry_y[i]-50,2) < 25*25){
                
               g->Fit("f_endcap_in","RQM","",-11,236);
               chi_tomean_inout[i] = (f_endcap_in->GetChisquare() / (1.0*f_endcap_in->GetNDF()-4.) );

            }else{

               g->Fit("f_endcap_out","RQM","",-11,236);
               chi_tomean_inout[i] = (f_endcap_out->GetChisquare() / (1.0*f_endcap_out->GetNDF()-4.) );

            }

         }

         delete g;
     
      }

      black_list_test=1;
      
   }


   //2D maps for the chi^2 values for each crystal shape compared to the mean fit
   TH2F * h_b_chi_tomean = new TH2F("h_b_chi_tomean","h_b_chi_tomean",360,0,360,86*2,-86,86);
   TH2F * h_ep_chi_tomean = new TH2F("h_ep_chi_tomean","h_ep_chi_tomean",100,0,100,100,0,100);
   TH2F * h_em_chi_tomean = new TH2F("h_em_chi_tomean","h_em_chi_tomean",100,0,100,100,0,100);
   TH2F * h_ep_chi_tomean_inout = new TH2F("h_ep_chi_tomean_inout","h_ep_chi_tomean_inout",100,0,100,100,0,100);
   TH2F * h_em_chi_tomean_inout = new TH2F("h_em_chi_tomean_inout","h_em_chi_tomean_inout",100,0,100,100,0,100);

   last_barrel_crystal=0; //Necessary for the endcap histograms

   for(int i=0; i<npt; i++){

      for(int l=0; l<bi; l++){

         if(rawid_value[i]==black_list[l]) black_list_test=0;

      }

      if(black_list_test==1){

         if(rawid_value[i]>839000000) break; //When the barrel crystals are finished, exits the cycle

         h_b_chi_tomean->Fill(cry_x[i], cry_y[i], chi_tomean[i]);

         last_barrel_crystal=i;

      }

      black_list_test=1;   

   }

   for(int i=last_barrel_crystal+1; i<npt; i++){

      for(int l=0; l<bi; l++){

         if(rawid_value[i]==black_list[l]) black_list_test=0;

      }

if(black_list_test==0) cout << rawid_value[i] << endl;
      if(black_list_test==1){

         if(cry_z[i]==-1){

            h_em_chi_tomean->Fill(cry_x[i], cry_y[i], chi_tomean[i]);
            h_em_chi_tomean_inout->Fill(cry_x[i], cry_y[i], chi_tomean_inout[i]);

         }else{

            h_ep_chi_tomean->Fill(cry_x[i], cry_y[i], chi_tomean[i]);
            h_ep_chi_tomean_inout->Fill(cry_x[i], cry_y[i], chi_tomean_inout[i]);

         }

      }

      black_list_test=1;

   }

  
   //Saves histograms
   char file_histograms[100];
   sprintf (file_histograms, "%s_%d_histograms.root", (setting).c_str(), year);

   TFile *f = new TFile(file_histograms,"update");

      h1_b_chi->Write("",TObject::kOverwrite);
      h1_b_A->Write("",TObject::kOverwrite);
      h1_b_t_0->Write("",TObject::kOverwrite);
      h1_b_alpha->Write("",TObject::kOverwrite);
      h1_b_beta->Write("",TObject::kOverwrite);

      h1_e_chi->Write("",TObject::kOverwrite);
      h1_e_A->Write("",TObject::kOverwrite);
      h1_e_t_0->Write("",TObject::kOverwrite);
      h1_e_alpha->Write("",TObject::kOverwrite);
      h1_e_beta->Write("",TObject::kOverwrite);

      h_b_chi->Write("",TObject::kOverwrite);
      h_b_A->Write("",TObject::kOverwrite);
      h_b_t_0->Write("",TObject::kOverwrite);
      h_b_alpha->Write("",TObject::kOverwrite);
      h_b_beta->Write("",TObject::kOverwrite);

      h_em_chi->Write("",TObject::kOverwrite);
      h_em_A->Write("",TObject::kOverwrite);
      h_em_t_0->Write("",TObject::kOverwrite);
      h_em_alpha->Write("",TObject::kOverwrite);
      h_em_beta->Write("",TObject::kOverwrite);

      h_ep_chi->Write("",TObject::kOverwrite);
      h_ep_A->Write("",TObject::kOverwrite);
      h_ep_t_0->Write("",TObject::kOverwrite);
      h_ep_alpha->Write("",TObject::kOverwrite);
      h_ep_beta->Write("",TObject::kOverwrite);

      h_b_chi_tomean->Write("",TObject::kOverwrite);
      h_em_chi_tomean->Write("",TObject::kOverwrite);
      h_ep_chi_tomean->Write("",TObject::kOverwrite);
      h_em_chi_tomean_inout->Write("",TObject::kOverwrite);
      h_ep_chi_tomean_inout->Write("",TObject::kOverwrite);

      h_nevents_b->Write("",TObject::kOverwrite);
      h_nevents_em->Write("",TObject::kOverwrite);
      h_nevents_ep->Write("",TObject::kOverwrite);

   f->Close();
   
}
