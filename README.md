Analysis-specific information and commands:
-------------------------------
-------------------------------

Cloning repository and setting up environment:
-------------------------------
<details>
  <summary>First time only [click to expand]</summary>

    git clone https://github.com/AzuelosG/VBF-WZ-lvll.git
    cd VBF-WZ-lvll
    ln -s /home/zp/azuelos/lvll/MVA/VBF-WZ-lvll_GL/Inputs/
    source setup.sh
</details>

Each time (you have to be in the main directory): 

    source setup.sh

General information about the code:
-------------------------------
Configuration of trainings:  
Training configuration is stored inside this python script: config_OPT_NN.py <br>
It contains all relevant settings like:
* hyper-parameters
* list of input variables
* list of samples used for backgrounds and signals

The NN models are implemented using the Keras package with Tensoflow as backend. 
The training dataset used is a set of simple ntuples containing all the necessary variables like Mjj, Detajj etc.

### Samples
* Backgrounds: Your background is a combination of the SM WZ QCD and WZ EW processes.
* Signals: You can choose either of the following as your signal:
  * the combined GM H5 samples 
  * the HVT signal samples 

### General configuration of the network
This is a simple fully connected NN, the principal hyper parameters are:
* the number of layers
* number of neurons per layer
* the learning rate
* momentum.  

Currently the chosen number of hidden layers is 3. 

The input is split into a training and validation set in percentage ratio 70%/30% (testing set will be added soon).
After each epoch the accuracy is measured on the validation set and only the model with best performance is saved.
Each signal mass is assigned a label corresponding to the resonance mass.
The background events have a randomly assigned label, taken of the same probability distribution as the signals.
This should allow an optimal performance for all resonance masses. All Hyperparameters can be specified (see help). 
Currently the number of folds is decided to be at least 4. 

Running examples:
-------------------------------
Currently for running examples, see:
  
    README_GM,         README_HVT,         README_QQ   

Useful git-specific commands:
-------------------------------
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
