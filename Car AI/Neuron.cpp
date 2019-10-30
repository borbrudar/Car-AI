#include "Neuron.h"

void Neuron::decide(std::vector<float> inputs, std::vector<int>& outputs)
{
	float o1 = 0, o2 = 0;
	//multiply the inputs by the weights
	for (int i = 0; i < 8; i++) {
		o1 += weights1[i] * inputs[i];
		o2 += weights2[i] * inputs[i];
	}
	//activation "function"
	if (o1 > 1) o1 = 1;// 1-move 	
	if (o1 < -1) o1 = -1; //move back
	else o1 = 0; //0 - dont move

	if (o2 > 1) o2 = 1; //1 -right
	else if (o2 < -1) o2 = -1; //-1 - left
	else o2 = 0; //0 - dont turn

	outputs[0] = o1;
	outputs[1] = o2;
}
