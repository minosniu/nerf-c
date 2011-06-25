#include <ansi_c.h>
//#include <cvirte.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


// prototypes
int Doer (double *stateMatrix,int bufferInd, int bufferLength,int numDataColumns, double samplFreq, double *motorVoltages, double *param, double *auxVar);
void IntegrateAndFire(double *neuron, double currTime);

int main (int argc, char *argv[])
{
	double *stateMatrix = (double *) calloc(500*67,sizeof(double));
	const int numMotorsTotal = 20;
	int bufferInd = 0;
	int bufferLength = 0;
	int numDataColumns = 67;
	double *motorVoltages = (double *) calloc(20,sizeof(double));
	double *param = (double *) calloc(1,sizeof(double));
	double *auxVar = (double *) calloc(2,sizeof(double));
	double samplFreq = 100;

	param[0] = 2.0;

	int i;

	for(i=0;i<1000;++i)
	{
		stateMatrix[0] = (double)i/samplFreq;
		Doer (stateMatrix, bufferInd, bufferLength, numDataColumns, samplFreq, motorVoltages, param, auxVar);
		printf("%.6f\n",auxVar[0]);
	}

	free(stateMatrix);
	free(motorVoltages);
	free(param);
	free(auxVar);
	return 0;
}

int Doer (double *stateMatrix,int bufferInd, int bufferLength,int numDataColumns, double samplFreq, double *motorVoltages, double *param, double *auxVar)
{

	motorVoltages[1] = 0.0;

	double currTime = stateMatrix[bufferInd*numDataColumns];

	double neuron[7];
	neuron[2] = 1.0;		// resistance
	neuron[3] = 50;			// capacitance
	neuron[4] = 0.001;		// refractory period (s)
	neuron[6] = 1.0;		// voltage threshold


		neuron[1] = param[0];

		neuron[0] = auxVar[0];	// soma voltage
		neuron[5] = auxVar[1];  // time of last spike

		IntegrateAndFire(neuron, currTime);	    // do one step of the integration
		auxVar[0] = neuron[0];				// update aux variables so that they are
		auxVar[1] = neuron[5];				// available the next time around

	if (auxVar[0] > neuron[6])
		motorVoltages[1] = param[1];

	return 0;
}

void IntegrateAndFire(double *neuron, double currTime)
{
	// neuron just spiked, needs to stay refractory
	if (neuron[0] == 5.0) neuron[0] = 0.0;
	if (currTime < neuron[5] + neuron[4]) neuron[0] = 0.0;
	// neuron voltage okay to build up
	else neuron[0] = (neuron[1]-neuron[0]/neuron[2])/neuron[3] + neuron[0];

	// neuron reaches threshold
	if (neuron[0] > neuron[6])
	{
		neuron[0] = 5.0;
		neuron[5] = currTime;
	}
}
