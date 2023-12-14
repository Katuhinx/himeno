#include <math.h>
#include <stdio.h>
#include "time.h"

#define Max (a, b) (( a ) > ( b ) ? ( a ) : ( b ) )
#define L 16000
#define M 6250
#define ITMAX 200

float MAXEPS = 0.5f;

#pragma dvm array distribute [ block ][ block ]
float A [ L ][ M ];
#pragma dvm array align ([ i ][ j ] with A [ i ][ j ])
float B [ L ][ M ];

int main ( int an , char ** as )
{
    double st , et ;
    int i , j , it ;
    float eps ;

    #pragma dvm region
    {

        #pragma dvm parallel ([ i ][ j ] on A [ i ][ j ])
        for ( i = 0; i < L ; i ++)
            for ( j = 0; j < M ; j ++)
            {
                A [i][j] = 0;
                if ( i == 0 || j == 0 || i == L - 1 || j == L - 1)
                    B [ i ][ j ] = 0;
                else
                    B [ i ][ j ] = 3 + i + j ;
            }
    }

    /* iteration loop */
    for ( it = 1; it <= ITMAX ; it ++)
    {
        eps = 0;
        #pragma dvm actual ( eps )
        #pragma dvm region
        {
            #pragma dvm parallel ([ i ][ j ] on A [ i ][ j ]) reduction ( max ( eps ) )
            for ( i = 1; i < L - 1; i ++)
                for ( j = 1; j < M - 1; j ++)
                {
                        float tmp = fabs ( B [ i ][ j ] - A [ i ][ j ]) ;
                        eps = (( tmp ) > ( eps ) ? ( tmp ) : ( eps ) );
                        A [ i ][ j ] = B [ i ][ j ];
                }
                
            #pragma dvm parallel ([ i ][ j ] on A [ i ][ j ]) shadow_renew ( A )
            for ( i = 1; i < L - 1; i ++)
                for ( j = 1; j < M - 1; j ++)
                    B [ i ][ j ] = ( A [i -1][ j ] + A [ i ][ j -1] + A [ i ][ j +1] + A [ i +1][ j ]) / 4.0f ;
        }

        #pragma dvm get_actual ( eps )
        printf ( " IT = %4i EPS = %14.7E \n " , it , eps ) ;
        if ( eps < MAXEPS )
        break ;
    }
    
    #pragma dvm get_actual (B)
    printf ("Size = %6d x %6d \n" , L, M ) ;
    printf ("Iterations = %12d\n" , ITMAX ) ;
    printf ("Operation type = floating point \n") ;

    return 0;
}