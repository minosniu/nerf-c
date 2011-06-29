/*
 ============================================================================
 Name        : izhpatt2.c
 Author      : Ivan
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
//pattern : tonic bursting

#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
  double a = 0.02;
  double b = 0.25;
  double c = -50.0;
  double d = 2.0;
  double v = -64.0;
  double u;
  double x = 5.0;
  double y = 140.0;
  double dt = 0.25;

  u = b * v;
  double t;
  double I;
  for (t = 0.0; t < 220.0 + dt; t = t + dt)
    {
      if (t > 22.0)
        I = 15;
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
