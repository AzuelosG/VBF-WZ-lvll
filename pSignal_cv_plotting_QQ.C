
#include <TString.h>
#include <TFile.h>
#include <TMath.h>
#include <TTree.h>
#include <TColor.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <TLine.h>
#include <string>
#include <algorithm>
#include <list>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#include <stdlib.h>
#include <bits/stdc++.h> 
// #include <sys/stat.h> 
// #include <sys/types.h> 
// #include <fstream>
// #include <cstdlib>
// #include <experimental/filesystem>
using namespace std;
//------------------------------------------------------------------------------
/*
Analysis of the pSignal of different masses and backgrounds following the
training of the neuron network at each masses seperatly. 

The pSignal and the normalized number of events after certain 
cut values are plotted. Are also plotted, the same curves for the same data 
but with the conditions of M_jj > 500 and Deta_jj > 3.5. The significance for
each of these are also drawn.
*/
//------------------------------------------------------------------------------

//========================== PARAMETERS TO EDIT ================================
string savedir = "ControlPlots/QQ_31-12-20/";   // MAKE SURE PSIGNAL EXISTS IN SUBDIRECTORY
string rdir   = "OutputRoot/QQ_31-12-20/";       // Subdirectory containing the root files produced by appluing the NN
string model  = "QQ";            // Model used
string opt_ID = "_";    // Optional file name identification
//=======================================================================

float AMS(float s, float b, bool debug=false) {
  if (s<=0 or b<=0) return 0;

  float br = 0.00001;// #KM: systematic unc?
  float sigma    = sqrt(b+br);
  float n        = s+b+br;
  float radicand = 2 *( n * log (n*(b+br+sigma)/(b*b+n*sigma+br))-b*b/sigma*log(1+sigma*(n-b)/(b*(b+br+sigma))));

  float ams= 0;
  if (radicand < 0) {
    std::cout << "AMS: radicand is negative. Returning 0." << std::endl;
  }
  else ams = sqrt(radicand);

  return ams;
}

