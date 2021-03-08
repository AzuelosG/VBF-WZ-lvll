// g++ -std=c++17 -Ijson/include -o plotVars PlotTrainVars.cpp `root-config --cflags --glibs`

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <typeinfo>

#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TTree.h"
#include "TMath.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TChain.h"
#include "ROOT/RDataFrame.hxx"

using json = nlohmann::json;

// apply_selection(ROOT::RDataFrame df)
// {
//     auto df_jet1cut = df.Filter("Jet1Pt>0");
//     auto df_jet2cut = df_jet1cut.Filter("Jet2Pt>0");
//     auto dfselected = df_jet2cut.Filter("M_jj>100");
//     return dfselected;
// }

struct hist_settings{
    int nbins;
    float xmin;
    float xmax;
    std::string xaxistitle;
    std::string yaxistitle;
};

hist_settings GetHistSettings(std::string varname)
{
    hist_settings settings;
    
    if (varname == "Njets"){
            settings.nbins = 10;
            settings.xmin = 0;
            settings.xmax = 10;
            settings.xaxistitle = "N_{jets}";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Jet1Eta"){
            settings.nbins = 50;
            settings.xmin = -5;
            settings.xmax = 5;
            settings.xaxistitle = "Leading jet #eta";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Jet2Eta"){
            settings.nbins = 30;
            settings.xmin = -5;
            settings.xmax = 5;
            settings.xaxistitle = "Subleading jet #eta";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Jet3Eta"){
            settings.nbins = 30;
            settings.xmin = -6;
            settings.xmax = 6;
            settings.xaxistitle = "Third jet #eta";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Lep1Eta"){
            settings.nbins = 30;
            settings.xmin = -3;
            settings.xmax = 3;
            settings.xaxistitle = "Leading lepton #eta";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Lep2Eta"){
            settings.nbins = 30;
            settings.xmin = -3;
            settings.xmax = 3;
            settings.xaxistitle = "Subleading lepton #eta";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Lep3Eta"){
            settings.nbins = 30;
            settings.xmin = -3;
            settings.xmax = 3;
            settings.xaxistitle = "Third lepton #eta";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="M_jj"){
            settings.nbins = 50;
            settings.xmin = 0;
            settings.xmax = 5000;
            settings.xaxistitle = "Dijet mass [GeV]";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Dphi_jj"){
            settings.nbins = 30;
            settings.xmin = 0;
            settings.xmax = TMath::Pi();
            settings.xaxistitle = "#Delta#Phi_{jj}";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Deta_jj"){
            settings.nbins = 25;
            settings.xmin = 0;
            settings.xmax = 10;
            settings.xaxistitle = "#Delta#eta_{jj}";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Jet1Pt"){
            settings.nbins = 30;
            settings.xmin = 0;
            settings.xmax = 600;
            settings.xaxistitle = "Leading jet p_{T} [GeV]";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Jet2Pt"){
            settings.nbins = 30;
            settings.xmin = 0;
            settings.xmax = 300;
            settings.xaxistitle = "Subleading jet p_{T} [GeV]";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Jet1E"){
            settings.nbins = 50;
            settings.xmin = 0;
            settings.xmax = 5000;
            settings.xaxistitle = "Leading jet energy [Gev]";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Jet2E"){
            settings.nbins = 50;
            settings.xmin = 0;
            settings.xmax = 5000;
            settings.xaxistitle = "Subleading jet energy [Gev]";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="PtBalanceZ"){
            settings.nbins = 30;
            settings.xmin = 0;
            settings.xmax = 3;
            settings.xaxistitle = "p_{T} balance Z";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="PtBalanceW"){
            settings.nbins = 30;
            settings.xmin = 0;
            settings.xmax = 3;
            settings.xaxistitle = "p_{T} balance W";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Eta_W"){
            settings.nbins = 30;
            settings.xmin = -6;
            settings.xmax = 6;
            settings.xaxistitle = "W boson #eta";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="Eta_Z"){
            settings.nbins = 30;
            settings.xmin = -6;
            settings.xmax = 6;
            settings.xaxistitle = "Z boson #eta";
            settings.yaxistitle = "Normalized to unity";
    }
    else if (varname=="ZetaLep"){
            settings.nbins = 60;
            settings.xmin = -6;
            settings.xmax = 6;
            settings.xaxistitle = "Centrality";
            settings.yaxistitle = "Normalized to unity";
    }
    else {
            settings.nbins = 50;
            settings.xmin = 0;
            settings.xmax = 50;
            settings.xaxistitle = "Varname";
            settings.yaxistitle = "Normalized to unity";
    }
    return settings;
}

void PlotATLASLabel(TCanvas* c, std::string label)
{
    float x=0.15, y=0.87, labelOffset=0.12;
	TLatex txt;
	txt.SetTextFont(72);
	txt.SetTextSize(0.05);
	txt.DrawLatexNDC(x,y,"ATLAS");
	txt.SetTextFont(42);
	txt.DrawLatexNDC(x+labelOffset,y, label.data());

}

