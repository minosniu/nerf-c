/*
 * prueba2.c
 *
 *  Created on: Jun 22, 2011
 *      Author: itrujillo
 */
/*simple spiking neuron*/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
const int Ne = 8;
const int Ni = 2;

int
InitArray(double **a, int x, int y, double (*f)(void))
{
  int i, j;
  for (i = 0; i < x; i++)
    {
      for (j = 0; j < y; j++)
        {
          a[i][j] = f();

        }

    }

  return 0;
}

int
main(int argc, char *argv[])
{

  // Allocating an array s[][] in a dynamic way.
  double **s = (double **) malloc((Ne + Ni) * sizeof(double));
  int i;
  for (i = 0; i < Ne + Ni; i++)
    {
      s[i] = (double*) malloc(sizeof(double));
    }

  int x;
  int y;

  //RANDOM MATRICES
  //matrix ri
  srand48(time(NULL));
  //function Init
  InitArray(s, Ne, Ni, &drand48);
  int w;
  int v;
  //double rs[Ne + Ni][Ni];
  FILE *out1;
  out1 = fopen("output.txt", "w");
  for (w = 0; w < Ne; w++)
    {
      for (v = 0; v < Ni; v++)
        {
          //printf("%.4f\t",rs[w][v]);
          if (out1 != NULL)
            fprintf(out1, "%.4f\t", s[w][v]);
        }
      //printf("\n");
      fprintf(out1, "\n");

    }
  //InitArray(arrayB, 100, 200, &AnotherFunc())
  return 0;
}

/*
 //        int z;
 //        double ri[Ni];
 //        for(z=0;z<Ni;z++)
 {
 ri[z]=drand48();
 //printf("%.4f\n",ri[z]);
 }

 //matrix re
 int y;
 double re[Ne];
 for(y=0;y<Ne;y++)
 {
 re[y]=drand48();
 //printf("%.4f\n",re[y]);
 }
 // rand matrices for S rs,rsi
 int w;
 int v;
 double rs[Ne+Ni][Ni];
 FILE *out1;
 out1 = fopen( "output.txt", "w" );
 for(w=0;w<Ne+Ni;w++)
 {
 for(v=0;v<Ne;v++)
 {
 rs[w][v]=drand48();
 //printf("%.4f\t",rs[w][v]);
 if( out1 != NULL )
 fprintf( out1, "%.4f\t", rs[w][v] );
 }
 printf("\n");
 fprintf( out1, "\n");

 }

 int t;
 int u;
 double rsi[Ne+Ni][Ni];
 FILE *out;
 out = fopen( "output2.txt", "w" );
 //out = fopen( "output2.txt", "w" );
 for(t=0;t<Ne+Ni;t++)
 {
 for(u=0;u<Ni;u++)
 {
 rsi[t][u]=drand48();
 //printf("%.4f\t",rsi[t][u]);
 if( out != NULL )
 fprintf( out, "%.4f\t", rs[t][u] );
 }
 printf("\n");
 fprintf( out, "\n");
 }


 //int aN;
 //printf("Input aN:");

 //PARAMETER MATRIXES a, b ,c ,d
 //matrix a
 int i;
 double a[Ne+Ni];
 for(i=0;i<Ne;i++)
 {
 a[i]=1*0.02;
 //printf("%.4f\n",a[i]);
 }
 for(i=Ne;i<Ne+Ni;i++)
 {
 a[i]=0.08*ri[i-Ne]+0.02;
 //printf("%.4f\n",a[i]);
 }

 //matrix b b=[0.2*ones(Ne,1);      0.25-0.05*ri]
 int j;
 double b[Ne+Ni];
 for(j=0;j<Ne;j++)
 {
 b[j]=1*0.2;
 //printf("%.4f\n",b[j]);
 }
 for(j=Ne;j<Ne+Ni;j++)
 {
 b[j]=0.25+(-0.05)*ri[j-Ne];
 //printf("%.4f\n",b[j]);
 }

 //matrix c c=[-65+15*re.^2;        -65*ones(Ni,1)];
 int k;
 double c[Ne+Ni];
 for(k=0;k<Ne;k++)
 {
 c[k]=(-65)+15*(pow(re[k],2));
 //printf("%.4f\n",c[k]);
 }
 for(k=Ne;k<Ne+Ni;k++)
 {
 c[k]=-65;
 //printf("%.4f\n",c[k]);
 }

 //matrix d d=[8-6*re.^2;           2*ones(Ni,1)];
 int l;
 double d[Ne+Ni];
 for(l=0;l<Ne;l++)
 {
 d[l]=8+(-6)*(pow(re[l],2));
 //printf("%.4f\n",d[l]);
 }
 for(l=Ne;l<Ne+Ni;l++)
 {
 d[l]=2;
 //printf("%.4f\n",d[l]);
 }

 // Matrix S:synaptic connection weights between the neurons
 // S=[0.5*rand(Ne+Ni,Ne),  -rand(Ne+Ni,Ni)]
 //matrix S
 int m;
 int n;
 double s[Ne+Ni][Ne+Ni];
 for(m=0;m<Ne+Ni;m++)
 {
 for(n=Ne;n<Ne+Ni;n++)
 {
 s[m][n]=0.5*rs[m][n];
 //printf("%.4f\n",s[m][n]);
 }
 }
 for(m=0;m<Ne+Ni;m++)
 {
 for(n=Ne;n<Ne+Ni;n++)
 {
 s[n]=-rsi[m][n]
 //printf("%.4f\n",s[m]);
 }



 }
 */

