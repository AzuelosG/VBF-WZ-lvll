
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
string savedir = "ControlPlots/GM_03-02-2021/";   
string rdir   = "OutputRoot/GM_03-02-2021/";       // Subdirectory containing the root files produced by applying the NN
string model  = "GM";            // Model used GM or HVT.  For QQ, use  pSignal_cv_plotting_QQ.C
string opt_ID = "_";    // Optional file name identification
//=======================================================================

float AMS(float s, float b, bool debug=false) {
  if (s<=0 or b<=0) return 0;

  float br = 0.00001;// #KM: systematic unc?
  float sigma    = sqrt(b+br);
  float n        = s+b+br;
  float radicand = 2 *( n * log (n*(b+br+sigma)/(b*b+n*sigma+br))-b*b/sigma*log(1+sigma*(n-b)/(b*(b+br+sigma))));

  float ams= 0;
  if (radicand < 0) std::cout << "AMS: radicand is negative. Returning 0." << std::endl;
  else ams = sqrt(radicand);

  return ams;
}

int main() {
  string rootdir = rdir; 
  
  const int imaxGM=20; 
  string GMname[imaxGM] = {
    "GM_resonance.450765_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m200_ntuples.root",
    "GM_resonance.502511_aMCPy8EG_GM_H5_WZ_lvll_m225_lepfilt_ntuples.root",
    "GM_resonance.450766_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m250_ntuples.root",
    "GM_resonance.502512_aMCPy8EG_GM_H5_WZ_lvll_m275_lepfilt_ntuples.root",
    "GM_resonance.450767_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m300_ntuples.root",
    "GM_resonance.502513_aMCPy8EG_GM_H5_WZ_lvll_m325_lepfilt_ntuples.root",
    "GM_resonance.450768_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m350_ntuples.root",
    "GM_resonance.502514_aMCPy8EG_GM_H5_WZ_lvll_m375_lepfilt_ntuples.root",
    "GM_resonance.450769_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m400_ntuples.root",
    "GM_resonance.502515_aMCPy8EG_GM_H5_WZ_lvll_m425_lepfilt_ntuples.root",
    "GM_resonance.450770_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m450_ntuples.root ",
    "GM_resonance.502516_aMCPy8EG_GM_H5_WZ_lvll_m475_lepfilt_ntuples.root",
    "GM_resonance.450771_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m500_ntuples.root",
    "GM_resonance.502517_aMCPy8EG_GM_H5_WZ_lvll_m525_lepfilt_ntuples.root",
    "GM_resonance.502518_aMCPy8EG_GM_H5_WZ_lvll_m550_lepfilt_ntuples.root",
    "GM_resonance.502519_aMCPy8EG_GM_H5_WZ_lvll_m600_lepfilt_ntuples.root",
    "GM_resonance.502520_aMCPy8EG_GM_H5_WZ_lvll_m700_lepfilt_ntuples.root",
    "GM_resonance.502521_aMCPy8EG_GM_H5_WZ_lvll_m800_lepfilt_ntuples.root",
    "GM_resonance.502522_aMCPy8EG_GM_H5_WZ_lvll_m900_lepfilt_ntuples.root",
    "GM_resonance.502523_aMCPy8EG_GM_H5_WZ_lvll_m1000_lepfilt_ntuples.root"
  };
  
	//        mass           200,  225,  250,  275, 300, 325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 600, 700, 800, 900, 1000

    const int imaxHVT=11;
    string HVTname[imaxHVT] = {
      "HVT_resonance.307730_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0250_ntuples.root",
      "HVT_resonance.307731_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0300_ntuples.root",
      "HVT_resonance.309528_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0350_ntuples.root",
      "HVT_resonance.307732_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0400_ntuples.root",
      "HVT_resonance.309529_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0450_ntuples.root",
      "HVT_resonance.307733_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0500_ntuples.root",
      "HVT_resonance.307734_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0600_ntuples.root",
      "HVT_resonance.307735_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0700_ntuples.root",
      "HVT_resonance.307736_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0800_ntuples.root",
      "HVT_resonance.307737_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0900_ntuples.root",
      "HVT_resonance.307738_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m1000_ntuples.root",
    };

    // mass                 250   350   400   450   500   600    700   1000

    // background files
    string bname1, bname2;
    if(model=="GM"){
      bname1 = "GM_resonance.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.0.root";
      bname2 = "GM_resonance.364742_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_SFPlus_ntuples.root";
    }
    if(model=="HVT"){
      bname1 = "HVT_resonance.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.0.root";
      bname2 = "HVT_resonance.364742_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_SFPlus_ntuples.root";
    }
    

    // // Initial mass and mass file ID
    // int mass_num_arr[6];
    // string f_ID1;
    // string f_ID2;
    // string f_ID3;
    // if (model == "GM") {
    //     int mass_num_GM[6] = {66, 67, 69, 71, 73, 74};
    //     for (int ig=0; ig<6; ig++) {mass_num_arr[ig] = mass_num_GM[ig];}
    //     f_ID1 = "4507";
    //     f_ID2 = "_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m";
    //     f_ID3 = "";
    // }

    // if (model == "HVT") {
    //     int mass_num_HVT[6] = {30, 31, 32, 33, 35, 36};
    //     for (int ih=0; ih<6; ih++) {mass_num_arr[ih] = mass_num_HVT[ih];}
    //     f_ID1 = "3077";
    //     f_ID2 = "_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m";
    //     f_ID3 = "";
    // }

    
    
    // Looping on all masses
    //    const char *savedir2[20];
    int imax;
    int mass_arrGM[imaxGM] = {200,  225,  250,  275, 300, 325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 600, 700, 800, 900, 1000};
    int mass_arrHVT[imaxHVT] = {250,   350,   350,  400,   450,   500,   600,    700,  800, 900,  1000};
    if(model == "GM") {
      imax=imaxGM;
    }
    if(model == "HVT") {
      imax=imaxHVT; 
    }
    for (int i=0; i<imax; i++) { // MAKE SURE THE FILES EXIST FOR EACH "i" VALUE
      int mass;
      if(model=="GM") mass=mass_arrGM[i];
      if(model=="HVT") mass=mass_arrHVT[i];
      
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
      // int nr = fdir.length();
      // char fdir2[nr+1];
      // strcpy(fdir2,fdir.c_str());
      // if (mkdir(fdir2,0777) == 0) 
      // 	cout << "Directory created"; 
      //      else
      //	cerr << "Error :  " << strerror(errno) << endl; 
      
    
    // if (model=="HVT" && mass<1000) {
    //     mass_ID = "0" + mass_ID;
    // }
    //        if (model=="GM" && mass==900) {f_ID1 = "3050"; f_ID2 = "_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_"; f_ID3 = "_qcd0"; mass_num = 35;}
    //        string fname = "new_" + model + "_mainMVA." + f_ID1 + to_string(mass_num) + f_ID2 + mass_ID + f_ID3 + "_ntuples.root";
      string fname;
      if(model=="GM")
	fname = GMname[i];
      else if(model=="HVT")
	fname = HVTname[i];

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
      data->SetLineColor(99);
      if(model=="GM") data->Draw("pSignal_GM >> pSig","(WeightNormalized*WZInclusive)","HIST");
      if(model=="HVT") data->Draw("pSignal_HVT >> pSig","(WeightNormalized*WZInclusive)","HIST");
      TH1F *hist = (TH1F*)gDirectory->Get("pSig");
      hist->SetTitle(Form("pSignal - mass %i",mass));
      gPad->SetLogy();
      
      // Applying condtions to signal
      c1->cd(3);
      data->SetLineColor(99);
      if(model=="GM") data->Draw("pSignal_GM >> pSig_f","(WeightNormalized*WZInclusive)*(M_jj>500)*(Deta_jj>3.5)","HIST");
      if(model=="HVT") data->Draw("pSignal_HVT >> pSig_f","(WeightNormalized*WZInclusive)*(M_jj>500)*(Deta_jj>3.5)","HIST");
      //      data->Draw("pSignal >> pSig_f","(WeightNormalized*WZInclusive)*(M_jj>500)*(Deta_jj>3.5)","HIST");
      TH1F *hist_f = (TH1F*)gDirectory->Get("pSig_f");
      hist_f->SetTitle("pSignal - cut-based");
      gPad->SetLogy();
      
      // Reading the background files
      string bpath1 = fdir + "/" + bname1;
      string bpath2 = fdir + "/" + bname2;
      
      char const *bpath1_c = bpath1.c_str();
      char const *bpath2_c = bpath2.c_str();
      
      cout << "Reading background file 1 : " << bpath1_c << endl;
      cout << "Reading background file 2 : " << bpath2_c << endl;
      
      TFile *bfile1 = new TFile(bpath1_c, "READ");
      TFile *bfile2 = new TFile(bpath2_c, "READ");
      
      TTree *b1;
      TTree *b2;
      
      bfile1->GetObject("nominal", b1);
      bfile2->GetObject("nominal", b2);
      
      c1->cd(1);
      b1->SetLineColor(77);
      b2->SetLineColor(4);
      if(model=="GM") b1->Draw("pSignal_GM >> pSig_b1","(WeightNormalized*WZInclusive)","SAME HIST");
      if(model=="HVT") b1->Draw("pSignal_HVT >> pSig_b1","(WeightNormalized*WZInclusive)","SAME HIST");
      //      b1->Draw("pSignal >> pSig_b1","(WeightNormalized*WZInclusive)","SAME HIST");
      if(model=="GM") b2->Draw("pSignal_GM >> pSig_b2","(WeightNormalized*WZInclusive)","SAME HIST");
      if(model=="HVT") b2->Draw("pSignal_HVT >> pSig_b2","(WeightNormalized*WZInclusive)","SAME HIST");
      //      b2->Draw("pSignal >> pSig_b2","(WeightNormalized*WZInclusive)","SAME HIST");
      
      TH1F *hist_b1 = (TH1F*)gDirectory->Get("pSig_b1");
      TH1F *hist_b2 = (TH1F*)gDirectory->Get("pSig_b2");
      
      // Applying conditions to background
      c1->cd(3);
      b1->SetLineColor(77);
      b2->SetLineColor(4);
      if(model=="GM") b1->Draw("pSignal_GM >> pSig_b1_f","(WeightNormalized*WZInclusive)*(M_jj>500)*(Deta_jj>3.5)","SAME HIST");
      if(model=="HVT") b1->Draw("pSignal_HVT >> pSig_b1_f","(WeightNormalized*WZInclusive)*(M_jj>500)*(Deta_jj>3.5)","SAME HIST");
      //      b1->Draw("pSignal >> pSig_b1_f","(WeightNormalized*WZInclusive)*(M_jj>500)*(Deta_jj>3.5)","SAME HIST");
      if(model=="GM") b2->Draw("pSignal_GM >> pSig_b2_f","(WeightNormalized*WZInclusive)*(M_jj>500)*(Deta_jj>3.5)","SAME HIST");
      if(model=="HVT") b2->Draw("pSignal_HVT >> pSig_b2_f","(WeightNormalized*WZInclusive)*(M_jj>500)*(Deta_jj>3.5)","SAME HIST");
      //      b2->Draw("pSignal >> pSig_b2_f","(WeightNormalized*WZInclusive)*(M_jj>500)*(Deta_jj>3.5)","SAME HIST");
      
      TH1F *hist_b1_f = (TH1F*)gDirectory->Get("pSig_b1_f");
      TH1F *hist_b2_f = (TH1F*)gDirectory->Get("pSig_b2_f");
      
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
      bkg_chain->Add(bpath2_c);
      
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
	bckgd2_integral[j]   = hist_b2  ->Integral(cut_bin,high_bin);
	signal_integral_f[j] = hist_f   ->Integral(cut_bin,high_bin);
	bckgd1_integral_f[j] = hist_b1_f->Integral(cut_bin,high_bin);
	bckgd2_integral_f[j] = hist_b2_f->Integral(cut_bin,high_bin);
	
	// Events without conditions inside of mass window (NN)
	int bkg_events = bckgd1_integral[j]+bckgd2_integral[j];
	int sig_events = signal_integral[j];
	
	// Events with conditions inside of mass window (cut-based)
	int bkg_events_f = bckgd1_integral_f[0]+bckgd2_integral_f[0];
	int sig_events_f = signal_integral_f[0];
	
	// Calculating significance
	sig[j]   = AMS(sig_events, bkg_events);
	sig_f[j] = AMS(sig_events_f, bkg_events_f);
      }
      
      // Drawing the integrals
      c1->cd(2);
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
      string sfname3 = savedir + "m" + to_string(mass)+"/" + "pSig_integrals_"+model+"_m" + to_string(mass) + "_and_bckgrd" + opt_ID + ".root";
      char const *sfname1_c = sfname1.c_str();
      char const *sfname2_c = sfname2.c_str();
      char const *sfname3_c = sfname3.c_str();
      c1->SaveAs(sfname1_c);
      c1->SaveAs(sfname2_c);
      c1->SaveAs(sfname3_c);
      
      c1->Close();
      cout << "--------------------------" << endl;
    }
    return 0;
}

void pSignal_cv_plotting() {main();}
