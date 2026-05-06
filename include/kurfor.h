/* WCA & Kuramoto forces */
void kurfor(){

  double dx, dy, dfx, dfy, dmt;
  double sij, rij, sr6, fij;

  /* Calculate forces fx, fy ( i < j ) */
  pot = 0.0;

  for( int i = 0; i < NP-1; i++ ){
  for( int j = i+1; j < NP; j++ ){

    dx = x[i] - x[j];
    dy = y[i] - y[j];

    if( dx >  0.5*LX ){ dx -= LX; dy += LY-WS;}
    if( dx < -0.5*LX ){ dx += LX; dy -= LY-WS;}

    rij = sqrt(dx*dx+dy*dy);
    sij = r[i] + r[j];

    if( rij < pow(2.0,1.0/6.0)*sij ){

      sr6 = pow(sij/rij,6.0);
      fij = (24.0*EP/rij)*sr6*(2.0*sr6-1.0);

      dfx = fij*dx/rij; /* x-component of f_{ij} */
      dfy = fij*dy/rij; /* y-component of f_{ij} */
      dmt = KR*sin(th[j]-th[i]);

      fx[i] += dfx;
      fy[i] += dfy;
      mt[i] += dmt;

      fx[j] -= dfx;
      fy[j] -= dfy;
      mt[j] -= dmt;

      pot += 4.0*EP*sr6*(sr6-1.0) + EP;
    }
  } /* END j loop */
  } /* END i loop */

  return;
}
