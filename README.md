# Analysis-specific information and commands:

# Getting started
-------------------------------
<details> <summary>First time only [click to expand]</summary>
Cloning repository and setting up environment:

    git clone https://github.com/AzuelosG/VBF-WZ-lvll.git
    cd VBF-WZ-lvll
    git submodule update --init --recursive
    ln -s /home/zp/azuelos/lvll/MVA/VBF-WZ-lvll_GL/Inputs/
    source setup.sh
</details>

From the next time you login each time (you have to be in the main directory): 

    source setup.sh



# General information about the code:
<details> <summary>Expand/collapse details</summary>

## Configuration of trainings:
General training configuration is stored in the file [config_OPT_NN.py](config_OPT_NN.py). It contains relevant settings like samples and input variables, which are detailed below.
The NN model is implemented in [this part](OPT_VBS_NN.py#L25-L37) using the Keras package with Tensoflow as backend. 
The training dataset used is a set of simple root ntuples containing all the necessary variables like Mjj, Detajj etc.

### Samples
* Backgrounds: Your background is a combination of the SM WZ QCD and WZ EW processes. See [here](config_OPT_NN.py#L81-L90) for the actual samples used.
* Signals: You can choose either of the following as your signal:
  * [GM signal samples](config_OPT_NN.py#L100-L122)
  * [HVT signal samples](config_OPT_NN.py#L132-L144)
  * the QQ signal samples

### Input features/variables
You can have a look at the list of input features [here](config_OPT_NN.py#L149-L160)

### NN model and hyper-parameters
The model we fit/train is a simple fully connected NN. The configurable hyper-parameters can be found [here](OPT_VBS_NN.py#L64-L71).
The typical ones are:
* epochs: number of epochs to run in the training. Training stops after seeing no improvement for n epochs you designate by the patience parameter
* numlayer: number of hidden layers
* numn: number of neurons per layer
* lr: learning rate
* momentum
* patience: patience parameter. Training automatically stops after N epochs without any improvemnt in the loss.
* dropout: fraction that is droped out in the NN training, has an averaging effect which mitigates the overtraining. See [this link]() for details.

The input is split into a training and validation set in percentage ratio 70%/30% (testing set will be added soon).
After each epoch the accuracy is measured on the validation set and only the model with best performance is saved.

### Mass label
In order to perform only one training that can take care of all signal mass points at the same time. One important technique, tentatively called 'mass label', is employed.
Each signal mass is assigned with a label corresponding to the resonance mass, either by looking at the sample or the reconstructed resonnance mass:
   * if you pass the option [*--use_sig_masslabel=True*](OPT_VBS_NN.py#L75) with three mass points of [200, 300, 400] for instance, they will have the mass labels of [0, 1, 2], respecitvely.
   * if you don't pass the option of *use_sig_masslabel*, the mass label is determined by the reconstructed M_WZ (0 if M_WZ<250, 1 if 250<M_WZ<350, 2 if 350<MWZ).

The background events can have either of the following two options for the mass labels
   * a randomly assigned label is used if the option [*--use_bkg_randomlabel=True*](OPT_VBS_NN.py#L76) is given. It takes of the same probability distribution as the signals, hence should allow an optimal performance for all resonance masses.
   * when the above option isn've give, the label will be determined by looking at reconstructed M_WZ, just like the signal example.

### n-fold cross-validation
General explanation of the cross vadlidation can be found in [this link](https://towardsdatascience.com/cross-validation-explained-evaluating-estimator-performance-e51e5430ff85). It allows us to fully use the available statistics for our benchmark evaluation. So this becomes very powerful when your available statistics is limited. In the training command, you can find two arguments. *Findex* and *nFold* correspond to the index and the number of folds, respectively. The Findex runs from 0 to nFold-1, so in our example from 0 to 3, while your number of fold sticks always at 4.

</details>

# How to run training and application processes

## Plotting training variables
<details> <summary>Expand/collapse the command</summary>
The main code to prepare plots of the training variables is located in PlotTrainVars.cpp file. 
So far, plotting is implemented only for the GM and HVT models in the VBF category. 
The information needed to run the code comes from two sources:

* [config_OPT_NN.py](config_OPT_NN.py) file:
    * variables used in the training,
    * samples associated with different mass points,
* user
    * theoretical model,
    * mass points,
    * directory name (to be created within VariablePlots directory) to hold the pdf and png files.

Code can be run in two ways: when running the training (no need to run anything other 
than the usual training command) or in the standalone mode, by running:
``` 
    python3 config_OPT_NN.py
    make
    ./plotVars --mass_points <mass-points-list> --model <model> --dir <dirname>
```
Executing [config_OPT_NN.py](config_OPT_NN.py) file is essential if this file has been modified 
(i.e. if the sample or training variable list has been updated). If a new variable is 
added to the list of training variables, it is required to add histogram settings in 
order to adjust the range and binning. This can be done by modifying function GetHistSettings() 
in [PlotTrainVars.cpp](PlotTrainVars.cpp) file.

</details>


## Running training process
<details> <summary>Expand/collapse the command</summary>

    python3 OPT_VBS_NN.py --mass_points 200 225 250 275 300 325 350 375 400 425 450 475 500 525 550 600 700 800 900 1000 --model GM --dropout=0.20 --lr=0.013 --patience=18 --numn=10 --epochs=30 --Findex 0 --nFold 4 --sdir mMultiTest0
where the options for this command looks like below:
   * For **dropout**, **lr**, **patience**, **numn**, **numlayer**, **epochs**, see the hyper-parameter section above.
   * **mass_points**: the mass point you want to use for your training
   * **model**: The physics model either of [GM, HVT, QQ]
   * **Findex**: index of the n-fold cross validation.
   * **nFold**: number of folds you use for cross-validation.
   * **sdir**: output subdirecotry you store almost all of your output. This will be always under the *OutputModel* directory.

</details>

## Running application process
<details> <summary>Expand/collapse the command</summary>

The basic command to apply the NN to the ntuple looks like below.

    python3 Apply_NN.py --sdir mMulti_test
where the options for this command looks like the following:
   * **sdir**: Sub-directory. This has to correspond to the direcotry under *OutputModel/* which stores your output model files from trainings you ran for all folds.
   * **input**: This used to be a mandatory argument but not any more. You can still give this option to the command for which case it will use the argument given. If this argument was not given, the code will look into the *sdir* above and automatically selects the output model files by looking at the timestamps and the names of the files. It will use the latest model files produced. The older model files are going to be ignored. So make sure to use this option if you want to apply your old trainings in the same directory.
   * **target_dir**: This is the directory containing your 'target' ntuple files. It will look into the direcotry automatically and apply the NN to all of the samples/files inside, unless the next option is given.
   * **single_file**: If you give this option, it will apply NN only to the single sample you designated. This is to speed up the processing time for many systematic variations because this allows each job to take care of all the systematic variations in the *single file*, and you can run many jobs in parallel so that the total processing time is reduced typically from ~1 day to a few hours, compared to a single job taking care of all the samples and the systematic variations.
   * **run_systematics**: Whether to run the systematic variations or not. When given it will look at a list file to refer the names of the systematic variations.
   * **syst_list**: Totally optional to use this argument. Once given, it will look into the file which lists all the systematic variations. This is useful when you want to apply NN only to some specific systematic variations.
</details>

## Work flow examples:

### Running one training for all mass points and apply
<details> <summary>Expand/collapse the command</summary>

    #1. Run 4 trainings for 4 fold
    for i in {0..3}; do python3 OPT_VBS_NN.py --mass_points 200 225 250 275 300 325 350 375 400 425 450 475 500 525 550 600 700 800 900 1000 --model GM --dropout=0.20 --lr=0.013 --patience=18 --numn=10 --epochs=30 --Findex $i --nFold 4 --sdir testGM >& testGM_$i.log &; done
    #2. Apply NN to ntuples
    python3 Apply_NN.py --sdir testGM
    #3. analyse

</details>

### Running many single mass point trainings and apply
<details> <summary>Expand/collapse the command</summary>

The example command to submit many single-masspoint trainings can be found in [train_single_masspoints.sh](train_single_masspoints.sh).
This produces output files (pkl, and h5) under *ControlPlots/GM_test/m200/*.

Choose  all masses in config_OPT_NN.py, array "shortlist"

    list_apply_bkg = [
        'resonance.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.0.root',
        'resonance.364742_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_SFPlus_ntuples.root'
        ]
    #GM  sig files to apply NN to
        shortList= [
      450765,  502511,  450766,  502512,  450767,  502513,  450768,  502514,  450769,  502515,  450770,  502516,  450771,  502517,  502518,  502519,  502520,  502521,  502522, 502523]

The example command to apply many single-masspoints can be found in [apply_single_masspoints.sh](apply_single_masspoints.sh).
This produces output files (root) under *OutputRoot/GM_test/m200/*

edit  pSignal_cv_plotting.C (see "parameters to edit")

    //========================== PARAMETERS TO EDIT ================================
    string savedir = "ControlPlots/GM_test/";   // MAKE SURE PSIGNAL EXISTS IN SUBDIRECTORY
    string rdir   = "OutputRoot/GM_test/";       // Subdirectory containing the root files produced by appluing the NN
    string model  = "GM";            // Model used
    string opt_ID = "_";    // Optional file name identification
    //=======================================================================
    
Run the following command

    root -l pSignal_cv_plotting.C

which produces png files under *ControlPlots/GM_03-02-2021/m200/*

edit nn_per_mass.C and set:

    vector<int> masses{0,200,225,250,275,300,325, 350, 375, 400,425,450,475, 500,525,550,600,700,800,900,1000}; // background + masses for which trained NN was applied

then run

    python3 nn_per_mass.py GM_test m300 pSignal_GM 1 GM 1 0  

</details>



* [README_HVT](README_HVT.md)
* [README_QQ](README_QQ.md)

# Useful git-specific commands:
<details>
  <summary>Click to expand!</summary>
Setting up the recommended (and less buggy) git version and enabling coloring:
    
    lsetup git

Checking which branch you are working on, which files are modified and which are added to the staging area:

    git status

Adding file or directory (including its contents) to staging area:

    git add <filename1> <filename2>
    git add <dirname>

Committing changes in files added to the staging area (before committing check the status by running ```git status``` to make sure only the files you wish to commit are staged):

    git commit -m "Meaningful description of implemented changes"

Checking whether there were any updates in the remote repository (but not merge them with your local version):

    git fetch

Pulling changes from remote repository (this will merge the changes with your local version of the code so be careful):

    git pull

Pushing changes to the remote repository if you are working on branch you want to push to (before doing that make sure to update your code to the recent version from remote repository to avoid conflicts):

    git push

Removing recently added local changes in a file (not added to staging area):

    git checkout -- <filename>

Removing file from staging area (without removing added changes):

    git reset HEAD <filename>

</details>
