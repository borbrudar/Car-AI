#pragma once
#include <SFML/Graphics.hpp>
#include <random>

class Neuron {
public:
	Neuron() {
		weights1.resize(8); weights2.resize(8);
		weights3.resize(8); weights4.resize(8);
		std::random_device rd;
		std::default_random_engine engine(rd());
		std::uniform_real_distribution<float> dist(-1, 1);
		for (int i = 0; i < 8; i++) {
			weights1[i] = dist(engine);
			weights2[i] = dist(engine);
			weights3[i] = dist(engine);
			weights4[i] = dist(engine);
		}
	}
	void decide(std::vector<float> inputs, std::vector<float> &outputs);
	void copyNeuron(Neuron &target);
	std::vector<float> weights1, weights2, weights3, weights4;
	float bias[4] = { 1,1,1,1 };
};