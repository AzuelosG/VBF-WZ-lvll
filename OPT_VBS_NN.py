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
from common_function import dataset, AMS, read_data, prepare_data, drawfigure, calc_sig, calc_sig_new, f1, f1_loss, conv_mass_points
import config_OPT_NN as conf
import ROOT, pickle
from pathlib import Path
import os
import subprocess

def KerasModel(input_dim,numlayer,numn, dropout):
    model = Sequential()
    model.add(Dense(numn, input_dim=int(input_dim)))
    model.add(Activation('relu'))
    if dropout>0: model.add(Dropout(dropout))
    for i in range(numlayer-1):
        model.add(Dense(numn))
        model.add(Activation('relu'))
        if dropout>0: model.add(Dropout(dropout))
    model.add(Dense(1))
    model.add(Activation('sigmoid'))

    return model

"""Neural Network Optimisation
Usage:
  python3 OPT_VBS_NN.py 

Options:
  -h --help             Show this screen.
Optional arguments
  --v = <verbose> Set verbose level
  --model =<model> Specify signal model ('HVT' or 'GM')
  --output =<output>    Specify output name
  --numlayer=<numlayer>     Specify number of hidden layers.
  --numn=<numn> Number of neurons per hidden layer
  --dropout=<dropout> Dropout to reduce overfitting: 0 to turn off
  --epoch=<epochs> Specify training epochs
  --patience=<patience> Set patience for early stopping
  --lr=<lr> Learning rate for SGD optimizer
"""

def RunCppPlottingMacro(mpoints, model, dir):
    conf.dump_to_JSON()
    # print("Executing cpp code")
    cmd1 = "make"
    ret1 = subprocess.run(cmd1, shell=True)
    if not ret1.returncode == 0:
        print("WARNING: PlotTrainVars.cpp compilation failed, will continue without making plots!")
        return
    strmpoints = " ".join([str(x) for x in mpoints])
    cmd2 = "./plotVars --mass_points {} --model {} --dir {}".format(strmpoints, model, dir)
    ret2 = subprocess.run(cmd2, shell=True)
    if not ret2.returncode == 0:
        print("WARNING: Execution of PlotTrainVars.cpp failed, will continue without making plots!")
        return
    # print("Successfully executed cpp code")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'NN optimisation')
    parser.add_argument("--v", "--verbose", help="increase output verbosity", default=0, type=int)
    parser.add_argument("--model", help="Specify Model (HVT or GM or QQ)", default='GM', type=str)
    parser.add_argument("--output", help="Specify Output name", default='', type=str)

    #NN specific arguments/options
    parser.add_argument('--numlayer', help = "Specifies the number of layers of the Neural Network", default=3, type=int)
    parser.add_argument('--numn', help = "Specifies the number of neurons per hidden layer", default=200, type=int)
    parser.add_argument('--lr','--learning_rate', help = "Specifies the learning rate for SGD optimizer", default=0.01, type=float)
    parser.add_argument('--momentum', help = "Momentum", default=0.6, type=float)
    parser.add_argument('--dropout', help = "Specifies the applied dropout", default=0.05, type=float)
    parser.add_argument('--epochs', help = "Specifies the number of epochs", default=80, type=int)
    parser.add_argument('--patience', help = "Specifies the patience for early stopping", default=5, type=int)
    
    parser.add_argument('--no_train', help = "Skip training process", default=False, type=bool)
    parser.add_argument('--mass_points', help = "mass points to be included in the training", default=list(), type=int,nargs='+')
    parser.add_argument('--use_sig_masslabel', help = "signal mass label to be the same as mass point, not as a func of mass.", default=False, type=bool)
    parser.add_argument('--use_bkg_randomlabel', help = "bkg mass label to be random in the training", default=False, type=bool)
    parser.add_argument('--nFold', help = "number of folds", default=2, type=int)
    parser.add_argument('--Findex', help = "index in nfolds", default=0, type=int)
    parser.add_argument('--sdir', help = 'Name of subdirectory within Controlplots/', default='', type=str)
    parser.add_argument('--use_weights', help = 'Use x-sections and other weights for training', default=0, type=int)
    parser.add_argument('--debug', help = 'Debug flag to dump output info', default=False, type=bool)

    args = parser.parse_args()
    print('\n=====================================================================')
    print('  MODEL       : {}'.format(args.model))
    print('  MASS POINTS : {}'.format(args.mass_points))
    print('  FOLD INDEX  : {}/{}'.format(args.Findex,args.nFold))
    print('=====================================================================\n')

    if args.patience==0: args.patience=args.epochs
    
    # Checking for or creating subdirectory
    sub_dir_cp = "ControlPlots/"+args.sdir
    sub_dir_om = "OutputModel/"+args.sdir
    sub_dir_vp = "VariablePlots/"+args.sdir
    Path(sub_dir_cp).mkdir(parents=True, exist_ok=True)
    Path(sub_dir_om).mkdir(parents=True, exist_ok=True)
    Path(sub_dir_vp).mkdir(parents=True, exist_ok=True)

    #HBB: Add plotting of train variables 
    if args.Findex == 0: #to run only once
        RunCppPlottingMacro(args.mass_points,args.model,args.sdir)

    #Load input_sample class from config file
    input_sample=conf.input_samples
    if args.model=="QQ" : input_sample = conf.input_samples_qq

    #Additional name from model and hyper parameters
    nameadd=args.output+args.model

    mass_list = [200,225,250,275,300,325,350,375, 400,425, 450,475, 500,525, 550, 600,700,800,900,1000]  # for GM model
    if args.model=="HVT": mass_list = [250,300,350,400,450,500,600,700,800,900,1000]
    if args.model=="QQ":  mass_list = [500,600,700,800,900,1000]   # sig_HVT list, same masses for signal and background

    if args.model=="HVT" and 200 in args.mass_points:
        print ("WARNING: you specified 200 GeV mass point for HVT which does not exist, 200 GeV will be removed!!")
        args.mass_points.remove(200)
