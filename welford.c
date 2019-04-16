
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int verbose = 0;
static int µ = 0;

void
report_verbose( ) {
}

int main( int argc, char **argv ) {
    int n = 0;
    double mu = 0;
    double sigma = 0;
    double x, previous;

    while ( feof(stdin) == 0 ) {
        int count = scanf( "%lf\n", &x );
        if ( count != 1 ) {
            fprintf( stderr, "failed to scan value\n" );
            exit( 1 );
        }
        // printf( "count %d  value %lf\n", count, x );
        previous = mu;
        mu += (x-mu) / ++n;
        // sigma += (x - mu) * (x - previous);
        double a = (x - previous);
        a *= a;
        a *= n - 1;
        a /= n;
        sigma += a;
    }
    double s = sqrt( sigma / (n-1) );

    printf( "%d µ=%lf σ=%lf\n", n, mu, s );
}

/* vim: set autoindent expandtab sw=4 : */
