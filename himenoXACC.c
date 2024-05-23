/********************************************************************

 This benchmark test program is measuring a cpu performance
 of floating point operation by a Poisson equation solver.

 If you have any question, please ask me via email.
 written by Ryutaro HIMENO, November 26, 2001.
 Version 3.0
 ----------------------------------------------
 Ryutaro Himeno, Dr. of Eng.
 Head of Computer Information Division,
 RIKEN (The Institute of Pysical and Chemical Research)
 Email : himeno@postman.riken.go.jp
 ---------------------------------------------------------------
 You can adjust the size of this benchmark code to fit your target
 computer. In that case, please chose following sets of
 (mimax,mjmax,mkmax):
 small : 33,33,65
 small : 65,65,129
 midium: 129,129,257
 large : 257,257,513
 ext.large: 513,513,1025
 This program is to measure a computer performance in MFLOPS
 by using a kernel which appears in a linear solver of pressure
 Poisson eq. which appears in an incompressible Navier-Stokes solver.
 A point-Jacobi method is employed in this solver as this method can 
 be easyly vectrized and be parallelized.
 ------------------
 Finite-difference method, curvilinear coodinate system
 Vectorizable and parallelizable on each grid point
 No. of grid points : imax x jmax x kmax including boundaries
 ------------------
 A,B,C:coefficient matrix, wrk1: source term of Poisson equation
 wrk2 : working area, OMEGA : relaxation parameter
 BND:control variable for boundaries and objects ( = 0 or 1)
 P: pressure
********************************************************************/

#include <stdio.h>
#include <sys/time.h>
#define LARGE
#include "parametr.h"
//#include <xmp.h>
//#include "/home/katrin/omni-compiler/libxmp/xmp.h"

double second();
float jacobi();
void initmt();
double fflop(int,int,int);
double mflops(int,double,double);

static float  p[MIMAX][MJMAX][MKMAX];
static float  a[4][MIMAX][MJMAX][MKMAX],
              b[3][MIMAX][MJMAX][MKMAX],
              c[3][MIMAX][MJMAX][MKMAX];
static float  bnd[MIMAX][MJMAX][MKMAX];
static float  wrk1[MIMAX][MJMAX][MKMAX],
              wrk2[MIMAX][MJMAX][MKMAX];

static int imax, jmax, kmax;
static float omega;


#pragma xmp template t[MIMAX][MJMAX][MKMAX]//шаблон для матрицы
#pragma xmp nodes n [1][1][1]// выделяем 1 узел  --p[0][0][0]
#pragma xmp distribute t[block][block][block] onto n// распределяем массив t между набором узлов n
#pragma xmp align p [i][j][k] with t[i][j][k]//выравниваем массив p по шаблону t
#pragma xmp align bnd [i][j][k] with t[i][j][k]
#pragma xmp align wrk1 [i][j][k] with t[i][j][k]
#pragma xmp align wrk2 [i][j][k] with t[i][j][k]
#pragma xmp align a[*] [i][j][k] with t[i][j][k]
#pragma xmp align b[*] [i][j][k] with t[i][j][k]
#pragma xmp align c[*] [i][j][k] with t[i][j][k]
#pragma xmp shadow p[1][1][1]// определяем теневые грани следующих массивов



int
main()
{
  int    i,j,k,nn;
  double  gosa;
  double cpu,cpu0,cpu1,flop,target;

  target= 60.0;
  omega= 0.8;
  imax = MIMAX-1;
  jmax = MJMAX-1;
  kmax = MKMAX-1;

  /*
   *    Initializing matrixes
   */
 
  initmt();// инициализация матрицы

/*
   *    Print matrixes
   */
  #pragma xmp task on t[0][0][0]//узел p[0][0][0] выполняет print и выводит указанный текст на экран
  {
   printf("PID %d", getpid());
  printf("mimax = %d mjmax = %d mkmax = %d\n",MIMAX, MJMAX, MKMAX);//размеры матрицы
  printf("imax = %d jmax = %d kmax =%d\n",imax,jmax,kmax);
  }
  

  nn= 10;//100 итерации алгоритма Якоби
  #pragma xmp task on t[0][0][0]
  {
  printf(" Start  measurement process.\n");
  printf(" Measure the performance in %d times.\n\n",nn);
  printf(" Wait for a while\n\n");
  }
  /*
   *    Start measuring
   */
  #pragma acc enter data copyin(p, bnd, wrk1, wrk2, a, b, c)//передает распределенные массивы из памяти хоста в память ускорителя
 {
  cpu0 = second();
  gosa = jacobi(nn);
  cpu1 = second();

  cpu= cpu1 - cpu0;
    #pragma xmp reduction(max:cpu)//вычисляет максимальное значение переменной cpu, хранящееся в памяти каждого ускорителя в каждом узле.
  
  flop= fflop(imax,jmax,kmax);//флопсы
  #pragma xmp task on t(0,0,0)
  {
  printf(" Loop executed for %d times\n",nn);//количество итераций
  printf(" Gosa : %e \n",gosa);//результат выполнения Якоби
  printf(" MFLOPS measured : %f\tcpu : %f\n",mflops(nn,cpu,flop),cpu);
  printf(" Score based on Pentium III 600MHz : %f%d\n", mflops(nn,cpu,flop)/82,84);// оценка производительности
  }
 }
  return (0);
}

