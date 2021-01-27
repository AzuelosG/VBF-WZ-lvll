
--> to start:

    git clone https://github.com/AzuelosG/VBF-WZ-lvll.git VBF-WZ-lvll_GA
    ln -s /home/zp/azuelos/lvll/MVA/VBF-WZ-lvll_GL/Inputs/

--> to setup each time:

    source setup.sh


--> for running examples, see:
  
      README_GM,         README_HVT,         README_QQ   

--> to modify :

    1) define git directory:  git remote set-url origin https://USER@github.com/AzuelosG/VBF-WZ-lvll.git
        (replace USER by your Github ID)
    2) check status
   	   git status
    3) pull file to be modified, if you don't have the latest version
         git pull  <filename>
    4) edit it
    5) commit it
         git commit <filename> -m " comment"
    6) push it
        git push origin master  

