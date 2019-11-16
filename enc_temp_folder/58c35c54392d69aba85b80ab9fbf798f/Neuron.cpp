#include "Neuron.h"

void Neuron::decide(std::vector<float> inputs, std::vector<float>& outputs)
{
	outputs = { 0,0,0,0};
	//calculate hidden layer
	for (int i = 0; i < weights[0].size(); i++) {
		for (int j = 0; j < 8; j++) {
			hiddenLayer[i] += weights[i][j] * inputs[i];
		}
	}
	//biases for hidden layer
	for (int i = 0; i < 8; i++) {
		hiddenLayer[i] += bias[i];
	}
	//calculate final layer
	for (int i = 8; i < 12; i++) {
		for (int j = 0; j < weights[0].size(); j++) {
			outputs[i - 8] += hiddenLayer[i - 8] * weights[i][j];
		}
	}
	//add bias and sigmoid function
	for (int i = 0; i < outputs.size(); i++) {
		outputs[i] += bias[i + 8];
		outputs[i] = 1 / (1 + std::pow(2.71828,-outputs[i])); //better sigmoid function i guess:  1 / 1 + e^-x
	}	
}

void Neuron::copyNeuron(Neuron & target)
{
	//copy weights
	for (int i = 0; i < weights.size(); i++) {
		for (int j = 0; j < weights[i].size(); j++) {
			target.weights[i][j] = weights[i][j];
		}
	}
	//copy bias
	for (int i = 0; i < 12; i++) target.bias[i] = bias[i];
}
