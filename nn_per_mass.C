#include <TString.h>
#include <TFile.h>
#include <TMath.h>
#include <TTree.h>
#include <TLine.h>
#include <TColor.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TChain.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TDirectory.h>
#include <iostream>
#include <unordered_map>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

string idir, tmass, sdir;
//
//
// ======================================================

string get_file_name(int mass, string phys_model="GM") {
  //string insert_str="main";
  string              file_path="OutputRoot/"+sdir+phys_model+"_";
  if      (mass==200) file_path+="resonance.450765_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m200_ntuples.root";
  else if (mass==225) file_path+="resonance.502511_aMCPy8EG_GM_H5_WZ_lvll_m225_lepfilt_ntuples.root";
  else if (mass==250) file_path+="resonance.450766_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m250_ntuples.root";
  else if (mass==275) file_path+="resonance.502512_aMCPy8EG_GM_H5_WZ_lvll_m275_lepfilt_ntuples.root";
  else if (mass==300) file_path+="resonance.450767_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m300_ntuples.root";
  else if (mass==325) file_path+="resonance.502513_aMCPy8EG_GM_H5_WZ_lvll_m325_lepfilt_ntuples.root";
  else if (mass==350) file_path+="resonance.450768_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m350_ntuples.root";
  else if (mass==375) file_path+="resonance.502514_aMCPy8EG_GM_H5_WZ_lvll_m375_lepfilt_ntuples.root";
  else if (mass==400) file_path+="resonance.450769_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m400_ntuples.root";
  else if (mass==425) file_path+="resonance.502515_aMCPy8EG_GM_H5_WZ_lvll_m425_lepfilt_ntuples.root";
  else if (mass==450) file_path+="resonance.450770_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m450_ntuples.root";
  else if (mass==475) file_path+="resonance.502516_aMCPy8EG_GM_H5_WZ_lvll_m475_lepfilt_ntuples.root";
  else if (mass==500) file_path+="resonance.450771_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m500_ntuples.root";
  else if (mass==525) file_path+="resonance.502517_aMCPy8EG_GM_H5_WZ_lvll_m525_lepfilt_ntuples.root";
  else if (mass==550) file_path+="resonance.502518_aMCPy8EG_GM_H5_WZ_lvll_m550_lepfilt_ntuples.root";
  else if (mass==600) file_path+="resonance.502519_aMCPy8EG_GM_H5_WZ_lvll_m600_lepfilt_ntuples.root";
  else if (mass==700) file_path+="resonance.502520_aMCPy8EG_GM_H5_WZ_lvll_m700_lepfilt_ntuples.root";
  else if (mass==800) file_path+="resonance.502521_aMCPy8EG_GM_H5_WZ_lvll_m800_lepfilt_ntuples.root";
  else if (mass==900) file_path+="resonance.502522_aMCPy8EG_GM_H5_WZ_lvll_m900_lepfilt_ntuples.root";
  else if (mass==1000) file_path+="resonance.502523_aMCPy8EG_GM_H5_WZ_lvll_m1000_lepfilt_ntuples.root";

  if (phys_model=="HVT") {
    file_path="OutputRoot/"+sdir+phys_model+"_";
    if      (mass== 250) file_path+="resonance.307730_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0250_ntuples.root";
    else if (mass== 300) file_path+="resonance.307731_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0300_ntuples.root";
    else if (mass== 350) file_path+="resonance.309528_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0350_ntuples.root";
    else if (mass== 400) file_path+="resonance.307732_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0400_ntuples.root";
    else if (mass== 450) file_path+="resonance.309529_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0450_ntuples.root";
    else if (mass== 500) file_path+="resonance.307733_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0500_ntuples.root";
    else if (mass== 600) file_path+="resonance.307734_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0600_ntuples.root";
    else if (mass== 700) file_path+="resonance.307735_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0700_ntuples.root";
    else if (mass== 800) file_path+="resonance.307736_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0800_ntuples.root";
    else if (mass== 900) file_path+="resonance.307737_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0900_ntuples.root";
    else if (mass==1000) file_path+="resonance.307738_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m1000_ntuples.root";
  }

  if (phys_model=="QQ") {
    file_path="OutputRoot/"+sdir+phys_model+"_";
    if      (mass== 250) file_path+="resonance.307730_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0250_ntuples.root";
    else if (mass== 300) file_path+="resonance.307731_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0300_ntuples.root";
    else if (mass== 350) file_path+="resonance.309528_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0350_ntuples.root";
    else if (mass== 400) file_path+="resonance.307732_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0400_ntuples.root";
    else if (mass== 450) file_path+="resonance.309529_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0450_ntuples.root";
    else if (mass== 500) file_path+="resonance.307733_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0500_ntuples.root";
    else if (mass== 600) file_path+="resonance.307734_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0600_ntuples.root";
    else if (mass== 700) file_path+="resonance.307735_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0700_ntuples.root";
    else if (mass== 800) file_path+="resonance.307736_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0800_ntuples.root";
    else if (mass== 900) file_path+="resonance.307737_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0900_ntuples.root";
    else if (mass==1000) file_path+="resonance.307738_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m1000_ntuples.root";
  }

  return file_path;
}
  
