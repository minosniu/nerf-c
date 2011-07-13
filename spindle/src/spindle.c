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
  const double F0ACT = 0.0289;
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
  const double LCEIN=1.0;
  const double L = 1.0;
  const double B0 = 0.0605;
  const double B1 = 0.2592;
  const double R = 0.46;
  const double freq = 60.0;
  double B;
  //double c = 1.0;
  double gd = 80.0;
  double fd;
  double x0 = 0.0;
  double x1 = (LPR0*KPR+LCEIN*KSR-LSR0*KSR-F0ACT)/(KSR+KPR);
  double x2 = 0.0;
  double dx0;
  double dx1;
  double dx2;
  double fib;
  double max_n;
  double w;
  int i;
  double BDAMP = 0.2356;

  //double bag1[5] = { gd, lce, dx0, dx1, dx2};
  //B = B0 + B1 * fd;
  //printf("%.6f\n", B);

  //dx0 = ((pow(gd, 2) / (pow(gd, 2) + pow(freq, 2))) - x0) / 0.149; // Tao: 0.149
  //dx1 = x2;
  // if C; if cl =1 , cs =0.42
  double CSS;
//  if (x2 > 0)
//    s = 1;
//  else
//    s = -1;
//  if (-1000.0*x2 > 100.0)
//      CSS = -1.0;
//      else if (-1000.0*x2 < -100.0)
//      CSS = 1.0;
//  else
//      CSS = (2.0 / (1.0 + exp(-1000.0*x2) ) ) - 1.0;
  //dx2 = (KSR * (L - x1 - LSR0) - (B * c * (x1 - R) * s * pow(fabs(x2), 2) - (KPR * (x1 - LPR0)) - FU1 * x0) / M);


  // Euler Method 0DE1

  //double n[1024];
  int n;
  double lce[1024];
  double mingd;
  const double SAMPLING_RATE = 1024.0;
  const double F = 4; // in Hz, continuous freq
  const double T = 1.0 / SAMPLING_RATE; // in seconds
  const double PERIODS = 4;
  const double BIAS = 1.0;
  const double AMP = 0.2;
  double xx0, xx1, xx2;
  int m;
  max_n = (2.0 * M_PI * PERIODS / w) - 1.0;
  for (n = 0; n < 1024; n++)
    {
      w = F * 2.0 * M_PI * T;
      lce[n] = AMP * sin(w * n) + BIAS;
      //printf("%.6f\n", lce[n]);
    }

  for (i = 0; i < 1024 ; i++ )
    {
      mingd = pow(gd, 2) / (pow(gd, 2) + pow(freq, 2));
      dx0 = (mingd - x0) / 0.149; // Tao: 0.149
      dx1 = x2;
      if (-1000.0*x2 > 100.0)
          CSS = -1.0;
        else if (-1000.0*x2 < -100.0)
            CSS = 1.0;
        else
          {
            CSS = (2.0 / (1.0 + exp(-1000.0*x2) ) ) - 1.0;
          }
      //printf("%.6f\n", lce[i]);
      //dx2 = (1/M) * (KSR*lce[i] - (KSR+KPR)*x1 - CSS*(BDAMP*x0)*(fabs(x2)) - 0.4);

      dx2 = (1/M) * (KSR*lce[i] - (KSR+KPR)*x1 - CSS*(BDAMP*x0)*(fabs(x2)) - 0.4);
      xx0 = x0 + dx0 * (1.0 / SAMPLING_RATE);
      xx1 = x1 + dx1 * (1.0 / SAMPLING_RATE);
      xx2 = x2 + dx2 * (1.0 / SAMPLING_RATE);
      //printf("%.6f\n", xx2);

      fib = fmax(GI * (lce[i]- xx1 - LSR0),0.0);//GI * (lce[i+1] - x1 - LSR0));

      x0 = xx0;
      x1 = xx1;
      x2 = xx2;
      printf("%.6f\n", fib);
      //printf("%.6f\n", lce[i]);


    }


  return 0;
}


