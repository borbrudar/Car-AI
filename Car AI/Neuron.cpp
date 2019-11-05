#include "Neuron.h"

void Neuron::decide(std::vector<float> inputs, std::vector<float>& outputs)
{
	outputs = { 0 };
	//multiply the inputs by the weights
	for (int i = 0; i < 8; i++) {
		outputs[0] = weights1[i] * inputs[i];
		outputs[1] = weights2[i] * inputs[i];
		outputs[2] = weights3[i] * inputs[i];
		outputs[3] = weights4[i] * inputs[i];
	}
	//add bias and sigmoid function
	for (int i = 0; i < 4; i++) {
		outputs[i] += bias[i];
		outputs[i] = outputs[i] / (1 + std::abs(outputs[i]));//better sigmoid function i guess:  1 / 1 + e^-x
	}
	
}

void Neuron::copyNeuron(Neuron & target)
{
	for (int i = 0; i < 8; i++) {
		target.weights1[i] = this->weights1[i];
		target.weights2[i] = this->weights2[i];
		target.weights3[i] = this->weights3[i];
		target.weights4[i] = this->weights4[i];
		if(i < 4) target.bias[i] = this->bias[i];
	}
}
