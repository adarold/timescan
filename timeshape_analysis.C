#define timeshape_analysis_cxx
#include "timeshape_analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "Riostream.h"
#include "TGraph.h"
#include <unistd.h>
#include "TLegend.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TMath.h"
#include "TF1.h"

int zside(int id) { return (id&0x10000)?(1):(-1); }
int ietaAbs(int id) { return (id>>9)&0x7F; }
int ieta(int id) { return zside(id)*ietaAbs(id); }
int iphi(int id) { return id&0x1FF; }

int iz(int id) { return (id&0x4000)?(1):(-1); }
int ix(int id) { return (id>>7)&0x7F; }
int iy(int id) { return id&0x7F; }

double alphabeta(double* x, double*p){

   double fitval;

   if( ( 1+(x[0]-p[1]) / (p[2]*p[3]) ) > 0. ){
      fitval = p[0] * pow(1 + (x[0]-p[1]) / (p[2]*p[3]) , p[2] ) * exp ( - (x[0]-p[1]) / p[3]);
   }else{
      fitval = 0;
   }

   return fitval;

}

void timeshape_analysis::Loop(int year, int correction)
{

   if (fChain == 0) return;

cout << year << " " << correction << endl;
   
   //Branches
   fChain->SetBranchStatus("*",0);
   fChain->SetBranchStatus("lumisec",1);
   fChain->SetBranchStatus("run",1);
   fChain->SetBranchStatus("rawid",1);
   fChain->SetBranchStatus("pulse",1);
   fChain->SetBranchStatus("barrel",1);
   fChain->SetBranchStatus("ene",1);
   fChain->SetBranchStatus("time",1);


   //External input options
   //string setting="nocorr"; //if nocorr, no correction on the integral normalisation is applied, do this for the first run
   string setting="nocorr"; 
   if (correction == 1) setting="corr"; 


   //Histograms
   TH1F * h_b_ene = new TH1F("h_b_ene","h_b_ene",300,0,50);
   TH1F * h_e_ene = new TH1F("h_e_ene","h_e_ene",300,0,50);
   TH1F * h_e_in_ene = new TH1F("h_e_in_ene","h_e_in_ene",300,0,50);
   TH1F * h_e_out_ene = new TH1F("h_e_out_ene","h_e_out_ene",300,0,50);


   //Variables
   int n=0;
   int npt=0;
   int delay=-99;
   int key=0;
   int key_retrieve=0;
   int rawid_value[80000];
   int sum=0;
   double correction_barrel[23]; //Equal to the number of delays
   double correction_endcap[23]; //Equal to the number of delays
   int cindex=0;
   int npt_cb=0;
   int npt_ce=0;
   float cry_x=0;
   float cry_y=0;

   //Preliminary operations
   
   //Creates the map and the struct

   struct values {float val[23];};

   map<int, values> overall;

   values v_val;


   //Reads the corrections on the normalization integral
   //Remove the variable correction[i] inside the filling cycle to get the raw data
   if(setting=="corr"){

      char file_corr_barrel[100];
      sprintf (file_corr_barrel, "%d_corrections_barrel.txt", year);
      
      ifstream txt_corrections_barrel;
      txt_corrections_barrel.open(file_corr_barrel);

      while ( kTRUE ) {
         txt_corrections_barrel >> correction_barrel[npt_cb];
         if ( ! txt_corrections_barrel.good() ) break;
         npt_cb++;
      }

      txt_corrections_barrel.close();

      char file_corr_endcap[100];
      sprintf (file_corr_barrel, "%d_corrections_endcap.txt", year);
      
      ifstream txt_corrections_endcap;
      txt_corrections_endcap.open(file_corr_barrel);

      while ( kTRUE ) {
         txt_corrections_endcap >> correction_endcap[npt_ce];
         if ( ! txt_corrections_endcap.good() ) break;
         npt_ce++;
      }

      txt_corrections_endcap.close();

   }
   

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

         //Main analysis
         //2017
            if(run==305745){

               if(lumisec>=5  && lumisec<=11){ delay=-11; cindex=0;}
               if(lumisec>=15 && lumisec<=19){ delay=-7;  cindex=4;}
               if(lumisec>=22 && lumisec<=26){ delay=-3;  cindex=8;}
               if(lumisec>=30 && lumisec<=34){ delay=+1;  cindex=12;}
               if(lumisec>=37 && lumisec<=41){ delay=+5;  cindex=16;}
               if(lumisec>=45 && lumisec<=49){ delay=+9;  cindex=20;}

            }

            if(run==305746){

               if(lumisec>=2  && lumisec<=8) { delay=-9;  cindex=2;}
               if(lumisec>=11 && lumisec<=15){ delay=-5;  cindex=6;}
               if(lumisec>=19 && lumisec<=22){ delay=-1;  cindex=10;}
               if(lumisec>=27 && lumisec<=30){ delay=+3;  cindex=14;}
               if(lumisec>=34 && lumisec<=38){ delay=+7;  cindex=18;}
               if(lumisec>=42 && lumisec<=45){ delay=+11; cindex=22;}

            }

            if(run==305751){ delay=-10; cindex=1;}
            if(run==305752){ delay=-8;  cindex=3;}
            if(run==305753){ delay=-6;  cindex=5;}
            if(run==305747){ delay=-4;  cindex=7;}
            if(run==305748){ delay=-2;  cindex=9;}

            if(run==305757){ delay=+0;  cindex=11;}

            if(run==305749){ delay=+2;  cindex=13;}
            if(run==305750){ delay=+4;  cindex=15;}
            if(run==305754){ delay=+6;  cindex=17;}
            if(run==305755){ delay=+8;  cindex=19;}
            if(run==305756){ delay=+10; cindex=21;}
         
         //20181
            if(run==318661){

               if(lumisec>=1   && lumisec<=12){ delay=-10; cindex=1;}
               if(lumisec>=16  && lumisec<=20){ delay=-8; cindex=3;}
               if(lumisec>=23  && lumisec<=27){ delay=-6; cindex=5;}
               if(lumisec>=31  && lumisec<=34){ delay=-4; cindex=7;}
               if(lumisec>=38  && lumisec<=42){ delay=-2; cindex=9;}
               if(lumisec>=45  && lumisec<=49){ delay=0; cindex=11;}
               if(lumisec>=53  && lumisec<=57){ delay=+2; cindex=13;}
               if(lumisec>=60  && lumisec<=65){ delay=+4; cindex=15;}
               if(lumisec>=67  && lumisec<=71){ delay=+6; cindex=17;}
               if(lumisec>=75  && lumisec<=79){ delay=+8; cindex=19;}
               if(lumisec>=82  && lumisec<=86){ delay=+10; cindex=21;}

            }

            if(run==318662){

               if(lumisec>=1   && lumisec<=6 ){ delay=-11; cindex=0;}
               if(lumisec>=13  && lumisec<=15){ delay=-9; cindex=2;}
               if(lumisec>=19  && lumisec<=23){ delay=-7; cindex=4;}
               if(lumisec>=26  && lumisec<=30){ delay=-5; cindex=6;}
               if(lumisec>=33  && lumisec<=37){ delay=-3; cindex=8;}
               if(lumisec>=41  && lumisec<=47){ delay=-1; cindex=10;}

            }

            if(run==318663){

               if(lumisec>=1   && lumisec<=6 ){ delay=-1; cindex=10;}
               if(lumisec>=12  && lumisec<=15){ delay=+1; cindex=12;}
               if(lumisec>=19  && lumisec<=23){ delay=+3; cindex=14;}
               if(lumisec>=27  && lumisec<=32){ delay=+5; cindex=16;}
               if(lumisec>=36  && lumisec<=40){ delay=+7; cindex=18;}
               if(lumisec>=43  && lumisec<=48){ delay=+9; cindex=20;}
               if(lumisec>=52  && lumisec<=56){ delay=+11; cindex=22;}

            }

            //20182_test

            if(run==323395){

               if(lumisec>=55 && lumisec<=61 && time>-8.){ delay=+2;  cindex=13;}
               if(lumisec>=64 && lumisec<=71 && time>-8.){ delay=+4;  cindex=15;}
               if(lumisec>=75 && lumisec<=82 && time>-8.){ delay=+6;  cindex=17;}
               if(lumisec>=85 && lumisec<=91 && time>-8.){ delay=+8;  cindex=19;}
               if(lumisec>=95 && lumisec<=102 && time>-8.){ delay=+10;  cindex=21;}

            }

            if(run==323398){

               if(lumisec>=35  && lumisec<=40 && time>-8.) { delay=+1; cindex=12;}
               if(lumisec>=43  && lumisec<=50 && time>-8.) { delay=+3; cindex=14;}
               if(lumisec>=54  && lumisec<=61 && time>-8.) { delay=+5; cindex=16;}
               if(lumisec>=64  && lumisec<=71 && time>-8.) { delay=+7; cindex=18;}
               if(lumisec>=75  && lumisec<=81 && time>-8.) { delay=+9; cindex=20;}
               if(lumisec>=84  && lumisec<=92 && time>-8.) { delay=+11; cindex=22;}

            }

            if(run==323395){

               if(lumisec>=2  && lumisec<=6 && time<8) { delay=-10; cindex=1;}
               if(lumisec>=11 && lumisec<=17 && time<8){ delay=-8;  cindex=3;}
               if(lumisec>=21 && lumisec<=27 && time<8){ delay=-6;  cindex=5;}
               if(lumisec>=31 && lumisec<=37 && time<8){ delay=-4;  cindex=7;}
               if(lumisec>=40 && lumisec<=45 && time<8){ delay=-2;  cindex=9;}

            }

            if(run==323397){

               if(lumisec>=1  && lumisec<=7 && time<8) { delay=-11; cindex=0;}
               if(lumisec>=11 && lumisec<=17 && time<8){ delay=-9;  cindex=2;}
               if(lumisec>=21 && lumisec<=28 && time<8){ delay=-7;  cindex=4;}

            }

            if(run==323398){

                if(lumisec>=10  && lumisec<=15 && time<8) { delay=-5; cindex=6;}
                if(lumisec>=19  && lumisec<=23 && time<8) { delay=-3; cindex=8;}
                if(lumisec>=27  && lumisec<=32 && time<8) { delay=-1; cindex=10;}

            }

 
         if(delay==-99) continue; //Allows to skip the lumisections where the delay is not defined

         //Defines the key for the map: each key refers to a single crystal and a single scan delay
         if(rawid<839000000){
            key=( (rawid-838000000) *1000) + (barrel*100) + (delay+11);
         }else{
            key=( (rawid-872000000) *1000) + (delay+11);  
         }

         v_val.val[20] = delay;
         v_val.val[21] = rawid;
         v_val.val[22] = overall[key].val[22]+1;

         sum=0;
         for(int i=0; i<10; i++){
            sum=sum+pulse[i];
         }

         if(setting=="nocorr"){
            correction_barrel[cindex]=1.0;
            correction_endcap[cindex]=1.0;
         }

         for(int i=0; i<10; i++){

            if(rawid<839000000){
               v_val.val[i]=overall[key].val[i] + pulse[i]/(sum*correction_barrel[cindex]);
               v_val.val[i+10]=overall[key].val[i+10] +  TMath::Power(pulse[i]/(sum*correction_barrel[cindex]),2);
            }else{
               v_val.val[i]=overall[key].val[i] + pulse[i]/(sum*correction_endcap[cindex]);
               v_val.val[i+10]=overall[key].val[i+10] +  TMath::Power(pulse[i]/(sum*correction_endcap[cindex]),2);
            }

         }

         overall[key]=v_val;

         delay=-99;

   }


   //Open txt file with crystals rawid and saves it into a vector
   ifstream txt_rawid;
   txt_rawid.open("rawid_list.txt");

   while ( kTRUE ) {
      txt_rawid >> rawid_value[npt];
      if ( ! txt_rawid.good() ) break;
      npt++;
   }

   txt_rawid.close();


   //Writes all the information of the map into a root tree file
   char filename[100];
   sprintf (filename, "event_%d.root", year);

   TFile * file = new TFile(filename,"recreate");
   TTree * tree = new TTree("tree","tree");

   int t_rawid=0;
   int t_delay=0;
   int t_nevents=0;
   double t_pulse[10];
   double t_square[10];

   for(int i=0; i<10; i++){
      t_pulse[i]=0;
      t_square[i]=0;
   }

   tree->Branch("rawid",&t_rawid,"t_rawid/I");
   tree->Branch("delay",&t_delay,"t_delay/I");
   tree->Branch("nevents",&t_nevents,"t_nevents/I");
   tree->Branch("pulse",t_pulse,"t_pulse[10]/D");
   tree->Branch("square",t_square,"t_square[10]/D");

   for(int k=0; k<npt; k++){ //Cycle on all crystals
      for(int i=0; i<23; i++){ //Cycle on all delays

         //Retreive the key
         if(rawid_value[k]<839000000){
            key_retrieve=( (rawid_value[k]-838000000) *1000) + 100 + i;
         }
         else{
            key_retrieve=( (rawid_value[k]-872000000) *1000) + i;
         }

         if(overall[key_retrieve].val[22]!=0){

            t_rawid = rawid_value[k];
            t_delay = overall[key_retrieve].val[20];
            t_nevents = overall[key_retrieve].val[22];

            for(int l=0; l<10; l++){

               t_pulse[l] = overall[key_retrieve].val[l]/(overall[key_retrieve].val[22]*1.0);
               t_square[l] = sqrt( overall[key_retrieve].val[l+10]/(overall[key_retrieve].val[22]*1.0) - TMath::Power( (overall[key_retrieve].val[l])/(overall[key_retrieve].val[22]*1.0),2)  );

            }

         tree->Fill();

         }

      }

   }

   tree->Write();
   file->Close();

}
