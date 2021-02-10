
Example of run with GM samples



===   03-02-2021  ===

  A few masses :  GM_03-02-2021

    #Variables used for training
    variables = ['M_jj','Deta_jj', 'Dphi_jj',
                 'Jet1Pt', 'Jet2Pt',
                 'Jet1Eta','Jet2Eta',
                 'Jet1E','Jet2E','Jet3Eta',
                 'Lep1Eta','Lep2Eta', 'Lep3Eta', 
                 'PtBalanceZ','PtBalanceW',
                 'Eta_W', 'Eta_Z',
                 'ZetaLep','Njets']

for mass in 200  275  350  400    600  800 1000;
 do for n in 0 1 2 3;
     do python3 OPT_VBS_NN.py --mass_points $mass --model GM --dropout=0.20 --lr=0.013 --patience=18 --numn=10 --numlayer=3 --epochs=100 --Findex $n --nFold 4 --sdir GM_03-02-2021/m$mass;
     done;
   done   &>  log/training_GM_03-02-2021.log


  ==>> produces ControlPlots/GM_03-02-2021/m200/ , ... , /m1000
		            OutputModel/GM_23_01-21/m200... 1000/*pkl, *.h5

-----------

			--> choose  all masses in config_OPT_NN.py, array "shortlist" 

    list_apply_bkg = [
        'resonance.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.0.root',
        'resonance.364742_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_SFPlus_ntuples.root'
        ]

#GM  sig files to apply NN to
    shortList= [
  450765,  502511,  450766,  502512,  450767,  502513,  450768,  502514,  450769,  502515,  450770,  502516,  450771,  502517,  502518,  502519,  502520,  502521,  502522, 502523]
#      200,         225,        250,         275,        300,         325,         350,        375,        400,         425,         450,         475,        500,         525,         550,        600,        700,        800,         900,     1000


for mass in 200 225 250 275 300 325 350 375 400 425 450 475 500 525 550 600 700 800 900 1000;
do  input="";
    for path in OutputModel/GM_03-02-2021/m$mass/sig*NN.h5;
    do  model=GM_03-02-2021/m$mass/${path##*/};
        input+=$model",";
        done;
    python3 Apply_NN.py --input ${path##*/} --sdir GM_03-02-2021/m$mass; 
    done  | tee  log/Apply_GM_03-02-2021

==>> produces OutputRoot/GM_03-02-2021/m200 ... m1000/GM_*.root

------------

				 >> edit  pSignal_cv_plotting.C (see "parameters to edit")
//========================== PARAMETERS TO EDIT ================================
string savedir = "ControlPlots/GM_03-02-2021/";   // MAKE SURE PSIGNAL EXISTS IN SUBDIRECTORY
string rdir   = "OutputRoot/GM_03-02-2021/";       // Subdirectory containing the root files produced by appluing the NN
string model  = "GM";            // Model used
string opt_ID = "_";    // Optional file name identification
//=======================================================================


root -l pSignal_cv_plotting.C

     	==>>  produces ControlPlots/GM_03-02-2021/m200... m1000/pSig_integrals_GM_m*_.C,   .png

------------
  --> edit nn_per_mass.C and set:
  vector<int> masses{0,200,225,250,275,300,325, 350, 375, 400,425,450,475, 500,525,550,600,700,800,900,1000}; // background + masses for which trained NN was applied

python3   nn_per_mass.py "GM_03-02-2021" "m350"

Info in <TCanvas::Print>: png file ControlPlots/GM_03-02-2021/m350/NN_output_pSignal.png has been created
Info in <TCanvas::SaveSource>: C++ Macro file: ControlPlots/GM_03-02-2021/m350/NN_output_pSignal.C has been generated
Info in <TCanvas::Print>: png file ControlPlots/GM_03-02-2021/m350/NN_output_significance.png has been created
Info in <TCanvas::SaveSource>: C++ Macro file: ControlPlots/GM_03-02-2021/m350/NN_output_significance.C has been generated
Info in <TCanvas::Print>: png file ControlPlots/GM_03-02-2021/m350/NN_output_AMS_curves.png has been created
Info in <TCanvas::SaveSource>: C++ Macro file: ControlPlots/GM_03-02-2021/m350/NN_output_AMS_curves.C has been generated

