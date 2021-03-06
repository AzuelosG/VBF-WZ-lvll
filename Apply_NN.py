import argparse
import keras
from keras.utils.np_utils import to_categorical
from keras.models import Sequential, load_model
from keras.layers import Dense, Activation, Dropout
from keras import backend as K
from keras.optimizers import SGD
from keras import optimizers
import numpy as np
import pandas as pd
import math, os
from root_numpy import root2array, tree2array, array2root
import ROOT
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from keras.models import model_from_json
from common_function import read_data_apply, calc_sig, f1, f1_loss
import config_OPT_NN as conf
from pathlib import Path

def check_tree(sample_file,syst_var):
    
    rootfile = ROOT.TFile(sample_file)
    tree     = rootfile.Get(syst_var)
    flag = bool(tree)

    # print()
    # print(flag)

    #rootfile.Close()
    ROOT.gROOT.CloseFiles()

    if not flag:
        print("WARNING: ",syst_var, "missing! Skipping to process...")
        if syst_var=='nominal':
            print("FatalError: nominal is null! Aborting...")
            exit(1)
            pass
        pass

    return flag

def read_phys_model(file_name):

    s_model_name='GM'
    if   file_name.find('_HVT')!=-1: s_model_name='HVT'
    elif file_name.find('_QQ')!=-1:s_model_name='QQ'

    return s_model_name

def read_phys_models(model_files):
    idx=0
    phys_model=''
    for File in model_files:
        if idx==0: phys_model=read_phys_model(File)
        else: 
            if phys_model!=read_phys_model(File): 
                print ("Something is wrong with signal model names, aborting...")
                exit(1)
                pass
            pass
        idx+=1
        pass

    return phys_model

def read_cut_fold(files):

    cut_values=list()
    for File in files:
        cut_values.append(read_cut(File))
        print(cut_values[-1])
        pass

    return cut_values

def read_cut(file_name):

    s_cut=file_name[file_name.find("CV")+2:file_name.find("_F")]
    #print(s_cut)
    
    cut_value=float(s_cut[1:])
    if s_cut.find("m")==0: cut_value*=-1
    #print(cut_value)

    return cut_value

def calculate_pred(model,X,cut_value,verbose):
    if verbose: print('\t nEvents / file=\t',len(X))
    prob_predict=model.predict(X.values, verbose=False)
    #pcutNN = np.percentile(prob_predict,40.)
    Yhat=prob_predict[:] > cut_value
    return Yhat, prob_predict

def calculate_pred_fold(models,data,X,cut_values):

    probabilities=list()
    predictions  =list()

    for idx in range(len(models)): #KM: loop over fold
        pred_fold, prob_fold = calculate_pred(models[idx],X,cut_values[idx],verbose=(idx==0))
        predictions.append(pred_fold)
        probabilities.append(prob_fold)
        pass

    #print('=============================')
    #print(probabilities[0])
    #print('=============================')
    #print(probabilities[1])

    #for i in range(len(probabilities[0])):
    #    x_str = np.array_repr(X.values[i]).replace('\n', '')
    #    print(data["EventNumber"][i],x_str,probabilities[0][i][0])
            
    for idx in range(len(probabilities[0])): #KM: loop over events
        list_idx=data['EventNumber'][idx]%len(models)#idx%len(models)
        if list_idx!=0:
            probabilities[0][idx] = probabilities[list_idx][idx]
            predictions  [0][idx] = predictions  [list_idx][idx]
            pass
        pass

    #print('=============================')
    #print(probabilities[0])

    pred_fold=predictions  [0]
    prob_fold=probabilities[0]

    #print(len(pred_fold),len(prob_fold))
    
    return pred_fold,prob_fold

