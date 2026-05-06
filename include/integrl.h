void seuler(){
 
int i;    
    
    for(i =0; i< NP ; i++){
            
            vx[i] = vx[i] + (fx[i]/MS)*DT;
            vy[i] = vy[i] + (fy[i]/MS)*DT;
             x[i] =  x[i] + vx[i]*DT;
             y[i] =  y[i] + vy[i]*DT;
        
            
    }
    return;
}


void ovdamp(){
 
int i;    
    
    for(i =0; i< NP ; i++){
            
             x[i] +=  fx[i]*DT + xix[i]*sqrt(DT);
             y[i] +=  fy[i]*DT + xiy[i]*sqrt(DT);
             th[i]+=  mt[i]*DT + xit[i]*sqrt(DT);
        
            
    }
    return;
}
