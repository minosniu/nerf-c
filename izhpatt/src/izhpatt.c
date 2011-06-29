/*
 ============================================================================
 Name        : izhpatt.c
 Author      : Ivan
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
//pattern : tonic spiking

#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
  double a = 0.02;
  double b = 0.2;
  double c = -65.0;
  double d = 6.0;
  double v = -70.0;
  double u;
  double x = 5.0;
  double y = 140.0;
  double dt = 0.25;

  u = b * v;
  double t;
  double I;
  for (t = 0.0; t < 100.0 + dt; t = t + dt)
    {
      if (t > 10.0)
        I = 14.0;
      else
        I = 0.0;

      v += dt * (0.04 * pow(v, 2) + x * v + y - u + I);
      u += dt * a * (b * v - u);
      if (v >= 30.0)
        {
          v = c;
          u = u + d;
          printf("%.3f\t%.3f\n", 30.0, t);
        }
      else
        printf("%.3f\t%.3f\n", v, t);
    }
  return 0;
}

