#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
#include <math.h>

class Neuron {
public:
	Neuron() {
		for (int i = 0; i < weights1.size(); i++) {
			weights1[i].resize(15); weights2[i].resize(22);
		}
		for (int i = 0; i < weights3.size(); i++) weights3[i].resize(22);
		hiddenLayer1.resize(22);
		hiddenLayer2.resize(22);
		for (int i = 0; i < hiddenLayer1.size(); i++) {
			hiddenLayer1[i] = 0; hiddenLayer2[i] = 0;
		}
		std::random_device rd;
		std::default_random_engine engine(rd());
		std::uniform_real_distribution<float> dist(-1, 1);
		for (int i = 0; i < weights1.size(); i++) {
			for (int j = 0; j < weights1[i].size(); j++) {
				weights1[i][j] = dist(engine);
			}
		}
		for (int i = 0; i < weights2.size(); i++) {
			for (int j = 0; j < weights2[i].size(); j++) {
				weights2[i][j] = dist(engine);
			}
		}
		for (int i = 0; i < weights3.size(); i++) {
			for (int j = 0; j < weights3[i].size(); j++) {
				weights3[i][j] = dist(engine);
			}
		}
	}
	void decide(std::vector<float> inputs, std::vector<float> &outputs);
	std::vector<std::vector<float>> weights1 { 22 };
	std::vector<std::vector<float>> weights2 { 22 };
	std::vector<std::vector<float>> weights3 { 4 };
	std::vector<float> hiddenLayer1;
	std::vector<float> hiddenLayer2;
	std::vector<float> bias{ 1,1,1 };
};