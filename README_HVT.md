
Example of run with HVT vbf vs background


===  08-02-2021

  without jet3E and Njets

    #Variables used for training
    variables = ['M_jj','Deta_jj', 'Dphi_jj',
                 'Jet1Pt', 'Jet2Pt',
                 'Jet1Eta','Jet2Eta',
                 'Jet1E','Jet2E',
# 'jet3E',
                 'Lep1Eta','Lep2Eta', 'Lep3Eta', 
                 'PtBalanceZ','PtBalanceW',
                 'Eta_W', 'Eta_Z',
# 'Njets',
                 'ZetaLep']

for mass in 250 300 350 400 450 500 600 700 800 900 1000 ;
do for n in 0 3 ;
    do python3 OPT_VBS_NN.py --mass_points $mass --model HVT --dropout=0.20 --lr=0.013 --patience=18 --numn=10 --numlayer=3 --epochs=100 --Findex $n --nFold 4 --sdir HVT_08-02-2021/m$mass;
        done;
    done  &> log/training_HVT_08-02-2021


   --> produces ControlPlots/HVT_08-02-2021/m250... 1000/


--------------

			--> choose  masses on which to apply the training. here: 250, 350, 400, 600 
			and produce outputs root files for train ing with  masses 250, 400 and 600 

	in config_OPT_NN.py, array "shortlist" ; include masses and backgrounds used for training.

# HVT samples:
       shortlist = [
               307730,307731,309528,307732,309529,307733,307734,307735,307736,307737,307738]       #HVT sig
#                  250    300    350    400    450    500    600    700    800    900    1000 	shortList= [307730,  307732, 309532, 307734] # masses 250, 350, 400, 600

     list_apply_bkg = [
       'resonance.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.0.root',
       'resonance.364742_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_SFPlus_ntuples.root'
        ]

for mass in 250 300 350 400 450 500 600 700 800 900 1000
do  input="";
    for path in OutputModel/HVT_08-02-2021/m$mass/sig*NN.h5;
    do  model=HVT_08-02-2021/m$mass/${path##*/};
        input+=$model",";
        done;
    python3 Apply_NN.py --input ${path##*/} --sdir HVT_08-02-2021/m$mass; 
    done  |tee log/Apply_HVT_08-02-2021

	==>> produces:  OutputRoot/HVT_08-02-2021/m250,400,600 /HVT_*.root  (masses 250, 350, 400, 600 and backgs.)
                                    OutputRoot/HVT_08-02-2021/m350/HVT_*.root  
------------

				 >> edit  pSignal_cv_plotting.C (see "parameters to edit")
//========================== PARAMETERS TO EDIT ================================
string savedir = "ControlPlots/HVT_08-02-2021/";  
string rdir   = "OutputRoot/HVT_08-02-2021/";       // Subdirectory containing the root files produced by appluing the NN
string model  = "HVT";            // Model used
string opt_ID = "_";    // Optional file name identification
//=======================================================================

root -l pSignal_cv_plotting.C

     	==>>  produces ControlPlots/HVT_08-02-2021/m250, 400, 600/pSig_integrals_HVT_m250_and_bckgrd_.png, C

  --> something strange with mass 300/350...

------------
  --> edit nn_per_mass.C and set:

  vector<int> masses{0,250,300, 350, 400, 450, 500, 600, 700, 800, 900, 1000}; 

python3   nn_per_mass.py "HVT_08-02-2021" "m250" "pSignal_HVT" true "HVT"
python3   nn_per_mass.py "HVT_08-02-2021" "m300" "pSignal_HVT" true "HVT"
python3   nn_per_mass.py "HVT_08-02-2021" "m350" "pSignal_HVT" true "HVT"

Info in <TCanvas::Print>: png file ControlPlots/HVT_08-02-2021/m250/NN_output_pSignal.png has been created
Info in <TCanvas::SaveSource>: C++ Macro file: ControlPlots/HVT_08-02-2021/m250/NN_output_pSignal.C has been generated
Info in <TCanvas::Print>: png file ControlPlots/HVT_08-02-2021/m250/NN_output_significance.png has been created
Info in <TCanvas::SaveSource>: C++ Macro file: ControlPlots/HVT_08-02-2021/m250/NN_output_significance.C has been generated
Info in <TCanvas::Print>: png file ControlPlots/HVT_08-02-2021/m250/NN_output_AMS_curves.png has been created
Info in <TCanvas::SaveSource>: C++ Macro file: ControlPlots/HVT_08-02-2021/m250/NN_output_AMS_curves.C has been generated
  also: NN_output_CbvsNN.csv


------------

cd ControlPlots/HVT_08-02-2021/m250 
display -density 60 'vid:*.png'

------------