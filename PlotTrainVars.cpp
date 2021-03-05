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
#include "TMath.h"
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

    // HBB: Using RDataFrames for easier handling root files
    //background
    ROOT::RDataFrame bdf("nominal", fbkgname);
    std::cout << "bkgfile entries (so before cut): " << tbkg->GetEntries() << std::endl;
    auto bdfselected = bdf.Filter("Jet1Pt>0 && Jet2Pt>0 && M_jj>100");
    std::cout << typeid(bdfselected).name() << std::endl;
    auto bdfsel_aug =  bdfselected.Define("WeightFinalized", [](float w1, int w2) { return w1 * w2; }, {"WeightNormalized", "WZInclusive"});

    /**************************
    auto h_b_etaZ = bdfsel_aug.Histo1D({"h_b_etaZ", "", 10, 0, 10}, "Njets", "WeightFinalized");
    // auto h_b_etaZ = bdfsel_aug.Histo1D({"h_b_etaZ", "", 100, -10, 10}, "Eta_Z", "WeightFinalized");
    std::string bscale_str = std::to_string(1./(h_b_etaZ->Integral(0, h_b_etaZ->GetNbinsX()+1)));
    float bscale = stof(bscale_str);
    std::cout << "Background scale factor: " << bscale << std::endl;
    h_b_etaZ->Scale(bscale);
    h_b_etaZ->SetLineColor(kAzure+1);
    h_b_etaZ->Draw("hist");
    ****************************/


    //signal
    ROOT::RDataFrame sdf("nominal", fsigname);
    std::cout << "sigfile entries (so before cut): " << tsig->GetEntries() << std::endl;
    auto sdfselected = sdf.Filter("Jet1Pt>0 && Jet2Pt>0 && M_jj>100");
    std::cout << typeid(sdfselected).name() << std::endl;
    auto sdfsel_aug =  sdfselected.Define("WeightFinalized", [](float w1, int w2) { return w1 * w2; }, {"WeightNormalized", "WZInclusive"});

    /****************************
    auto h_s_etaZ = sdfsel_aug.Histo1D({"h_s_etaZ", "", 10, 0, 10}, "Njets", "WeightFinalized");
    // auto h_s_etaZ = sdfsel_aug.Histo1D({"h_s_etaZ", "", 100, -10, 10}, "Eta_Z", "WeightFinalized");
    std::string sscale_str = std::to_string(1./(h_s_etaZ->Integral(0, h_s_etaZ->GetNbinsX()+1)));
    float sscale = stof(sscale_str);
    std::cout << "Signal scale factor: " << sscale << std::endl;
    h_s_etaZ->Scale(sscale);
    h_s_etaZ->SetLineColor(kOrange+10);
    h_s_etaZ->Draw("hist same");
    c.SaveAs("sb.pdf");
    *****************************/

   //initialise scale factors
   float bscale = 1;
   float sscale = 1;
    for (int i=0; i<vars.size(); ++i){
        hist_settings settings;
        settings = GetHistSettings(vars.at(i));
        int nbins = settings.nbins;
        float xmin = settings.xmin;
        float xmax = settings.xmax;
        std::string xaxistitle = settings.xaxistitle;
        std::string yaxistitle = settings.yaxistitle;

        auto shist = sdfsel_aug.Histo1D({"shist", "", nbins, xmin, xmax}, vars.at(i), "WeightFinalized");
        std::cout << typeid(shist).name() << std::endl;
        auto bhist = bdfsel_aug.Histo1D({"bhist", "", nbins, xmin, xmax}, vars.at(i), "WeightFinalized");
        if (i==0){
            std::string sscale_str = std::to_string(1./(shist->Integral(0, shist->GetNbinsX()+1)));
            sscale = stof(sscale_str);
            std::string bscale_str = std::to_string(1./(bhist->Integral(0, bhist->GetNbinsX()+1)));
            bscale = stof(bscale_str);
        }

        //clear canvas before plotting
        c.Clear();

        //scale histograms to corresponding scale factors
        shist->Scale(sscale);
        bhist->Scale(bscale);

        float ymax = 0;
        float ymaxs = shist->GetBinContent(shist->GetMaximumBin());
        float ymaxb = bhist->GetBinContent(bhist->GetMaximumBin());
        ymaxs>ymaxb ? ymax=ymaxs : ymax=ymaxb;

        shist->SetLineColor(kOrange+10);
        shist->SetLineWidth(2);
        shist->GetXaxis()->SetTitle(xaxistitle.data());
        shist->GetYaxis()->SetTitle(yaxistitle.data());
        shist->GetYaxis()->SetRangeUser(0, ymax*1.3);
        shist->Draw("hist");

        bhist->SetLineColor(kAzure+1);
        bhist->SetLineWidth(2);
        bhist->Draw("hist same");

        std::string cname = vars.at(i).data()+std::string(".pdf");
        c.SaveAs(cname.data());
        
    }


    //Close root files
    fsig->Close();
    fbkg->Close();
}
