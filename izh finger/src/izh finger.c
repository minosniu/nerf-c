/*
 ============================================================================
 Name        : izh.c
 Author      : Ivan
 Version     :
 Copyright   : Your copyright notice
 Description : Izhikevich, spindle & exponential filter. (closed-loop)
 ============================================================================
 */
#include <ansi_c.h>
//#include <cvirte.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int
Doer(double *stateMatrix, int bufferInd, int bufferLength, int numDataColumns,
    double samplFreq, double *motorVoltages, double *param, double *auxVar);

void
Izhikevich(double *neuron_state, double *neuron_input);


int
main(int argc, char *argv[])
{
  double *stateMatrix = (double *) calloc(500 * 67, sizeof(double));
  const int numMotorsTotal = 20;

  int bufferInd = 0;
  int bufferLength = 0;
  int numDataColumns = 67;

  double *motorVoltages = (double *) calloc(20, sizeof(double));
  double *param = calloc(6, sizeof(double));
  double *auxVar = calloc(11, sizeof(double));
  double samplFreq = 1000.0;
  double spkcnt = 0.0;
  const double MAX_VOLTAGE = 5.0;
  const double KSR = 10.4649;
  const double KPR = 0.1127;
  const double LSR0 = 0.04;
  const double LPR0 = 0.76;
  const double F0ACT = 0.0289;
  const double LCEIN=1.0;
  const double F = 4; // in Hz, continuous freq
  const double T = 1.0 / samplFreq; // in seconds
  const double PERIODS = 4;
  const double BIAS = 1.0;
  const double AMP = 0.02;
  double max_n;
  double w;
  double pps[1000];
  double lce[1000];
  int n;
  int i;


  w = F * 2.0 * M_PI * T;
  max_n = 2.0 * M_PI * PERIODS / w;
 constant lce
  for (n = 0; n < (int) max_n / 2 + 1; n ++)
    {
      lce[n] = 1.0;
    }
  for (n = (int) max_n / 2 + 1; n < max_n; n++)
    {
      lce[n] = 1.0; //fmax(AMP * sin(w * n) + BIAS, 0.0);

    }

/*  // to have waveform
  for (n = 0; n < max_n; n++)
    {
      lce[n] = fmax(AMP * sin(w * n) + BIAS, 0.0);
    }
*/
  spkcnt = 0.0;

  auxVar[0] = 0.0; // spindle - xo = activation coefficient
  auxVar[1] = (LPR0*KPR+LCEIN*KSR-LSR0*KSR-F0ACT)/(KSR+KPR); //  spindle -  x1 = Lpr,spindle
  auxVar[2] = 0.0; //  spindle -  x2 = Lpr /dt
  auxVar[3] = 0.0; //  spindle - pps output from sipndle
  auxVar[4] = -70.0; // spindle neuron - v: membrane voltage
  auxVar[5] = -14.0; // spindle neuron - b * v0 = 0.2 * (-70.0) = -14.0
  auxVar[6] = 0.0; // spindle neuron - 0 for non spiking ; MAX_VOLTAGE for spiking
  auxVar[7] = -70.0; // motor neuron - v: membrane voltage
  auxVar[8] = -14.0; // motor neuron - b * v0 = 0.2 * (-70.0) = -14.0
  auxVar[9] = 0.0; // motor neuron - 0 for non spiking ; MAX_VOLTAGE for spiking
  auxVar[10] = 0.0; // muscle force



  for (i = 0; i < max_n; ++i)
    {

      param[0] = 80.0;
      param [1] = lce[i];
      pps[i] = auxVar[3] * 0.5;// spindle
      //param[2] = fmax(0.43 * pps[i]  - 5.7, 0.0);
      param[2] = 0.43 * pps[i]  - 5.7;  //  empirically measured, pps - I relation
      param[3] = MAX_VOLTAGE;
      //param[4] =  fmax(0.43 * pps[i]  - 5.7, 0.0);
      stateMatrix[0] = (double) i / samplFreq;
      param[4] = 0.43 * 50.0  - 5.7;
      param[5] = MAX_VOLTAGE;

      Doer(stateMatrix, bufferInd, bufferLength, numDataColumns, samplFreq,
          motorVoltages, param, auxVar);
      //spkcnt = spkcnt + auxVar[2];
      printf("%.6lf\t", lce[i]);
      printf("%.6lf\t",auxVar[2]);
      printf("%.6lf\t", auxVar[3]);
      printf(" %.6lf\t", auxVar[4]);
      printf(" %.6lf\t", auxVar[7]);
      printf(" %.6lf\t", auxVar[10]);
      printf(" %.6lf\n", fmin(auxVar[6] + auxVar[9], MAX_VOLTAGE));

    }
  //printf("Total spikes = %.1lf\n", spkcnt);


  free(stateMatrix);
  free(motorVoltages);
  free(param);
  free(auxVar);
  return 0;
}

