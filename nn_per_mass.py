import ROOT
import sys

ROOT.gROOT.SetBatch(True)
ROOT.gROOT.ProcessLine(".L nn_per_mass.C+")

# 1  void nn_per_mass(string dir="", 
# 2                  string name="",
# 3                  TString varname="pSignal_GM",
# 4                  bool norm2yield=true, 
# 5                  TString phys_model="GM", 
# 6                  bool drawCB=true, 
# 7                  bool mMulti=false,
# 8                  int refMass4cut=200

if   len(sys.argv)>8: ROOT.nn_per_mass(sys.argv[1],sys.argv[2],sys.argv[3],bool(int(sys.argv[4])),sys.argv[5], bool(int(sys.argv[6])), bool(int(sys.argv[7])),int(sys.argv[8]))
elif len(sys.argv)>7: ROOT.nn_per_mass(sys.argv[1],sys.argv[2],sys.argv[3],bool(int(sys.argv[4])),sys.argv[5], bool(int(sys.argv[6])), bool(int(sys.argv[7])))
elif len(sys.argv)>6: ROOT.nn_per_mass(sys.argv[1],sys.argv[2],sys.argv[3],bool(int(sys.argv[4])),sys.argv[5], bool(int(sys.argv[6])))
elif len(sys.argv)>5: ROOT.nn_per_mass(sys.argv[1],sys.argv[2],sys.argv[3],bool(int(sys.argv[4])),sys.argv[5])
elif len(sys.argv)>4: ROOT.nn_per_mass(sys.argv[1],sys.argv[2],sys.argv[3],bool(int(sys.argv[4])))
elif len(sys.argv)>3: ROOT.nn_per_mass(sys.argv[1],sys.argv[2],sys.argv[3])
elif len(sys.argv)>2: ROOT.nn_per_mass(sys.argv[1],sys.argv[2])
elif len(sys.argv)>1: ROOT.nn_per_mass(sys.argv[1])
else:                 ROOT.nn_per_mass()
