void monrad(){

    int i;

    for( i = 0; i < N; i++ ) r[i] = R;

    return;
}

void latpos(){

    int i;
    int m = 23;

    for( i = 0; i < N; i++ ){
        x[i] = LY/(2.0*(double)m) + (i%m)*(LY/(double)m);
        y[i] = LY/(2.0*(double)m) + (i/m)*(LY/(double)m);
    }

    return;
}

void ranpos(){

    int i, j, c;
    double dx, dy, rij, sij;

    for( i = 0; i < N; i++ ){

        c = 1;

        while( c ){

            c = 0;

            x[i] = LX*(double)rand()/((double)RAND_MAX + 1.0);
            y[i] = (LY-2.0*R)*(double)rand()/((double)RAND_MAX + 1.0) + R;

            for( j = 0; j < i; j++ ){

                dx = x[i] - x[j];
                dy = y[i] - y[j];

                if( dx >  0.5*LX ){ dx -= LX;}
                if( dy >  0.5*LY ){ dy -= LY;}
                if( dx < -0.5*LX ){ dx += LX;}
                if( dy < -0.5*LY ){ dy += LY;}

                rij = sqrt(dx*dx+dy*dy);
                sij = r[i] + r[j];

                if( rij < pow(2.0,1.0/6.0)*sij ){
                    c = 1;
                    break;
                }
            }
        }
    }

    return;
}

void shppos(){

    int i, j, c;
    double dx, dy, rij, sij;
    double mR = 1.1225*R; /*!*/

    for( i = 0; i < N; i++ ){

        c = 1;

        while( c ){

            c = 0;

            x[i] = (0.5*LX)*(double)rand()/((double)RAND_MAX + 1.0);
            y[i] = (WS-2.0*mR)*(double)rand()/((double)RAND_MAX + 1.0) + mR + LY - WS; /*!*/

            for( j = 0; j < i; j++ ){

                dx = x[i] - x[j];
                dy = y[i] - y[j];

                rij = sqrt(dx*dx+dy*dy);
                sij = r[i] + r[j];

                if( rij < pow(2.0,1.0/6.0)*sij ){
                    c = 1;
                    break;
                }
            }
        }
    }

    return;
}

void ranvel(){

    int i;
    double TMP = 0.0; // temperature
    double vmp = sqrt(2.0*TMP/MS);

    for( i = 0; i < N; i++ ){
        vx[i] = vmp*sqrt(-log((double)rand()/(double)RAND_MAX))*cos(2.0*M_PI*(double)rand()/(double)RAND_MAX);
        vy[i] = vmp*sqrt(-log((double)rand()/(double)RAND_MAX))*cos(2.0*M_PI*(double)rand()/(double)RAND_MAX);
    }

    return;
}

void ranthe(){

    int i;

    for( i = 0; i < N; i++ ){
        th[i] = 2.0*M_PI*((double)rand()/(double)RAND_MAX);
    }

    return;
}

void inithe(){

    int i;

    for( i = 0; i < N; i++ ) th[i] = 0.0;

    return;
}

void inifor(){

    int i;

    for( i = 0; i < N; i++ ){
        fx[i] = 0.0;
        fy[i] = 0.0;
        mt[i] = 0.0;
    }

    return;
}