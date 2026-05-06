void wallfor(){

    int i;

    double dx;
    double dy,dfy;

    double sij,rij,sr6,fij;

    double ywl = 0.0;
    double ywu = LY;


    for(i =0; i< NP ; i++){  
            dx = 0.0;
            dy = y[i]-ywl ;  //yのwallのlow

           
            rij = sqrt(dx*dx + dy*dy); //distance
            sij = r[i] ;



            if(rij < pow(2.0,1.0/6.0)*sij ){

                sr6 = pow(sij/rij,6.0);
                fij = (24.0*EPS/rij)*sr6*(2.0*sr6-1.0);
                dfy = fij*dy/rij;
                
                fy[i] += dfy;

            }

            dx = 0.0;
            dy = y[i]-ywu ;  //yのwallのup

           
            rij = sqrt(dx*dx + dy*dy); //distance
            sij = r[i] ;



            if(rij < pow(2.0,1.0/6.0)*sij ){

                sr6 = pow(sij/rij,6.0);
                fij = (24.0*EPS/rij)*sr6*(2.0*sr6-1.0);
                dfy = fij*dy/rij;
                
                fy[i] += dfy;

            }
    }
    

    return;
}