def save_file(data, pred, proba, filename, phys_model, sub_dir, syst_var):
    #data['isSignal'] = pred
    data['pSignal_'+phys_model] = proba[:]
    #print("Input file  =\t\t\t",filename)
    
    # Checking for or creating subdirectory
    sub_dir_or = "OutputRoot/"+sub_dir
    if args.appOutdir!='': sub_dir_or=args.appOutdir #    if args.appOutdir!='': sub_dir_or+='/'+args.appOutdir
    Path(sub_dir_or).mkdir(parents=True, exist_ok=True)
    #    outputPath=sub_dir_or+'/new_'+phys_model+'_'+filename     #print(outputPath)
    outputPath=sub_dir_or+'/'+phys_model+'_'+filename     #print(outputPath)
    if args.appSameOutfile: outputPath=sub_dir_or+'/'+filename
    
    save_mode='update'
    if syst_var=='nominal': save_mode='recreate'
    
    array2root(np.array(data.to_records(index=False)), outputPath, syst_var, mode=save_mode)
    
    print('Save file as= {}'.format(outputPath))
    print()

    return

#def analyze_data(filedir,filename, model, X_mean, X_dev, label, variables, sigmodel,cut_value,sub_dir):
#    data, X = read_data_apply(filedir+filename, X_mean, X_dev, label, variables, sigmodel)
#    if len(X)==0: return
#    pred, proba = calculate_pred(model,X,cut_value)
#    save_file(data, pred, proba, filename, sigmodel, sub_dir)

def get_prob_files(nFold,phys_model,sub_dir):

    prob_files=list()

    for i in range(nFold):
        pfile='OutputModel/'+sub_dir+'/prob'+phys_model+'_F{}o{}'.format(i,nFold)+'.npy'
        prob_files.append(pfile)
        pass
    
    return prob_files

def analyze_data_folds(filedir,filename, models, tr_files, label, variables, phys_model,cut_values,sub_dir,syst_var,mass_points,debug=False):

    prob_files = get_prob_files(len(models),phys_model,sub_dir)

    use_app_randomlabel=False
    if os.path.isfile(('OutputModel/'+sub_dir+'use_bkg_randomlabel')): use_app_randomlabel=True

    #print()
    #print(use_app_randomlabel)
    #print()

    data, X = read_data_apply(filedir+filename, tr_files, label, variables, prob_files, syst_var,mass_points=mass_points,use_app_randomlabel=use_app_randomlabel)

    if len(X)==0: return
    #print(len(data),len(X))
    
    pred_fold, proba_fold = calculate_pred_fold(models,data,X,cut_values)
    save_file(data, pred_fold, proba_fold, filename, phys_model, sub_dir,syst_var)
    if debug:
        for i in range(len(data['EventNumber'])):
            print (data['EventNumber'][i], proba_fold[i][0])
            #x_str = np.array_repr(X.values[i]).replace('\n', '')
            #print (data['EventNumber'][i], x_str, proba_fold[i][0])

"""Run Trained Neural Network on samples
Usage:
  python3 Apply_NN.py 

Options:
  -h --help             Show this screen.
Optional arguments
  --input =<input>    Specify input name of trained NN
  --phys_model =<phys_model> Specify signal phys_model ('HVT' or 'GM')
"""

def parse_model_files(sdir,input):

    files=list()
    Findices=list()
    nFold=int(0)
    lockFold=False
    for File in input.split(","):
        files.append(sdir+"/"+File)
        if '_F' in File:
            f_str=File[File.find('_F')+2:File.rfind("_")]
            
            # sanity check
            if not lockFold:
                nFold =int(f_str[f_str.find("o")+1:])
                lockFold=True
                pass
            Findex=int(f_str[:f_str.find("o")])
            print(File, 'Findex / nFold=\t',Findex,"/",nFold)
            
            if int(f_str[f_str.find("o")+1:])!=nFold:
                print("Something is wrong with number of fold configuration, aborting...")
                exit(1)
                pass

            if not Findex in Findices:
                Findices.append(Findex)
            else:
                print("Same fold index has been found, abroting...")
                exit(1)
                pass

            pass
        pass

    tr_files=list()
    for filename in files:
        tr_file=filename
        tr_file=tr_file.replace(".h5",".pkl")
        tr_files.append(tr_file)
        pass

    return files,tr_files

