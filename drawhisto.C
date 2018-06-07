void drawhisto() {

   //Reads histograms from file
   TFile * f = new TFile("histograms.root");

      TH1F * h1_b_A = (TH1F*)f->Get("h1_b_A");
      TH1F * h1_b_t_0 = (TH1F*)f->Get("h1_b_t_0");
      TH1F * h1_b_alpha = (TH1F*)f->Get("h1_b_alpha");
      TH1F * h1_b_beta = (TH1F*)f->Get("h1_b_beta");

      TH1F * h1_e_A = (TH1F*)f->Get("h1_e_A");
      TH1F * h1_e_t_0 = (TH1F*)f->Get("h1_e_t_0");
      TH1F * h1_e_alpha = (TH1F*)f->Get("h1_e_alpha");
      TH1F * h1_e_beta = (TH1F*)f->Get("h1_e_beta");

      TH2F * h_b_chi = (TH2F*)f->Get("h_b_chi");
      TH2F * h_b_A = (TH2F*)f->Get("h_b_A");
      TH2F * h_b_t_0 = (TH2F*)f->Get("h_b_t_0");
      TH2F * h_b_alpha = (TH2F*)f->Get("h_b_alpha");
      TH2F * h_b_beta = (TH2F*)f->Get("h_b_beta");

      TH2F * h_ep_chi = (TH2F*)f->Get("h_ep_chi");
      TH2F * h_ep_A = (TH2F*)f->Get("h_ep_A");
      TH2F * h_ep_t_0 = (TH2F*)f->Get("h_ep_t_0");
      TH2F * h_ep_alpha = (TH2F*)f->Get("h_ep_alpha");
      TH2F * h_ep_beta = (TH2F*)f->Get("h_ep_beta");

      TH2F * h_em_chi = (TH2F*)f->Get("h_em_chi");
      TH2F * h_em_A = (TH2F*)f->Get("h_em_A");
      TH2F * h_em_t_0 = (TH2F*)f->Get("h_em_t_0");
      TH2F * h_em_alpha = (TH2F*)f->Get("h_em_alpha");
      TH2F * h_em_beta = (TH2F*)f->Get("h_em_beta");

      TH2F * h_b_chi_tomean = (TH2F*)f->Get("h_b_chi_tomean");
      TH2F * h_ep_chi_tomean = (TH2F*)f->Get("h_ep_chi_tomean");
      TH2F * h_em_chi_tomean = (TH2F*)f->Get("h_em_chi_tomean");
      TH2F * h_ep_chi_tomean_inout = (TH2F*)f->Get("h_ep_chi_tomean_inout");
      TH2F * h_em_chi_tomean_inout = (TH2F*)f->Get("h_em_chi_tomean_inout");


   //Draws histograms

   gStyle->SetOptStat(kFALSE);


   TCanvas * c1 = new TCanvas("c1","c1",0,0,1600,1000);
   c1->cd();
   h_b_chi->SetMinimum(0.02);
   h_b_chi->SetMaximum(0.1);
   h_b_chi->GetXaxis()->SetTitleSize(0.06);
   h_b_chi->GetXaxis()->SetTitleOffset(0.7);
   h_b_chi->GetYaxis()->SetTitleSize(0.06);
   h_b_chi->GetYaxis()->SetTitleOffset(0.7);
   h_b_chi->GetXaxis()->SetTitle("i#phi");
   h_b_chi->GetYaxis()->SetTitle("i#eta");
   h_b_chi->SetTitle("Barrel crystals - #chi^{2}");
   h_b_chi->Draw("COLZ");



   TCanvas * c2 = new TCanvas("c2","c2",0,0,2800,1600);

   TPad *pad1_c2 = new TPad("pad1_c2","pad1_c2",0.,0.50,0.48,1);
   pad1_c2->SetBottomMargin(0.1);
   pad1_c2->SetRightMargin(0.15);
   pad1_c2->Draw();
   pad1_c2->cd();
   h_b_A->GetXaxis()->SetTitleSize(0.06);
   h_b_A->GetXaxis()->SetTitleOffset(0.7);
   h_b_A->GetYaxis()->SetTitleSize(0.06);
   h_b_A->GetYaxis()->SetTitleOffset(0.7);
   h_b_A->GetXaxis()->SetTitle("i#phi");
   h_b_A->GetYaxis()->SetTitle("i#eta");
   h_b_A->SetMinimum(0.238);
   h_b_A->SetMaximum(0.248);
   h_b_A->GetXaxis()->SetTitle("i#phi");
   h_b_A->GetYaxis()->SetTitle("i#eta");
   h_b_A->SetTitle("Barrel crystals - A");
   h_b_A->Draw("COLZ");

   c2->cd();
   TPad *pad2_c2 = new TPad("pad2_c2","pad2_c2",0.52,0.5,1,1);
   pad2_c2->SetBottomMargin(0.1);
   pad2_c2->SetRightMargin(0.15);
   pad2_c2->Draw();
   pad2_c2->cd();
   h_b_t_0->GetXaxis()->SetTitleSize(0.06);
   h_b_t_0->GetXaxis()->SetTitleOffset(0.7);
   h_b_t_0->GetYaxis()->SetTitleSize(0.06);
   h_b_t_0->GetYaxis()->SetTitleOffset(0.7);
   h_b_t_0->GetXaxis()->SetTitle("i#phi");
   h_b_t_0->GetYaxis()->SetTitle("i#eta");
   h_b_t_0->SetMinimum(120);
   h_b_t_0->SetMaximum(127);
   h_b_t_0->SetTitle("Barrel crystals - t_{0}");
   h_b_t_0->Draw("COLZ");

   c2->cd();
   TPad *pad3_c2 = new TPad("pad3_c2","pad3_c2",0.,0.,0.48,0.5);
   pad3_c2->SetBottomMargin(0.1);
   pad3_c2->SetRightMargin(0.15);
   pad3_c2->Draw();
   pad3_c2->cd();
   h_b_alpha->GetXaxis()->SetTitleSize(0.06);
   h_b_alpha->GetXaxis()->SetTitleOffset(0.7);
   h_b_alpha->GetYaxis()->SetTitleSize(0.06);
   h_b_alpha->GetYaxis()->SetTitleOffset(0.7);
   h_b_alpha->GetXaxis()->SetTitle("i#phi");
   h_b_alpha->GetYaxis()->SetTitle("i#eta");
   h_b_alpha->SetMinimum(1.05);
   h_b_alpha->SetMaximum(1.25);
   h_b_alpha->SetTitle("Barrel crystals - #alpha");
   h_b_alpha->Draw("COLZ");

   c2->cd();
   TPad *pad4_c2 = new TPad("pad4_c2","pad4_c2",0.52,0.,1,0.5);
   pad4_c2->SetBottomMargin(0.1);
   pad4_c2->SetRightMargin(0.15);
   pad4_c2->Draw();
   pad4_c2->cd();
   h_b_beta->GetXaxis()->SetTitleSize(0.06);
   h_b_beta->GetXaxis()->SetTitleOffset(0.7);
   h_b_beta->GetYaxis()->SetTitleSize(0.06);
   h_b_beta->GetYaxis()->SetTitleOffset(0.7);
   h_b_beta->GetXaxis()->SetTitle("i#phi");
   h_b_beta->GetYaxis()->SetTitle("i#eta");
   h_b_beta->SetMinimum(39);
   h_b_beta->SetMaximum(42);
   h_b_beta->SetTitle("Barrel crystals - #beta");
   h_b_beta->Draw("COLZ");



   TCanvas * c3 = new TCanvas("c3","c3",0,0,1000,1000);
   c3->cd();
   h_em_chi->GetXaxis()->SetTitleSize(0.06);
   h_em_chi->GetXaxis()->SetTitleOffset(0.7);
   h_em_chi->GetYaxis()->SetTitleSize(0.06);
   h_em_chi->GetYaxis()->SetTitleOffset(0.7);
   h_em_chi->GetXaxis()->SetTitle("ix");
   h_em_chi->GetYaxis()->SetTitle("iy");
   h_em_chi->SetMaximum(2.5);
   h_em_chi->SetMinimum(0.);
   h_em_chi->SetTitle("Endcap crystals -side - #chi^{2}");
   h_em_chi->Draw("COLZ");



   TCanvas * c4 = new TCanvas("c4","c4",0,0,1800,1600);

   TPad *pad1_c4 = new TPad("pad1_c4","pad1_c4",0.,0.5,0.48,1);
   pad1_c4->SetBottomMargin(0.1);
   pad1_c4->SetRightMargin(0.15);
   pad1_c4->Draw();
   pad1_c4->cd();
   h_em_A->GetXaxis()->SetTitleSize(0.06);
   h_em_A->GetXaxis()->SetTitleOffset(0.7);
   h_em_A->GetYaxis()->SetTitleSize(0.06);
   h_em_A->GetYaxis()->SetTitleOffset(0.7);
   h_em_A->GetXaxis()->SetTitle("ix");
   h_em_A->GetYaxis()->SetTitle("iy");
   h_em_A->SetMinimum(0.21);
   h_em_A->SetMaximum(0.24);
   h_em_A->SetTitle("Endcap crystals -side - A");
   h_em_A->Draw("COLZ");

   c4->cd();
   TPad *pad2_c4 = new TPad("pad2_c4","pad2_c4",0.52,0.5,1,1);
   pad2_c4->SetBottomMargin(0.1);
   pad2_c4->SetRightMargin(0.15);
   pad2_c4->Draw();
   pad2_c4->cd();
   h_em_t_0->GetXaxis()->SetTitleSize(0.06);
   h_em_t_0->GetXaxis()->SetTitleOffset(0.7);
   h_em_t_0->GetYaxis()->SetTitleSize(0.06);
   h_em_t_0->GetYaxis()->SetTitleOffset(0.7);
   h_em_t_0->GetXaxis()->SetTitle("ix");
   h_em_t_0->GetYaxis()->SetTitle("iy");
   h_em_t_0->SetMinimum(114);
   h_em_t_0->SetMaximum(129);
   h_em_t_0->SetTitle("Endcap crystals -side - t_{0}");
   h_em_t_0->Draw("COLZ");
   
   c4->cd();
   TPad *pad3_c4 = new TPad("pad3_c4","pad3_c4",0.,0.,0.48,0.5);
   pad3_c4->SetBottomMargin(0.1);
   pad3_c4->SetRightMargin(0.15);
   pad3_c4->Draw();
   pad3_c4->cd();
   h_em_alpha->GetXaxis()->SetTitleSize(0.06);
   h_em_alpha->GetXaxis()->SetTitleOffset(0.7);
   h_em_alpha->GetYaxis()->SetTitleSize(0.06);
   h_em_alpha->GetYaxis()->SetTitleOffset(0.7);
   h_em_alpha->GetXaxis()->SetTitle("ix");
   h_em_alpha->GetYaxis()->SetTitle("iy");
   h_em_alpha->SetMinimum(1.1);
   h_em_alpha->SetMaximum(1.75);
   h_em_alpha->SetTitle("Endcap crystals -side - #alpha");
   h_em_alpha->Draw("COLZ");
   
   c4->cd();
   TPad *pad4_c4 = new TPad("pad4_c4","pad4_c4",0.52,0.,1,0.5);
   pad4_c4->SetBottomMargin(0.1);
   pad4_c4->SetRightMargin(0.15);
   pad4_c4->Draw();
   pad4_c4->cd();
   h_em_beta->GetXaxis()->SetTitleSize(0.06);
   h_em_beta->GetXaxis()->SetTitleOffset(0.7);
   h_em_beta->GetYaxis()->SetTitleSize(0.06);
   h_em_beta->GetYaxis()->SetTitleOffset(0.7);
   h_em_beta->GetXaxis()->SetTitle("ix");
   h_em_beta->GetYaxis()->SetTitle("iy");
   h_em_beta->SetMinimum(34);
   h_em_beta->SetMaximum(44);
   h_em_beta->SetTitle("Endcap crystals -side - #beta");
   h_em_beta->Draw("COLZ");



   TCanvas * c5 = new TCanvas("c5","c5",0,0,1000,1000);
   c5->cd();
   h_ep_chi->GetXaxis()->SetTitleSize(0.06);
   h_ep_chi->GetXaxis()->SetTitleOffset(0.7);
   h_ep_chi->GetYaxis()->SetTitleSize(0.06);
   h_ep_chi->GetYaxis()->SetTitleOffset(0.7);
   h_ep_chi->GetXaxis()->SetTitle("ix");
   h_ep_chi->GetYaxis()->SetTitle("iy");
   h_ep_chi->SetMaximum(2.5);
   h_ep_chi->SetMinimum(0.);
   h_ep_chi->SetTitle("Endcap crystals +side - #chi^{2}");
   h_ep_chi->Draw("COLZ");



   TCanvas * c6 = new TCanvas("c6","c6",0,0,1800,1600);

   TPad *pad1_c6 = new TPad("pad1_c6","pad1_c6",0.,0.5,0.48,1);
   pad1_c6->SetBottomMargin(0.1);
   pad1_c6->SetRightMargin(0.15);
   pad1_c6->Draw();
   pad1_c6->cd();
   h_ep_A->GetXaxis()->SetTitleSize(0.06);
   h_ep_A->GetXaxis()->SetTitleOffset(0.7);
   h_ep_A->GetYaxis()->SetTitleSize(0.06);
   h_ep_A->GetYaxis()->SetTitleOffset(0.7);
   h_ep_A->GetXaxis()->SetTitle("ix");
   h_ep_A->GetYaxis()->SetTitle("iy");
   h_ep_A->SetMinimum(0.21);
   h_ep_A->SetMaximum(0.24);
   h_ep_A->SetTitle("Endcap crystals +side - A");
   h_ep_A->Draw("COLZ");

   c6->cd();
   TPad *pad2_c6 = new TPad("pad2_c6","pad2_c6",0.52,0.5,1,1);
   pad2_c6->SetBottomMargin(0.1);
   pad2_c6->SetRightMargin(0.15);
   pad2_c6->Draw();
   pad2_c6->cd();
   h_ep_t_0->GetXaxis()->SetTitleSize(0.06);
   h_ep_t_0->GetXaxis()->SetTitleOffset(0.7);
   h_ep_t_0->GetYaxis()->SetTitleSize(0.06);
   h_ep_t_0->GetYaxis()->SetTitleOffset(0.7);
   h_ep_t_0->GetXaxis()->SetTitle("ix");
   h_ep_t_0->GetYaxis()->SetTitle("iy");
   h_ep_t_0->SetMinimum(114);
   h_ep_t_0->SetMaximum(129);
   h_ep_t_0->SetTitle("Endcap crystals +side - t_{0}");
   h_ep_t_0->Draw("COLZ");

   c6->cd();
   TPad *pad3_c6 = new TPad("pad3_c6","pad3_c6",0.,0.,0.48,0.5);
   pad3_c6->SetBottomMargin(0.1);
   pad3_c6->SetRightMargin(0.15);
   pad3_c6->Draw();
   pad3_c6->cd();
   h_ep_alpha->GetXaxis()->SetTitleSize(0.06);
   h_ep_alpha->GetXaxis()->SetTitleOffset(0.7);
   h_ep_alpha->GetYaxis()->SetTitleSize(0.06);
   h_ep_alpha->GetYaxis()->SetTitleOffset(0.7);
   h_ep_alpha->GetXaxis()->SetTitle("ix");
   h_ep_alpha->GetYaxis()->SetTitle("iy");
   h_ep_alpha->SetMinimum(1.1);
   h_ep_alpha->SetMaximum(1.75);
   h_ep_alpha->SetTitle("Endcap crystals +side - #alpha");
   h_ep_alpha->Draw("COLZ");

   c6->cd();
   TPad *pad4_c6 = new TPad("pad4_c6","pad4_c6",0.52,0.,1,0.5);
   pad4_c6->SetBottomMargin(0.1);
   pad4_c6->SetRightMargin(0.15);
   pad4_c6->Draw();
   pad4_c6->cd();
   h_ep_beta->GetXaxis()->SetTitleSize(0.06);
   h_ep_beta->GetXaxis()->SetTitleOffset(0.7);
   h_ep_beta->GetYaxis()->SetTitleSize(0.06);
   h_ep_beta->GetYaxis()->SetTitleOffset(0.7);
   h_ep_beta->GetXaxis()->SetTitle("ix");
   h_ep_beta->GetYaxis()->SetTitle("iy");
   h_ep_beta->SetMinimum(34);
   h_ep_beta->SetMaximum(44);
   h_ep_beta->SetTitle("Endcap crystals +side - #beta");
   h_ep_beta->Draw("COLZ");



   TCanvas * c7 = new TCanvas("c7","c7",0,0,1800,1600);

   TPad *pad1_c7 = new TPad("pad1_c7","pad1_c7",0.,0.5,0.48,1);
   pad1_c7->SetBottomMargin(0.1);
   pad1_c7->SetRightMargin(0.15);
   pad1_c7->Draw();
   pad1_c7->cd();
   pad1_c7->SetLogy();
   h1_b_A->SetTitle("Barrel crystals - A");
   h1_b_A->GetXaxis()->SetTitle("A");
   h1_b_A->GetXaxis()->SetNdivisions(505);
   h1_b_A->Draw("COLZ");

   c7->cd();
   TPad *pad2_c7 = new TPad("pad2_c7","pad2_c7",0.52,0.5,1,1);
   pad2_c7->SetBottomMargin(0.1);
   pad2_c7->SetRightMargin(0.15);
   pad2_c7->Draw();
   pad2_c7->cd();
   pad2_c7->SetLogy();
   h1_b_t_0->SetTitle("Barrel crystals - t_{0}");
   h1_b_t_0->GetXaxis()->SetTitle("t_{0}");
   h1_b_t_0->GetXaxis()->SetNdivisions(505);
   h1_b_t_0->Draw("COLZ");

   c7->cd();
   TPad *pad3_c7 = new TPad("pad3_c7","pad3_c7",0.,0.,0.48,0.5);
   pad3_c7->SetBottomMargin(0.1);
   pad3_c7->SetRightMargin(0.15);
   pad3_c7->Draw();
   pad3_c7->cd();
   pad3_c7->SetLogy();
   h1_b_alpha->SetTitle("Barrel crystals - #alpha");
   h1_b_alpha->GetXaxis()->SetTitle("#alpha");
   h1_b_alpha->GetXaxis()->SetNdivisions(505);
   h1_b_alpha->Draw("COLZ");

   c7->cd();
   TPad *pad4_c7 = new TPad("pad4_c7","pad4_c7",0.52,0.,1,0.5);
   pad4_c7->SetBottomMargin(0.1);
   pad4_c7->SetRightMargin(0.15);
   pad4_c7->Draw();
   pad4_c7->cd();
   pad4_c7->SetLogy();
   h1_b_beta->SetTitle("Barrel crystals - #beta");
   h1_b_beta->GetXaxis()->SetTitle("#beta");
   h1_b_beta->GetXaxis()->SetNdivisions(505);
   h1_b_beta->Draw("COLZ");



   TCanvas * c8 = new TCanvas("c8","c8",0,0,1800,1600);

   TPad *pad1_c8 = new TPad("pad1_c8","pad1_c8",0.,0.5,0.48,1);
   pad1_c8->SetBottomMargin(0.1);
   pad1_c8->SetRightMargin(0.15);
   pad1_c8->Draw();
   pad1_c8->cd();
   pad1_c8->SetLogy();
   h1_e_A->SetTitle("Endcap crystals - A");
   h1_e_A->GetXaxis()->SetTitle("A");
   h1_e_A->GetXaxis()->SetNdivisions(505);
   h1_e_A->Draw("COLZ");

   c8->cd();
   TPad *pad2_c8 = new TPad("pad2_c8","pad2_c8",0.52,0.5,1,1);
   pad2_c8->SetBottomMargin(0.1);
   pad2_c8->SetRightMargin(0.15);
   pad2_c8->Draw();
   pad2_c8->cd();
   pad2_c8->SetLogy();
   h1_e_t_0->SetTitle("Endcap crystals - t_{0}");
   h1_e_t_0->GetXaxis()->SetTitle("t_{0}");
   h1_e_t_0->GetXaxis()->SetNdivisions(505);
   h1_e_t_0->Draw("COLZ");

   c8->cd();
   TPad *pad3_c8 = new TPad("pad3_c8","pad3_c8",0.,0.,0.48,0.5);
   pad3_c8->SetBottomMargin(0.1);
   pad3_c8->SetRightMargin(0.15);
   pad3_c8->Draw();
   pad3_c8->cd();
   pad3_c8->SetLogy();
   h1_e_alpha->SetTitle("Endcap crystals - #alpha");
   h1_e_alpha->GetXaxis()->SetTitle("#alpha");
   h1_e_alpha->GetXaxis()->SetNdivisions(505);
   h1_e_alpha->Draw("COLZ");

   c8->cd();
   TPad *pad4_c8 = new TPad("pad4_c8","pad4_c8",0.52,0.,1,0.5);
   pad4_c8->SetBottomMargin(0.1);
   pad4_c8->SetRightMargin(0.15);
   pad4_c8->Draw();
   pad4_c8->cd();
   pad4_c8->SetLogy();
   h1_e_beta->SetTitle("Endcap crystals - #beta");
   h1_e_beta->GetXaxis()->SetTitle("#beta");
   h1_e_beta->GetXaxis()->SetNdivisions(505);
   h1_e_beta->Draw("COLZ");



   TCanvas * c9 = new TCanvas("c9","c9",0,0,1600,1000);
   c9->cd();
   h_b_chi_tomean->GetXaxis()->SetTitleSize(0.06);
   h_b_chi_tomean->GetXaxis()->SetTitleOffset(0.7);
   h_b_chi_tomean->GetYaxis()->SetTitleSize(0.06);
   h_b_chi_tomean->GetYaxis()->SetTitleOffset(0.7);
   h_b_chi_tomean->GetXaxis()->SetTitle("i#phi");
   h_b_chi_tomean->GetYaxis()->SetTitle("i#eta");
   h_b_chi_tomean->SetMinimum(0.);
   h_b_chi_tomean->SetMaximum(0.5);
   h_b_chi_tomean->SetTitle("Barrel crystals - #chi^{2} to mean fit");
   h_b_chi_tomean->Draw("COLZ");



   TCanvas * c10 = new TCanvas("c10","c10",0,0,1000,1000);
   c10->cd();
   h_ep_chi_tomean->GetXaxis()->SetTitleSize(0.06);
   h_ep_chi_tomean->GetXaxis()->SetTitleOffset(0.7);
   h_ep_chi_tomean->GetYaxis()->SetTitleSize(0.06);
   h_ep_chi_tomean->GetYaxis()->SetTitleOffset(0.7);
   h_ep_chi_tomean->GetXaxis()->SetTitle("ix");
   h_ep_chi_tomean->GetYaxis()->SetTitle("iy");
   h_ep_chi_tomean->SetMinimum(0.);
   h_ep_chi_tomean->SetMaximum(5);
   h_ep_chi_tomean->SetTitle("Endcap crystals +side - #chi^{2} to mean fit");
   h_ep_chi_tomean->Draw("COLZ");



   TCanvas * c10_bis = new TCanvas("c10_bis","c10_bis",0,0,1000,1000);
   c10_bis->cd();
   h_ep_chi_tomean_inout->GetXaxis()->SetTitleSize(0.06);
   h_ep_chi_tomean_inout->GetXaxis()->SetTitleOffset(0.7);
   h_ep_chi_tomean_inout->GetYaxis()->SetTitleSize(0.06);
   h_ep_chi_tomean_inout->GetYaxis()->SetTitleOffset(0.7);
   h_ep_chi_tomean_inout->GetXaxis()->SetTitle("ix");
   h_ep_chi_tomean_inout->GetYaxis()->SetTitle("iy");
   h_ep_chi_tomean_inout->SetMinimum(0.);
   h_ep_chi_tomean_inout->SetMaximum(5);
   h_ep_chi_tomean_inout->SetTitle("Endcap crystals +side - #chi^{2} to mean fit - IN OUT");
   h_ep_chi_tomean_inout->Draw("COLZ");



   TCanvas * c11 = new TCanvas("c11","c11",0,0,1000,1000);
   c11->cd();
   h_em_chi_tomean->GetXaxis()->SetTitleSize(0.06);
   h_em_chi_tomean->GetXaxis()->SetTitleOffset(0.7);
   h_em_chi_tomean->GetYaxis()->SetTitleSize(0.06);
   h_em_chi_tomean->GetYaxis()->SetTitleOffset(0.7);
   h_em_chi_tomean->GetXaxis()->SetTitle("ix");
   h_em_chi_tomean->GetYaxis()->SetTitle("iy");
   h_em_chi_tomean->SetMinimum(0.);
   h_em_chi_tomean->SetMaximum(5);
   h_em_chi_tomean->SetTitle("Endcap crystals -side - #chi^{2} to mean fit");
   h_em_chi_tomean->Draw("COLZ");



   TCanvas * c11_bis = new TCanvas("c11_bis","c11_bis",0,0,1000,1000);
   c11_bis->cd();
   h_em_chi_tomean_inout->GetXaxis()->SetTitleSize(0.06);
   h_em_chi_tomean_inout->GetXaxis()->SetTitleOffset(0.7);
   h_em_chi_tomean_inout->GetYaxis()->SetTitleSize(0.06);
   h_em_chi_tomean_inout->GetYaxis()->SetTitleOffset(0.7);
   h_em_chi_tomean_inout->GetXaxis()->SetTitle("ix");
   h_em_chi_tomean_inout->GetYaxis()->SetTitle("iy");
   h_em_chi_tomean_inout->SetMinimum(0.);
   h_em_chi_tomean_inout->SetMaximum(5);
   h_em_chi_tomean_inout->SetTitle("Endcap crystals -side - #chi^{2} to mean fit - IN OUT");
   h_em_chi_tomean_inout->Draw("COLZ");



   TH2F * ratio_b = (TH2F*) h_b_chi_tomean->Clone();
      ratio_b->Divide(h_b_chi);

   TH2F * ratio_ep = (TH2F*) h_ep_chi_tomean->Clone();
      ratio_ep->Divide(h_ep_chi);

   TH2F * ratio_em = (TH2F*) h_em_chi_tomean->Clone();
      ratio_em->Divide(h_em_chi);

   TH2F * ratio_ep_inout = (TH2F*) h_ep_chi_tomean_inout->Clone();
      ratio_ep_inout->Divide(h_ep_chi);

   TH2F * ratio_em_inout = (TH2F*) h_em_chi_tomean_inout->Clone();
      ratio_em_inout->Divide(h_em_chi);

  
   TCanvas * c12 = new TCanvas("c12","c12",0,0,1600,1000);
   c12->cd();
   c12->SetRightMargin(0.15);
   ratio_b->GetXaxis()->SetTitleSize(0.06);
   ratio_b->GetXaxis()->SetTitleOffset(0.7);
   ratio_b->GetYaxis()->SetTitleSize(0.06);
   ratio_b->GetYaxis()->SetTitleOffset(0.7);
   ratio_b->GetXaxis()->SetTitle("i#phi");
   ratio_b->GetYaxis()->SetTitle("i#eta");
   ratio_b->SetTitle("Barrel #chi^{2}_{to mean} / #chi^{2}");
   ratio_b->SetMinimum(1.);
   ratio_b->SetMaximum(4);
   ratio_b->Draw("colz");



   TCanvas * c13 = new TCanvas("c13","c13",0,0,2000,1000);
   c13->Divide(2,1);
   c13->cd(1);
   ratio_ep->GetXaxis()->SetTitleSize(0.06);
   ratio_ep->GetXaxis()->SetTitleOffset(0.7);
   ratio_ep->GetYaxis()->SetTitleSize(0.06);
   ratio_ep->GetYaxis()->SetTitleOffset(0.7);
   ratio_ep->GetXaxis()->SetTitle("ix");
   ratio_ep->GetYaxis()->SetTitle("iy");
   ratio_ep->SetTitle("Endcap +side #chi^{2}_{to mean} / #chi^{2}");
   ratio_ep->SetMinimum(1.);
   ratio_ep->SetMaximum(5.);
   ratio_ep->Draw("colz");
   c13->cd(2);
   ratio_em->GetXaxis()->SetTitleSize(0.06);
   ratio_em->GetXaxis()->SetTitleOffset(0.7);
   ratio_em->GetYaxis()->SetTitleSize(0.06);
   ratio_em->GetYaxis()->SetTitleOffset(0.7);
   ratio_em->GetXaxis()->SetTitle("ix");
   ratio_em->GetYaxis()->SetTitle("iy");
   ratio_em->SetTitle("Endcap -side #chi^{2}_{to mean} / #chi^{2}");
   ratio_em->SetMinimum(1.);
   ratio_em->SetMaximum(5.);
   ratio_em->Draw("colz");



   TCanvas * c13_bis = new TCanvas("c13_bis","c13_bis",0,0,2000,1000);
   c13_bis->Divide(2,1);
   c13_bis->cd(1);
   ratio_ep_inout->GetXaxis()->SetTitleSize(0.06);
   ratio_ep_inout->GetXaxis()->SetTitleOffset(0.7);
   ratio_ep_inout->GetYaxis()->SetTitleSize(0.06);
   ratio_ep_inout->GetYaxis()->SetTitleOffset(0.7);
   ratio_ep_inout->GetXaxis()->SetTitle("ix");
   ratio_ep_inout->GetYaxis()->SetTitle("iy");
   ratio_ep_inout->SetTitle("Endcap +side #chi^{2}_{to mean} / #chi^{2} - IN OUT");
   ratio_ep_inout->SetMinimum(1.);
   ratio_ep_inout->SetMaximum(5.);
   ratio_ep_inout->Draw("colz");
   c13_bis->cd(2);
   ratio_em_inout->GetXaxis()->SetTitleSize(0.06);
   ratio_em_inout->GetXaxis()->SetTitleOffset(0.7);
   ratio_em_inout->GetYaxis()->SetTitleSize(0.06);
   ratio_em_inout->GetYaxis()->SetTitleOffset(0.7);
   ratio_em_inout->GetXaxis()->SetTitle("ix");
   ratio_em_inout->GetYaxis()->SetTitle("iy");
   ratio_em_inout->SetTitle("Endcap -side #chi^{2}_{to mean} / #chi^{2} - IN OUT");
   ratio_em_inout->SetMinimum(1.);
   ratio_em_inout->SetMaximum(5.);
   ratio_em_inout->Draw("colz");



   c1->SaveAs("plots/barrel_chi2.jpg");
   c2->SaveAs("plots/barrel_parameters.pdf");
   c3->SaveAs("plots/endcap_m_chi2.jpg");
   c4->SaveAs("plots/endcap_m_parameters.jpg");
   c5->SaveAs("plots/endcap_p_chi2.jpg");
   c6->SaveAs("plots/endcap_p_parameters.jpg");
   c7->SaveAs("plots/barrel_1d_distribution_parameters.pdf");
   c8->SaveAs("plots/endcap_1d_distribution_parameters.pdf");
   c9->SaveAs("plots/barrel_chi2_tomean.jpg");
   c10->SaveAs("plots/endcap_p_chi2_tomean.jpg");
   c10_bis->SaveAs("plots/endcap_p_chi2_tomean_inout.jpg");
   c11->SaveAs("plots/endcap_m_chi2_tomean.jpg");
   c11_bis->SaveAs("plots/endcap_m_chi2_tomean_inout.jpg");
   c12->SaveAs("plots/barrel_ratio_chi2.jpg");
   c13->SaveAs("plots/endcap_ratio_chi2.jpg");
   c13_bis->SaveAs("plots/endcap_ratio_chi2_inout.jpg");

}