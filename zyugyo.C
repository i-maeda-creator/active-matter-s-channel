#include "include/system.h"
#include "include/output.h"
#include "include/bdcond.h"
#include "include/eforce.h"
#include "include/bforce.h"
#include "include/vforce.h"
#include "include/integrl.h"
#include "include/initia.h"








int main(){

    FILE *fp, *fq;

    fp = fopen("out.dat","w");
    fq = fopen("ene.dat","w");

    
    int i; 
    int istep,ostep;
    

    
    

    double px;
    double py;

    double time;

    monrad();  
    latpos();   
    ranvel();
    
    for(ostep = 0;ostep < OSTEP; ostep++){

     for(istep = 0;istep < ISTEP; istep++){
        
        time += DT; 
         pot =0.0;
            
            inifor();
            eforce();
            //bforce();
            vforce();
            seuler();
            period();
     }


            output(fp);
       

   

    px = 0.0; //syokika
    py = 0.0;
    kin= 0.0;

    for(i =0; i< N ; i++){

        px += MS*vx[i];
        py += MS*vy[i];

        kin += 0.5*MS*(vx[i]*vx[i] + vy[i]+vy[i]);

    }
       /*  px = MS*vx[0] + MS*vx[1];
         py = MS*vy[0] + MS*vy[1];
         kin = 0.5*MS*(vx[0]*vx[0] + vy[0]+vy[0]) + 0.5*MS*(vx[1]*vx[1] + vy[1]+vy[1]); 
     これらをforループに入れて上のようにまとめた
      */


       /*pot = 0.0;

             if(dl > 0.0){

        pot = 0.5*KN*dl*dl;
            }     
            これらも上でまとめた 
        */

    ene = kin + pot;

    fprintf(fq,"%lf %lf %lf %lf\n",time,kin,pot,ene);

    }

    fclose(fp);
    fclose(fq);

      return 0;
}