def read_models(model_files):

    models=list()

    first_model = True
    for File in model_files:
        model = load_model('OutputModel/'+File)
        if first_model: 
            model.summary()
            first_model=False
        models.append(model)
        pass

    print("")
    print(len(model_files)," models loaded.")
    print("")

    return models

def read_syst_variations(file):
    syst_variations=list()
    for line in open(args.syst_list, 'r').read().splitlines(): 
        if '#' in line: continue
        syst_variations.append(line)
        pass
    return syst_variations

def read_sample_dir(input_dirpath):
    list_bkg=list() #    input_dirpath=args.target_dir
    for r,d,f in os.walk(input_dirpath): #args.target_dir):
        for file_name in f:
            if not('.root' in file_name): continue
            list_bkg.append(file_name)
            pass
        pass
    return list_bkg

def parse_directory(sdir):

    #get list of files in the dir, ordered with timestamp, latest at the end
    paths = sorted(Path('OutputModel/'+sdir).iterdir(), key=os.path.getmtime)

    strpaths=list()
    # remove unnecessary files
    for ppath in paths:
        path=str(ppath)
        if not('sigvalid' in path and '.h5' in path): continue
        strpaths.append(path)
        pass

    # revert order
    strpaths.reverse()

    # remove remnants of old trainings
    nFold=0
    last_paths=list()
    nFilled=0
    for path in strpaths:
        if nFilled==0: nFold = int(path[path.find('_F')+4:path.find('_F')+5])
        last_paths.append(path)
        nFilled+=1
        if nFilled==nFold: break
        pass

    if nFilled<nFold:
        print('Error: The number of model files is',nFilled,' which is smaller than the number of folds,',nFold)
        print('Error: Abroting. Please make sure you have completed all trainings for every fold and model files exist in the directory.')
        exit(1)
        pass

    #print(nFold,len(last_paths))
    #for path in last_paths: print(path)

    # revert back again
    last_paths.reverse()

    # re-sort files according to fold-index
    final_paths=list()
    for idx in range(nFold):
        for aPath in last_paths:
            #print(aPath[aPath.find('_F')+2:aPath.find('_F')+3])
            if idx==int(aPath[aPath.find('_F')+2:aPath.find('_F')+3]):
                final_paths.append(aPath)
                break
            pass
        pass
    
    #put together into a string
    first_file=True
    models_str=''
    for path in final_paths:
        if first_file:
            models_str +=path[path.rfind('/')+1:]
            first_file=False
        else: models_str +=','+path[path.rfind('/')+1:]
        pass
    
    #print(models_str)

    return models_str
    
    
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Apply NN on ntuples')
    parser.add_argument("--input", help="Name of saved trained NN", default='', type=str)
    parser.add_argument("--sdir", help="Subdirectory of saved output", default="", type=str)
    #parser.add_argument("--syst_var", help="Target systematic variation", default="nominal", type=str)
    parser.add_argument("--syst_list", help="list of systematic variation", default="syst_vars.list", type=str)
    parser.add_argument("--run_systematics", help="Bool whether to run syst variations", default=False, type=bool)
    parser.add_argument("--target_dir", help="Target directory of root files to apply NN ouput", default="", type=str)
    parser.add_argument("--single_file", help="Single target file", default="", type=str)
    parser.add_argument('--mass_points', help = "mass points to be included in the training", default=list(), type=int,nargs='+')
    # parser.add_argument("--phys_model", help="Specify Model (HVT or GM)", default='GM', type=str)

    # for Miaoran's application
    parser.add_argument('--appSameOutfile', help = 'Debug flag to dump output info', default=False, type=bool)
    parser.add_argument('--appOutdir', help = 'Debug flag to dump output info', default='', type=str)

    args = parser.parse_args()
    print(args)

    #If there's no argument given, dig into the directory automatically
    input_models = parse_directory(args.sdir)
    if args.input!='': input_models = args.input
    else:
        print("No input argument was given, the specified directory is used to find the following models")
        print(input_models)

    model_files,tr_files=parse_model_files(args.sdir,input_models)

    print()
    print(model_files)
    print(tr_files)
    print()

    #Load input_sample class from config file
    input_sample=conf.input_samples
    apply_sample=conf.apply_samples

    #Restores Model and compiles automatically
    models = read_models(model_files)

    cut_values= read_cut_fold(model_files)

    phys_model=read_phys_models(model_files)

    #Load Mean and std dev
    if not(phys_model=='GM' or phys_model=='HVT' or phys_model=='QQ'):
        raise NameError('Model needs to be either GM, HVT or QQ')