// ======================================================

float width=0.4;
float m_lo=1-width,m_up=1+width;

string QQ_filename(int mass) {
  // HVT's qq-fusion ntuples' filenames per mass
  string insert_str="main";
  //  string               file_path="OutputRoot/"+sdir+"/new_QQ_"+insert_str;
  string               file_path="OutputRoot/"+sdir+"/QQ_";
  if      (mass==300) file_path+="resonance.302274_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1300_ntuples.root";
  else if(mass==500) file_path+="resonance.302266_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0500_ntuples.root";
  else if (mass==600)  file_path+="resonance.302267_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0600_ntuples.root";
  else if(mass==700)   file_path+="resonance.302268_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0700_ntuples.root";
  else if(mass==800)   file_path+="resonance.302269_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0800_ntuples.root";
  else if(mass==900)   file_path+="resonance.302270_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0900_ntuples.root";
  else if (mass==1000) file_path+="resonance.302271_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1000_ntuples.root";
  return file_path;
}

  // ======================================================

int get_color(int mass) {

  int color = kBlack;
  if      (mass==200 or mass== 450 or mass==1100)  color=kBlue-5;
  else if (mass==225 or mass== 475 or mass==1200)  color=kGray+2;
  else if (mass==250 or mass== 500 or mass==1300)  color=kMagenta;
  else if (mass==275 or mass== 525 or mass==1400)  color=kBlue;
  else if (mass==300 or mass== 550 or mass==1500)  color=kCyan+1;
  else if (mass==325 or mass== 600 or mass==1600)  color=kGreen+3; 
  else if (mass==350 or mass== 700 or mass==1700)  color=kYellow+2;
  else if (mass==375 or mass== 800 or mass==1800)  color=kOrange;
  else if (mass==400 or mass== 900 or mass==1900)  color=kRed;
  else if (mass==425 or mass==1000 or mass==2000)  color=kRed+3;

  return color;
}

// index           = 0
// Yields          = 0
// isMC            = 0
// Channel         = 0
// Year            = 0
// NormSF          = 0
// WeightNormalized = 0
// Weight          = 0
// PtReweight      = 0

TString title, proj_str,select_weight;
TString proj_option="";
int nbins = 50; float xmin =0, xmax = 1;

// ======================================================

TH1F* get_bkg_hist(TString phys_model="GM") {
  TChain* chain = new TChain("nominal");
  //  TString ins_str="main";
  //chain->Add("OutputRoot/new_GM_"+ins_str+"MVA.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.root");

  //  chain->Add(((TString)"OutputRoot/")+sdir.data()+"/new_"+phys_model+"_"+ins_str+"MVA.361292_MGaMcAtNloPy8EG_NNPDF30LO_A14NNPDF23LO_WZ_lvll_FxFx_ntuples.root");
  //   chain->Add(((TString)"OutputRoot/")+sdir.data()+"/new_"+phys_model+"_"+ins_str+"MVA.364284_Sherpa_222_NNPDF30NNLO_lllvjj_EW6_ntuples.root");
  if(phys_model=="QQ"){
    chain->Add( ((TString)"OutputRoot/")+sdir+phys_model+"_"+"resonance.302267_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0600_ntuples.root"); // 600 GeV HVT qq
  }
  else{
    chain->Add( ((TString)"OutputRoot/")+sdir+phys_model+"_"+"resonance.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.0.root");
    chain->Add( ((TString)"OutputRoot/")+sdir+phys_model+"_"+"resonance.364742_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_SFPlus_ntuples.root");
  }
  TH1F* hist = new TH1F("bkg",title,nbins,xmin,xmax);
  chain->Project(hist->GetName(),proj_str,select_weight,proj_option);

  return hist;
}

