#include <SFML/Graphics.hpp>
#include <math.h>
#include "Car.h"
#include "Track.h"
#include "Neuron.h"
#include "Reward.h"
using namespace sf;

//function prototypes
void rotateCorner(float cx, float cy, float theta, Vector2f &c1);
float weightG1(float input, float bias, float weight);
float weightG2(float input, float bias, float weight);
float biasG1  (float input, float bias, float weight);
float biasG2  (float input, float bias, float weight);
float sigmoid(float x);

//struct for experience replay
struct expRep {
expRep(std::vector<float> state, std::vector<float> newState, std::vector<float> actions, float reward, float maxQ ) :
		state(state), newState(newState), actions(actions), reward(reward), maxQ(maxQ) {};
	std::vector<float> state;
	std::vector<float> newState;
	std::vector<float> actions;
	float reward, maxQ;
};



int main() {
	RenderWindow window;
	window.create(VideoMode(800, 600), "Car AI");

	Event e;
	
	//variables setup
	std::vector<expRep> xp;
	Neuron network, target_network;
	Car c;
	Track t;
	Reward r;
	bool right = 0, left = 0, up = 0, down = 0;
	int iteration = 0, replay = -1, descent = 0;
	float epsilon = 1, discount = 0.9f, loss = 0, lr = 0.1f;
	int cumulativeReward = 0;
	bool randomAction = false;

	//for epsilon greedy strategy
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> dist(0, 1);
 
	//initialize action vectors
	std::vector<float> actions{ 0,0,0,0 };
	std::vector<float> maxActions{ 0,0,0,0 };
	std::vector<float> randAct{ 0,0,0,0 };

	//game loop
	while (window.isOpen()) {
		std::uniform_int_distribution<int> randomXP(0, xp.size());
		//update iteration count
		if (iteration == 20) {
			iteration = 0;
			network.copyNeuron(target_network);
			//remove xp replay
			if (xp.size() > 5000) {
				for (int i = 0; i < 5; i++) xp.erase(xp.begin() + i);
			}
		}
		//Check for events
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::S) t.drawLines = !t.drawLines;
		}
		//updating the car n stuff
		Vector2f corner[4];
		{
			if (Keyboard::isKeyPressed(Keyboard::Left))  left = 1; else left = 0;
			if (Keyboard::isKeyPressed(Keyboard::Right)) right = 1; else right = 0;
			if (Keyboard::isKeyPressed(Keyboard::Up))    up = 1; else up = 0;
			if (Keyboard::isKeyPressed(Keyboard::Down))  down = 1; else down = 0;

			//TODO move to a separate car.h function
			float cx = c.shape.getPosition().x, cy = c.shape.getPosition().y, theta = c.shape.getRotation() * (3.14159f / 180.f);
			corner[0] = Vector2f(cx - c.shape.getSize().x / 2, cy - c.shape.getSize().y / 2);
			corner[1] = Vector2f(cx + c.shape.getSize().x / 2, cy - c.shape.getSize().y / 2);
			corner[2] = Vector2f(cx + c.shape.getSize().x / 2, cy + c.shape.getSize().y / 2);
			corner[3] = Vector2f(cx - c.shape.getSize().x / 2, cy + c.shape.getSize().y / 2);

			bool didCollide = false;
			for (int i = 0; i < 4; i++) rotateCorner(cx, cy, theta, corner[i]);
			if (t.isColliding(corner[0], corner[1])) didCollide = true;
			if (t.isColliding(corner[1], corner[2])) didCollide = true;
			if (t.isColliding(corner[2], corner[3])) didCollide = true;
			if (t.isColliding(corner[3], corner[0])) didCollide = true;
			if (didCollide) {
				c.pos = c.startPos; c.angle = 0.f;
			}
		}//DONT TOUCH
		//----------------------------------------------------
		//Implement the Q-learning alg

		//store for xp replay (for now just for gradient descent)
		std::vector<float> state = c.distances;
		int reward = -1;

		for (int i = 0; i < c.lines.size(); i++) c.distances[i] = t.lineIntersection(c.lines[i][0].position, c.lines[i][1].position, i);

		//choose action
		if (epsilon > dist(engine)) {
			if (replay == 60 || replay == -1) {
				replay = 0;
				//choose random actions
				std::uniform_int_distribution<int> ra(0, 1);
				actions[0] = ra(engine);
				actions[1] = ra(engine);
				actions[2] = ra(engine);
				actions[3] = ra(engine);
				randAct = actions;

				//variables update
				if (epsilon > 0.2f) epsilon -= 0.001f;
				std::cout << "epsilon: " << epsilon << std::endl;
			}
			else {
				replay++;
				actions = randAct;
			}
		}//Chosen action
		else {
			//choose an action (for gradient descent n shit)
			network.decide(c.distances, actions);
			target_network.decide(c.distances, maxActions);
		}

		if (actions[0] >= 0.90f) left = true;
		if (actions[1] >= 0.90f) right = true;
		if (actions[2] >= 0.90f) up = true;
		if (actions[3] >= 0.90f) down = true;

		c.update(up, down, left, right);

		
		//reward test and line intersection info
		for (int i = 0; i < c.lines.size(); i++) {
			c.distances[i] = t.lineIntersection(c.lines[i][0].position, c.lines[i][1].position, i);
			if (t.isColliding(corner[0], corner[1])) {r.canInteract = 0; reward = -100;} else reward = -1;
			if (t.isColliding(corner[0], corner[3])) {r.canInteract = 0; reward = -100;} else reward = -1;
			if (t.isColliding(corner[1], corner[2])) {r.canInteract = 0; reward = -100;} else reward = -1;
			if (t.isColliding(corner[2], corner[3])) {r.canInteract = 0; reward = -100;} else reward = -1;
		}
		//measure reward (improve lol)
		if (reward != -100) {
			if (r.reward(corner[0], corner[1]) == 10) { reward = 100000;  goto a; }
			if (r.reward(corner[0], corner[3]) == 10) { reward = 100000;  goto a; }
			if (r.reward(corner[1], corner[2]) == 10) { reward = 100000;  goto a; }
			if (r.reward(corner[2], corner[3]) == 10) { reward = 100000;  goto a; }
		}
	a:
		cumulativeReward += reward;

		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 8; j++) {
				if (!(network.weights[i][j] == network.weights[i][j])) {
					int c = 0;
				}
			}
		}

		if (reward == -100 || reward == 100000) {
			//experience replay
			std::vector<float> newState = { 0,0,0,0,0,0,0,0 };
			for (int i = 0; i < c.lines.size(); i++) newState[i] = t.lineIntersection(c.lines[i][0].position, c.lines[i][1].position, i);

			//max q
			std::vector<float> newActions = { 0,0,0,0,0,0,0,0 };
			network.decide(newState, newActions);
			float a = std::max(newActions[0], newActions[1]); float b = std::max(newActions[2], newActions[3]);
			float maxQ = std::max(a, b);

			xp.push_back(expRep(state, newState, actions, cumulativeReward, maxQ));

			if (descent >= 7 && reward == -100) { 

				descent = 0;
				cumulativeReward *= discount;
				float loss2 = cumulativeReward;


				//std::sort(xp.begin(), xp.end(), [](const expRep &x, const expRep &y) {return x.reward < y.reward; });

				for (int n = 0; n < 125; n++) {
					std::uniform_int<int> rxp(0, xp.size() - 1);
					int random = rxp(engine);
					//i and j for the weight looping, k for actions, h for hidden layer and some other shit aswell
					for (int i = 11; i > -1; i--) {
						for (int j = 0; j < 8; j++) {
							for (int k = 0; k < 4; k++)
								for (int h = 0; h < 8; h++) {
									//actions[i] for 8-12, else neuron activation i > 7 w1
									if (i > 7)
										 network.weights[i][j] += lr * weightG1(network.hiddenLayer[h], network.bias[i], network.weights[i][j]);
									else network.weights[i][j] += lr * weightG2(xp[random].state[h], network.bias[i], network.weights[k + 8][h]);
									//same for the biases
									if (i > 7)
										 network.bias[i] += lr * biasG1(network.hiddenLayer[h], network.bias[i], network.weights[i][j]);
									else network.bias[i] += lr * biasG2(xp[random].state[h], network.bias[i], network.weights[k + 8][h]);

								}
						}
					}
					cumulativeReward = 0;
				}
				iteration++;

				float loss1 = 0, loss;
				for (int i = 0; i < 12; i++) {
					for (int j = 0; j < 8; j++) {
						loss1 += network.weights[i][j];
					}
				}
				loss = std::pow(loss2 - loss1, 2);
				std::cout << loss << std::endl;
			}
			else descent++;
			cumulativeReward = 0;
		}
		//----------------------------------------------------
			//lines rotation and stuff
			{
				//draw
				window.clear(Color::Black);
				t.draw(window);
				c.draw(window);
				for (int i = 0; i < r.rewards.size(); i++) window.draw(r.rewards[i], 2, Lines);
				//for (int i = 0; i < c.lines.size(); i++) window.draw(c.lines[i], 2, Lines);
				window.display();
			}
	}
	return 0;
}

