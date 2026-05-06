#include "include/system.h"
#include "include/output.h"
#include "include/bdcond.h"
#include "include/integrl.h"
#include "include/initia.h"
#include "include/rforce.h"
#include "include/aforce.h"
#include "include/kurfor.h"
#include "include/shpfor.h"

int main(){

    FILE *fp, *fq, *fr;

    fp = fopen("dat/out.dat","w");
    fq = fopen("dat/ene.dat","w");
    fr = fopen("dat/params.txt","w");

    if( fp == NULL || fq == NULL || fr == NULL ){
        printf("Failed to open output files. Make sure the dat directory exists.\n");
        return 1;
    }
   
    int istep,ostep;
    
    double time = 0.0;

    fprintf(fr,"N %d\n",N);
    fprintf(fr,"OSTEP %d\n",OSTEP);
    fprintf(fr,"ISTEP %d\n",ISTEP);
    fprintf(fr,"DT %.15g\n",DT);
    fprintf(fr,"R %.15g\n",R);
    fprintf(fr,"LX %.15g\n",LX);
    fprintf(fr,"LY %.15g\n",LY);
    fprintf(fr,"WS %.15g\n",WS);
    fprintf(fr,"WE %.15g\n",WE);
    fprintf(fr,"EP %.15g\n",EP);
    fprintf(fr,"FA %.15g\n",FA);
    fprintf(fr,"KR %.15g\n",KR);
    fprintf(fr,"DD %.15g\n",DD);
    fprintf(fr,"DR %.15g\n",DR);
    fclose(fr);
    
    srand(0);
    monrad();  
    //ranpos();  
    shppos(); 
    inithe();
    
    for(ostep = 0;ostep < OSTEP; ostep++){

     for(istep = 0;istep < ISTEP; istep++){
        
        time += DT; 
            
            inifor();
           // wcafor();
            kurfor();
            rforce();
            aforce();
            shpfor();
            ovdamp();
            pershp();
            
     }


            output(fp);
       

   

    kin= 0.0;

    /*for(i =0; i< N ; i++){


        kin += 0.5*MS*(vx[i]*vx[i] + vy[i] + vy[i]);

    } */
      

    ene = kin + pot + wpo;

    fprintf(fq,"%lf %lf %lf %lf %lf\n",time,kin,pot,wpo,ene);

    }

    fclose(fp);
    fclose(fq);

      return 0;
}
