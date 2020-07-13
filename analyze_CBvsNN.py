import argparse
import sys
import keras
from keras.utils.np_utils import to_categorical
from keras.models import Sequential, load_model
from keras.layers import Dense, Activation, Dropout
from keras import backend as K
from keras.optimizers import SGD
from keras import optimizers
from keras.callbacks import EarlyStopping, ModelCheckpoint
#import sklearn
import numpy as np
import pandas as pd
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import math
from root_numpy import root2array, tree2array, array2root
from common_function import dataset, AMS, read_data, prepare_data, drawfigure, calc_sig, calc_sig_new, f1, f1_loss
import config_OPT_NN as conf
import ROOT 
from pathlib import Path
import os

#===============================================================================
#-------------------------------------------------------------------------------
#
# Shows the number of events of EW and QCD background as well as the individual
# mass' number of events, within mass window and cut-based. 
# Calculates the significance before the training.
#
#-------------------------------------------------------------------------------
#===============================================================================

def getSamples(mass):
    """
    Similar to config_OPT_NN.input_samples, except this class uses the output
    of a training for an individual mass
    """
    class input_samples_NN:
        # Assumed luminosity
        lumi = 140.

        # Fraction used for training 
        trafrac = 0.9

        # Prefixe added to the ntuples of the output
        prestr = "new_GM_main"

        # Directory where ntuples are located
        filedir = "OutputRoot/0708_GM/m300/"+prestr


        # Background samples
        bckgr = {
            'name' : ['MVA.361292_MGaMcAtNloPy8EG_NNPDF30LO_A14NNPDF23LO_WZ_lvll_FxFx_ntuples.root',
                      'MVA.364284_Sherpa_222_NNPDF30NNLO_lllvjj_EW6_ntuples.root']}
        # Signal samples
        sigGM = {
            'name' : ['MVA.305028_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_200_qcd0_ntuples.root',
                      'MVA.305029_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_300_qcd0_ntuples.root',
                      'MVA.305030_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_400_qcd0_ntuples.root',
                      'MVA.305031_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_500_qcd0_ntuples.root',
                      'MVA.305032_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_600_qcd0_ntuples.root',
                      'MVA.305033_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_700_qcd0_ntuples.root',
                      'MVA.305034_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_800_qcd0_ntuples.root',
                      'MVA.305035_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_900_qcd0_ntuples.root']}

    return input_samples_NN

def AMS(n_sig, n_bkg, br=0.00001):
    """ 
    Approximate Median Significance
    """
    A = n_sig + n_bkg + br
    B = math.log(1+n_sig/(n_bkg+br))
    C = 2*(A*B - n_sig)
    
    return math.sqrt(C)

def bkgEvents(cins, mass, cut=0):
    """
    Returns the cut-based, normalize-weighted number of background events within
    the mass window - total, QCD and EW

    cins - config.input_samples or similar object
    mass - 
    cut  - user selection to use instead of cut-based selection
    """

    bkg = np.array([0,0])    # Number of events for QCD and EW
    bkg_nEv_sum = 0          # Sum of all background events

    for i in range(len(cins.bckgr['name'])):
        # Accessing the data
        bkg_file = ROOT.TFile(cins.filedir+cins.bckgr['name'][i])
        tree = bkg_file.Get('nominal')

        # Cut-based selections
        cuts = 'Jet1Pt>0 && Jet2Pt>0 && M_jj>500 && Deta_jj>3.5 && M_WZ>({0}*0.88) && M_WZ<({0}*1.12)'.format(mass)
        
        # Selecting the data
        if cut: bkg_DF = pd.DataFrame(tree2array(tree, selection=cut))
        else:   bkg_DF = pd.DataFrame(tree2array(tree, selection=cuts))

        # Calculating the number of events
        bkg[i] = sum(bkg_DF['WeightNormalized'])
        bkg_nEv_sum += bkg[i]

    return bkg_nEv_sum, bkg[0], bkg[1]

def sigEvents(cins, mass, cut=0):
    """
    Returns the cut-based, normalize-weighted number of events of the signal
    for an individual mass within the mass window

    cins - config.input_samples or similar object
    mass - 
    cut  - user selection to use instead of cut-based selection
    """
    # Accessing the data
    isf = np.where([f"{mass}" in cins.sigGM['name'][i] for i in range(len(cins.sigGM['name']))])[0]
    isf = int(isf)
    sig_file = ROOT.TFile(cins.filedir+cins.sigGM['name'][isf])
    tree = sig_file.Get('nominal')
    
    # Cut-based selections
    CB_cut = "M_jj>500 && Deta_jj>3.5"

    # Selecting the data
    if cut: cuts = 'Jet1Pt>0 && Jet2Pt>0 && M_WZ>({0}*0.88) && M_WZ<({0}*1.12) && {1}'.format(mass, cut)
    else:   cuts = 'Jet1Pt>0 && Jet2Pt>0 && M_WZ>({0}*0.88) && M_WZ<({0}*1.12) && {1}'.format(mass, CB_cut)
    sig_DF = pd.DataFrame(tree2array(tree, selection=cuts))

    # Calculating the number of events
    sig_DF = pd.DataFrame(tree2array(tree, selection=cuts))
    sig_nEv = sum(sig_DF['WeightNormalized'])

    return sig_nEv   

#-------------------------------------------------------------------------------

# Initializing arrays
res_arr  = np.zeros((8,2,4))
mass_arr = np.arange(200,901,100)

# Input samples for cut-based analysis
cins_CB = conf.input_samples

for c in range(len(mass_arr)):
    mass = mass_arr[c]
    # Number of events for cut-based analysis
    n_bkg, QCD, EW = bkgEvents(cins_CB, mass)
    n_sig = sigEvents(cins_CB, mass)

    # Number of events for NN analysis
    NN_bkg, NN_QCD, NN_EW = bkgEvents(getSamples(mass), mass, cut="pSignal>0.5")
    NN_sig = sigEvents(getSamples(mass), mass, cut="pSignal>0.5")

    # Printing number of events
    print(f"\nMass : {mass} \t Cut-based \t NN")
    print(   "Signal        \t {:.2f}    \t {:.2f}\
            \nQCD           \t {}        \t {}\
            \nEW            \t {}        \t {}".format(n_sig, NN_sig, QCD, NN_QCD, EW, NN_EW))
    
    # Calculating significance
    ams = NN_ams = None
    if n_bkg>=0 and n_sig>=0:
        ams = "{:.2f}".format(AMS(n_sig, n_bkg))
    if NN_bkg>=0 and NN_sig>=0:
        NN_ams = "{:.2f}".format(AMS(NN_sig, NN_bkg))
    print(f"Significance \t {ams}     \t {NN_ams}")    

    # Updating results' array
    res_arr[c][0] = n_sig, QCD, EW, ams
    res_arr[c][1] = NN_sig, NN_QCD, NN_EW, NN_ams

print(res_arr)