// g++ -std=c++17 -Ijson/include -o plotVars PlotTrainVars.cpp `root-config --cflags --glibs`

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <typeinfo>

#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TTree.h"
#include "ROOT/RDataFrame.hxx"

using json = nlohmann::json;

// apply_selection(ROOT::RDataFrame df)
// {
//     auto df_jet1cut = df.Filter("Jet1Pt>0");
//     auto df_jet2cut = df_jet1cut.Filter("Jet2Pt>0");
//     auto dfselected = df_jet2cut.Filter("M_jj>100");
//     return dfselected;
// }


int main()
{
    // HBB: Reading info stored in config json file: 
    std::ifstream config_file("config_NN.json");
    auto conf = json::parse(config_file);

    std::string path_to_bkg_files = conf["input_samples"]["filedir"];
    std::string path_to_sig_files = conf["input_samples"]["filedirsig"];
    // std::cout << "Relative path to sig files: " << path_to_sig_files << " and to bkg files: " << path_to_bkg_files << std::endl;
    std::vector<std::string> sig_GM_files = conf["input_samples"]["sigGM"]["name"];
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
    
    //HBB: Getting path to a single sig file for testing
    std::string fsigname = path_to_sig_files.data() + std::string("") + sig_GM_files.at(0).data();
    std::cout << "Sig file to open: " << fsigname << std::endl;
    //retrieve the nominal tree from signal samples
    TFile *fsig = TFile::Open(fsigname.data());
    TTree *tsig = (TTree*)fsig->Get("nominal");
    //retrieve the nominal tree from background samples
    std::string fbkgname = path_to_bkg_files.data() + std::string("") + bkg_files.at(0).data();
    std::cout << "Bkg file to open: " << fbkgname << std::endl;
    TFile *fbkg = TFile::Open(fbkgname.data());
    TTree *tbkg = (TTree*)fbkg->Get("nominal");

    TCanvas c("c", "c", 800, 600);
    // tbkg->Draw(vars.at(0).data(), "M_jj>100");
    // tsig->Draw(vars.at(0).data(), "M_jj>100", "same");
    // tsig->Draw(vars.at(0).data(), "Jet2Pt>0");
    // c.SaveAs("exampleplot.pdf");

    // HBB: Using RDataFrames for easier handling root files
    //background
    ROOT::RDataFrame bdf("nominal", fbkgname);
    std::cout << "bkgfile entries (so before cut): " << tbkg->GetEntries() << std::endl;
    auto bdfselected = bdf.Filter("Jet1Pt>0 && Jet2Pt>0 && M_jj>100");
    std::cout << typeid(bdfselected).name() << std::endl;

    auto h_b_etaZ = bdfselected.Histo1D({"h_b_etaZ", "", 50, -5, 5}, "Eta_Z", "WeightNormalized");
    h_b_etaZ->SetLineColor(kAzure+1);
    h_b_etaZ->Draw("hist");

    //signal
    ROOT::RDataFrame sdf("nominal", fsigname);
    std::cout << "sigfile entries (so before cut): " << tsig->GetEntries() << std::endl;
    auto sdfselected = sdf.Filter("Jet1Pt>0 && Jet2Pt>0 && M_jj>100");
    std::cout << typeid(sdfselected).name() << std::endl;
    auto sdf_u1 =  sdfselected.Define("WeightFinalized", [](float w1, int w2) { return w1 * w2; }, {"WeightNormalized", "WZInclusive"});

    auto h_s_etaZ = sdf_u1.Histo1D({"h_s_etaZ", "", 50, -5, 5}, "Eta_Z", "WeightFinalized");
    std::string sscale = std::to_string(h_s_etaZ->Integral(0, h_s_etaZ->GetNbinsX()+1));
    float scale = stof(sscale);
    std::cout << "integral: " << scale << std::endl;
    h_s_etaZ->SetLineColor(kOrange+10);
    h_s_etaZ->Draw("hist same");
    // auto sdf_u2 =  sdf_u1.Define("Scale", scale);

    
    // h_s_etaZ = sdfselected.Histo1D({"h_s_etaZ", "", 50, -5, 5}, "Eta_Z", "WeightFinalized");
    // std::cout << "integral: " << h_s_etaZ->Integral(0, 10) << std::endl;
    // h_s_etaZ->SetLineColor(kOrange+10);
    // h_s_etaZ->Draw("hist same");

    c.SaveAs("sb.pdf");

    //Close root files
    fsig->Close();
    fbkg->Close();
}
