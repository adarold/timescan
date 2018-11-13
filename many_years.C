
double alphabeta(double* x, double*p){

   double fitval;

   if( ( 1+(x[0]-p[1]) / (p[2]*p[3]) ) > 0. ){
      fitval = p[0] * pow(1 + (x[0]-p[1]) / (p[2]*p[3]) , p[2] ) * exp ( - (x[0]-p[1]) / p[3]);
   }else{
      fitval = 0;
   }

   return fitval;

}

void many_years(string path){

  float b_2017[4];
  float e_2017[4];
  float b_20181[4];
  float e_20181[4];
  float b_20182[4];
  float e_20182[4];

  //Opens files with mean parameters for the different years
  char file_meanpar_2017[300];
  sprintf (file_meanpar_2017, "%s/etaring_2017_meanpar.txt", (path).c_str());
  ifstream meanpar_2017;
  meanpar_2017.open(file_meanpar_2017);
  meanpar_2017 >> b_2017[0] >> b_2017[1] >> b_2017[2] >> b_2017[3] >> e_2017[0] >> e_2017[1] >> e_2017[2] >> e_2017[3]; 
  meanpar_2017.close();

  char file_meanpar_20181[300];
  sprintf (file_meanpar_20181, "%s/etaring_20181_meanpar.txt", (path).c_str());
  ifstream meanpar_20181;
  meanpar_20181.open(file_meanpar_20181);
  meanpar_20181 >> b_20181[0] >> b_20181[1] >> b_20181[2] >> b_20181[3] >> e_20181[0] >> e_20181[1] >> e_20181[2] >> e_20181[3];
  meanpar_20181.close();

  char file_meanpar_20182[300];
  sprintf (file_meanpar_20182, "%s/etaring_20182_meanpar.txt", (path).c_str());
  ifstream meanpar_20182;
  meanpar_20182.open(file_meanpar_20182);
  meanpar_20182 >> b_20182[0] >> b_20182[1] >> b_20182[2] >> b_20182[3] >> e_20182[0] >> e_20182[1] >> e_20182[2] >> e_20182[3];
  meanpar_20182.close();

  TF1 *fb_2017 = new TF1("fb_2017", alphabeta, -11, 236, 4);
  TF1 *fe_2017 = new TF1("fe_2017", alphabeta, -11, 236, 4);
  TF1 *fb_20181 = new TF1("fb_20181", alphabeta, -11, 236, 4);
  TF1 *fe_20181 = new TF1("fe_20181", alphabeta, -11, 236, 4);
  TF1 *fb_20182 = new TF1("fb_20182", alphabeta, -11, 236, 4);
  TF1 *fe_20182 = new TF1("fe_20182", alphabeta, -11, 236, 4);

  for (int i = 0; i < 4; i++) {

    fb_2017->FixParameter(i, b_2017[i]);
    fe_2017->FixParameter(i, e_2017[i]);
    fb_20181->FixParameter(i, b_20181[i]);
    fe_20181->FixParameter(i, e_20181[i]);
    fb_20182->FixParameter(i, b_20182[i]);
    fe_20182->FixParameter(i, e_20182[i]);

  }

  TLegend * leg = new TLegend(0.6,0.75,0.89,0.89);
    leg->AddEntry(fb_2017,"October 2017","l");
    leg->AddEntry(fb_20181,"June 2018","l");
    leg->AddEntry(fb_20182,"September 2018","l");

  TCanvas * c1 = new TCanvas("c1", "c1", 0, 0, 1000, 500);
  c1->Divide(2,1);
  c1->cd(1);
  fb_2017->GetXaxis()->SetTitleSize(0.06);
  fb_2017->GetXaxis()->SetTitleOffset(0.7);
  fb_2017->GetYaxis()->SetTitleSize(0.06);
  fb_2017->GetYaxis()->SetTitleOffset(0.7);
  fb_2017->GetXaxis()->SetTitle("t (ns)");
  fb_2017->SetTitle("Barrel shape - Year comparison");
  fb_2017->SetMinimum(0.);
  fb_2017->SetMaximum(0.3);
  fb_2017->SetLineColor(kBlue);
  fb_2017->SetLineWidth(2);
  fb_2017->Draw();
  fb_20181->SetLineColor(kRed);
  fb_20181->SetLineWidth(2);
  fb_20181->Draw("same");
  fb_20182->SetLineColor(kGreen);
  fb_20182->SetLineWidth(2);
  fb_20182->Draw("same");
  leg->SetBorderSize(0);
  leg->Draw("same");

  c1->cd(2);
  fe_2017->GetXaxis()->SetTitleSize(0.06);
  fe_2017->GetXaxis()->SetTitleOffset(0.7);
  fe_2017->GetYaxis()->SetTitleSize(0.06);
  fe_2017->GetYaxis()->SetTitleOffset(0.7);
  fe_2017->GetXaxis()->SetTitle("t (ns)");
  fe_2017->SetTitle("Endcap shape - Year comparison");
  fe_2017->SetMinimum(0.);
  fe_2017->SetMaximum(0.3);
  fe_2017->SetLineColor(kBlue);
  fe_2017->SetLineWidth(2);
  fe_2017->Draw();
  fe_20181->SetLineColor(kRed);
  fe_20181->SetLineWidth(2);
  fe_20181->Draw("same");
  fe_20182->SetLineColor(kGreen);
  fe_20182->SetLineWidth(2);
  fe_20182->Draw("same");
  leg->SetBorderSize(0);
  leg->Draw("same");























}
