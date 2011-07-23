/*
 ============================================================================
 Name        : izh.c
 Author      : Ivan
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <ansi_c.h>
//#include <cvirte.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

// prototypes
int
Doer(double *stateMatrix, int bufferInd, int bufferLength, int numDataColumns,
    double samplFreq, double *motorVoltages, double *param, double *auxVar);

void
Izhikevich(double *neuron_state, double *neuron_input);
const int NUM_SPINDLE_PARAM = 2;
const int NUM_SPINDLE_STATE = 4;
const int NUM_IZH_PARAM = 2;
const int NUM_IZH_STATE = 3;

int
main(int argc, char *argv[])
{
  double *stateMatrix = (double *) calloc(500 * 67, sizeof(double));
  const int numMotorsTotal = 20;

  const int NUM_PARAM = NUM_SPINDLE_PARAM + NUM_IZH_PARAM;
  const int NUM_STATE = NUM_SPINDLE_STATE + NUM_IZH_STATE;
  int bufferInd = 0;
  int bufferLength = 0;
  int numDataColumns = 67;

  double *motorVoltages = (double *) calloc(20, sizeof(double));
  double *param = calloc(NUM_PARAM, sizeof(double));
  double *auxVar = calloc(NUM_STATE, sizeof(double));
  double samplFreq = 1000;
  double spkcnt = 0.0;
  const double MAX_VOLTAGE = 5.0;
  int i;
  const double KSR = 10.4649;
  const double KPR = 0.1127;
  const double LSR0 = 0.04;
  const double LPR0 = 0.76;
  const double F0ACT = 0.0289;
  const double LCEIN=1.0;
  //double RATE_PPS = 200.0; // pulses per second
  const double F = 4; // in Hz, continuous freq
  const double T = 1.0 / samplFreq; // in seconds
  const double PERIODS = 4;
  const double BIAS = 1.0;
  const double AMP = 0.2;
  double pps[1000];
  double lce[1000];
  int n;
  double max_n;
  double w;


  w = F * 2.0 * M_PI * T;
  max_n = 2.0 * M_PI * PERIODS / w;
  //printf("%.6f\t", max_n);
  for (n = 0; n < max_n; n++)
    {
      lce[n] = fmax(AMP * sin(w * n) + BIAS, 0.0);
      //printf("%.6f\t", pps[n]);
    }

  //param[0] = 400.0;
  //param[1] = 5.0;
   //param[0] = 1.2435 * RATE_PPS - 19.014; // empirically measured, see sangerlab.wikidot.com/izhikevich_firing

  spkcnt = 0.0;

  auxVar[0] = 0.0; // xo = firing rate, auxvar[0] in sipndle
  auxVar[1] = (LPR0*KPR+LCEIN*KSR-LSR0*KSR-F0ACT)/(KSR+KPR); // x1 = Lpr,spindle
  auxVar[2] = 0.0; // x2 = Lpr /dt, auxvar[2] in sipndle
  auxVar[3] = 0.0; //pps output from sipndle
  auxVar[4] = -70.0; // v: membrane voltage
  auxVar[5] = -14.0; // b * v0 = 0.2 * (-70.0) = -14.0
  auxVar[6] = 0.0; // 0 for non spiking ; MAX_VOLTAGE for spiking



  for (i = 0; i < max_n; ++i)
    {
      // param[0] and param [1] are for spindle
      param[0] = 80.0; //param[0] in spindle
      param [1] = lce[i];  //param[1] in spindle

      pps[i] = auxVar[3] * 0.1; // mult by 0.1 becasue spndle model is NOT completed
      // param[2] and param[3] are for izhikevich
      param[2] = 100.0 * pps[i] - 56.2;
      param[3] = MAX_VOLTAGE;
      stateMatrix[0] = (double) i / samplFreq;
      Doer(stateMatrix, bufferInd, bufferLength, numDataColumns, samplFreq,
          motorVoltages, param, auxVar);
      ////spkcnt += auxVar[2];
      //pkcnt = spkcnt + auxVar[2];
      printf("%.6lf\t", auxVar[NUM_SPINDLE_STATE - 1]);
      printf("%.6lf\n", auxVar[NUM_STATE - 1]);

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

  //double neuron_state[3], neuron_input[3];
  double *spindle_state = calloc(NUM_SPINDLE_STATE, sizeof(double)); // State variables that keep updating by themselves
  double *spindle_param = calloc(NUM_SPINDLE_PARAM, sizeof(double)); // Sim parameters that should be pre-defined w/o updating
  double *izh_state = calloc(NUM_IZH_STATE, sizeof(double)); // State variables that keep updating by themselves
  double *izh_param = calloc(NUM_IZH_PARAM, sizeof(double)); // Sim parameters that should be pre-defined w/o updating


  memcpy(spindle_state, &auxVar[0], NUM_SPINDLE_STATE * sizeof(double)); // dump the previous states to neuron_state[]
  spindle_param[0] = param[0]; // Syaptic input current
  spindle_param[1] = param[1]; // Output voltage to the motor when spikes
  spindle_param[2] = 1.0 / samplFreq; // dt

  Spindle(spindle_state, spindle_param); // do one step of the integration
  memcpy(&auxVar[0], spindle_state, NUM_SPINDLE_STATE * sizeof(double)); // dump the previous states to neuron_state[]


  memcpy(izh_state, &auxVar[0+NUM_SPINDLE_STATE], NUM_IZH_STATE * sizeof(double)); // dump the previous states to neuron_state[]

  izh_param[0] = param[2]; // gamma dynamic for bag1, neuron_input[0] in spindle
  izh_param[1] = param[3]; // Lce the muscle stretch, neuron_input[1] in spindle
  izh_param[2] = 1.0 / samplFreq; // dt, , neuron_input[2] in spindle, so it stays the same

  Izhikevich(izh_state, izh_param); // do one step of the integration
  memcpy(&auxVar[0+NUM_SPINDLE_STATE], izh_state, NUM_IZH_STATE * sizeof(double)); // save the current states in auxVar

  motorVoltages[1] = izh_state[NUM_IZH_STATE - 1];
 //motorVoltages[1] = neuron_state[3]; in spindle, gotta check it!

  return 0;

}

void
Izhikevich(double *neuron_state, double *neuron_input)
{
  double const A = 0.02; // a: time scale of the recovery variable u
  double const B = 0.2; // b:sensitivity of the recovery variable u to the subthreshold fluctuations of v.
  double const C = -65.0; // c: reset value of v caused by fast high threshold (K+)
  double const D = 6.0; // d: reset of u caused by slow high threshold Na+ K+ conductances
  double const TH = 30.0; // voltage threshold
  double const X = 5.0; // x
  double const Y = 140.0; // y

  double v = neuron_state[0];
  double u = neuron_state[1];
  double vv, uu;
  double I = neuron_input[0];
  double vol_spike = neuron_input[1];
  double DT = neuron_input[2];

  vv = v + DT * (0.04 * pow(v, 2) + X * v + Y - u + I); // neuron[0] = v;
  uu = u + DT * A * (B * v - u); // neuron[1] = u; See iZhikevich model


  if (vv >= TH) // if spikes
    {
      neuron_state[0] = C;
      neuron_state[1] = uu + D;
      neuron_state[2] = vol_spike; // a flag in double that tells if the neuron is spiking
      //printf("%.3f\t\n", neuron_state[2]);

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
  //double fib;

  double mingd;
  double CSS;

  double x0 = neuron_state[0];
  double x1 = neuron_state[1];

  double x2 = neuron_state[2];
  double fib = neuron_state[3];

  double xx0, xx1, xx2;



//  for (i = 0; i < 1024 ; i++ )
  //  {
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
  //printf("%.6f\n", lce);
  dx2 = (1 / M) * (KSR * lce - (KSR + KPR) * x1 - CSS * (BDAMP * x0) * (fabs(x2)) - 0.4);
  xx0 = x0 + dx0 * DT;
  xx1 = x1 + dx1 * DT;
  xx2 = x2 + dx2 * DT;

  fib = fmax(GI * (lce - xx1 - LSR0), 0.0);

  neuron_state[0] = xx0;
  neuron_state[1] = xx1;
  neuron_state[2] = xx2;
  neuron_state[3] = fib;
  //printf("%.6f\n", xx0);
  //printf("%.6f\n", xx2);

}
