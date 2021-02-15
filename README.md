# Analysis-specific information and commands:

# Getting started
-------------------------------
<details> <summary>First time only [click to expand]</summary>
Cloning repository and setting up environment:

    git clone https://github.com/AzuelosG/VBF-WZ-lvll.git
    cd VBF-WZ-lvll
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
* number of layers
* number of neurons per layer, currently chosen as 3. 
* learning rate
* momentum
* patience

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

### Running examples:
Currently for running examples, see:
* [README_GM](README_GM.md) 
* [README_HVT](README_HVT.md)
* [README_QQ](README_QQ.md)

### Running training process
<details> <summary>Expand/collapse the command</summary>

    python3 OPT_VBS_NN.py --mass_points 200 225 250 275 300 325 350 375 400 425 450 475 500 525 550 600 700 800 900 1000 --model GM --dropout=0.20 --lr=0.013 --patience=18 --numn=10 --numlayer=3 --epochs=30 --Findex 0 --nFold 4 --sdir mMultiTest0
where the options for this command looks like below:
   * **mass_points**: the mass point you want to use for your training
   * **model**: The physics model
   * **dropout**: Dropout fraction used in the NN training.
   * **lr**: learning rate
   * **patience**: patience parameter to designate how many epochs you want to see if the training doesn't improve anymore.
   * **numn**: number of neurons per layer
   * **numlayer**: number of hidden layers in the network
   * **epochs**: number of epochs you run in the training. Training still stops after seeing no improvement for the number you designate by the patience parameter.
   * **Findex**: index of the n-fold cross validation.
   * **nFold**: number of folds you use for cross-validation.
   * **sdir**: output subdirecotry you store almost all of your output. This will be always under the *OutputModel* directory.

</details>

### Running application process
<details> <summary>Expand/collapse the command</summary>

The basic command to apply the NN to the ntuple looks like below.

    python3 Apply_NN.py --sdir mMulti_test --target_dir=./Inputs/
where the options for this command looks like the following:
   * **sdir**: Sub-directory. This has to correspond to the direcotry under *OutputModel/* which stores your output model files from trainings you ran for all folds.
   * **input**: This used to be a mandatory argument but not any more. You can still give this option to the command for which case it will use the argument given. If this argument was not given, the code will look into the *sdir* above and automatically selects the output model files by looking at the timestamps and the names of the files. It will use the latest model files produced. The older model files are going to be ignored. So make sure to use this option if you want to apply your old trainings in the same directory.
   * **target**: This is the directory containing your 'target' ntuple files. It will look into the direcotry automatically and apply the NN to all of the samples/files inside, unless the next option is given.
   * **single_file**: If you give this option, it will apply NN only to the single sample you designated. This is to speed up the processing time for many systematic variations because this allows each job to take care of all the systematic variations in the *single file*, and you can run many jobs in parallel so that the total processing time is reduced typically from ~1 day to a few hours, compared to a single job taking care of all the samples and the systematic variations.
   * **run_systematics**: Whether to run the systematic variations or not. When given it will look at a list file to refer the names of the systematic variations.
   * **syst_list**: Totally optional to use this argument. Once given, it will look into the file which lists all the systematic variations. This is useful when you want to apply NN only to some specific systematic variations.


</details>

### Useful git-specific commands:
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
