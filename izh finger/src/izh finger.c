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

int
main(int argc, char *argv[])
{
  double *stateMatrix = (double *) calloc(500 * 67, sizeof(double));
  const int numMotorsTotal = 20;
  int bufferInd = 0;
  int bufferLength = 0;
  int numDataColumns = 67;
  double *motorVoltages = (double *) calloc(20, sizeof(double));
  double *param = calloc(2, sizeof(double));
  double *auxVar = calloc(3, sizeof(double));
  double samplFreq = 1000;
  double spkcnt = 0.0;
  int i;
  //double RATE_PPS = 200.0; // pulses per second
  const double F = 4; // in Hz, continuous freq
  const double T = 1.0 / samplFreq; // in seconds
  const double PERIODS = 4;
  const double BIAS = 100.0;
  const double AMP = 20;
  double pps[1000];
  int n;
  double max_n;
  double w;



  w = F * 2.0 * M_PI * T;
  max_n = 2.0 * M_PI * PERIODS / w;
  //printf("%.6f\t", max_n);
  for (n = 0; n < max_n; n++)
    {
      pps[n] = AMP * sin(w * n) + BIAS;
      //printf("%.6f\t", pps[n]);
    }

  //param[0] = 400.0;
  param[1] = 5.0;
  //srand48(time(NULL));

  //param[0] = 1.2435 * RATE_PPS - 19.014; // empirically measured, see sangerlab.wikidot.com/izhikevich_firing

  spkcnt = 0.0;

  auxVar[0] = -70.0; // v: membrane voltage
  auxVar[1] = -14.0; // b * v0 = 0.2 * (-70.0) = -14.0
  auxVar[2] = 0.0; // not spiking at the beginning

  for (i = 0; i < max_n; ++i)
    {
      //param [0] = 500.0 * drand48();
      param [0] = 500.0;//pps[i];
      stateMatrix[0] = (double) i / samplFreq;
      Doer(stateMatrix, bufferInd, bufferLength, numDataColumns, samplFreq,
          motorVoltages, param, auxVar);
      ////spkcnt += auxVar[2];
      //spkcnt = spkcnt + auxVar[2];
      printf("%.6lf\t", auxVar[2]);
      printf("%.6lf\n", pps[i]);

    }
  //printf("%.1lf\n", spkcnt);


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
  double *neuron_state = calloc(3, sizeof(double)); // State variables that keep updating by themselves
  double *neuron_input = calloc(3, sizeof(double)); // Sim parameters that should be pre-defined w/o updating

  neuron_input[0] = param[0]; // Syaptic input current
  neuron_input[1] = param[1]; // Output voltage to the motor when spikes
  neuron_input[2] = 1.0 / samplFreq; // dt


  memcpy(neuron_state, auxVar, 3 * sizeof(double)); // dump the previous states to neuron_state[]
  Izhikevich(neuron_state, neuron_input); // do one step of the integration
  memcpy(auxVar, neuron_state, 3 * sizeof(double)); // save the current states in auxVar

  motorVoltages[1] = neuron_state[2];

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
  //printf("%.3f\t\n", neuron_state[2]);

}

