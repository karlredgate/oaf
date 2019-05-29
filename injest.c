
/*
 * read values from stdin, one value per line, update
 * the moments as they are read.
 * The moments are printed at the end of stdin.
 *
 * The output of this can be merged with another set
 * of moments using "merge" and translated to the
 * mean/stddev/skew/kurtosis using "derived".
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct moments {
    double m1;
    double m2;
    double m3;
    double m4;
    long long n;
};

void
clear( struct moments *this ) {
    this->m1 = this->m2 = this->m3 = this->m4 = 0.0;
    this->n = 0;
}

void
push( struct moments *this, double x ) {
    double delta = 0;
    double delta_n = 0;
    double delta_n2 = 0;
    double term1 = 0;
    long long n1 = this->n++;
    long long n = this->n;

    delta = x - this->m1;
    delta_n = delta / this->n;
    delta_n2 = delta_n * delta_n;
    term1 = delta * delta_n * n1;
    this->m1 += delta_n;
    this->m4 += term1 * delta_n2 * (n*n - 3*n + 3) + 6 * delta_n2 * this->m2 - 4 * delta_n * this->m3;
    this->m3 += term1 * delta_n * (n - 2) - 3 * delta_n * this->m2;
    this->m2 +=  term1;
}

int main( int argc, char **argv ) {
    struct moments s;
    clear( &s );

    double x;

    while ( feof(stdin) == 0 ) {
        int count = scanf( "%lf\n", &x );
        if ( count != 1 ) {
            fprintf( stderr, "failed to scan value\n" );
            exit( 1 );
        }
        push( &s, x );
    }

    printf( "%lld %lf %lf %lf %lf\n", s.n, s.m1, s.m2, s.m3, s.m4 );
}

/* vim: set autoindent expandtab sw=4 : */
