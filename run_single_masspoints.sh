#Run a few single masspoint trainings for 4-fold

# loop over mass points, change the mass points according to your needs
for mass in 200  300  400  500;
 do for n in 0 1 2 3; # loop over nfolds
     do python3 OPT_VBS_NN.py --mass_points $mass --model GM --dropout=0.20 --lr=0.013 --patience=18 --numn=10 --numlayer=3 --epochs=100 --Findex $n --nFold 4 --sdir GM_test/m$mass;
     done;
   done   &>  log/training_GM_test.log
