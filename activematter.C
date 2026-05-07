#include "include/system.h"
#include "include/output.h"
#include "include/bdcond.h"
#include "include/integrl.h"
#include "include/initia.h"
#include "include/rforce.h"
#include "include/aforce.h"
#include "include/kurfor.h"
#include "include/shpfor.h"
#include "include/ringroad.h"

#include <errno.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

int make_dir(const char *path){
#ifdef _WIN32
    return _mkdir(path);
#else
    return mkdir(path, 0755);
#endif
}

int ensure_dir(const char *path){
    char tmp[512];
    int len = snprintf(tmp,sizeof(tmp),"%s",path);

    if( len <= 0 || len >= (int)sizeof(tmp) ) return 1;

    for( int i = 1; tmp[i] != '\0'; i++ ){
        if( tmp[i] == '/' || tmp[i] == '\\' ){
            char save = tmp[i];
            tmp[i] = '\0';
            if( make_dir(tmp) != 0 && errno != EEXIST ) return 1;
            tmp[i] = save;
        }
    }

    if( make_dir(tmp) != 0 && errno != EEXIST ) return 1;

    return 0;
}

int main(int argc, char **argv){

    FILE *fp, *fq, *fr;
    char out_path[512];
    char ene_path[512];
    char params_path[512];
    const char *output_dir = "runs/latest";
    const char *init_mode = "shape";
    int output_steps = OSTEP;
    int inner_steps = ISTEP;

    if( argc >= 2 ) output_dir = argv[1];
    if( argc >= 3 ){
        output_steps = atoi(argv[2]);
        if( output_steps <= 0 ){
            printf("Output step count must be positive.\n");
            return 1;
        }
    }
    if( argc >= 4 ){
        inner_steps = atoi(argv[3]);
        if( inner_steps <= 0 ){
            printf("Inner step count must be positive.\n");
            return 1;
        }
    }
    if( argc >= 5 ) init_mode = argv[4];
    if( argc >= 6 ){
        R = atof(argv[5]);
        if( R <= 0.0 ){
            printf("Radius must be positive.\n");
            return 1;
        }
    }
    if( argc >= 7 ){
        NP = atoi(argv[6]);
        if( NP <= 0 || NP > N ){
            printf("Particle count must be between 1 and %d.\n",N);
            return 1;
        }
    }
    if( argc >= 8 ){
        LX = atof(argv[7]);
        if( LX <= 0.0 ){
            printf("LX must be positive.\n");
            return 1;
        }
    }
    if( argc >= 9 ){
        LY = atof(argv[8]);
        if( LY <= 0.0 ){
            printf("LY must be positive.\n");
            return 1;
        }
    }
    if( argc >= 10 ){
        RC = atof(argv[9]);
        if( RC <= 0.0 ){
            printf("RC must be positive.\n");
            return 1;
        }
    }
    if( argc >= 11 ){
        RW = atof(argv[10]);
        if( RW <= 0.0 ){
            printf("RW must be positive.\n");
            return 1;
        }
    }

    if( ensure_dir(output_dir) != 0 ){
        printf("Failed to create output directory: %s\n",output_dir);
        return 1;
    }

    snprintf(out_path,sizeof(out_path),"%s/out.dat",output_dir);
    snprintf(ene_path,sizeof(ene_path),"%s/ene.dat",output_dir);
    snprintf(params_path,sizeof(params_path),"%s/params.txt",output_dir);

    fp = fopen(out_path,"w");
    fq = fopen(ene_path,"w");
    fr = fopen(params_path,"w");

    if( fp == NULL || fq == NULL || fr == NULL ){
        printf("Failed to open output files in %s.\n",output_dir);
        return 1;
    }
   
    int istep,ostep;
    
    double time = 0.0;

    fprintf(fr,"N_MAX %d\n",N);
    fprintf(fr,"NP %d\n",NP);
    fprintf(fr,"OSTEP %d\n",OSTEP);
    fprintf(fr,"OUTPUT_STEPS %d\n",output_steps);
    fprintf(fr,"ISTEP %d\n",ISTEP);
    fprintf(fr,"INNER_STEPS %d\n",inner_steps);
    fprintf(fr,"DT %.15g\n",DT);
    fprintf(fr,"R %.15g\n",R);
    fprintf(fr,"LX %.15g\n",LX);
    fprintf(fr,"LY %.15g\n",LY);
    fprintf(fr,"WS %.15g\n",WS);
    fprintf(fr,"RC %.15g\n",RC);
    fprintf(fr,"RW %.15g\n",RW);
    fprintf(fr,"WE %.15g\n",WE);
    fprintf(fr,"EP %.15g\n",EP);
    fprintf(fr,"FA %.15g\n",FA);
    fprintf(fr,"KR %.15g\n",KR);
    fprintf(fr,"DD %.15g\n",DD);
    fprintf(fr,"DR %.15g\n",DR);
    fprintf(fr,"SEED %d\n",0);
    fprintf(fr,"OUTPUT_DIR %s\n",output_dir);
    fprintf(fr,"INIT_MODE %s\n",init_mode);
    fclose(fr);
    
    srand(0);
    monrad();  
    if( strcmp(init_mode,"random") == 0 ){
        ranpos();
    }else if( strcmp(init_mode,"shape") == 0 ){
        shppos();
    }else if( strcmp(init_mode,"ring") == 0 ){
        ringpos();
    }else{
        printf("Unknown init mode: %s. Use shape, random, or ring.\n",init_mode);
        return 1;
    }
    if( strcmp(init_mode,"ring") != 0 ) inithe();
    
    for(ostep = 0;ostep < output_steps; ostep++){

     for(istep = 0;istep < inner_steps; istep++){
        
        time += DT; 
            
            inifor();
            if( strcmp(init_mode,"ring") == 0 ){
                pairfor_direct();
            }else{
                kurfor();
            }
            rforce();
            aforce();
            if( strcmp(init_mode,"ring") == 0 ){
                ringfor();
            }else{
                shpfor();
            }
            ovdamp();
            if( strcmp(init_mode,"ring") != 0 ) pershp();
            
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
