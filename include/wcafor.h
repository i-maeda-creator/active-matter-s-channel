void wcafor(){

    int i,j;

    double dx,dfx;
    double dy,dfy;

    double sij,rij,sr6,fij;



    for(i =0; i< N-1 ; i++){
    for(j =i+1; j< N ; j++){    

            dx = x[i] -x[j];
            dy = y[i] -y[j]; 

            if(dx > 0.5*LX){dx -= LX;}
            if(dy > 0.5*LY){dy -= LY;}
            if(dx < -0.5*LX){dx += LX;}
            if(dy < -0.5*LY){dy += LY;}



            rij = sqrt(dx*dx + dy*dy); //distance
            sij = r[i] + r[j];



            if(rij < pow(2.0,1.0/6.0)*sij ){

                sr6 = pow(sij/rij,6.0);
                fij = (24.0*EPS/rij)*sr6*(2.0*sr6-1.0);

                dfx = fij*dx/rij;
                dfy = fij*dy/rij;

              

                fx[i] += dfx;
                fy[i] += dfy;

                fx[j] -= dfx; //sayo hansayo
                fy[j] -= dfy; //sayo hansayo

            



            }

    }
    }

    return;
}
