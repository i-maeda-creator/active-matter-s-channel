void pairfor_direct(){

    double dx, dy, dfx, dfy, dmt;
    double sij, rij, sr6, fij;

    pot = 0.0;

    for( int i = 0; i < NP-1; i++ ){
    for( int j = i+1; j < NP; j++ ){

        dx = x[i] - x[j];
        dy = y[i] - y[j];

        rij = sqrt(dx*dx+dy*dy);
        sij = r[i] + r[j];

        if( rij < pow(2.0,1.0/6.0)*sij ){

            sr6 = pow(sij/rij,6.0);
            fij = (24.0*EP/rij)*sr6*(2.0*sr6-1.0);

            dfx = fij*dx/rij;
            dfy = fij*dy/rij;
            dmt = KR*sin(th[j]-th[i]);

            fx[i] += dfx;
            fy[i] += dfy;
            mt[i] += dmt;

            fx[j] -= dfx;
            fy[j] -= dfy;
            mt[j] -= dmt;

            pot += 4.0*EP*sr6*(sr6-1.0) + EP;
        }
    }
    }

    return;
}

void wcaring(int i, double dx, double dy){

    double dfx, dfy;
    double sij, rij, sr6, fij;

    rij = sqrt(dx*dx+dy*dy);
    sij = r[i];

    if( rij < pow(2.0,1.0/6.0)*sij ){

        sr6 = pow(sij/rij,6.0);
        fij = (24.0*WE/rij)*sr6*(2.0*sr6-1.0);

        dfx = fij*dx/rij;
        dfy = fij*dy/rij;

        fx[i] += dfx;
        fy[i] += dfy;

        wpo += 4.0*WE*sr6*(sr6-1.0) + WE;
    }

    return;
}

void ringfor(){

    double cx = 0.5*LX;
    double cy = 0.5*LY;
    double rin = RC - 0.5*RW;
    double rout = RC + 0.5*RW;

    if( rin <= 0.0 ){
        printf("Ring inner radius <= 0!\n");
        exit(1);
    }

    wpo = 0.0;

    for( int i = 0; i < NP; i++ ){
        double lx = x[i] - cx;
        double ly = y[i] - cy;
        double rho = sqrt(lx*lx+ly*ly);
        double ex = lx/rho;
        double ey = ly/rho;

        wcaring(i,(rho-rin)*ex,(rho-rin)*ey);
        wcaring(i,(rho-rout)*ex,(rho-rout)*ey);

        th[i] = atan2(ey,ex) + 0.5*M_PI;
    }

    return;
}
