#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 250
#define OSTEP 200
#define ISTEP 2000
#define RDV 6

double DT = 1.0e-3;
double R = 0.5;

double LX = 200.0;
double LY = 80.0;
/* The width of S-shape wall */
double WS = 3.0;
/* WCA potential for walls */
double WE = 1.0;

/* Mass, spring constant */
double MS = 1.0;
double KN = 100.0;
/* WCA potentail, active force, damping force */
double EP = 1.0;
double FA = 1.0;
double BE = 1.0;
/* chirality */
double OM = 1.0;
/* Kuramoto model*/
double KR = 0.0;
/* Diffusion coefficients */
double DD = 1.0e-2;
double DR = 1.0e-2;

/* variables */
int NP = N;

double ene, kin, pot, wpo;

double r[N], th[N], mt[N], xit[N];
double x[N], vx[N], fx[N], xix[N];
double y[N], vy[N], fy[N], xiy[N];