int
Doer(double *stateMatrix, int bufferInd, int bufferLength, int numDataColumns,
    double samplFreq, double *motorVoltages, double *param, double *auxVar)
{

  motorVoltages[1] = 0.0;


  double *spindle_state = calloc(4, sizeof(double)); // State variables that keep updating by themselves
  double *spindle_param = calloc(3, sizeof(double)); // Sim parameters that should be pre-defined w/o updating
  double *izh_state = calloc(3, sizeof(double)); // State variables that keep updating by themselves
  double *izh_param = calloc(3, sizeof(double)); // Sim parameters that should be pre-defined w/o updating
  double *mn_state = calloc(3, sizeof(double)); // State variables that keep updating by themselves
  double *mn_param = calloc(3, sizeof(double)); // Sim parameters that should be pre-defined w/o updating


  memcpy(spindle_state, &auxVar[0], 4 * sizeof(double)); // dump the previous states to neuron_state[]
  spindle_param[0] = param[0]; // gamma dynamic for bag1
  spindle_param[1] = param[1]; // Lce the muscle stretch
  spindle_param[2] = 1.0 / samplFreq; // dt

  Spindle(spindle_state, spindle_param); // do one step of the integration
  memcpy(&auxVar[0], spindle_state, 4 * sizeof(double)); // dump the previous states to neuron_state[]


  memcpy(izh_state, &auxVar[0+4], 3 * sizeof(double)); // dump the previous states to neuron_state[]

  izh_param[0] = param[2]; // Synaptic input current
  izh_param[1] = param[3]; // Output voltage to the motor when spikes
  izh_param[2] = 1.0 / samplFreq; // dt
  Izhikevich(izh_state, izh_param); // do one step of the integration
  memcpy(&auxVar[0+4], izh_state, 3 * sizeof(double)); // save the current states in auxVar



  memcpy(mn_state, &auxVar[7], 3 * sizeof(double)); // dump the previous states to neuron_state[]

  mn_param[0] = param[4]; // Snyaptic input current
  mn_param[1] = param[5]; // Output voltage to the motor when spikes
  mn_param[2] = 1.0 / samplFreq; // dt
  Izhikevich(mn_state, mn_param); // do one step of the integration
  memcpy(&auxVar[7], mn_state, 3 * sizeof(double)); // save the current states in auxVar

  Muscle(auxVar[6] + auxVar[9], &auxVar[10]); // constant input + stretch


  motorVoltages[1] = auxVar[10];

  free(spindle_state);
  free(spindle_param);
  free(izh_state);
  free(izh_param);
  free(mn_state);
  free(mn_param);
  return 0;

}

void
Izhikevich(double *neuron_state, double *neuron_input)
{
  double DT = neuron_input[2] * 1000.0; // unit must be in milliseconds

  double const A = 0.02; // a: time scale of the recovery variable u
  double const B = 0.2; // b:sensitivity of the recovery variable u to the subthreshold fluctuations of v.
  double const C = -65.0; // c: reset value of v caused by fast high threshold (K+)
  double const D = 6.0; // d: reset of u caused by slow high threshold Na+ K+ conductances
  double const TH = 30.0; // voltage threshold
  double const X = 5.0; // x , constant in Izhikevich equation
  double const Y = 140.0; // y, constant in Izhikevich equation

  double v = neuron_state[0];
  double u = neuron_state[1];
  double vv, uu;
  double I = neuron_input[0];
  double vol_spike = neuron_input[1];

  vv = v + DT * (0.04 * pow(v, 2) + X * v + Y - u + I); // neuron[0] = v;
  uu = u + DT * A * (B * v - u); // neuron[1] = u; See Izhikevich model


  if (vv >= TH) // if spikes
    {
      neuron_state[0] = C;
      neuron_state[1] = uu + D;
      neuron_state[2] = vol_spike; // a flag in double that tells if the neuron is spiking

    }
  else
    {
      neuron_state[0] = vv;
      neuron_state[1] = uu;
      neuron_state[2] = 0.0;
    };
}


void
Spindle(double *neuron_state, double *neuron_input)
{

  // constants spindle model, see Mileusnic 2006
  const double KSR = 10.4649;
  const double KPR = 0.1127;
  const double BDAMP=0.2356;
  const double GI = 20000;
  const double LSR0 = 0.04;
  const double M = 0.0002;
  const double freq = 60.0;
  double gd = neuron_input[0];
  double lce = neuron_input[1];
  double DT = neuron_input[2];
  double dx0;
  double dx1;
  double dx2;
  double mingd;
  double CSS;
  double x0 = neuron_state[0];
  double x1 = neuron_state[1];
  double x2 = neuron_state[2];
  double fib = neuron_state[3];
  double xx0, xx1, xx2;

  mingd = pow(gd, 2) / (pow(gd, 2) + pow(freq, 2));
  dx0 = (mingd - x0) / 0.149; // Tao: 0.149
  dx1 = x2;
  if (-1000.0 * x2 > 100.0)
    CSS = -1.0;
  else if (-1000.0 * x2 < -100.0)
    CSS = 1.0;
  else
    {
      CSS = (2.0 / (1.0 + exp(-1000.0 * x2))) - 1.0;
    }

  dx2 = (1 / M) * (KSR * lce - (KSR + KPR) * x1 - CSS * (BDAMP * x0) * (fabs(x2)) - 0.4);
  xx0 = x0 + dx0 * DT;
  xx1 = x1 + dx1 * DT;
  xx2 = x2 + dx2 * DT;

  fib = fmax(GI * (lce - xx1 - LSR0), 0.0);

  neuron_state[0] = xx0;
  neuron_state[1] = xx1;
  neuron_state[2] = xx2;
  neuron_state[3] = fib;


}

void
Muscle(double x, double *y)
{
  double alfa = 0.988;
  double yy = 0.0;
  double y0 = *y;

  yy = alfa * y0 + (1 - alfa) * x;
  *y = yy;

}

