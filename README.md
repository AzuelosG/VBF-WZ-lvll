# Introduction
This training framework is made for A WZ->lvll VBS resonance selection using neural networks/BDT.

## How to download & initial setup
```
git clone https://github.com/mociduki/VBF-WZ-lvll.git VBF-WZ-lvll_test
cd VBF-WZ-lvll_test/Inputs/
ln -s /lcg/storage16/atlas/mociduki/lvll_samples/hybrid_20200427
ln -s hybrid_20200427 MVA
cd ..
. setup.sh
```

## How to setup each time
At every time you login and come down into this working directory, you need to setup the necessary environment as following:
```
. setup.sh
```

### Configuration of trainings
Training configuration is stored inside this python script: config_OPT_NN.py <br>
It contains all relevant settings like:
* hyper-parameters
* list of input variables
* list of samples used for backgrounds and signals
The NN models are implemented using the Keras package.
The training dataset used is a set of simple ntuples, containing all the necessary variables like Mjj, Detajj etc.

### Samples
* Backgrounds: Your background is a combination of the SM WZ QCD and WZ EW processes.
* Signals: You can choose either of the following as your signal:
  * the combined GM H5 samples with masses ranging from 200 to 900 GeV
  * the HVT signal samples ranging from 250 to 1000 GeV

### General configuration of the network
This is a simple fully connected NN, the principal hyper parameters are:
* the number of layers
* number of neurons per layer
* the learning rate
* momentum.

The input is split into a training and validation set (70%/30%).
After each epoch the accuracy is measured on the validation set and only the best performance is saved.
Each signal mass is assigned a label corresponding to the resonance mass.
The background events have a randomnly assigned label, taken of the same probability distribution as the signals.
This should allow an optimal performance for all resonance masses. All Hyperparameters can be specified (see help).

# Run training & application
## Run NN training
An example of commandline to run the training looks like below.
```
python3 OPT_VBS_NN.py --booldropout=1 --dropout=0.20 --lr=0.013 --patience=18 --numn=300 --numlayer=2 --epochs=100 --Findex 0 --nFold 4
```

where booldropout, dropout, lr, patience, numn, numlayer, and epochs are the hyper-parameters in NN.
The hyper-parameters in the above example correspond to the result of the preliminary scan done by Benjamin.
Note that all the relevant parameters other than designated in the commandline are configured via the aforementioned configuration scpript above.

### nfold cross validation
The last two arguments in the example above, Findex and nFold correspond to the index and the number of folds for the n-fold cross-validation technique.
The Findex runs from 0 to nFold-1, so in this example from 0 to 3.

# Applying the NN to all the ntuples
An example of commandline to run the application looks like below.
```
python3 Apply_NN.py --input sigvalid_GM_S1.798_CVp0.048660_F0o4_NN.h5,sigvalid_GM_S1.778_CVp0.047461_F1o4_NN.h5,sigvalid_GM_S1.794_CVp0.049493_F2o4_NN.h5,sigvalid_GM_S1.788_CVp0.049410_F3o4_NN.h5
```

This applies the trained NN selection to the entire dataset.
It takes the inputs of the models (h5 files), produced by the training in each fold of the cross-validtion, 
therefore you need to check names of the model files created in the each fold. 
The NN prediction is applied on samples in the input directory (Inputs/MVA).
The new samples with NN output are created under a directory called OutputRoot.
The new samples have the identical set of variables as before, but have this additional variable: pSignal, which is a predicted probability to be a signal.
These ntuples/samples can then be used to select the optimal cut on the NN output.

<!---
# Run BDT training and application (obsolete, left only for reference)
* OPT_VBS_BDT.py:

The BDTs are implemented with sklearn.
The Hyper parameters are: base_estimator forming the bossted ensemble, the learning rate and the boosting algorithm.

* Apply_BDT.py:

Similar to Apply_NN this program applies the BDT selection to a given dataset.
The output variable is added to the original ntuple and stored in a new root file.
--->