int main(int argc, char* argv[])
{
    enum argument {invalid = -1, mass_points = 1, model = 2, dir = 3};
    std::vector<std::string> mpoints;
    std::string mod; 
    std::string dirname;
    argument a = invalid;

    for (int i = 1; i<argc; ++i){
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
        if (argv[i] == std::string("--mass_points")){
            std::cout << "starting filling mass points " << std::endl;
            a = mass_points;
            std::cout << "a = " << a <<std::endl;
        }
        else if (argv[i] == std::string("--model")){
            a = model;
            std::cout << "a = " << a <<std::endl;
        }
        else if (argv[i] == std::string("--dir")){
            a = dir;
            std::cout << "a = " << a <<std::endl;
        }
        else {
            if (a == 1) {
                std::cout << "a = 1, filling mpoints vector" << std::endl;
                mpoints.push_back(argv[i]);
            }
            else if (a == 2) {
                std::cout << "a = 2, saving model type to mod variable" << std::endl;
                mod = argv[i];
            }
            else if (a == 3) {
                std::cout << "a = 3, saving directory name to dirname variable" << std::endl;
                dirname = argv[i];
            }
            else {
                std::cout << "Invalid argument provided, exiting..." << std::endl;
                exit(0);
            }
        }
    }
    std::cout << "Mass points: " << std::endl;
    for (auto mp : mpoints){
        std::cout << mp << "\t";
    }
    std::cout << std::endl;
    std::cout << "Model: " << mod << std::endl;
    std::cout << "Dirname: " << dirname << std::endl;
    //HBB: create the directory to save plots
    int status;
    std::string mkdir_command = std::string("mkdir -p VariablePlots/") + dirname.data();
    status = system(mkdir_command.data());
    if (status == -1)
        std::cerr << "Error : " << strerror(errno) << std::endl;
    else
        std::cout << "Directories are created" << std::endl;

    // HBB: Reading info stored in config json file: 
    std::ifstream config_file("config_NN.json");
    auto conf = json::parse(config_file);

    std::string path_to_bkg_files = conf["input_samples"]["filedir"];
    std::string path_to_sig_files = conf["input_samples"]["filedirsig"];
    // std::cout << "Relative path to sig files: " << path_to_sig_files << " and to bkg files: " << path_to_bkg_files << std::endl;
    
    std::vector<std::string> sig_files;
    // std::cout << typeid(conf["input_samples"]["sigGM"]["name"]).name() << std::endl;
    // sig_files = conf["input_samples"]["sigGM"]["name"];
    if (mod == std::string("GM")) {
        for (auto mp : mpoints){
            std::string fname = conf["input_samples"]["sigGM_map"][mp.data()];
            std::string fpath = std::string("./") + path_to_sig_files.data() + std::string("") + fname.data();
            sig_files.push_back(fpath);
        }
    }
    else if (mod == std::string("HVT")){
        for (auto mp : mpoints){
            std::string fname = conf["input_samples"]["sigHVT_map"][mp.data()];
            std::string fpath = std::string("./") + path_to_sig_files.data() + std::string("") + fname.data();
            sig_files.push_back(fpath);
        }
    }
    else {std::cout << "Provide GM or HVT model to plot training variables, exiting..." << std::endl; exit(0);}
    std::cout << "sig_files size " << sig_files.size() << std::endl;
    for (auto f : sig_files){
        std::cout << "Using the following file: " << f << std::endl;
    }

    // std::vector<std::string> sig_GM_files = conf["input_samples"]["sigGM"]["name"];
    std::vector<std::string> bkg_files = conf["input_samples"]["bckgr"]["name"];
    std::vector<std::string> vars = conf["input_samples"]["variables"];
    // std::cout << conf["input_samples"]["variables"] << std::endl;
    // std::cout << typeid(conf["input_samples"]["variables"]).name() << std::endl;
    // std::cout << "variable vector length = " << vars.size() << std::endl;
    // std::cout << "Sig GM files vector length = " << sig_GM_files.size() << std::endl;
    // std::cout << "Signal files:" << std::endl;
    // for (auto sfile : sig_GM_files){
    //     std::cout << "Current file: " << sfile << std::endl;
    // }
    // std::cout << "----------------------------" << std::endl;
    // std::cout << "Background files:" << std::endl;
    // for (auto bfile : bkg_files){
    //     std::cout << "Current file: " << bfile << std::endl;
    // }
    std::cout << "Training variables:" << std::endl;
    for (auto var : vars){
        std::cout << var << "\t";
    }
    std::cout << std::endl;

    //retrieve the nominal tree from background samples
    std::string fbkgname = path_to_bkg_files.data() + std::string("") + bkg_files.at(0).data();
    std::cout << "Bkg file to open: " << fbkgname << std::endl;
    TFile *fbkg = TFile::Open(fbkgname.data());
    TTree *tbkg = (TTree*)fbkg->Get("nominal");

    TCanvas* c = new TCanvas("c", "", 800, 600);
    c->SetMargin(0.12, 0.02, 0.12, 0.03);
    c->SetTicks();
    gStyle->SetOptStat(0);

    // HBB: Using RDataFrames for easier handling root files
    //background
    ROOT::RDataFrame bdf("nominal", fbkgname);
    std::cout << "bkgfile entries (so before cut): " << tbkg->GetEntries() << std::endl;
    auto bdfselected = bdf.Filter("Jet1Pt>0 && Jet2Pt>0 && M_jj>100");
    std::cout << typeid(bdfselected).name() << std::endl;
    auto bdfsel_aug =  bdfselected.Define("WeightFinalized", [](float w1, int w2) { return w1 * w2; }, {"WeightNormalized", "WZInclusive"});

    //signal
    ROOT::RDataFrame sdf("nominal", sig_files);
    // std::cout << "sigfile entries (so before cut): " << tsig->GetEntries() << std::endl;
    auto sdfselected = sdf.Filter("Jet1Pt>0 && Jet2Pt>0 && M_jj>100");
    std::cout << typeid(sdfselected).name() << std::endl;
    auto sdfsel_aug =  sdfselected.Define("WeightFinalized", [](float w1, int w2) { return w1 * w2; }, {"WeightNormalized", "WZInclusive"});

   //retrieve scale factors from a plot with a good range 
   //first signal
    float sscale = 1.;
    auto shnjets = sdfsel_aug.Histo1D({"shnjets", "", 10, 0, 10}, "Njets", "WeightFinalized");
    std::string sscale_str = std::to_string(1./(shnjets->Integral(0, shnjets->GetNbinsX()+1)));
    sscale = stof(sscale_str);
    //then background
    float bscale = 1.;
    auto bhnjets = bdfsel_aug.Histo1D({"bhnjets", "", 10, 0, 10}, "Njets", "WeightFinalized");
    std::string bscale_str = std::to_string(1./(bhnjets->Integral(0, bhnjets->GetNbinsX()+1)));
    bscale = stof(bscale_str);

    TLegend leg(0.7, 0.76, 0.9, 0.92);
    leg.SetFillStyle(0);
    leg.SetBorderSize(0);
    leg.SetTextSize(0.048);
    leg.SetTextFont(42);

    for (int i=0; i<vars.size(); ++i){
        hist_settings settings;
        settings = GetHistSettings(vars.at(i));
        int nbins = settings.nbins;
        float xmin = settings.xmin;
        float xmax = settings.xmax;
        std::string xaxistitle = settings.xaxistitle;
        std::string yaxistitle = settings.yaxistitle;

        auto shist = sdfsel_aug.Histo1D({"shist", "", nbins, xmin, xmax}, vars.at(i), "WeightFinalized");
        // std::cout << typeid(shist).name() << std::endl;
        auto bhist = bdfsel_aug.Histo1D({"bhist", "", nbins, xmin, xmax}, vars.at(i), "WeightFinalized");

        //clear canvas and legend before plotting
        c->Clear();
        leg.Clear();

        //scale histograms to corresponding scale factors
        shist->Scale(sscale);
        bhist->Scale(bscale);

        //set y axis range
        float ymax = 0;
        float ymaxs = shist->GetBinContent(shist->GetMaximumBin());
        float ymaxb = bhist->GetBinContent(bhist->GetMaximumBin());
        ymaxs>ymaxb ? ymax=ymaxs : ymax=ymaxb;

        //plotting settings
        shist->SetLineColor(kOrange+10);
        shist->SetLineWidth(2);
        shist->GetXaxis()->SetTitle(xaxistitle.data());
        shist->GetYaxis()->SetTitle(yaxistitle.data());
        shist->GetYaxis()->SetRangeUser(0, ymax*1.3);
        shist->GetXaxis()->SetTitleFont(42);
        shist->GetXaxis()->SetTitleSize(0.05);
        shist->GetXaxis()->SetTitleOffset(0.96);
        shist->GetXaxis()->SetLabelSize(0.05);
        shist->GetYaxis()->SetTitleFont(42);
        shist->GetYaxis()->SetLabelSize(0.05);
        shist->GetYaxis()->SetTitleSize(0.05);
        shist->GetYaxis()->SetTitleOffset(1.14);
        shist->Draw("hist");

        bhist->SetLineColor(kAzure+1);
        bhist->SetLineWidth(2);
        bhist->Draw("hist same");

        //add entries to legend
        leg.AddEntry("shist", "Signal", "L");
        leg.AddEntry("bhist", "Background", "L");
        leg.Draw();

        PlotATLASLabel(c, "Internal");

        std::string cname = std::string("./VariablePlots/")+dirname.data()+std::string("/")+vars.at(i).data()+std::string(".pdf");
        c->SaveAs(cname.data());
        
    }


    //Close root files
    // fsig->Close();
    fbkg->Close();
}
