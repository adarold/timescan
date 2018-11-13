
double alphabeta(double* x, double*p){

   double fitval;

   if( ( 1+(x[0]-p[1]) / (p[2]*p[3]) ) > 0. ){
      fitval = p[0] * pow(1 + (x[0]-p[1]) / (p[2]*p[3]) , p[2] ) * exp ( - (x[0]-p[1]) / p[3]);
   }else{
      fitval = 0;
   }

   return fitval;

}

void many_years(string path, int run_condition){

   string setting = "standard";
   if (run_condition == 1) setting = "etaring";
   if (run_condition == 2) setting = "etaring_fix";
   if (run_condition == 3) setting = "supermodule_fix";
   if (run_condition == 4) setting = "triggertower_fix";

  float b_2017[4];
  float e_2017[4];
  float b_20181[4];
  float e_20181[4];
  float b_20182[4];
  float e_20182[4];

  if (run_condition == 1) {
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

    TF1 *fb_2017  = new TF1("fb_2017",  alphabeta, -11, 236, 4);
    TF1 *fe_2017  = new TF1("fe_2017",  alphabeta, -11, 236, 4);
    TF1 *fb_20181 = new TF1("fb_20181", alphabeta, -11, 236, 4);
    TF1 *fe_20181 = new TF1("fe_20181", alphabeta, -11, 236, 4);
    TF1 *fb_20182 = new TF1("fb_20182", alphabeta, -11, 236, 4);
    TF1 *fe_20182 = new TF1("fe_20182", alphabeta, -11, 236, 4);

    for (int i = 0; i < 4; i++) {

      fb_2017 ->FixParameter(i, b_2017[i]);
      fe_2017 ->FixParameter(i, e_2017[i]);
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

    char pathc1[300];
    sprintf (pathc1, "/home/darold/html/shape/summary/pulseshape_all_years.pdf");
    c1->SaveAs(pathc1);

  }

  char file_histo_2017[300];
  sprintf (file_histo_2017, "%s/%s_2017_histograms.root", (path).c_str(), (setting).c_str());
  TFile * f_2017 = new TFile(file_histo_2017); 
  char file_histo_20181[300];
  sprintf (file_histo_20181, "%s/%s_20181_histograms.root", (path).c_str(), (setting).c_str());
  TFile * f_20181 = new TFile(file_histo_20181); 
  char file_histo_20182[300];
  sprintf (file_histo_20182, "%s/%s_20182_histograms.root", (path).c_str(), (setting).c_str());
  TFile * f_20182 = new TFile(file_histo_20182); 
  
  TH2F * h_2017_ep_t_0  = (TH2F*)f_2017->Get("h_ep_t_0");
  TH2F * h_2017_em_t_0  = (TH2F*)f_2017->Get("h_em_t_0");
  TH2F * h_20181_ep_t_0 = (TH2F*)f_20181->Get("h_ep_t_0");
  TH2F * h_20181_em_t_0 = (TH2F*)f_20181->Get("h_em_t_0");
  TH2F * h_20182_ep_t_0 = (TH2F*)f_20182->Get("h_ep_t_0");
  TH2F * h_20182_em_t_0 = (TH2F*)f_20182->Get("h_em_t_0");

  TH2F * diff_20181_ep_t_0 = (TH2F*) h_20181_ep_t_0->Clone();
  diff_20181_ep_t_0->Add(h_2017_ep_t_0,-1);
  TH2F * diff_20182_ep_t_0 = (TH2F*) h_20182_ep_t_0->Clone();
  diff_20182_ep_t_0->Add(h_2017_ep_t_0,-1);

  TH2F * diff_20181_em_t_0 = (TH2F*) h_20181_em_t_0->Clone();
  diff_20181_em_t_0->Add(h_2017_em_t_0,-1);
  TH2F * diff_20182_em_t_0 = (TH2F*) h_20182_em_t_0->Clone();
  diff_20182_em_t_0->Add(h_2017_em_t_0,-1);

  gStyle->SetOptStat(kFALSE);

  TCanvas * c2 = new TCanvas("c2", "c2", 0, 0, 1500, 500);
  c2->Divide(3,1);
  c2->cd(1);
  h_2017_ep_t_0->GetXaxis()->SetTitleSize(0.06);
  h_2017_ep_t_0->GetXaxis()->SetTitleOffset(0.7);
  h_2017_ep_t_0->GetYaxis()->SetTitleSize(0.06);
  h_2017_ep_t_0->GetYaxis()->SetTitleOffset(0.7);
  h_2017_ep_t_0->GetXaxis()->SetTitle("t (ns)");
  h_2017_ep_t_0->SetMaximum(130.);
  h_2017_ep_t_0->SetMinimum(115.);
  h_2017_ep_t_0->SetTitle("t_{0} Endcap + side - 2017");
  h_2017_ep_t_0->Draw("COLZ");
  c2->cd(2);
  h_20181_ep_t_0->GetXaxis()->SetTitleSize(0.06);
  h_20181_ep_t_0->GetXaxis()->SetTitleOffset(0.7);
  h_20181_ep_t_0->GetYaxis()->SetTitleSize(0.06);
  h_20181_ep_t_0->GetYaxis()->SetTitleOffset(0.7);
  h_20181_ep_t_0->GetXaxis()->SetTitle("t (ns)");
  h_20181_ep_t_0->SetMaximum(130.);
  h_20181_ep_t_0->SetMinimum(115.);
  h_20181_ep_t_0->SetTitle("t_{0} Endcap + side - June 2018");
  h_20181_ep_t_0->Draw("COLZ");
  c2->cd(3);
  diff_20181_ep_t_0->GetXaxis()->SetTitleSize(0.06);
  diff_20181_ep_t_0->GetXaxis()->SetTitleOffset(0.7);
  diff_20181_ep_t_0->GetYaxis()->SetTitleSize(0.06);
  diff_20181_ep_t_0->GetYaxis()->SetTitleOffset(0.7);
  diff_20181_ep_t_0->GetXaxis()->SetTitle("t (ns)");
  diff_20181_ep_t_0->SetMaximum(10.);
  diff_20181_ep_t_0->SetMinimum(0.);
  diff_20181_ep_t_0->SetTitle("Difference June2018 - 2017");
  diff_20181_ep_t_0->Draw("COLZ");

  TCanvas * c3 = new TCanvas("c3", "c3", 0, 0, 1500, 500);
  c3->Divide(3,1);
  c3->cd(1);
  h_2017_em_t_0->GetXaxis()->SetTitleSize(0.06);
  h_2017_em_t_0->GetXaxis()->SetTitleOffset(0.7);
  h_2017_em_t_0->GetYaxis()->SetTitleSize(0.06);
  h_2017_em_t_0->GetYaxis()->SetTitleOffset(0.7);
  h_2017_em_t_0->GetXaxis()->SetTitle("t (ns)");
  h_2017_em_t_0->SetMaximum(130.);
  h_2017_em_t_0->SetMinimum(115.);
  h_2017_em_t_0->SetTitle("t_{0} Endcap - side - 2017");
  h_2017_em_t_0->Draw("COLZ");
  c3->cd(2);
  h_20181_em_t_0->GetXaxis()->SetTitleSize(0.06);
  h_20181_em_t_0->GetXaxis()->SetTitleOffset(0.7);
  h_20181_em_t_0->GetYaxis()->SetTitleSize(0.06);
  h_20181_em_t_0->GetYaxis()->SetTitleOffset(0.7);
  h_20181_em_t_0->GetXaxis()->SetTitle("t (ns)");
  h_20181_em_t_0->SetMaximum(130.);
  h_20181_em_t_0->SetMinimum(115.);
  h_20181_em_t_0->SetTitle("t_{0} Endcap - side - June 2018");
  h_20181_em_t_0->Draw("COLZ");
  c3->cd(3);
  diff_20181_em_t_0->GetXaxis()->SetTitleSize(0.06);
  diff_20181_em_t_0->GetXaxis()->SetTitleOffset(0.7);
  diff_20181_em_t_0->GetYaxis()->SetTitleSize(0.06);
  diff_20181_em_t_0->GetYaxis()->SetTitleOffset(0.7);
  diff_20181_em_t_0->GetXaxis()->SetTitle("t (ns)");
  diff_20181_em_t_0->SetMaximum(10.);
  diff_20181_em_t_0->SetMinimum(0.);
  diff_20181_em_t_0->SetTitle("Difference June 2018 - 2017");
  diff_20181_em_t_0->Draw("COLZ");

  TCanvas * c4 = new TCanvas("c4", "c4", 0, 0, 1500, 500);
  c4->Divide(3,1);
  c4->cd(1);
  h_2017_ep_t_0->GetXaxis()->SetTitleSize(0.06);
  h_2017_ep_t_0->GetXaxis()->SetTitleOffset(0.7);
  h_2017_ep_t_0->GetYaxis()->SetTitleSize(0.06);
  h_2017_ep_t_0->GetYaxis()->SetTitleOffset(0.7);
  h_2017_ep_t_0->GetXaxis()->SetTitle("t (ns)");
  h_2017_ep_t_0->SetMaximum(130.);
  h_2017_ep_t_0->SetMinimum(115.);
  h_2017_ep_t_0->SetTitle("t_{0} Endcap + side - 2017");
  h_2017_ep_t_0->Draw("COLZ");
  c4->cd(2);
  h_20182_ep_t_0->GetXaxis()->SetTitleSize(0.06);
  h_20182_ep_t_0->GetXaxis()->SetTitleOffset(0.7);
  h_20182_ep_t_0->GetYaxis()->SetTitleSize(0.06);
  h_20182_ep_t_0->GetYaxis()->SetTitleOffset(0.7);
  h_20182_ep_t_0->GetXaxis()->SetTitle("t (ns)");
  h_20182_ep_t_0->SetMaximum(130.);
  h_20182_ep_t_0->SetMinimum(115.);
  h_20182_ep_t_0->SetTitle("t_{0} Endcap + side - September 2018");
  h_20182_ep_t_0->Draw("COLZ");
  c4->cd(3);
  diff_20182_ep_t_0->GetXaxis()->SetTitleSize(0.06);
  diff_20182_ep_t_0->GetXaxis()->SetTitleOffset(0.7);
  diff_20182_ep_t_0->GetYaxis()->SetTitleSize(0.06);
  diff_20182_ep_t_0->GetYaxis()->SetTitleOffset(0.7);
  diff_20182_ep_t_0->GetXaxis()->SetTitle("t (ns)");
  diff_20182_ep_t_0->SetMaximum(10.);
  diff_20182_ep_t_0->SetMinimum(0.);
  diff_20182_ep_t_0->SetTitle("Difference September 2018 - 2017");
  diff_20182_ep_t_0->Draw("COLZ");

  TCanvas * c5 = new TCanvas("c5", "c5", 0, 0, 1500, 500);
  c5->Divide(3,1);
  c5->cd(1);
  h_2017_em_t_0->GetXaxis()->SetTitleSize(0.06);
  h_2017_em_t_0->GetXaxis()->SetTitleOffset(0.7);
  h_2017_em_t_0->GetYaxis()->SetTitleSize(0.06);
  h_2017_em_t_0->GetYaxis()->SetTitleOffset(0.7);
  h_2017_em_t_0->GetXaxis()->SetTitle("t (ns)");
  h_2017_em_t_0->SetMaximum(130.);
  h_2017_em_t_0->SetMinimum(115.);
  h_2017_em_t_0->SetTitle("t_{0} Endcap - side - 2017");
  h_2017_em_t_0->Draw("COLZ");
  c5->cd(2);
  h_20182_em_t_0->GetXaxis()->SetTitleSize(0.06);
  h_20182_em_t_0->GetXaxis()->SetTitleOffset(0.7);
  h_20182_em_t_0->GetYaxis()->SetTitleSize(0.06);
  h_20182_em_t_0->GetYaxis()->SetTitleOffset(0.7);
  h_20182_em_t_0->GetXaxis()->SetTitle("t (ns)");
  h_20182_em_t_0->SetMaximum(130.);
  h_20182_em_t_0->SetMinimum(115.);
  h_20182_em_t_0->SetTitle("t_{0} Endcap - side - September 2018");
  h_20182_em_t_0->Draw("COLZ");
  c5->cd(3);
  diff_20182_em_t_0->GetXaxis()->SetTitleSize(0.06);
  diff_20182_em_t_0->GetXaxis()->SetTitleOffset(0.7);
  diff_20182_em_t_0->GetYaxis()->SetTitleSize(0.06);
  diff_20182_em_t_0->GetYaxis()->SetTitleOffset(0.7);
  diff_20182_em_t_0->GetXaxis()->SetTitle("t (ns)");
  diff_20182_em_t_0->SetMaximum(10.);
  diff_20182_em_t_0->SetMinimum(0.);
  diff_20182_em_t_0->SetTitle("Difference September 2018 - 2017");
  diff_20182_em_t_0->Draw("COLZ");

  char pathc2[300];
  sprintf (pathc2, "/home/darold/html/shape/summary/%s_ep_17_181_t_0.pdf", (setting).c_str());
  c2->SaveAs(pathc2);
  sprintf (pathc2, "/home/darold/html/shape/summary/%s_em_17_181_t_0.pdf", (setting).c_str());
  c3->SaveAs(pathc2);
  sprintf (pathc2, "/home/darold/html/shape/summary/%s_ep_17_182_t_0.pdf", (setting).c_str());
  c4->SaveAs(pathc2);
  sprintf (pathc2, "/home/darold/html/shape/summary/%s_em_17_182_t_0.pdf", (setting).c_str());
  c5->SaveAs(pathc2);

}
