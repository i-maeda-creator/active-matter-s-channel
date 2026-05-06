#include "include/system.h"
#include "include/output.h"
#include "include/bdcond.h"
#include "include/integrl.h"
#include "include/initia.h"
#include "include/rforce.h"
#include "include/aforce.h"
#include "include/kurfor.h"
#include "include/shpfor.h"

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
        if( output_steps <= 0 || output_steps > OSTEP ){
            printf("Output step count must be between 1 and %d.\n",OSTEP);
            return 1;
        }
    }
    if( argc >= 4 ){
        inner_steps = atoi(argv[3]);
        if( inner_steps <= 0 || inner_steps > ISTEP ){
            printf("Inner step count must be between 1 and %d.\n",ISTEP);
            return 1;
        }
    }
    if( argc >= 5 ) init_mode = argv[4];

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

    fprintf(fr,"N %d\n",N);
    fprintf(fr,"OSTEP %d\n",OSTEP);
    fprintf(fr,"OUTPUT_STEPS %d\n",output_steps);
    fprintf(fr,"ISTEP %d\n",ISTEP);
    fprintf(fr,"INNER_STEPS %d\n",inner_steps);
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
    }else{
        printf("Unknown init mode: %s. Use shape or random.\n",init_mode);
        return 1;
    }
    inithe();
    
    for(ostep = 0;ostep < output_steps; ostep++){

     for(istep = 0;istep < inner_steps; istep++){
        
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
