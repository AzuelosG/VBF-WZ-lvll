import numpy as np
import json
import re
from copy import deepcopy

Filedir    = 'Inputs/' #to change input dataset, change the link in the Inputs directory

########################################################################################
class input_samples_qq:   ## input sample for qq HVT
    #Assumed luminosity
    lumi = 140.
    #Fraction used for training 
    trafrac = 0.9
    #Directory where ntuples are located
    filedir = Filedir
    filedirsig = filedir
    # HVT qq-fusion Samples
    bckgr = {  # here, bckgr is qq HVT
        'name' : [
                  'resonance.302266_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0500_ntuples.root',
                  'resonance.302267_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0600_ntuples.root',
                  'resonance.302268_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0700_ntuples.root',
                  'resonance.302269_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0800_ntuples.root',
                  'resonance.302270_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0900_ntuples.root',
                  'resonance.302271_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1000_ntuples.root'
#                  'resonance.302272_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1100_ntuples.root',
#                  'resonance.302273_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1200_ntuples.root',
#                  'resonance.302274_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1300_ntuples.root',
#                  'resonance.302275_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1400_ntuples.root',
#                  'resonance.302276_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1500_ntuples.root',
#                  'resonance.302277_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1600_ntuples.root',
#                  'resonance.302278_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1700_ntuples.root',
#                  'resonance.302279_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1800_ntuples.root',
#                  'resonance.302280_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m1900_ntuples.root',
#                  'resonance.302281_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m2000_ntuples.root',
#                  'resonance.302282_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m2200_ntuples.root',
#                  'resonance.302283_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m2400_ntuples.root',
#                  'resonance.302284_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m2600_ntuples.root'
#                  'resonance.302285_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m2800_ntuples.root',
#                  'resonance.302286_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m3000_ntuples.root',
#                  'resonance.302287_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m3500_ntuples.root',
#                  'resonance.302288_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m4000_ntuples.root',
#                  'resonance.302889_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m4500_ntuples.root',
#                  'resonance.302290_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m5000_ntuples.root'
                  ]}
    #  HVT VBF samples
    sigHVT = {
        'name' : [
#                 'resonance.307730_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0250_ntuples.root',
#                  'resonance.307731_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0300_ntuples.root',
#                  'resonance.309528_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0350_ntuples.root',
#                  'resonance.307732_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0400_ntuples.root',
#                  'resonance.309529_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0450_ntuples.root',
                  'resonance.307733_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0500_ntuples.root',
                  'resonance.307734_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0600_ntuples.root',
                  'resonance.307735_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0700_ntuples.root',
                  'resonance.307736_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0800_ntuples.root',
                  'resonance.307737_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0900_ntuples.root',
                  'resonance.307738_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m1000_ntuples.root',
                  ]
}

    #Variables used for training
    variables = ['M_jj','Deta_jj', 'Dphi_jj',
                 'Jet1Pt', 'Jet2Pt',
                 'Jet1Eta','Jet2Eta',
                 'Jet1E','Jet2E',#'Jet3Eta',
                 'Lep1Eta','Lep2Eta', 'Lep3Eta', 
                 'PtBalanceZ','PtBalanceW',
                 'Eta_W', 'Eta_Z',
                 'ZetaLep'#,'Njets'
                 ]

########################################################################################
class input_samples:   ## default input_samples for vbf vs qq
    #Assumed luminosity
    lumi = 140.
    #Fraction used for training 
    trafrac = 0.9
    #Directory where ntuples are located
    filedir = Filedir
    #Bkg Samples
    bckgr = {
        'name' : [ 
#                   'mainMVA.361292_MGaMcAtNloPy8EG_NNPDF30LO_A14NNPDF23LO_WZ_lvll_FxFx_ntuples.root',
#                   'resonance.364250_Sherpa_222_NNPDF30NNLO_llll_ntuples.root.0',
#                   'resonance.364250_Sherpa_222_NNPDF30NNLO_llll_ntuples.root.1',
#                   'resonance.364250_Sherpa_222_NNPDF30NNLO_llll_ntuples.root.2',
                   'resonance.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.0.root',   ## WZ QCD
#                   'resonance.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.1.root', #OK to skip this, only contians syst variation not nominal
#                   'resonance.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.2.root', #same as above line
                   'resonance.364739_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_OFMinus_ntuples.root',
                   'resonance.364740_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_OFPlus_ntuples.root',
                   'resonance.364741_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_SFMinus_ntuples.root',
                   'resonance.364742_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_SFPlus_ntuples.root' ## WZ EW
]
# ],
#        'xs' : [1704., 47.],
#        'nevents' : [3890000, 7325000]
    }