#    X_mean = np.load('mean'+phys_model+'.npy')
#    X_dev = np.load('std_dev'+phys_model+'.npy')
    #Mean and std dev from training
    #print(X_mean)
    #print(X_dev)

    #list_data = apply_sample.list_apply_data

    #Apply NN on all samples in config file
    list_bkg = apply_sample.list_apply_bkg # not only background but all sample is listed in this
    #if phys_model=='GM': 
    #    list_sig = apply_sample.list_apply_sigGM
    #elif phys_model=='HVT':
    #    list_sig = apply_sample.list_apply_sigHVT  
    input_dirpath=apply_sample.filedirapp
    if args.target_dir!='': 
        input_dirpath = args.target_dir
        list_bkg = read_sample_dir(input_dirpath)
        pass

    if args.single_file!='':
        list_bkg.clear()
        list_bkg.append(args.single_file)
        pass

    syst_variations=['nominal']
    if args.run_systematics and args.syst_list!="" and not('data' in  args.single_file): syst_variations = read_syst_variations(args.syst_list)
    print(syst_variations)

        
    print("Starting  >>>>>>>" )  #  GA
    print('Applying on all samples')
    print(apply_sample.filedirapp)
    print(models)
    print(tr_files)
    print(input_sample.variables)
    print(phys_model)
    print(cut_values)
    print(args.sdir)
    print(list_bkg)
    print(">>>>>>> Starting  >>>>>>>" )  #  GA

    mass_points_GM =[200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,600,700,800,900,1000]
    mass_points_HVT=[250,300,350,400,450,500,600,700,800,900,1000]
    mass_points=mass_points_GM
    if phys_model=="HVT":mass_points=mass_points_HVT

    #in case the argument is given, take that instead of default values
    if len(args.mass_points)>0: mass_points=args.mass_points

    for bkg_file in list_bkg:
        print(bkg_file)
        #if "450765" in bkg_file:
        for syst_var in syst_variations:
            #if ("JET" in syst_var) or ("MET" in syst_var) or ("EG" in syst_var) or ("EL" in syst_var): continue
            #if ('450772' in bkg_file) and (('FT_EFF_Eigen_C' in syst_var) or syst_var=='FT_EFF_Eigen_Light_0__1down'): continue
            #if ('410155' in bkg_file) and syst_var=='MUON_EFF_TTVA_SYS__1up' : continue
            print('Current file and systematic variation= ',bkg_file,"\t",syst_var,end='')
            if not check_tree(input_dirpath+bkg_file,syst_var): continue
            analyze_data_folds(input_dirpath,bkg_file,models, tr_files,-1,input_sample.variables,phys_model,cut_values,args.sdir,syst_var,mass_points)
        pass

    print("Successful run!!!")
    
#    print('Applying on sig sample')
#    for i in range(len(list_sig)):
#        print(i)
#        analyze_data_folds(apply_sample.filedirsig,list_sig[i],models, X_mean, X_dev, i,input_sample.variables,phys_model,cut_values)
#        pass
    
    #    print('Applying on data sample')
#    for i in range(len(list_data)):
#        print('input file  =',list_data[i])
#        analyze_data(apply_sample.filedirdata,list_data[i],model, X_mean, X_dev,i,input_sample.variables,phys_model,cut_value)
