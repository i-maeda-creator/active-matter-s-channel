void wcawal(int i, double dx, double dy){

    double dfx, dfy;
    double sij, rij, sr6, fij;

    rij = sqrt(dx*dx+dy*dy);
    sij = r[i];

    if( rij < pow(2.0,1.0/6.0)*sij ){

        sr6 = pow(sij/rij,6.0);
        fij = (24.0*WS/rij)*sr6*(2.0*sr6-1.0);

        dfx = fij*dx/rij; /* x-component of f_{ij} */
        dfy = fij*dy/rij; /* y-component of f_{ij} */

        fx[i] += dfx;
        fy[i] += dfy;
    }

    return;
}

void shpfor(){

    int i;

    double cx, lx, dx;
    double cy, ly, dy;
    double ll;

    double R1 = (LY-3.0*WS)/4.0;
    double R2 = (LY+WS)/4.0;

    if( R1 < 0.0 ){
        printf("R1 < 0!\n");
        exit(1);
    }

    if( R2 > 0.5*LX ){
        printf("R2 > 0.5*LX!\n");
        exit(1);
    }

    for( i = 0; i < N; i++ ){

        if( x[i] < 0.5*LX ){

            /* Area 1 */
            if( y[i] > LY-WS ){

                dx = 0.0;
                dy = y[i] - LY;
                wcawal(i,dx,dy);

                dx = 0.0;
                dy = y[i] - (LY-WS);
                wcawal(i,dx,dy);
            }

            /* Area 2 */
            if( y[i] < 2.0*R2 ){

                cx = 0.5*LX;
                cy = R2;

                lx = x[i] - cx;
                ly = y[i] - cy;
                ll = sqrt(lx*lx+ly*ly);

                dx = (ll-R1)*lx/ll;
                dy = (ll-R1)*ly/ll;
                wcawal(i,dx,dy);

                dx = (ll-R2)*lx/ll;
                dy = (ll-R2)*ly/ll;
                wcawal(i,dx,dy);
            }

        }else{

            /* Area 3 */
            if( y[i] < WS ){

                dx = 0.0;
                dy = y[i];
                wcawal(i,dx,dy);

                dx = 0.0;
                dy = y[i] - WS;
                wcawal(i,dx,dy);
            }

            /* Area 4 */
            if( y[i] > R1+R2 ){

                cx = 0.5*LX;
                cy = R1 + 2.0*R2;

                lx = x[i] - cx;
                ly = y[i] - cy;
                ll = sqrt(lx*lx+ly*ly);

                dx = (ll-R1)*lx/ll;
                dy = (ll-R1)*ly/ll;
                wcawal(i,dx,dy);

                dx = (ll-R2)*lx/ll;
                dy = (ll-R2)*ly/ll;
                wcawal(i,dx,dy);
            }
        }
    }

    return;
}