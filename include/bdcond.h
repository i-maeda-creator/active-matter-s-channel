void period(){

    int i;

    for( i = 0; i < N; i++ ){
        if( x[i] < 0.0 ) x[i] += LX;
        if( y[i] < 0.0 ) y[i] += LY;
        if( x[i] >= LX ) x[i] -= LX;
        if( y[i] >= LY ) y[i] -= LY;
    }

    return;
}

void pershp(){

    int i;

    for( i = 0; i < N; i++ ){
        if( x[i] < 0.0 ){ x[i] += LX; y[i] -= LY-WS;}
        if( x[i] >= LX ){ x[i] -= LX; y[i] += LY-WS;}
    }

    return;
}