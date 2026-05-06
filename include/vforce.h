void vforce(){

    int i,j;

    double dd,dl,ff;
    double dx,nx,gx,dfx;
    double dy,ny,gy,dfy;



    for(i =0; i< N-1 ; i++){
    for(j =i+1; j< N ; j++){    

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

                gx = vx[i] - vx[j];
                gy = vy[i] - vy[j];

                dfx = ff * nx - BE*gx;
                dfy = ff * ny - BE*gy;

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
