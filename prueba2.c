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
const Ne=800;
const Ni=200;
int main(void)
  {
        //RANDOM MATRICES
        //matrix ri
        srand48(time(NULL));
        int z;
        double ri[Ni];
        for(z=0;z<Ni;z++)
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
        double rs[Ne+Ni][Ne];
        for(w=0;w<Ne+Ni;w++)
          {
            for(v=0;v<Ne;v++)
              {
                rs[w][v]=drand48(); //% 1+0);
                printf("%.4f\n",rs[w][v]);
              }
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
		//printf("%g\n",a[i]);
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
	         //printf("%g\n",b[j]);
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
	          //printf("%g\n",c[k]);
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
	          //printf("%g\n",d[l]);
	    }
	  for(l=Ne;l<Ne+Ni;l++)
	    {
	          d[l]=2;
	          //printf("%.4f\n",d[l]);
	    }

	  // Matrix S:synaptic connection weights between the neurons
	  // S=[0.5*rand(Ne+Ni,Ne),  -rand(Ne+Ni,Ni)]



}
