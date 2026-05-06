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

    FILE *fp, *fq;

    fp = fopen("dat/out.dat","w");
    fq = fopen("dat/ene.dat","w");
   
    int i; 
    int istep,ostep;
    
    double time;
    
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
      

    ene = kin + pot;

    fprintf(fq,"%lf %lf %lf %lf\n",time,kin,pot,ene);

    }

    fclose(fp);
    fclose(fq);

      return 0;
}