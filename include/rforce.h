void rforce(){

    int i, k;

    double rdv[10];
    double MLG = 1.0e-1;
    double vmp = sqrt(2.0*DD);
    double omp = sqrt(2.0*DR);

    for( i = 0; i < NP; i++ ){

        for( k = 0; k < RDV; k++ ) rdv[k] = (double)rand()/(double)RAND_MAX;

        xix[i] = vmp*sqrt(-log(MLG+(1.0-MLG)*rdv[0]))*cos(2.0*M_PI*rdv[3]);
        xiy[i] = vmp*sqrt(-log(MLG+(1.0-MLG)*rdv[1]))*cos(2.0*M_PI*rdv[4]);
        xit[i] = omp*sqrt(-log(MLG+(1.0-MLG)*rdv[2]))*cos(2.0*M_PI*rdv[5]);
    }

    return;
}
