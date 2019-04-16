
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int verbose = 1;

struct moments {
    double m1;
    double m2;
    double m3;
    double m4;
    long long n;
};

static double µ = 0;
static double σ = 0;
static double γ = 0;
static double κ = 0;

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

double
mean( struct moments *this ) {
    return this->m1;
}

double
skewness( struct moments *this ) {
    return sqrt( (double)this->n ) * this->m3 / pow( this->m2, 1.5 );
}

double
kurtosis( struct moments *this ) {
    double n = (double)this->n;
    return ( n * this->m4) / (this->m2 * this->m2) - 3.0;
}

double
variance( struct moments *this ) {
    return this->m2 / (this->n - 1.0);
}

double
stddev( struct moments *this ) {
    return sqrt( variance(this) );
}

void
simple_print( struct moments *s ) {
    printf( "%lf %lf %lf %lf %lf\n", mean(s), stddev(s), skewness(s), kurtosis(s), variance(s) );
}

void
verbose_print( struct moments *s ) {
    printf( "%lld µ=%lf σ=%lf γ=%lf κ=%lf %lf\n", s->n, mean(s), stddev(s), skewness(s), kurtosis(s), variance(s) );
}

int main( int argc, char **argv ) {
    struct moments s;
    clear( &s );

    double x, previous;

    while ( feof(stdin) == 0 ) {
        int count = scanf( "%lf\n", &x );
        if ( count != 1 ) {
            fprintf( stderr, "failed to scan value\n" );
            exit( 1 );
        }
        push( &s, x );
    }

    if ( verbose ) {
        verbose_print( &s );
    } else {
        simple_print( &s );
    }
}

/* vim: set autoindent expandtab sw=4 : */
