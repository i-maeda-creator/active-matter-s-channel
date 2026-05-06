void eforce(){

    int i,j;

    double dd,dl,ff;
    double dx,nx,dfx;
    double dy,ny,dfy;



    for(i =0; i< NP-1 ; i++){
    for(j =i+1; j< NP ; j++){    

            dx = x[i] -x[j];
            dy = y[i] -y[j]; 

            if(dx > 0.5*L){dx -= L;}
            if(dy > 0.5*L){dy -= L;}
            if(dx < -0.5*L){dx += L;}
            if(dy < -0.5*L){dy += L;}



            dd = sqrt(dx*dx + dy*dy); //distance
            dl = r[i] + r[j] -dd; //overlap



            if(dl > 0.0){

                ff = KN * dl;

                nx = dx/dd;
                ny = dy/dd;

                dfx = ff * nx;
                dfy = ff * ny;

                fx[i] += dfx;
                fy[i] += dfy;

                fx[j] -= dfx; //sayo hansayo
                fy[j] -= dfy; //sayo hansayo

                pot += 0.5 * KN * dl * dl;



            }

    }
    }

    return;
}