void
initmt()
{
	int i,j,k;

   
#pragma xmp loop [i][j][k] on t[i][j][k] //параллельное выполнение оператора цикла
  for(i=0 ; i<MIMAX ; i++)
    for(j=0 ; j<MJMAX ; j++)
      for(k=0 ; k<MKMAX ; k++){
        a[0][i][j][k]=0.0;
        a[1][i][j][k]=0.0;
        a[2][i][j][k]=0.0;
        a[3][i][j][k]=0.0;
        b[0][i][j][k]=0.0;
        b[1][i][j][k]=0.0;
        b[2][i][j][k]=0.0;
        c[0][i][j][k]=0.0;
        c[1][i][j][k]=0.0;
        c[2][i][j][k]=0.0;
        p[i][j][k]=0.0;
        wrk1[i][j][k]=0.0;
        bnd[i][j][k]=0.0;
      }
     
#pragma xmp loop [i][j][k] on t[i][j][k]//параллельное выполнение оператора цикла
  for(i=0 ; i<imax ; i++)
    for(j=0 ; j<jmax ; j++)
      for(k=0 ; k<kmax ; k++){
        a[0][i][j][k]=1.0;
        a[1][i][j][k]=1.0;
        a[2][i][j][k]=1.0;
        a[3][i][j][k]=1.0/6.0;
        b[0][i][j][k]=0.0;
        b[1][i][j][k]=0.0;
        b[2][i][j][k]=0.0;
        c[0][i][j][k]=1.0;
        c[1][i][j][k]=1.0;
        c[2][i][j][k]=1.0;
        p[i][j][k]=(float)(i*i)/(float)((imax-1)*(imax-1));
        wrk1[i][j][k]=0.0;
        bnd[i][j][k]=1.0;
      }
  
}

float
jacobi(int nn)
{
  int i,j,k,n;
  float gosa, s0, ss;
  //#pragma acc create(gosa)
   #pragma acc data present(p, bnd, wrk1, wrk2, a, b, c) create(gosa)
 // #pragma acc data present(a, b, c,bnd, wrk1, wrk2,p) create(gosa)//передает распределенные массивы из памяти хоста в мапять ускорителя
 // create - выделяем память на графическом процессоре для новой переменной gosa
 {
  for(n=0 ; n<nn ; ++n){
    gosa = 0.0;
#pragma acc update device(gosa)// так как выше была выделена память на графическом процессоре для переменной gosa,то в данной строке мы обнавляем её значение(так как мы в цикле) в памяти графического процессора значением из ЦП
#pragma xmp reflect(p) acc
#pragma xmp loop [i][j][k] on t[i][j][k] //параллельное выполнение оператора цикла
#pragma acc parallel loop reduction(+:gosa) collapse(2)//выполняется редукция над переменной gosa и все вложенные циклы превращаются в один

    for(i=1 ; i<imax-1 ; i++){
      for(j=1 ; j<jmax-1 ; j++){
        #pragma acc loop reduction(+:gosa) vector// указывает, что итерации цикла исполняются в векторном режиме
        for(k=1 ; k<kmax-1 ; k++){
        
          s0 = a[0][i][j][k] * p[i+1][j  ][k  ]
             + a[1][i][j][k] * p[i  ][j+1][k  ]
             + a[2][i][j][k] * p[i  ][j  ][k+1]
             + b[0][i][j][k] * ( p[i+1][j+1][k  ] - p[i+1][j-1][k  ]
                              - p[i-1][j+1][k  ] + p[i-1][j-1][k  ] )
             + b[1][i][j][k] * ( p[i  ][j+1][k+1] - p[i  ][j-1][k+1]
                               - p[i  ][j+1][k-1] + p[i  ][j-1][k-1] )
             + b[2][i][j][k] * ( p[i+1][j  ][k+1] - p[i-1][j  ][k+1]
                               - p[i+1][j  ][k-1] + p[i-1][j  ][k-1] )
             + c[0][i][j][k] * p[i-1][j  ][k  ]
             + c[1][i][j][k] * p[i  ][j-1][k  ]
             + c[2][i][j][k] * p[i  ][j  ][k-1]
             + wrk1[i][j][k];

          ss = ( s0 * a[3][i][j][k] - p[i][j][k] ) * bnd[i][j][k];

          gosa+= ss*ss;
          /* gosa= (gosa > ss*ss) ? a : b; */

          wrk2[i][j][k] = p[i][j][k] + omega * ss;
        }
      }
    }
#pragma xmp loop [i][j][k] on t[i][j][k] //параллельное выполнение оператора цикла
   #pragma acc parallel loop collapse(2) //2 вложенных цикла будут выполняться какодин
   for(i=1 ; i<imax-1 ; ++i){
      for(j=1 ; j<jmax-1 ; ++j){
         #pragma acc loop vector// указывает, что итерации цикла исполняются в векторном режиме
           for(k=1 ; k<kmax-1 ; ++k)
              {p[i][j][k] = wrk2[i][j][k];}
      }
    } 
  
  /* end n loop */
  #pragma acc update host(gosa)//обновить значение в памяти ЦП из памяти графического
  
  #pragma xmp reduction(+:gosa)
  }
 
 }
  
  return(gosa);
}

double
fflop(int mx,int my, int mz)
{
  return((double)(mz-2)*(double)(my-2)*(double)(mx-2)*34.0);
}

double
mflops(int nn,double cpu,double flop)
{
  return(flop/cpu*1.e-6*(double)nn);
}

double
second()
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
}
