/*
 ============================================================================
 Name        : spindle.c
 Author      : Ivan
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int
main()
{
  const double KSR = 10.4649;
  const double KPR = 0.1127;
  //const double BDAMP_PASSIVE=0.0605;
  //const double BDAMP=0.2356;
  //const double F0ACT=0;
  const double FU1 = 0.0289;
  //const double XII=1;
  //const double LPRN=1;
  const double GI = 20000;
  //const double GII=20000;
  //const double ANONLINEAR=0.25;
  //const double RLDFV=0.46;
  const double LSR0 = 0.04;
  const double LPR0 = 0.76;
  //const double L2ND=1;
  //const double TAO=0.192;
  const double M = 0.0002;
  //const double FSAT=1;
  //const double LCEIN=1.0;
  const double L = 1.0;
  const double B0 = 0.0605;
  const double B1 = 0.2592;
  const double R = 0.46;
  const double freq = 60.0;
  double B;
  double c = 1.0;
  double gd = 80.0;
  double fd;
  double x0;
  double x1;
  double x2;
  double dx0;
  double dx1;
  double dx2;
  double sampr = 1024.0;
  double fib;
  double mn;
  double w;
  int i;
  //double bag1[5] = { gd, lce, dx0, dx1, dx2};

  B = B0 + B1 * fd;
  dx0 = ((pow(gd, 2) / (pow(gd, 2) + pow(freq, 2))) - x0) / 0.149; // Tao: 0.149
  dx1 = x2;
  // if C; if cl =1 , cs =0.42
  double s;
  if (x2 > 0)
    s = 1;
  else
    s = -1;

  // if sign, s
  dx2 = (KSR * (L - x1 - LSR0) - (B * c * (x1 - R) * s * pow(fabs(x2), 2)
      - (KPR * (x1 - LPR0)) - FU1 * x0) / M);

  // Euler Method 0DE1
  w = 4.0 * 2.0 * M_PI * (1.0 / 1024.0);
  mn = (2.0 * M_PI * 4.0 / w) - 1.0;
  //printf("%.6f\n", mn);
  //printf("%.6f\n", w);
  for (i = 0.0; i < mn + 1.0 ; i++ )
    {
      double n[i];
      n[i] = i + 1.00098;
     // printf("%.6f\n", n[i]);
      double lce[i];
      lce[i] = 0.2 * sin(w * n[i]) + 1.0;
 //     printf("%.6f\n", lce[i]);

      x0 = x0 + dx0 * (1.0 / sampr);
      x1 = x1 + dx1 * (1.0 / sampr);
      x2 = x2 + dx2 * (1.0 / sampr);

      fib = fmax(GI * (lce[i] - x1 - LSR0), 0);//GI * (lce[i+1] - x1 - LSR0));
      printf("%.6f\n", fib);
      //printf("%.6f\n", lce[i]);

    }

  return 0;
}