void rotateCorner(float cx, float cy, float theta, Vector2f &c1) {
	//translate to origin
	float tempx1 = c1.x - cx;
	float tempy1 = c1.y - cy;
	//rotation
	float rotatedx1 = (tempx1 * std::cos(theta)) - (tempy1 * std::sin(theta));
	float rotatedy1 = (tempx1 * std::sin(theta)) + (tempy1 * std::cos(theta));
	//translate back
	c1.x = rotatedx1 + cx, c1.y = rotatedy1 + cy;
}


float weightG1(float input, float bias,float weight) {
	//partial derivate implementation
	float x = weight * input + bias;
	//partial in respect to z , a and c multiplied
    return  -1 * input * sigmoid(x) * (1 - sigmoid(x)); 
}

float weightG2(float input , float bias, float weight) {
	//partial derivate implementation
	float x = weight * input + bias;
	//return gradient of w
	return input * weight * -1 * sigmoid(x) * (1 - sigmoid(x));
}

float biasG1(float input, float bias, float weight) {
	//partial derivate implementation
	float x = weight * input + bias;
	//partial in respect to z , a and c multiplied (z is just 1)
	return -1 * sigmoid(x) * (1 - sigmoid(x));
}

float biasG2(float input, float bias, float weight) {
	//partial derivate implementation
	float x = weight * input + bias;
	//return gradient of the bias
	return weight * -1 * (sigmoid(x) * (1 - sigmoid(x)));
}

float sigmoid(float x) {
	return 1 / (1 + std::powf(2.718281, -x));
}