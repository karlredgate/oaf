
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

static int verbose = 1;
static int json = 0;
static char *command;

static char jsonOpt[] = "--json";

static void
init( int argc, char **argv ) {
    command = *argv++; argc--;
    while ( argc > 0 ) {
        do {
            if ( strncmp(*argv, jsonOpt, sizeof jsonOpt) == 0 ) {
                json = 1;
                break;
            }
            printf( "Unknown arg '%s'\n", *argv );
            exit( -1 );
        } while (0);
        argv++; argc--;
    }
}

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

/*
 * this reduces precision from inline a * a - since the inline
 * expressions are stored in 80bit registers
 */

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

void
print_json( struct moments *s ) {
    printf( "{\"n\":%lld,", s->n );
    printf( " \"µ\":%lf,", mean(s) );
    printf( " \"σ\":%lf,", stddev(s) );
    printf( " \"γ\":%lf,", skewness(s) );
    printf( " \"κ\":%lf}", kurtosis(s) );
    printf( "\n" );
}

int main( int argc, char **argv ) {
    init( argc, argv );

    struct moments s;
    clear( &s );

    int count = scanf( "%lld %lf %lf %lf %lf\n", &s.n, &s.m1, &s.m2, &s.m3, &s.m4 );

    if ( count != 5 ) {
        fprintf( stderr, "failed to scan value (%d)\n", count );
        exit( 1 );
    }

    if ( json ) {
        print_json( &s );
        exit( 0 );
    }
    if ( verbose ) {
        verbose_print( &s );
        exit( 0 );
    }
    simple_print( &s );
}

/* vim: set autoindent expandtab sw=4 : */
