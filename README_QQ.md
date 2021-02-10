
Example of run with training of qqf vs vbf in HVT samples


26-01-21
======

--> in  OPT_VBS_NN.py:
if args.model=="QQ":  mass_list = [500,600,700,800,900,1000]   # sig_HVT list, same masses for signal and background

for mass in 600 700 800 900 1000  ;
do for n in 0 1 2 3 ;
    do python3 OPT_VBS_NN.py --mass_points $mass --model QQ --dropout=0.20 --lr=0.013 --patience=18 --numn=10 --numlayer=3 --epochs=100 --Findex $n --nFold 4 --sdir QQ_26-01-21/m$mass;
        done;
    done | tee log/training_QQ_26-01-21

  ==>> produces ControlPlots/QQ_26-01-21/m600...//m1000
		            OutputModel/QQ_26-01-21/m600/*pkl, *.h5
			                                             /m1000/*pkl, *h5

-----

      --> in config_OPT_NN.py: 

    list_apply_bkg = [  'resonance.302267_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0600_ntuples.root']   # QQ HVT, 600 GeV

      shortList= [307733, 307734, 307735]   # 500, 600, 700 GeV   HVT signal

for path in OutputModel/QQ_26-01-21/m600/sig*NN.h5
    do python3 Apply_NN.py --input  ${path##*/}    --sdir QQ_26-01-21/m600
    done |tee log/Apply_QQ_26-01-21.log

or

python3 Apply_NN.py --input 'QQ_26-01-21/m600/sigvalid_QQ_m[600]_S0_CVp0.000_F0o2_NN.h5'  --sdir QQ_26-01-21/m600

	==>> produces:   
OutputRoot/QQ_26-01-21/m600/QQ_resonance.302267_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0600_ntuples.root
OutputRoot/QQ_26-01-21/m600/QQ_resonance.307734_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0600_ntuples.root
OutputRoot/QQ_26-01-21/m600/QQ_resonance.307735_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0700_ntuples.root
OutputRoot/QQ_26-01-21/m600/QQ_resonance.307733_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0500_ntuples.root

------------

				 >> edit  pSignal_cv_plotting_QQ.C (see "parameters to edit")
//========================== PARAMETERS TO EDIT ================================
string savedir = "ControlPlots/QQ_26-01-21/";   // MAKE SURE PSIGNAL EXISTS IN SUBDIRECTORY
string rdir   = "OutputRoot/QQ_3112-20/";       // Subdirectory containing the root files produced by appluing the NN
string model  = "QQ";            // Model used
string opt_ID = "_";    // Optional file name identification
//=======================================================================

check background name: 
        string bname1 = "QQ_resonance.302267_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0600_ntuples.root";
and mass to test:
	int mass_arr[1] = {600};


root -l pSignal_cv_plotting_QQ.C

  -->> produces   ControlPlots/QQ_26-01-21/m600/pSig_integrals_QQ_m600_and_bckgrd_.C 
                           ControlPlots/QQ_26-01-21/m600/pSig_integrals_QQ_m600_and_bckgrd_.png

------------
  --> edit nn_per_mass.C and set:
if (phys_model=="QQ") masses={0,600};

python3   nn_per_mass.py "QQ_26-01-21" "m600" "pSignal"  true "QQ"

Info in <TCanvas::Print>: png file ControlPlots/QQ_26-01-21/m600/NN_output_pSignal.png has been created
Info in <TCanvas::SaveSource>: C++ Macro file: ControlPlots/QQ_26-01-21/m600/NN_output_pSignal.C has been generated
Info in <TCanvas::Print>: png file ControlPlots/QQ_26-01-21/m600/NN_output_significance.png has been created
Info in <TCanvas::SaveSource>: C++ Macro file: ControlPlots/QQ_26-01-21/m600/NN_output_significance.C has been generated
Info in <TCanvas::Print>: png file ControlPlots/QQ_26-01-21/m600/NN_output_AMS_curves.png has been created
Info in <TCanvas::SaveSource>: C++ Macro file: ControlPlots/QQ_26-01-21/m600/NN_output_AMS_curves.C has been generated
Info in <TCanvas::Print>: png file ControlPlots/QQ_26-01-21/m600/NN_output_QQ_pSignal.png has been created
Info in <TCanvas::SaveSource>: C++ Macro file: ControlPlots/QQ_26-01-21/m600/NN_output_QQ_pSignal.C has been generated