int main() {
  string rootdir = rdir; 

    string QQname[1] = {
      // "QQ_resonance.307730_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0250_ntuples.root",
      // "QQ_resonance.309528_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0350_ntuples.root",
      // "QQ_resonance.307732_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0400_ntuples.root",
      // "QQ_resonance.309529_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0450_ntuples.root",
      // "QQ_resonance.307733_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0500_ntuples.root",
      "QQ_resonance.307734_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0600_ntuples.root",
      // "QQ_resonance.307735_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0700_ntuples.root",
      // // // //      "QQ_resonance.307738_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m1000_ntuples.root",
      // "QQ_mainMVA.307731_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0300_ntuples.root",
      // "QQ_mainMVA.307736_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0800_ntuples.root",
      // "QQ_mainMVA.307737_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0900_ntuples.root"
    };


    // background files  -- here, bckgr is qq HVT
        string bname1 = "QQ_resonance.302267_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0600_ntuples.root";
	//        string bname1 = "QQ_resonance.302271_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1000_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302269_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0800_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302270_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0900_ntuples.root";
	//	//        string bname1 =                   "QQ_resonance.302271_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1000_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302273_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1200_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302274_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1300_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302276_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1500_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302277_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1600_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302279_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1800_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302280_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1900_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302281_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m2000_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302282_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m2200_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302283_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m2400_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302285_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m2800_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302286_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m3000_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302287_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m3500_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302288_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m4000_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302889_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m4500_ntuples.root";
	//        string bname1 =                   "QQ_resonance.302290_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m5000_ntuples.root";
	//        string bname1 =                   "QQ_mainMVA.302266_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0500_ntuples.root";
	//        string bname1 =                   "QQ_mainMVA.302268_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0700_ntuples.root";
	//        string bname1 =                   "QQ_mainMVA.302272_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1100_ntuples.root";
	//        string bname1 =                   "QQ_mainMVA.302275_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1400_ntuples.root";
 	//        string bname1 =                   "QQ_mainMVA.302278_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1700_ntuples.root";
	//        string bname1 =                   "QQ_mainMVA.302284_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m2600_ntuples.root"
    

	int mass_arr[1] = {600};
    
    
    // Looping on all masses
      //    const char *savedir2[20];
    for (int i=0; i<20; i++) { // MAKE SURE THE FILES EXIST FOR EACH "i" VALUE
      int mass = mass_arr[i];
      //        int mass_num = mass_num_arr[i];
      
      // create the save directory  
      string fdir  = savedir + "m" + to_string(mass)+"/";
      int ns = fdir.length();
      char savedir2[ns+1];
      strcpy(savedir2,savedir.c_str());
      if (mkdir(savedir2,0777) == 0) 
	cout << "Directory created"; 
      //      else
      //	cerr << "Error :  " << strerror(errno) << endl; 
      
      // Creating the path for the data file
      fdir  = rootdir + "m" + to_string(mass);
      
    
      string fname = QQname[i];
      string fpath = fdir + "/" + fname;
      char const *fpath_c = fpath.c_str();
      
      cout << "  PLOTTING FOR MASS : " << mass << endl;
      cout << "Attempting to read signal file : " << fpath_c << endl;
      
      // Reading the signal file
      TFile *sfile = new TFile(fpath_c, "READ");
      if (sfile->IsZombie()) {
	std::cout << "Error opening file" << std::endl;
	continue;
      }
      TTree *data;
      sfile->GetObject("nominal", data);
      
      // Drawing the data
      TCanvas *c1 = new TCanvas("c1","c2",800,600);
      c1->Divide(2,2);
      c1->cd(1);
      cout << " ===>>> canvas - 1 " << endl;
      data->SetLineColor(99);
      data->Draw("pSignal >> pSig","(WeightNormalized)","HIST");
      TH1F *hist = (TH1F*)gDirectory->Get("pSig");
      hist->SetTitle(Form("pSignal - mass %i",mass));
      gPad->SetLogy();
      
      // Applying condtions to signal
      c1->cd(3);
      cout << " ===>>> canvas - 3 " << endl;
      data->SetLineColor(99);
      data->Draw("pSignal >> pSig_f","(WeightNormalized)*(M_jj>500)*(Deta_jj>3.5)","HIST");
      TH1F *hist_f = (TH1F*)gDirectory->Get("pSig_f");
      hist_f->SetTitle("pSignal - cut-based");
      gPad->SetLogy();
      
      // Reading the background files
      string bpath1 = fdir + "/" + bname1;
      //      string bpath2 = fdir + "/" + bname2;
      
      char const *bpath1_c = bpath1.c_str();
      //      char const *bpath2_c = bpath2.c_str();
      
      cout << "Reading background file 1 : " << bpath1_c << endl;
      //      cout << "Reading background file 2 : " << bpath2_c << endl;
      
      TFile *bfile1 = new TFile(bpath1_c, "READ");
      //      TFile *bfile2 = new TFile(bpath2_c, "READ");
      
      TTree *b1;
      TTree *b2;
      
      bfile1->GetObject("nominal", b1);
      //      bfile2->GetObject("nominal", b2);
      
      c1->cd(1);
      b1->SetLineColor(77);
      //      b2->SetLineColor(4);
      b1->Draw("pSignal >> pSig_b1","(WeightNormalized)","SAME HIST");
      //      b2->Draw("pSignal >> pSig_b2","(WeightNormalized)","SAME HIST");
      
      TH1F *hist_b1 = (TH1F*)gDirectory->Get("pSig_b1");
      //      TH1F *hist_b2 = (TH1F*)gDirectory->Get("pSig_b2");
      
      // Applying conditions to background
      c1->cd(3);
      b1->SetLineColor(77);
      //      b2->SetLineColor(4);
      b1->Draw("pSignal >> pSig_b1_f","(WeightNormalized)*(M_jj>500)*(Deta_jj>3.5)","SAME HIST");
      //      b2->Draw("pSignal >> pSig_b2_f","(WeightNormalized)*(M_jj>500)*(Deta_jj>3.5)","SAME HIST");
      
      TH1F *hist_b1_f = (TH1F*)gDirectory->Get("pSig_b1_f");
      //      TH1F *hist_b2_f = (TH1F*)gDirectory->Get("pSig_b2_f");
      
      // Initializing cut values and integrals
      const int ncv = 100;
      float cut_value[ncv];
      float signal_integral[ncv];
      float bckgd1_integral[ncv];
      float bckgd2_integral[ncv];
      float signal_integral_f[ncv];
      float bckgd1_integral_f[ncv];
      float bckgd2_integral_f[ncv];
      
      // Background chain for significance
      TChain *bkg_chain = new TChain("nominal");
      bkg_chain->Add(bpath1_c);
      //      bkg_chain->Add(bpath2_c);
      
      // Initializing significance
      float sig[ncv];
      float sig_f[ncv];
      
      for (int j=0; j<ncv; j++) {
	// Finding the bin associated with the cut value
	cut_value[j] = j/(ncv-1.);
	int cut_bin  = hist->GetXaxis()->FindBin(cut_value[j]);
	int high_bin = hist->GetXaxis()->FindBin(1);
	
	// Calculating integrals
	signal_integral[j]   = hist     ->Integral(cut_bin,high_bin);
	bckgd1_integral[j]   = hist_b1  ->Integral(cut_bin,high_bin);
	//	bckgd2_integral[j]   = hist_b2  ->Integral(cut_bin,high_bin);
	signal_integral_f[j] = hist_f   ->Integral(cut_bin,high_bin);
	bckgd1_integral_f[j] = hist_b1_f->Integral(cut_bin,high_bin);
	//	bckgd2_integral_f[j] = hist_b2_f->Integral(cut_bin,high_bin);
	
	// Events without conditions inside of mass window (NN)
	//	int bkg_events = bckgd1_integral[j]+bckgd2_integral[j];
	int bkg_events = bckgd1_integral[j];
	int sig_events = signal_integral[j];
	
	// Events with conditions inside of mass window (cut-based)
	//	int bkg_events_f = bckgd1_integral_f[0]+bckgd2_integral_f[0];
	int bkg_events_f = bckgd1_integral_f[0];
	int sig_events_f = signal_integral_f[0];
	
	// Calculating significance
	sig[j]   = AMS(sig_events, bkg_events);
	cout << "a :" << j << "  s = " << sig_events << "  b = " << bkg_events << endl;
	sig_f[j] = AMS(sig_events_f, bkg_events_f);
	cout << "b :" << j << "  s = " << sig_events << "  b = " << bkg_events << endl;
      }
      
      // Drawing the integrals
      c1->cd(2);
      cout << " ===>>> canvas -2 " << endl;
      TGraph* sig_integral = new TGraph(ncv, cut_value, signal_integral);
      sig_integral->SetName("sig_integral");
      sig_integral->SetLineWidth(2);
      sig_integral->SetLineColor(99);
      sig_integral->SetTitle("Integrals; Cut value");
      sig_integral->Draw();
      
      TGraph* b1_integral = new TGraph(ncv, cut_value, bckgd1_integral);
      b1_integral->SetName("b1_integral");
      b1_integral->SetLineWidth(2);
      b1_integral->SetLineColor(77);
      b1_integral->Draw("SAME");
      
      TGraph* b2_integral = new TGraph(ncv, cut_value, bckgd2_integral);
      b2_integral->SetName("b2_integral");
      b2_integral->SetLineWidth(2);
      b2_integral->SetLineColor(4);
      b2_integral->Draw("SAME");
      
      auto legend = new TLegend(0.12,0.12,0.38,0.29);
      legend->AddEntry("sig_integral", "signal" ,"lep");
      legend->AddEntry("b1_integral", "QCD background" ,"lep");
      legend->AddEntry("b2_integral", "EW background" ,"lep");
      legend->Draw();
      
      //        gPad->SetLogy();
      
      /*        c1->cd(5);
		TGraph* sig_integral_f = new TGraph(ncv, cut_value, signal_integral_f);
		sig_integral_f->SetName("sig_integral_f");
		sig_integral_f->SetLineWidth(2);
		sig_integral_f->SetLineColor(99);
		sig_integral_f->SetTitle("Integrals - cut-based; Cut value");
		sig_integral_f->Draw();

		TGraph* b1_integral_f = new TGraph(ncv, cut_value, bckgd1_integral_f);
		b1_integral_f->SetName("b1_integral_f");
		b1_integral_f->SetLineWidth(2);
		b1_integral_f->SetLineColor(77);
		b1_integral_f->Draw("SAME");
		
		TGraph* b2_integral_f = new TGraph(ncv, cut_value, bckgd2_integral_f);
		b2_integral_f->SetName("b2_integral_f");
		b2_integral_f->SetLineWidth(2);
		b2_integral_f->SetLineColor(4);
		b2_integral_f->Draw("SAME");
		
		//        gPad->SetLogy();    
		*/
      // Drawing significance
      c1->cd(4);
      cout << " ===>>> canvas -4 " << endl;
      TGraph* significance = new TGraph(ncv, cut_value, sig);
      significance->SetName("significance");
      significance->SetLineWidth(2);
      significance->SetLineColor(1);
      significance->SetTitle("Significance; Cut value");
      significance->Draw();
      
      TGraph* significance_f = new TGraph(ncv,cut_value,sig_f);
      significance_f->SetName("significance_f");
      significance_f->SetLineWidth(2);
      significance_f->SetLineColor(2);
      significance_f->SetTitle("Significance; Cut value");
      significance_f->Draw("SAME");
      
      auto legend2 = new TLegend(0.12,0.12,0.42,0.27);
      legend2->AddEntry("significance", "NN" ,"lep");
      legend2->AddEntry("significance_f", "cut-based" ,"lep");
      legend2->Draw();
      
      // Saving the figure as .png and .root
      //      string sfname1 = savedir + "pSig_integrals_"+model+"_m" + to_string(mass) + "_and_bckgrd" + opt_ID + ".png";
      //      string sfname2 = savedir + "pSig_integrals_"+model+"_m" + to_string(mass) + "_and_bckgrd" + opt_ID + ".root";
      string sfname1 = savedir + "m" + to_string(mass)+"/" + "pSig_integrals_"+model+"_m" + to_string(mass) + "_and_bckgrd" + opt_ID + ".png";
      string sfname2 = savedir + "m" + to_string(mass)+"/" + "pSig_integrals_"+model+"_m" + to_string(mass) + "_and_bckgrd" + opt_ID + ".C";
      char const *sfname1_c = sfname1.c_str();
      char const *sfname2_c = sfname2.c_str();
      c1->SaveAs(sfname1_c);
      c1->SaveAs(sfname2_c);
      
      c1->Close();
      cout << "--------------------------" << endl;
    }
    return 0;
}

void pSignal_cv_plotting_QQ() {main();}
