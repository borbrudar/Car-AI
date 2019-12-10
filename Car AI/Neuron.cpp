#include "Neuron.h"

void Neuron::decide(std::vector<float> inputs, std::vector<float>& outputs)
{
	outputs = { 0,0,0,0};
	//calculate hidden layer
	for (int i = 0; i < weights1.size(); i++) {
		for (int j = 0; j < weights1[0].size(); j++) {
			hiddenLayer1[i] += weights1[i][j] * inputs[j];
		}
	}
	//biases for hidden layer
	for (int i = 0; i < hiddenLayer1.size(); i++) {
		hiddenLayer1[i] += bias[0];
	}
	//calculate second hidden layer
	for (int i = 0; i < weights2.size(); i++) {
		for (int j = 0; j < weights2[0].size(); j++) {
			hiddenLayer2[i] += weights2[i][j] * hiddenLayer1[j];
		}
	}
	//biases for hidden layer
	for (int i = 0; i < weights2.size(); i++) {
		hiddenLayer2[i] += bias[1];
	}
	//calculate final layer
	for (int i = 0; i < weights3.size(); i++) {
		for (int j = 0; j < weights3[0].size(); j++) {
			outputs[i] += hiddenLayer2[j] * weights3[i][j];
		}
	}
	//add bias and sigmoid function
	for (int i = 0; i < outputs.size(); i++) {
		outputs[i] += bias[2];
		outputs[i] = 1 / (1 + std::pow(2.71828,-outputs[i])); //better sigmoid function i guess:  1 / 1 + e^-x
		if (outputs[i] != outputs[i]) {
			int c = 0;
		}
	}	
}

