/*
 ============================================================================
 Name        : fuego.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
/*simple spiking neuron*/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int
main(void)
{
  srand48(time(NULL));
  double a = 0.02;
  double b = 0.2;
  double c = -65 + 15 * pow(drand48(), 2);
  double d = 8 - 6 * pow(drand48(), 2);
  double S = 0.5 * drand48();
  double v = -65;
  double u = b * v;
  int t;
  double I;

  for (t = 0; t < 1000; t++)
    {
      I = 15 * drand48();
      c = -65.0 + 15.0 * pow(drand48(), 2);
      d = 8 - 6 * pow(drand48(), 2);

      if (v >= 30)
        {
          v = c;
          u = u + d;
          //printf("segundo v: %f\t%d\n",v,t);
          //printf("u: %f\n",u);
        }

        // printf("%f\t%d\n",v,t);
      v = v + 0.5 * (0.04 * pow(v, 2) + 5 * v + 140 - u + I);
      v = v + 0.5 * (0.04 * pow(v, 2) + 5 * v + 140 - u + I);
      u = u + a * (b * v - u);
      printf("%f\t%d\n", v, t);
    }
    //  printf("terc v: %f\n",v);
  //  printf("terc u: %f\n",u);
  return 0;
}
