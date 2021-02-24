#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <typeinfo>

#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TTree.h"
// #include "ROOT/RDataFrame.hxx"

using json = nlohmann::json;

int main()
{
    std::ifstream config_file("config_NN.json");
    auto conf = json::parse(config_file);
    std::string path_to_bkg_files = conf["input_samples"]["filedir"];
    std::string path_to_sig_files = conf["input_samples"]["filedirsig"];

    std::cout << "Relative path to sig files: " << path_to_sig_files << " and to bkg files: " << path_to_bkg_files << std::endl;

    std::vector<std::string> sig_GM_files = conf["input_samples"]["sigGM"]["name"];
    std::vector<std::string> bkg_files = conf["input_samples"]["bckgr"]["name"];
    std::vector<std::string> vars = conf["input_samples"]["variables"];
    // std::cout << conf["input_samples"]["variables"] << std::endl;
    // std::cout << typeid(conf["input_samples"]["variables"]).name() << std::endl;
    std::cout << "variable vector length = " << vars.size() << std::endl;
    std::cout << "Sig GM files vector length = " << sig_GM_files.size() << std::endl;

    std::cout << "Signal files:" << std::endl;
    for (auto itemek : sig_GM_files){
        std::cout << "Current file: " << itemek << std::endl;
    }
    std::cout << "----------------------------" << std::endl;
    std::cout << "Background files:" << std::endl;
    for (auto itemek : bkg_files){
        std::cout << "Current file: " << itemek << std::endl;
    }

    std::string fsigname = path_to_sig_files.data() + std::string("") + sig_GM_files.at(0).data();
    std::cout << "Sig file to open: " << fsigname << std::endl;
    //retrieve the nominal tree from signal samples
    TFile *fsig = TFile::Open(fsigname.data());
    // std::cout << fsig << std::endl;
    TTree *tsig = (TTree*)fsig->Get("nominal");
    //retrieve the nominal tree from background samples
    std::string fbkgname = path_to_bkg_files.data() + std::string("") + bkg_files.at(0).data();
    std::cout << "Bkg file to open: " << fbkgname << std::endl;
    TFile *fbkg = TFile::Open(fbkgname.data());
    std::cout << fbkg << std::endl;
    TTree *tbkg = (TTree*)fbkg->Get("nominal");


    TCanvas c("c", "c", 600, 800);
    tbkg->Draw(vars.at(0).data(), "M_jj>100");
    tsig->Draw(vars.at(0).data(), "M_jj>100", "same");

    std::cout << "bkg entries: " << tbkg->GetEntries() << std::endl;
    

    c.SaveAs("exampleplot.pdf");

    fsig->Close();

    // ROOT::RDataFrame d("nominal", fsigname, {"Lep1Pt"});


}