#Signal Samples  >> default, GM signal samples
#    filedirsig = filedir #+ 'new/signalsNew/mc16d/'
    filedirsig = 'Inputs' + '/' #+ 'new/signalsNew/mc16d/'
    #GM signal files
    sigGM = {
        'name' : [
            'resonance.450765_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m200_ntuples.root',
            'resonance.502511_aMCPy8EG_GM_H5_WZ_lvll_m225_lepfilt_ntuples.root',
            'resonance.450766_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m250_ntuples.root ',
            'resonance.502512_aMCPy8EG_GM_H5_WZ_lvll_m275_lepfilt_ntuples.root',
            'resonance.450767_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m300_ntuples.root ',
            'resonance.502513_aMCPy8EG_GM_H5_WZ_lvll_m325_lepfilt_ntuples.root',
            'resonance.450768_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m350_ntuples.root',
            'resonance.502514_aMCPy8EG_GM_H5_WZ_lvll_m375_lepfilt_ntuples.root',
            'resonance.450769_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m400_ntuples.root',
            'resonance.502515_aMCPy8EG_GM_H5_WZ_lvll_m425_lepfilt_ntuples.root',
            'resonance.450770_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m450_ntuples.root',
            'resonance.502516_aMCPy8EG_GM_H5_WZ_lvll_m475_lepfilt_ntuples.root',
            'resonance.450771_MGaMcAtNloPy8EG_A14NNPDF23LO_vbfGM_sH05_H5pWZ_lvll_m500_ntuples.root',
            'resonance.502517_aMCPy8EG_GM_H5_WZ_lvll_m525_lepfilt_ntuples.root',
            'resonance.502518_aMCPy8EG_GM_H5_WZ_lvll_m550_lepfilt_ntuples.root',
            'resonance.502519_aMCPy8EG_GM_H5_WZ_lvll_m600_lepfilt_ntuples.root',
            'resonance.502520_aMCPy8EG_GM_H5_WZ_lvll_m700_lepfilt_ntuples.root',
            'resonance.502521_aMCPy8EG_GM_H5_WZ_lvll_m800_lepfilt_ntuples.root',
            'resonance.502522_aMCPy8EG_GM_H5_WZ_lvll_m900_lepfilt_ntuples.root',
            'resonance.502523_aMCPy8EG_GM_H5_WZ_lvll_m1000_lepfilt_ntuples.root'
            ],

#        mass           200,  225,  250,  275, 300, 325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 600, 700, 800, 900, 1000
        'switch'  : [     1,    0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0],  # mass points to be included in training
#        'xs'      : [23.8, 20.31, 17.54, 15.04, 13.78, 11.33, 10.54, 9.147, 8.215, 7.116, 6.342, 5.291, 4.979, 3.902, 3.449, 2.782, 1.994, 1.327, 0.2495, 0.1842],
#        'nevents' : [ 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000, 200000 ]
    }
    #HVT vbf signal files
    filedir =   Filedir
    sigHVT = {
        'name' : ['resonance.307730_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0250_ntuples.root',
                  'resonance.307731_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0300_ntuples.root',
                  'resonance.309528_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0350_ntuples.root',
                  'resonance.307732_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0400_ntuples.root',
                  'resonance.309529_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0450_ntuples.root',
                  'resonance.307733_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0500_ntuples.root',
                  'resonance.307734_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0600_ntuples.root',
                  'resonance.307735_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0700_ntuples.root',
                  'resonance.307736_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0800_ntuples.root',
                  'resonance.307737_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0900_ntuples.root',
                  'resonance.307738_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m1000_ntuples.root'
                  ],
        'switch'  : [     1,      1,      1,      0,      0,      0,      0,        0,    0,     0,      0]  # switch on masses 250 , 350 and 400
# mass                 250   300  350   400   450   500   600    700  800  900   1000
    }
    #Variables used for training
    variables = ['M_jj','Deta_jj', 'Dphi_jj',
                 'Jet1Pt', 'Jet2Pt',
                 'Jet1Eta','Jet2Eta',
                 'Jet1E','Jet2E',
                 #'Jet3Eta', #added
                 #'Lep1Pt', 'Lep2Pt','Lep3Pt', ### removed to reduce mass dependence
                 'Lep1Eta','Lep2Eta', 'Lep3Eta', 
                 'PtBalanceZ','PtBalanceW',
                 #'Pt_W', 'Pt_Z',  ### removed to reduce mass dependence
                 'Eta_W', 'Eta_Z', ### added instead
                 'ZetaLep'#,'Njets' #,'Met'
                 ]

    #original set
    #variables = ['M_jj','Deta_jj',
    #             'Jet1Pt', 'Jet2Pt', 'Jet1Eta','Jet2Eta', 'Jet1E','Jet2E',
    #             'Lep1Pt', 'Lep2Pt', 'Lep3Pt', 'Lep1Eta','Lep2Eta','Lep3Eta',
    #             'PtBalanceZ','PtBalanceW',
    #             'Pt_W', 'Pt_Z', 
    #             'ZetaLep','Njets','Met']

