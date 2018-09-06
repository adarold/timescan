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


void triggertower(){

   int rawid_value[80000];
   float A[80000];
   float t_0[80000];
   float alpha[80000];
   float beta[80000];
  
   int rawid_e[80000], dbID_e[80000], hashedId_e[80000], side_e[80000], ix_e[80000], iy_e[80000], SC_e[80000], iSC_e[80000], Fed_e[80000], TT_e[80000], strip_e[80000], Xtal_e[80000], quadrant_e[80000];
   char EE_e[10];

   int npt=0;
   int nptb=0;
   int npte=0;


   //Reads fitted parameters for all the crystals
   ifstream txt_par;
   txt_par.open("everycrystal_par_OK.txt");

   while ( kTRUE ) {
      txt_par >> rawid_value[npt] >> A[npt] >> t_0[npt] >> alpha[npt] >> beta[npt];
      if ( ! txt_par.good() ) break;
      npt++;
   }

   txt_par.close();

   ifstream txt_proe;
   txt_proe.open("EEtot.txt");

   while ( kTRUE ) {
      txt_proe >> rawid_e[npte] >> dbID_e[npte] >> hashedId_e[npte] >> side_e[npte] >> ix_e[npte] >> iy_e[npte] >> SC_e[npte] >> iSC_e[npte] >> Fed_e[npte] >> EE_e >> TT_e[npte] >> strip_e[npte] >> Xtal_e[npte] >> quadrant_e[npte];

      if ( ! txt_proe.good() ) break;
      npte++;
   }

   txt_proe.close();

   struct valuese {float vale[1];};
   map<int, valuese> m_e;
   valuese v_vale;

   int key_e=0;

   for(int i=0; i<npte; i++){

      //There is one key corresponding to one TT
      if(Fed_e[i]==648){

         key_e=TT_e[i]*1000000 +Fed_e[i];

      }else{

         key_e=0;

      }

      v_vale.vale[0]=key_e;

      //I have a map that for a crystal rawid links a TT
      m_e[rawid_e[i]]=v_vale;

   }

   //This map is the one of the TT
   struct valuesf {float valf[5];};
   map<int, valuesf> m_f;
   valuesf v_valf;

   int key_ret=0;

   for(int i=0; i<npt; i++){

      if(rawid_value[i]<839000000){

      }else{

         //I get the key for my crystal, i.e. I know which TT it belongs to
         key_ret=m_e[rawid_value[i]].vale[0];

         v_valf.valf[0]=m_f[key_ret].valf[0]+1;
         v_valf.valf[1]=m_f[key_ret].valf[1]+A[i];
         v_valf.valf[2]=m_f[key_ret].valf[2]+t_0[i];
         v_valf.valf[3]=m_f[key_ret].valf[3]+alpha[i];
         v_valf.valf[4]=m_f[key_ret].valf[4]+beta[i];

         m_f[key_ret]=v_valf;

      }

      key_ret=0;

   }


   //Saves parameters

   ofstream txt_barrel;
   txt_barrel.open ("triggertower_info.txt");

   for(int i=0; i<npt; i++){

      if(rawid_value[i]<839000000){

      }else{

         key_ret=m_e[rawid_value[i]].vale[0];

            txt_barrel << rawid_value[i] << "\t" << m_f[key_ret].valf[1]/m_f[key_ret].valf[0] << "\t" << m_f[key_ret].valf[2]/m_f[key_ret].valf[0] << "\t" << m_f[key_ret].valf[3]/m_f[key_ret].valf[0] << "\t" << m_f[key_ret].valf[4]/m_f[key_ret].valf[0] << endl;

      }

   }

   txt_barrel.close();


}
