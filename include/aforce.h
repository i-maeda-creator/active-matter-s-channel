void aforce(){



    int i;
      
    

    for(i=0; i < NP; i++){

        fx[i] += FA*cos(th[i]);
        fy[i] += FA*sin(th[i]);
        

        
   
    }
    
    return;

}