// ======================================================
TH1F* get_hist(int mass,TString phys_model="GM", bool qqplot=false) {

  TH1F* hist;
  if (mass>0) {
    string fname=get_file_name(mass,phys_model.Data());
    if (qqplot==true) fname = QQ_filename(mass);
    //    cout << "fname = " << fname << endl;
    TFile* f = TFile::Open(fname.data(),"read");
    TTree* t = (TTree*)f->Get("nominal");
    
    TString histName = "mass"+TString::Itoa(mass,10);
    //    cout<< "histName = " << histName<<std::endl;

    hist = new TH1F(histName ,title,nbins,xmin,xmax);
    //select_weight += "*(abs(Weight)<10)";//No longer needed
    t->Project(hist->GetName(),proj_str,select_weight,proj_option);
  }

  else {
    hist = get_bkg_hist(phys_model.Data());
  }
  hist->SetLineColor(get_color(mass));
  hist->SetLineWidth(3);

  return hist;
}

// ======================================================

// float AMS_old(float s, float b, bool debug=false) {
//   // Older version of AMS the includes some strange terms in its log
//   if (s<=0 or b<=0) return 0;

//   float br = 0.00001;// #KM: systematic unc?
//   float sigma=sqrt(b+br);
//   float n=s+b+br;
//   float radicand = 2 *( n * log (n*(b+br+sigma)/(b*b+n*sigma+br))-b*b/sigma*log(1+sigma*(n-b)/(b*(b+br+sigma))));

//   float ams= 0;
//   if (radicand < 0) std::cout<<"AMS: radicand is negative. Returning 0."<<std::endl;
//   else       ams= sqrt(radicand);

//   if (debug) std::cout<<"s, b="<<s<<"\t"<<b<<", ams="<<ams<<std::endl;

//   return ams;
// }

// ======================================================

float AMS(float s, float b, bool debug=false, float br=0) {
  float rad=0.;
  float temp = 1+(s/(b+br));
  if(temp >0) {
    rad = 2*((s+b+br)*log(1+(s/(b+br)))-s);
  }
  if (rad>=0 && rad<1000) return sqrt(rad);
  else {
    //    cout << "AMS: radicand is negative. Returning 0" << endl; 
    return 0;}
}

// ======================================================


float Nsig_ocv, Nbkg_ocv, Nsig_cv, Nbkg_cv, AMS_cv;
int tmCV;
TH1F* get_significance_hist(TH1F* h_sig, TH1F* h_bkg, float sf=1, bool is_tm=false) {
  //  cout << " ==>>>> creating h_sig " << endl;  
  TString hname="significance_";
  TH1F* significance = (TH1F*) h_sig->Clone(hname+h_sig->GetName());
  significance->Reset();
  significance->SetTitle("Significance for yield / 140fb-1");

  h_sig->Scale(sf);
  h_bkg->Scale(sf);
  
  float Nsig[significance->GetNbinsX()], Nbkg[significance->GetNbinsX()];

  for( int i=0; i<significance->GetNbinsX()+1; i++) {
    Nsig[i] = h_sig->Integral(i,h_sig->GetNbinsX());
    Nbkg[i] =  h_bkg->Integral(i,h_bkg->GetNbinsX());
    //    cout << "i = " << i <<","<< Nsig[i] <<","<< Nbkg[i] <<","<< endl;
    significance->SetBinContent(i, AMS( Nsig[i], Nbkg[i]) );
  }
  
  //bin number of optimal cut
  int ocv_bin = significance->GetMaximumBin();
  Nsig_ocv = Nsig[ocv_bin];
  Nbkg_ocv = Nbkg[ocv_bin];

  std::cout<<". Optimal cut for this mass: " << significance->GetBinLowEdge(ocv_bin)<<std::endl;

  if (is_tm) tmCV = ocv_bin;
  Nsig_cv = Nsig[tmCV];
  Nbkg_cv = Nbkg[tmCV];
  AMS_cv  = significance->GetBinContent(tmCV);

  return significance;
}

