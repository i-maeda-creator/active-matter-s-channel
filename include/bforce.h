void bforce(){
 
int i;    
    
    for(i =0; i< NP ; i++){
            
             fx[i] -=  BE*vx[i];
             fy[i] -=  BE*vy[i];
        
            
    }
    return;
}
