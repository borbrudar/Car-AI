#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
#include <math.h>

class Neuron {
public:
	Neuron() {
		for (int i = 0; i < weights.size(); i++) weights[i].resize(8);
		std::random_device rd;
		std::default_random_engine engine(rd());
		std::uniform_real_distribution<float> dist(-10, 10);
		for (int i = 0; i < weights.size(); i++) {
			for (int j = 0; j < weights[i].size(); j++) {
				weights[i][j] = dist(engine);
			}
		}
	}
	void decide(std::vector<float> inputs, std::vector<float> &outputs);
	void copyNeuron(Neuron &target);
	std::vector<std::vector<float>> weights { 12 };
	std::vector<float> hiddenLayer{ 0,0,0,0,0,0,0,0 };
	std::vector<float> bias{ 1,1,1,1,1,1,1,1,1,1,1,1 };
};