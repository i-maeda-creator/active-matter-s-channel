void bforce(){
 
int i;    
    
    for(i =0; i< N ; i++){
            
             fx[i] -=  BE*vx[i];
             fy[i] -=  BE*vy[i];
        
            
    }
    return;
}