// ======================================================

void nn_per_mass(string dir="", string name="",TString varname="pSignal_GM",bool norm2yield=true, TString phys_model="GM", bool drawCB=true, bool mMulti=true) {

  //if (norm2yield) mfac=20;
  if (!norm2yield) proj_option="norm"; //normalize to 1

  idir  = dir;   // first input argument
  tmass = name;

  sdir  = idir+ "/"+(tmass=="mMulti" ? "" : tmass+"/");

  if      (varname .Contains("pSignal") ) title="NN output : "+tmass, proj_str=varname, nbins = 100, xmin =0, xmax = 1;
  else if (varname == "M_WZ"        ) title=varname, proj_str=varname, nbins = 25, xmin =0, xmax = 1500;
  else if (varname == "M_jj"        ) title=varname, proj_str=varname, nbins = 50, xmin =0, xmax = 1500;
  else if (varname == "ZetaLep"     ) title=varname, proj_str=varname, nbins = 50, xmin =-3.5, xmax = 3.5;
  else if (varname == "DY_jj"       ) title=varname, proj_str=varname, nbins = 50, xmin =0, xmax = 10;
  else if (varname == "Deta_jj"     ) title=varname, proj_str=varname, nbins = 50, xmin =0, xmax = 10;
  else if (varname == "Dphi_jj"     ) title=varname, proj_str=varname, nbins = 50, xmin =0, xmax = TMath::Pi();
  else if (varname == "Meff"        ) title=varname, proj_str= "Pt_W+Pt_Z+Jet1Pt+Jet2Pt+Met", nbins = 50, xmin =0, xmax = 2000;
  else if (varname == "dEta_WZ"     ) title=varname, proj_str= "abs(Eta_Z-Eta_W)", nbins = 50, xmin =0, xmax = 5;
  else if (varname == "Eta_Z"       ) title=varname, proj_str= varname, nbins = 50, xmin =-3, xmax = 3;
  else if (varname == "Eta_W"       ) title=varname, proj_str= varname, nbins = 50, xmin =-3, xmax = 3;
  else if (varname == "Pt_Z"        ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 1000;
  else if (varname == "Pt_W"        ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 1000;
  else if (varname == "Met"         ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 500;
  else if (varname == "Njets"       ) title=varname, proj_str= varname, nbins = 5 , xmin =2, xmax = 7;
  else if (varname == "NBjets"      ) title=varname, proj_str= varname, nbins = 5 , xmin =0, xmax = 5;
  else if (varname == "PtBalanceZ"  ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 2;
  else if (varname == "PtBalanceW"  ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 2;
  else if (varname == "ZetaLep"     ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 10;

  else if (varname == "Jet1E"       ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 1000;
  else if (varname == "Jet2E"       ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 500;
  else if (varname == "Jet3E"       ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 300;
  else if (varname == "Jet1Pt"      ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 300;
  else if (varname == "Jet2Pt"      ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 300;
  else if (varname == "Jet3Pt"      ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 200;
  else if (varname == "Jet1Eta"     ) title=varname, proj_str= varname, nbins = 50, xmin =-5, xmax = 5;
  else if (varname == "Jet2Eta"     ) title=varname, proj_str= varname, nbins = 50, xmin =-5, xmax = 5;
  else if (varname == "Jet3Eta"     ) title=varname, proj_str= varname, nbins = 50, xmin =-5, xmax = 5;

  else if (varname == "Lep1Pt"      ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 500;
  else if (varname == "Lep2Pt"      ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 500;
  else if (varname == "Lep3Pt"      ) title=varname, proj_str= varname, nbins = 50, xmin =0, xmax = 300;
  else if (varname == "Lep1Eta"     ) title=varname, proj_str= varname, nbins = 50, xmin =-2.5, xmax = 2.5;
  else if (varname == "Lep2Eta"     ) title=varname, proj_str= varname, nbins = 50, xmin =-2.5, xmax = 2.5;
  else if (varname == "Lep3Eta"     ) title=varname, proj_str= varname, nbins = 50, xmin =-2.5, xmax = 2.5;

  // Jet1Phi // Jet1Y           // Mt_WZ
  // Jet2Phi // Jet2Y           // M_Z
  // Jet3Phi // Jet3Y           

  // Mass points for GM
  vector<int> masses{0, 200,225,250,275, 300,325,350,375, 400,425,450,475, 500,525,550, 600,700,800,900,1000}; // background + masses for which trained NN was applied

  if (phys_model=="QQ") masses={0,600};
  int      hms = masses.size()/2+1;
  //const int ms = masses.size();
  
  TCanvas* c1 = new TCanvas ("name", "title", 1200, 600);
  c1->Divide(2,1);
  
  c1->cd(1);
  auto legend1 = new TLegend(0.6,0.6,0.9,0.9);
  legend1->SetHeader("Mass (GeV)","C"); 
  legend1->SetFillStyle(0); 
  legend1->SetLineWidth(0); 
  legend1->SetNColumns(2);
  
  c1->cd(2);
  auto legend2 = new TLegend(0.6,0.6,0.9,0.9);
  legend2->SetHeader("Mass (GeV)","C"); 
  legend2->SetFillStyle(0); 
  legend2->SetLineWidth(0); 
  legend2->SetNColumns(2);

  unordered_map<int,TH1F*> hists;
  unordered_map<int,TH1F*> hists_bkg;
  unordered_map<int,TH1F*> hists_bkg_cb;
  unordered_map<int,TH1F*> hists_cb;

  c1->cd(1); 
  auto legend=legend1;
  //  TH1F* hist;   // GA: commented out
  TString smass;
  
  for (auto mass : masses) {
    
    select_weight = "(M_jj>100)";
    if (norm2yield) select_weight += "*WeightNormalized*WZInclusive";   
    
    //Separating the curves on 2 figures
    if (mass==masses[hms]) {
      legend->Draw();
      gStyle->SetOptStat(0);
      if (varname.Contains("pSignal") and norm2yield) gPad->SetLogy();
      c1->cd(2); 
      legend=legend2; 
    
      //Plotting background on second figure
      TH1F* hist = get_hist(0,phys_model.Data());
      TString option="hist";
      hist->Draw(option);
      hists[mass]  = hist;  // GA: added
      //      cout << " hists[mass] , mass = " << mass << endl;
      smass= "BKG";
      legend->AddEntry(hist,smass,"f");
    }

    if (mass != 0) select_weight += Form("*(M_WZ>(%i*%f)*(M_WZ<(%i*%f)))",mass,m_lo,mass,m_up);
    // Background histogram
    hists_bkg[mass] = get_hist(0,phys_model.Data());
    //    cout << " hists_bkg[mass] , mass = " << mass << endl;
    // Current mass histogram
    TH1F* hist = get_hist(mass,phys_model.Data());  
    //    cout << " hists[mass] , mass = " << mass << endl;
    hists[mass]  = hist;

    if (mass != 0) {
      // Cut-based selection histogram
      select_weight = Form("(M_jj>500)*(Deta_jj>3.5)*(M_WZ>(%i*%f)*(M_WZ<(%i*%f)))",mass,m_lo,mass,m_up);
      if (norm2yield) select_weight += "*WeightNormalized*WZInclusive";
      hists_bkg_cb[mass] = get_hist(0,phys_model.Data());
      hists_cb[mass] = get_hist(mass,phys_model.Data());
    }

    // Drawing the curve
    TString option="same hist";
    if (mass==0) option="hist";

    hist->Draw(option);

    if (mass != 0) smass = TString::Itoa(mass,10);
    else           smass = "bkg";
    legend->AddEntry(hist,smass,"f");
  }

  if (varname.Contains("pSignal") and norm2yield) gPad->SetLogy();
  gStyle->SetOptStat(0);
  legend->Draw();

  //  string imagePath = "ControlPlots/"+idir+"/NN_output/"+varname.Data() + (idir!="" ? "_"+idir : "") + (tmass!="" ? "_"+tmass : "");
  string imagePath = "ControlPlots/"+idir + "/" + (tmass!="" ? tmass : "") + "/NN_output_"+varname.Data() ;

  c1->SaveAs((imagePath+".png" ).data());
  c1->SaveAs((imagePath+".pdf").data());

  //  cout << "  ==>>>> norm2yield = " << norm2yield << ",  varname = " << varname << endl;

  if (not (norm2yield and varname.Contains("pSignal") )) return;
  auto c2 = new TCanvas("c2","title",1200,600);
 
  c2->Divide(2,1);
  c2->cd(1);

  // CB vs NN table initializing
  //  ofstream cnfile("ControlPlots/"+idir+"/NN_output/CbvsNN"+ (idir!="" ? "_"+idir : "") + (tmass!="" ? "_"+tmass : "")+".csv");
  ofstream cnfile("ControlPlots/"+idir+ (tmass!="" ? "/"+tmass : "") +"/NN_output_CbvsNN"+".csv");
  map<int, float> sig_CB, sig_NN_ocv, bkg_CB, bkg_NN_ocv, ams_CB, ams_NN_ocv;
  map<int, float> sig_NN_cv, bkg_NN_cv, ams_NN_cv;

  if (mMulti==false)
    auto significance = get_significance_hist(hists[stoi(tmass.substr(1,4))], hists_bkg[stoi(tmass.substr(1,4))], 1., true);
  
  auto firstMassPoint=true;
  for (auto mass : masses) {

    if (mass==0) continue;
    cnfile << "," << mass << "," << mass;
    if (mMulti==false) cnfile << "," << mass;
    if (mass==masses[hms]) {
      legend1->Draw();
      c2->cd(2);
    }

    // Signal, background and significance
    std::cout<<"Calculating significance curve for mass: "<<mass;

    auto significance = get_significance_hist(hists[mass],hists_bkg[mass],1,firstMassPoint);
    sig_NN_ocv[mass] = Nsig_ocv;
    bkg_NN_ocv[mass] = Nbkg_ocv;
    ams_NN_ocv[mass] = significance->GetBinContent(significance->GetMaximumBin());
    sig_NN_cv[mass]  = Nsig_cv;
    bkg_NN_cv[mass]  = Nbkg_cv;
    ams_NN_cv[mass]  = AMS_cv;

    TString option="same hist";
    if (mass==masses[1] || mass==hms) option="hist";

    // Drawing significance curve
    significance->SetLineColor(get_color(mass));
    significance->SetLineWidth(3);
    significance->Draw(option);

    // Comparing with Cut-based
    if (drawCB) {
      float Nsig = hists_cb[mass]->Integral();
      float Nbkg = hists_bkg_cb[mass]->Integral();
      float cb_ams = AMS(Nsig, Nbkg);

      TLine *line = new TLine(0,cb_ams,1,cb_ams);
      line->SetLineColor(get_color(mass));
      line->SetLineStyle(7);
      line->Draw("same hist");

      bkg_CB[mass] = Nbkg;
      sig_CB[mass] = Nsig;
      ams_CB[mass] = cb_ams;
      significance->SetMaximum(30);
    }
    firstMassPoint=false;
  }
  legend2->Draw();

  //  string signPath = "ControlPlots/"+idir+"/NN_output/significance" + (idir!="" ? "_"+idir : "") + (tmass!="" ? "_"+tmass : "");
  //  string signPath = "ControlPlots/"+idir+"/NN_output_significance" + (idir!="" ? "_"+idir : "") + (tmass!="" ? "_"+tmass : "");
  string signPath = "ControlPlots/"+idir + "/" + (tmass!="" ? tmass : "") + "/NN_output_significance" ;
  c2->SaveAs((signPath+".png" ).data());
  c2->SaveAs((signPath+".pdf").data());

  // Plotting the significance per mass and method
  auto c3 = new TCanvas("c3","title",1000,800);
  auto ams_legend = new TLegend(0.6,0.6,0.9,0.9);
  map<int, float> ams_TD;
  TString ams_curve_opt;
  TString label;
  TString ams_title = "Significance per mass and method - "+tmass;
  for (int l=0; l<3; l++) {
    auto ams_curve = new TGraph();
    if (l==0)                       {ams_TD = ams_NN_ocv; ams_curve_opt = "APL"; ams_curve->SetMarkerStyle(20); label="NN optimal cut(m)";}
    else if (l==1)                  {ams_TD = ams_NN_cv;  ams_curve_opt = "PL";  ams_curve->SetMarkerStyle(21); label="NN cut ("+TString::Itoa(masses[1],10)+" GeV)";}
    else if (l==2)                  {ams_TD = ams_CB;     ams_curve_opt = "PL";  ams_curve->SetMarkerStyle(23); label="Cut-Based";}
    else continue;

    ams_curve->SetLineColor(2+l);
    ams_curve->SetMarkerColor(2+l);
    ams_curve->SetTitle(ams_title);

    int k = 0;
    for (auto mass : masses) {
      if (mass == 0) continue;
      ams_curve->SetPoint(k, mass, ams_TD[mass]);
      k++;
    }
    ams_curve->SetMinimum(0);
    if (mMulti==false && phys_model=="HVT" && (stoi(tmass.substr(1,4))>400)) {c3->SetLogy(); ams_curve->SetMinimum(10E-3);}
    ams_curve->SetMaximum(25);
    ams_curve->SetMinimum(-0.8);
    if (phys_model=="QQ") ams_curve->SetMaximum(2);
    if (phys_model=="QQ") ams_curve->SetMinimum(-0.1);
    ams_curve->Draw(ams_curve_opt);
    ams_legend->AddEntry(ams_curve,label,"lep");
  }
  ams_legend->Draw();
  
  c3->SaveAs(("ControlPlots/"+idir+ "/" + (tmass!="" ? tmass : "") + "/NN_output_AMS_curves" +".png").data());
  c3->SaveAs(("ControlPlots/"+idir+ "/" + (tmass!="" ? tmass : "") + "/NN_output_AMS_curves" +".pdf").data());;
  //  c3->SaveAs(("ControlPlots/"+idir+"/NN_output_AMS_curves" + (idir!="" ? "_"+idir : "") + (tmass!="" ? "_"+tmass : "") + ".root").data());
  
  //========================================= 
  if (phys_model=="QQ"){
    auto c4 = new TCanvas("c4","title",800,400);
    c4->Divide(2,1);
    c4->cd(1);
    
    for (auto mass : masses) {
      select_weight = "(M_jj>100)";
      if (norm2yield) select_weight += "*WeightNormalized*WZInclusive";
      
      //Separating the curves on 2 figures
      if (mass==masses[hms]) {
	legend1->Draw();
	gStyle->SetOptStat(0);
	if (varname.Contains("pSignal") and norm2yield) gPad->SetLogy();
	c4->cd(2);
      }
      
      if (mass != 0) select_weight += Form("*(M_WZ>(%i*%f)*(M_WZ<(%i*%f)))",mass,m_lo,mass,m_up);
      // Current mass histogram
      
      TH1F* hist = get_hist(mass,phys_model.Data(),true);
      
      // Drawing the curve
      TString option="same hist";
      if (mass==0) option="hist";
      hist->Draw(option);
    }
    legend2->Draw();
    c4->SaveAs(("ControlPlots/"+idir+ "/" + (tmass!="" ? tmass : "") +"/NN_output_QQ_pSignal" + ".png").data());
    c4->SaveAs(("ControlPlots/"+idir+ "/" + (tmass!="" ? tmass : "") +"/NN_output_QQ_pSignal" + ".pdf").data());
  }
  
  // Filling CSV file of number of signal and bakcground, and significance by method
  cnfile << endl;
  for (int j=0; j<4; j++) {
    if (j==0) cnfile << ",";
    if (j==1) cnfile << "Signal,";
    if (j==2) cnfile << "Background,";
    if (j==3) cnfile << "Significance,";
    for (auto mass : masses) {
      if (mass == 0) continue;
      if (j==0) cnfile << "CB,NN_ocv" << (mMulti==false ? ",NN_tcv" : "");
      if (j==1) { cnfile << sig_CB[mass] << "," << sig_NN_ocv[mass];
        if (mMulti==false) cnfile << "," << sig_NN_cv[mass];
        if (mass!=masses[masses.size()-1]) cnfile << ",";
      }
      if (j==2) { cnfile << bkg_CB[mass] << "," << bkg_NN_ocv[mass];
        if (mMulti==false) cnfile << "," << bkg_NN_cv[mass];
        if (mass!=masses[masses.size()-1]) cnfile << ",";
      }
      if (j==3) { cnfile << ams_CB[mass] << "," << ams_NN_ocv[mass];
        if (mMulti==false) cnfile << "," << ams_NN_cv[mass];
        if (mass!=masses[masses.size()-1]) cnfile << ",";
      }
    }
    cnfile << endl;
  }
  cnfile.close();

  return;
 
}

