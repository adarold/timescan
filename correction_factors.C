#include "Riostream.h"
#include "TF1.h"

//This programs calculates the correction factors for the normalisation of the time shape
//linked to the different integrating regions. Such factors are then exploited in the
//creation of the rootuples in order to get the correct scale factor. 
//
//This process must be implemented twice: the first time using the parameters for the
//analytical function obtained from the fit of the "discontinuous" function. The program
//must then be run again with the parameters obtained from the fit of the corrected
//distribution with the first computed correction factors. 


double alphabeta(double* x, double*p){

   double fitval;

   if( ( 1+(x[0]-p[1]) / (p[2]*p[3]) ) > 0. ){
      fitval = p[0] * pow(1 + (x[0]-p[1]) / (p[2]*p[3]) , p[2] ) * exp ( - (x[0]-p[1]) / p[3]);
   }else{
      fitval = 0;
   }

   return fitval;

}


void correction_factors(string path, int year) {

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
   char file_meanpar[100];
   sprintf (file_meanpar, "%s/standard_%d_meanpar.txt", (path).c_str(), year);

   ifstream txt_mean;
   txt_mean.open(file_meanpar);

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


   TF1 *function_alphabeta_b = new TF1("function_alphabeta_b", alphabeta, -11, 236, 4);
   function_alphabeta_b->FixParameter(0,A_b);
   function_alphabeta_b->FixParameter(1,t_0_b);
   function_alphabeta_b->FixParameter(2,alpha_b);
   function_alphabeta_b->FixParameter(3,beta_b);


   TF1 *function_alphabeta_e = new TF1("function_alphabeta_e", alphabeta, -11, 236, 4);
   function_alphabeta_e->FixParameter(0,A_e);
   function_alphabeta_e->FixParameter(1,t_0_e);
   function_alphabeta_e->FixParameter(2,alpha_e);
   function_alphabeta_e->FixParameter(3,beta_e);


   float base_b=function_alphabeta_b->Integral(0,225);
   float base_e=function_alphabeta_e->Integral(0,225);

   float correction_b[23];
   float correction_e[23];

   for(int i=0; i<23; i++){
    
      correction_b[i]= function_alphabeta_b->Integral(-11+i, 214+i) / base_b;
      correction_e[i]= function_alphabeta_e->Integral(-11+i, 214+i) / base_e;
      
   }

   char file_corr_barrel[100];
   sprintf (file_corr_barrel, "%d_corrections_barrel.txt", year);

   ofstream txt_file1;
   txt_file1.open (file_corr_barrel);

   for(int i=0; i<23; i++){
      txt_file1 << correction_b[i] << endl;
   }

   txt_file1.close();

   char file_corr_endcap[100];
   sprintf (file_corr_endcap, "%d_corrections_endcap.txt", year);

   ofstream txt_file2;
   txt_file2.open (file_corr_endcap);

   for(int i=0; i<23; i++){
      txt_file2 << correction_e[i] << endl;
   }

   txt_file2.close();

}