#    elif args.model=="GM" and 1000 in args.mass_points:
#        print ("WARNING: you specified 1000 GeV mass point for GM which does not exist, 1000 GeV will be removed!!")
#        args.mass_points.remove(1000)
#        pass

    tmp_switches=list() #switches for mass points
    if len(args.mass_points)>0:
        for mass in mass_list: tmp_switches.append(mass in args.mass_points)
        print("INFO: Overriding the mass switches because the mass_points argument was given!")
    else: print("INFO: No mass_points argument was given. Using default mass switches in the config_OP_NN.py!")

    #Read data files
    mwin = False
    mss  = 0
    if len(args.mass_points)==1:
        mwin=True
        mss=int(args.mass_points[0])
        pass

    #KM: create list of integers from mass_points
    mass_points_float=conv_mass_points(args.mass_points)
    data_set,tmp_switches,transform,prob= prepare_data(input_sample, args.model, args.Findex, args.nFold, args.sdir, arg_switches=tmp_switches, mass_window=mwin, mass=mss,mass_points=mass_points_float,use_sig_masslabel=args.use_sig_masslabel,use_bkg_randomlabel=args.use_bkg_randomlabel)
    np.save('./OutputModel/'+args.sdir+'/prob'+args.model+('_F{}o{}'.format(args.Findex,args.nFold)), prob)

    ar_switches = np.array(tmp_switches)
    ar_mass     = np.array(mass_list)
    tmp_array = ar_switches*ar_mass
    args.mass_points = tmp_array.tolist()
    #Remove extra zeros
    while 0 in args.mass_points: args.mass_points.remove(0)
    #print(args.mass_points)

    print("\nUsing mass switches:",end='')
    for a,b in zip(mass_list,tmp_switches): print("({},{}) ".format(a,b),end='')
    print()

    #Get input dimensions
    shape_train=data_set.X_train.shape
    shape_valid=data_set.X_valid.shape
    #shape_test=data_set.X_test.shape

    num_train=shape_train[0]
    num_valid=shape_valid[0]
    #num_test=shape_test[0]

    num_tot=num_train+num_valid#+num_test

    #print(data_set.X_train[(data_set.X_train['LabelMass']==0)])
    
    print('Number of training: {}, validation: {} and total events: {}.'.format(num_train,num_valid,num_tot))

    #Define model with given parameters
    model=KerasModel(shape_train[1],args.numlayer,args.numn,args.dropout)
    
    #Possible optimizers
    sgd = optimizers.SGD(lr=args.lr, decay=1e-6, momentum=args.momentum, nesterov=True)
    ada= optimizers.Adadelta(lr=args.lr, rho=0.95, epsilon=None, decay=0.01) #was lr=1
    nadam=keras.optimizers.Nadam(lr=args.lr, beta_1=0.9, beta_2=0.999, epsilon=None, schedule_decay=0.004) #was lr=0.002

    model.compile(loss='binary_crossentropy'#f1_loss
                  , optimizer=sgd, metrics=['accuracy'])#,f1])
    model.summary()
    #model.save("modelNN_initial_"+args.model+".h5")

    # Save the model architecture
    #with open('OutputModel/model_architecture.json', 'w') as f:
    #    f.write(model.to_json())

    #Define checkpoint to save best performing NN and early stopping
    path='./OutputModel/'+args.sdir+'/'+nameadd+'_F{}o{}'.format(args.Findex,args.nFold)+'_checkpoint_NN.h5'
    #checkpoint=keras.callbacks.ModelCheckpoint(filepath='output_NN.h5', monitor='val_acc', verbose=args.v, save_best_only=True)
    callbacks=[EarlyStopping(monitor='val_loss', patience=args.patience),ModelCheckpoint(filepath=path, monitor='val_loss', verbose=args.v, save_best_only=True)]

    if args.use_sig_masslabel:   Path('./OutputModel/'+args.sdir+'/use_sig_masslabel').touch()
    if args.use_bkg_randomlabel: Path('./OutputModel/'+args.sdir+'/use_bkg_randomlabel').touch()

    sample_weight_train = np.ones(num_train)
    valid_data=(data_set.X_valid.values, data_set.y_valid.values)
    if args.use_weights: 
        sample_weight_train = data_set.W_train.values[:,0]
        valid_data     =(data_set.X_valid.values, data_set.y_valid.values, data_set.W_valid.values[:,0])
        pass

    if not args.no_train:
        # Train Model
        logs = model.fit(data_set.X_train.values, data_set.y_train.values,sample_weight=sample_weight_train,
                         validation_data=valid_data,
                         epochs=args.epochs,batch_size=256, callbacks=callbacks, verbose =args.v, class_weight = 'auto')
        
        # accuracy vs epochs
        plt.plot(logs.history['acc'], label='train')
        plt.plot(logs.history['val_acc'], label='valid')
        plt.legend()
        plt.title('')
        if len(args.mass_points)>0: plt.title("mass points : {}".format(args.mass_points))
        plt.xlabel("Epoch")
        plt.ylabel("Accuracy")
        plt.savefig(sub_dir_cp + '/class_'+nameadd+('_F{0}o{1}'.format(args.Findex,args.nFold))+'.png')
        plt.clf()
        
        # plot loss vs epochs
        plt.plot(logs.history['loss'], label='train')
        plt.plot(logs.history['val_loss'], label='valid')
        plt.legend() #plt.title('Training loss')
        plt.xlabel("Epoch")
        plt.ylabel("Loss")
        plt.title('')
        if len(args.mass_points)>0: plt.title("mass points : {}".format(args.mass_points))
        plt.savefig(sub_dir_cp + '/loss_'+nameadd+('_F{0}o{1}'.format(args.Findex,args.nFold))+'.png')
        plt.clf()
        pass

    #Calculate Significance
    #Load saved weights which gave best result on training
    model = load_model(path)#, custom_objects={'f1': f1, 'f1_loss' : f1_loss})

    prob_predict_train_NN = model.predict(data_set.X_train.values, verbose=False)
    prob_predict_valid_NN = model.predict(data_set.X_valid.values, verbose=False)
    #prob_predict_test_NN = model.predict(data_set.X_test, verbose=False)

    if args.debug:
        for i in range(len(prob_predict_valid_NN)):
            if data_set.ch_valid.values[i][0]!=450765: continue
            print (data_set.evtNum_valid.values[i][0], prob_predict_valid_NN[i][0])
            #x_str = np.array_repr(data_set.X_valid.values[i]).replace('\n', '')
            #print (data_set.evtNum_valid.values[i][0], x_str, prob_predict_valid_NN[i][0])
            pass
        
    #Calculate significance in output
    #highsig,cut_value = calc_sig_new(data_set, prob_predict_train_NN[:,0], prob_predict_valid_NN[:,0], mass=200, '{0}_NN{1}_F{2}o{3}'.format(args.model,args.output,args.Findex,args.nFold) )
    highsig,cut_value = 0,0
    tmp_dict = {}
    if len(args.mass_points)>1: args.mass_points.append(-1)
    for mass in reversed(args.mass_points):
        #print ("\nEvaluating significance curve at mass: {}".format(mass))
        highsig,cut_value,yields = calc_sig_new(data_set, prob_predict_train_NN[:,0], prob_predict_valid_NN[:,0], '{0}_NN{1}_F{2}o{3}'.format(args.model,args.output,args.Findex,args.nFold),sub_dir_cp,args.mass_points,mass=mass, apply_mass_window=False)
        yields.append(cut_value)
        yields.append(highsig)
        tmp_dict[mass] = yields

    # Draw figures
    drawfigure(model,prob_predict_train_NN,data_set,data_set.X_valid.values,nameadd,cut_value,args.Findex,args.nFold,sub_dir_cp)

    idx_column=["Nsig-TR","Nsig-VA","Nbkg-TR","Nbkg-VA","cut-value","signif."]
    tmp_df = pd.DataFrame(tmp_dict, index= idx_column)
    print("\n Sig Bkg yields before NN cut (-1 = all mass points combined/added):")
    print(tmp_df.T)

    cv_str=''
    if   cut_value<0: cv_str="m%.3f" % cut_value
    else:             cv_str="p%.3f" % cut_value

    print("\n cut value chosen for lightest mass point:", cv_str)

    outputName='sigvalid_'+args.model+'_m{}'.format(args.mass_points if len(args.mass_points)==1 else "Multi")+'_S'+str(round(highsig,3))+args.output+"_CV"+cv_str+('_F{0}o{1}'.format(args.Findex,args.nFold))+'_NN'

    #Save model in OutputModel with the highest significance obtained on validation set
    model.save('./OutputModel/'+args.sdir+'/'+outputName+'.h5')
    pickle.dump(transform,open("OutputModel/"+args.sdir+'/'+outputName+".pkl", 'wb'))
