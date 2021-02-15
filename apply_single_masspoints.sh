#Example to apply only to those mass points

for mass in 200 300 400 500;
do  python3 Apply_NN.py --sdir GM_test/m$mass; 
    done  | tee  log/Apply_GM_test
