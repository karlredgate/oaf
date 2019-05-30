
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

void
copy( struct moments *dst, struct moments *src ) {
    dst->n = src->n;
    dst->m1 = src->m1;
    dst->m2 = src->m2;
    dst->m3 = src->m3;
    dst->m4 = src->m4;
}

double
square( double a ) {
    return a * a;
}

/*
 * this reduces precision from inline a * a - since the inline
 * expressions are stored in 80bit registers
 */
double
cube( double a ) {
    return a * a * a;
}

void
merge( struct moments *c, struct moments *a, struct moments *b ) {
    long long n_prod = a->n * b->n;
    c->n = a->n + b->n;

    long long n² = square( c->n );

    double delta = b->m1 - a->m1;
    double delta2 = delta * delta;
    double delta3 = delta * delta2;
    double delta4 = delta2 * delta2;

    c->m1 = ( (a->n * a->m1) + (b->n * b->m1) ) / c->n;

    c->m2 = a->m2 + b->m2;
    c->m2 += delta2 * a->n * b->n / c->n;

    c->m3  = a->m3 + b->m3;
    c->m3 += delta3 * n_prod * (a->n - b->n) / n²;
    c->m3 += 3.0 * delta * (a->n * b->m2 - b->n * a->m2) / c->n;

    c->m4  = a->m4 + b->m4;
    c->m4 += delta4 * n_prod * ( square(a->n) - n_prod + square(b->n) ) / cube(c->n);
    c->m4 += 6.0 * delta2 * ( square(a->n) * b->m2 + square(b->n) * a->m2 ) / n²;
    c->m4 += 4.0 * delta  * (a->n * b->m3 - b->n * a->m3) / c->n;
}

void
print_json( struct moments *s ) {
    printf( "{\"n\":%lld,", s->n );
    printf( " \"M1\":%lf,", s->m1 );
    printf( " \"M2\":%lf,", s->m2 );
    printf( " \"M3\":%lf,", s->m3 );
    printf( " \"M4\":%lf}", s->m4 );
}

int main( int argc, char **argv ) {
    init( argc, argv );

    struct moments s;
    struct moments t;
    struct moments u;
    clear( &s );

    while ( feof(stdin) == 0 ) {
        int count = scanf( "%lld %lf %lf %lf %lf\n", &t.n, &t.m1, &t.m2, &t.m3, &t.m4 );

        if ( count != 5 ) {
            fprintf( stderr, "failed to scan value\n" );
            exit( 1 );
        }
        copy( &u, &s );
        merge( &s, &t, &u );
    }

    if ( json ) {
        print_json( &s );
        exit( 0 );
    }
    printf( "%lld %lf %lf %lf %lf\n", s.n, s.m1, s.m2, s.m3, s.m4 );
}

/* vim: set autoindent expandtab sw=4 : */