#Contains list of samples to apply NN
class apply_samples:
    import os
    # filedirdata= FiledirData
#    filedirapp = Filedir+"/"
    filedirapp = Filedir
    filedirsig = filedirapp #+ 'new/signalsNew/mc16a/'

    # Signal files
    list_apply_sigGM = input_samples.sigGM['name']

    list_apply_sigHVT = input_samples.sigHVT['name']
    
    # parse all files in the directory, except signals
    list_apply_bkg = [
        'resonance.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.0.root',
        'resonance.364739_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_OFMinus_ntuples.root',
        'resonance.364740_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_OFPlus_ntuples.root',
        'resonance.364741_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_SFMinus_ntuples.root',
        'resonance.364742_MGPy8EG_NNPDF30NLO_A14NNPDF23LO_lvlljjEW6_SFPlus_ntuples.root'
        ]
#    list_apply_bkg = [  'resonance.302267_MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m0600_ntuples.root']   # QQ HVT, 600 GeV
    
#    shortList= [307733, 307734, 307735]   # 500, 600, 700 GeV   HVT signal
#    shortList= [450765,502511,450766,502512,450767,502513,450768,502514,450769,502515,450770,502516,450771,502517,502518,502519,502520,502521,502522,502523, #GM  sig
#                307730,307731,307732,307733,307734,307735,307736,307737,307738,       #HVT sig
#                361292,364284,                                                                #WZ bkg
#                302266,302267,302268,302269,302270,302271,302272,302273,302274,302275,302276,302277,302278,302279,302280,302281            #QQ HVT sig
#               ]
    
#GM  sig files to apply NN to
    shortList= [
        450765,  502511,  450766,  502512,  450767,  502513,  450768,  502514,  450769,  502515,  450770,  502516,  450771,  502517,  502518,  502519,  502520,  502521,  502522, 502523
#mass:     200,     225,     250,     275,     300,     325,     350,     375,     400,     425,     450,     475,     500,     525,     550,     600,     700,     800,     900,   1000

# HVT samples:
                ,307730,307731,309528,307732,309529,307733,307734,307735,307736,307737,307738]       #HVT sig
#                361292,364284,                                                                #WZ bkg
#                302266,302267,302268,302269,302270,302271,302272,302273,302274,302275,302276,302277,302278,302279,302280,302281           #QQ HVT sig
#                ]
#    shortList= [307730,  309528, 307732, 307734] # masses 250, 350, 400, 600
    
    #shortList=list() #uncomment when applying to all samples
    
    for r,d,f in os.walk(filedirapp):
        #print(f)
        for file in f:
            if 'history' in file: continue

            skipFlag=True
            for ch in shortList:
                if "{}".format(ch) in file: 
                    skipFlag=False
                    break
                pass
            if skipFlag: continue

            if '.root' in file:
                print(file)
                #if file in list_apply_sigGM: continue
                #elif file in list_apply_sigHVT: continue
                #else: list_apply_bkg.append(file)
                
#                if   file=="MVA.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.root" or   file=="MVA.364284_Sherpa_222_NNPDF30NNLO_lllvjj_EW6_ntuples.root": 
#                    print(file)
#                    exit(1)
#                    pass

                list_apply_bkg.append(file)
            pass
        pass
    #print(list_apply_bkg) #exit(0)

    # data files
    # list_apply_data = []
    # for r,d,f in os.walk(filedirdata):
    #     for file in f:        
    #         if '.root' in file:
    #             list_apply_data.append(file)
    #             pass
    #         pass
    #     pass

    labelGM = np.arange(len(list_apply_sigGM))
    labelHVT = np.arange(len(list_apply_sigHVT))


def decorate_class_dict(class_dict):
    '''
    Append {mass -> sample name} map to class_dict
    '''
    def is_model_sample_var(var_name):
        return type(class_dict[var_name]) == dict \
               and 'name' in class_dict[var_name].keys() \
               and 'switch' in class_dict[var_name].keys()
    model_sample_vars = deepcopy(filter(is_model_sample_var, class_dict.keys()))
    for var_name in model_sample_vars:
        sample_mass_dict = dict()
        for sample_name in class_dict[var_name]['name']:
            match = re.search(r'_m\d*_', sample_name)
            if not match:
                raise RuntimeError('Cannot find mass in sample name %s' % sample_name)
            mass = int(match[0].strip('_').strip('m'))
            sample_mass_dict[mass] = sample_name
        class_dict[var_name+'_map'] = sample_mass_dict


def dump_to_JSON():
    output_dict = dict()
    for c in [input_samples, input_samples_qq]:
        class_dict = dict([(k,v) for k,v in c.__dict__.items() if not k.startswith('__')])
        decorate_class_dict(class_dict)
        output_dict[c.__name__] = class_dict

    with open('config_NN.json', 'w') as f:
        json.dump(output_dict, f, indent=4)

if __name__ == '__main__':
    dump_to_JSON()        
