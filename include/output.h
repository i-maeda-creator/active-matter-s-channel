void output(FILE *fp){

    int i;
   
    for(i =0; i< N ; i++){

            fprintf(fp,"%lf %lf %lf\n",x[i],y[i],r[i]);
        }

    fprintf(fp,"\n\n");  //kaigyo ga 2kai hairuto index tosite wakeru kotoga dekiru. kaku zikan step deno itiwo plot dekiru  

 
    return;
}