import numpy as np

Filedir    = 'Inputs/MVA' #to change input dataset, change the link in the Inputs directory

class input_samples:
    #Assumed luminosity
    lumi = 140.
    #Fraction used for training 
    valfrac = 0.1
    #Directory where ntuples are located
    filedir = Filedir+"/"#+"main"
    #Bkg Samples
    bckgr = {
        'name' : ['MVA.364253_Sherpa_222_NNPDF30NNLO_lllv_ntuples.root', 
                  'MVA.364284_Sherpa_222_NNPDF30NNLO_lllvjj_EW6_ntuples.root'],
        'xs' : [4579., 47.],
        'nevents' : [75259300, 7325000]
    }

    #Signal Samples
    filedirsig = filedir #+ 'new/signalsNew/mc16d/'
    #GM signal files
    sigGM = {
        'name' : ['MVA.305028_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_200_qcd0_ntuples.root',
                  'MVA.309500_MadGraphPythia8EvtGen_A14NNPDF23LO_VBSWZlvlljj_m250_ISH0p5_ntuples.root',
                  'MVA.305029_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_300_qcd0_ntuples.root', #down to here it's working for all mass points
                  'MVA.309501_MadGraphPythia8EvtGen_A14NNPDF23LO_VBSWZlvlljj_m350_ISH0p5_ntuples.root',
                  'MVA.305030_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_400_qcd0_ntuples.root',
                  'MVA.309498_MadGraphPythia8EvtGen_A14NNPDF23LO_VBSWZlvlljj_m450_ISH0p5_ntuples.root',
                  'MVA.305031_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_500_qcd0_ntuples.root',
                  'MVA.305032_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_600_qcd0_ntuples.root',
                  'MVA.305033_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_700_qcd0_ntuples.root',
                  'MVA.305034_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_800_qcd0_ntuples.root',
                  'MVA.305035_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_900_qcd0_ntuples.root'
                  ],
        'xs' : [1.765,1.4911,0.98095,0.8874,0.6107,0.6391,0.4028,0.2751,0.1934,0.1386,0.1010],
        'nevents' : [160000,160000,160000,280000,160000,160000,160000,160000,160000,160000,160000]
    }
    #HVT signal files
    sigHVT = {
        'name' : ['MVA.307730_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0250_ntuples.root',
                  'MVA.307731_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0300_ntuples.root',
                  'MVA.307732_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0400_ntuples.root',
                  'MVA.307733_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0500_ntuples.root',
                  'MVA.307734_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0600_ntuples.root',
                  'MVA.307735_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0700_ntuples.root',
                  'MVA.307736_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0800_ntuples.root',
                  'MVA.307737_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0900_ntuples.root',
                  'MVA.307738_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m1000_ntuples.root',
                  'MVA.309528_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0350_ntuples.root',
                  'MVA.309529_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0450_ntuples.root'
                  ],
        'xs' : [24.42,10.54,2.299,0.7975,0.3408,0.1663,0.087984,0.049882,0.02961,1.3050,4.4843],
        'nevents' : [190000,190000,190000,190000,190000,190000,190000,175000,190000,160000,160000]
    }
    #Variables used for training
    variables = ['M_jj','Deta_jj','Jet1Pt', 'Jet2Pt',
                 'Jet1Eta','Jet2Eta','Jet1E','Jet2E',
                 ###Lepton related variables
                 #'Lep1Pt', 'Lep2Pt','Lep3Pt', ### removed to reduce mass dependence
                 'Lep1Eta','Lep2Eta', 'Lep3Eta',
                 ###WZ properties
                 'PtBalanceZ','PtBalanceW',
                 #'Pt_W', 'Pt_Z', ### removed to reduce mass dependence
                 'Eta_W', 'Eta_Z',
                 'ZetaLep','Njets','Met']
#No 3rd jet
#                     'm_Pt_jet3',,'m_Eta_jet3''m_E_jet3',

#Contains list of samples to apply NN
class apply_samples:
    import os
    # filedirdata= FiledirData
    filedirapp = Filedir+"/"
    filedirsig = filedirapp #+ 'new/signalsNew/mc16a/'

    # Signal files
    list_apply_sigGM = ['MVA.305028_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_200_qcd0_ntuples.root',
                        'MVA.309500_MadGraphPythia8EvtGen_A14NNPDF23LO_VBSWZlvlljj_m250_ISH0p5_ntuples.root',
                        'MVA.305029_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_300_qcd0_ntuples.root',
                        'MVA.309501_MadGraphPythia8EvtGen_A14NNPDF23LO_VBSWZlvlljj_m350_ISH0p5_ntuples.root',
                        'MVA.305030_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_400_qcd0_ntuples.root',
                        'MVA.309498_MadGraphPythia8EvtGen_A14NNPDF23LO_VBSWZlvlljj_m450_ISH0p5_ntuples.root',
                        'MVA.305031_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_500_qcd0_ntuples.root',
                        'MVA.305032_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_600_qcd0_ntuples.root',
                        'MVA.305033_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_700_qcd0_ntuples.root',
                        'MVA.305034_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_800_qcd0_ntuples.root',
                        'MVA.305035_MGPy8_A14NNPDF30NLO_VBS_H5p_lvll_900_qcd0_ntuples.root'
                        ]

    list_apply_sigHVT = ['MVA.307730_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0250_ntuples.root',
                         'MVA.307731_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0300_ntuples.root',
                         'MVA.307732_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0400_ntuples.root',
                         'MVA.307733_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0500_ntuples.root',
                         'MVA.307734_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0600_ntuples.root',
                         'MVA.307735_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0700_ntuples.root',
                         'MVA.307736_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0800_ntuples.root',
                         'MVA.307737_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0900_ntuples.root',
                         'MVA.307738_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m1000_ntuples.root',
                         'MVA.309528_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0350_ntuples.root',
                         'MVA.309529_MGPy8EG_A14NNPDF23LO_vbfHVT_Agv1_VzWZ_lvll_m0450_ntuples.root'
                         ]

    
    # parse all files in the directory, except signals
    list_apply_bkg = []
    
    for r,d,f in os.walk(filedirapp):
        #print(f)
        for file in f:
            if 'history' in file: continue
            if '.root' in file:
                #print(file)
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
