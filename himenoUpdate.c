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
 [mimax][mjmax][mkmax]:
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
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

//#define MR(mt,n,r,c,d)  mt->m[(n) * mt->mrows * mt->mcols * mt->mdeps + (r) * mt->mcols* mt->mdeps + (c) * mt->mdeps + (d)]

// struct Mat {
//   float* m;
//   int mnums;
//   int mrows;
//   int mcols;
//   int mdeps;
// };

/* prototypes */
//typedef struct Mat Matrix;

void newMat(void);//инициализация матрицы
//void clearMat(Matrix* Mat);
//void set_param(int i[],char *size);
//void mat_set(Matrix* Mat,int l,float z);
//void mat_set_init(Matrix* Mat);

float jacobi(int n);// реализация алгоритма якоби

double fflop(int,int,int);//функции которые считают флопсы и время
double mflops(int,double,double);
double second();

static float  p[i1][j1][k1],
              a[4][i1][j1][k1],
              b[3][i1][j1][k1],
              c[3][i1][j1][k1],
              bnd[i1][j1][k1],
              wrk1[i1][j1][k1],
              wrk2[i1][j1][k1];
float   omega=0.8;

//Matrix  a,b,c,p1,bnd,wrk1,wrk2,p;





int
main(int argc, char *argv[])
{
  int    i,j,k,nn;
  int    imax,jmax,kmax,mimax,mjmax,mkmax,msize[3];
  float  gosa,target;
  double  cpu0,cpu1,cpu,flop;
  char   size[10];

  if(argc == 2){
    strcpy(size,argv[1]);
  } else {
    printf("For example: \n");
    printf(" Grid-size= XS (32x32x64)\n");
    printf("\t    S  (64x64x128)\n");
    printf("\t    M  (128x128x256)\n");
    printf("\t    L  (256x256x512)\n");
    printf("\t    XL (512x512x1024)\n\n");
    printf("Grid-size = ");
    scanf("%s",size);
    printf("\n");
  }

  set_param(msize,size);
  
  mimax= msize[0];
  mjmax= msize[1];
  mkmax= msize[2];
  imax= mimax-1;
  jmax= mjmax-1;
  kmax= mkmax-1;

  target = 60.0;

  newMat();//инициализация

  printf("mimax = %d mjmax = %d mkmax = %d\n",mimax,mjmax,mkmax);
  printf("imax = %d jmax = %d kmax =%d\n",imax,jmax,kmax);

   /*
   *    Start measuring
   */

  nn= 3; // 3 итерации алгоритма якоби
  printf(" Start rehearsal measurement process.\n");
  printf(" Measure the performance in %d times.\n\n",nn);

  cpu0= second();
  gosa= jacobi(nn);
  cpu1= second();
  cpu= cpu1 - cpu0; //время выполнения якоби
  flop= fflop(imax,jmax,kmax);

  printf(" MFLOPS: %f time(s): %f %e\n\n",
         mflops(nn,cpu,flop),cpu,gosa);

  nn= (int)(target/(cpu/3.0));//общее количество итераций которое можно выполнить за минуту

  printf(" Now, start the actual measurement process.\n");
  printf(" The loop will be excuted in %d times\n",nn);
  printf(" This will take about one minute.\n");
  printf(" Wait for a while\n\n");

  cpu0 = second();
  gosa = jacobi(nn);
  cpu1 = second();
  cpu = cpu1 - cpu0;

  printf(" Loop executed for %d times\n",nn);// количество итераций
  printf(" Gosa : %e \n",gosa);//результат выполнения якоби
  printf(" MFLOPS measured : %f\tcpu : %f\n",mflops(nn,cpu,flop),cpu);
  printf(" Score based on Pentium III 600MHz using Fortran 77: %f / %d\n", mflops(nn,cpu,flop)/82,84);//оценка производительности

 
  
  return (0);
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

void
set_param(int is[],char *size)
{
  if(!strcmp(size,"XS") || !strcmp(size,"xs")){
    is[0]= 32;
    is[1]= 32;
    is[2]= 64;
    return;
  }
  if(!strcmp(size,"S") || !strcmp(size,"s")){
    is[0]= 64;
    is[1]= 64;
    is[2]= 128;
    return;
  }
  if(!strcmp(size,"M") || !strcmp(size,"m")){
    is[0]= 128;
    is[1]= 128;
    is[2]= 256;
    return;
  }
  if(!strcmp(size,"L") || !strcmp(size,"l")){
    is[0]= 256;
    is[1]= 256;
    is[2]= 512;
    return;
  }
  if(!strcmp(size,"XL") || !strcmp(size,"xl")){
    is[0]= 512;
    is[1]= 512;
    is[2]= 1024;
    return;
  } else {
    printf("Invalid input character !!\n");
    exit(6);
  }
}

void
newMat()
{
  int i,j,k;

  for(i=0 ; i< i1; ++i)
    for(j=0 ; j< j1; ++j)
      for(k=0 ; k< k1; ++k)
      {
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
        wrk2[i][j][k]=0.0;
        bnd[i][j][k]=0.0;
      }


  for(i=0 ; i<imax ; ++i)
    for(j=0 ; j<jmax ; ++j)
      for(k=0 ; k<kmax ; ++k)
      {
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
	      p[i][j][k]=(float)((i)*(i))/(float)((imax-1)*(imax-1));
        wrk1[i][j][k]=0.0;
        wrk2[i][j][k]=0.0;
        bnd[i][j][k]=1.0;
      }
}




float
jacobi(int nn)
{
  int    i,j,k,n,imax,jmax,kmax;
  float  gosa,s0,ss;

  imax= p->mrows-1;
  jmax= p->mcols-1;
  kmax= p->mdeps-1;

 
  for(n=0 ; n<nn ; n++){
    gosa = 0.0;

    
    for(i=1 ; i<imax; i++)
      for(j=1 ; j<jmax ; j++)
        for(k=1 ; k<kmax ; k++){
           s0 = a[0][i][j][k] * p[i+1][j ][k  ]
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

          wrk2[i][j][k] = p[i][j][k] + omega * ss;
        }

  
    for(i=1 ; i<imax ; i++)
      for(j=1 ; j<jmax ; j++)
        for(k=1 ; k<kmax ; k++)
          p[i][j][k] = wrk2[i][j][k];
    
  } /* end n loop */

  return(gosa);
}




double
second()
{

  struct timeval tm;
  double t ;

  static int base_sec = 0,base_usec = 0;

  gettimeofday(&tm, NULL);
  
  if(base_sec == 0 && base_usec == 0)
    {
      base_sec = tm.tv_sec;
      base_usec = tm.tv_usec;
      t = 0.0;
  } else {
    t = (double) (tm.tv_sec-base_sec) + 
      ((double) (tm.tv_usec-base_usec))/1.0e6 ;
  }

  return t ;
}

