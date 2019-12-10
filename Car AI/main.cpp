#include <SFML/Graphics.hpp>
#include <math.h>
#include "Car.h"
#include "Track.h"
#include "Neuron.h"
#include "Reward.h"
using namespace sf;

//function prototypes
void rotateCorner(float cx, float cy, float theta, Vector2f &c1);
float weightG1(float reward, float maxQ, float newS, float input, float bias, float weight);
float weightG2(float reward, float maxQ, float newS, float input, float bias, float weight, float weight1);
float weightG3(float reward, float maxQ, float newS, float input, float bias, float weight, float weight1, float weight2);
float biasG1  (float reward, float maxQ, float newS, float input, float bias, float weight);
float biasG2  (float reward, float maxQ, float newS, float input, float bias, float weight, float weight1);
float biasG3  (float reward, float maxQ, float newS, float input, float bias, float weight, float weight1, float weight2);
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

//1 parameter less
float discount = 0.99f;

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
	float epsilon = 1, loss = 0, lr = 0.05f;
	int cumulativeReward = 0, timestep = 0;
	bool randomAction = false, over = false, isRandom = false;

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
			//remove xp replay
			if (xp.size() > 50) {
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
				c.accV = Vector2f( 0,0 ); c.vel = Vector2f( 0,0 );
			}
		}
		//----------------------------------------------------
		//Implement the Q-learning alg


		for (int i = 0; i < c.lines.size(); i++) c.distances[i] = t.lineIntersection(c.lines[i][0].position, c.lines[i][1].position, i);

		//store for xp replay (for now just for gradient descent)
		std::vector<float> state = c.distances;
		state.push_back(c.pos.x);
		state.push_back(c.pos.y);
		state.push_back(c.angle);
		state.push_back(c.accV.x);
		state.push_back(c.accV.y);
		state.push_back(c.vel.x);
		state.push_back(c.vel.y);

		int reward = -10;

		//choose action
		if (timestep == 100 || timestep == 0) {
			timestep = 1;
			if (epsilon > dist(engine)) {
				//choose random actions
				std::uniform_int_distribution<int> ra(0, 1);
				actions[0] = ra(engine);
				actions[1] = ra(engine);
				actions[2] = ra(engine);
				actions[3] = ra(engine);
				randAct = actions;

				//variables update
				if (epsilon > 0.1f) epsilon -= 0.001f;
				std::cout << "epsilon: " << epsilon << std::endl;
				isRandom = true;
			}//Chosen action
			else {
				//choose an action (for gradient descent n shit)
				network.decide(state, actions);
				randAct = actions;
				isRandom = false;
			}
		}
		else {
			//actions = randAct;
			timestep++;
		}
		

		if (actions[0] >= 0.85f) left = true;
		if (actions[1] >= 0.85f) right = true;
		if (actions[2] >= 0.85f) up = true;
		if (actions[3] >= 0.85f) down = true;

		c.update(up, down, left, right);


		//reward test and line intersection info
		for (int i = 0; i < c.lines.size(); i++) {
			c.distances[i] = t.lineIntersection(c.lines[i][0].position, c.lines[i][1].position, i);
			if (t.isColliding(corner[0], corner[1])) { r.canInteract = 0; reward = -100000; over = true; } else over = false;
			if (t.isColliding(corner[0], corner[3])) { r.canInteract = 0; reward = -100000; over = true; } else over = false;
			if (t.isColliding(corner[1], corner[2])) { r.canInteract = 0; reward = -100000; over = true; }	else over = false;
			if (t.isColliding(corner[2], corner[3])) { r.canInteract = 0; reward = -100000; over = true; } else over = false;
		}
		//measure reward (improve lol)
		if (!over) {
			if (r.reward(corner[0], corner[1]) == 10) { reward = 1000;  goto a; }
			if (r.reward(corner[0], corner[3]) == 10) { reward = 1000;  goto a; }
			if (r.reward(corner[1], corner[2]) == 10) { reward = 1000;  goto a; }
			if (r.reward(corner[2], corner[3]) == 10) { reward = 1000;  goto a; }
		}
	a:
		cumulativeReward += reward;

		if ((timestep == 100) || over) {
			timestep = 0; 
			//experience replay
			std::vector<float> newState = { 0,0,0,0,0,0,0,0};
			for	(int i = 0; i < c.lines.size(); i++) newState[i] = t.lineIntersection(c.lines[i][0].position, c.lines[i][1].position, i);
			newState.push_back(c.pos.x);
			newState.push_back(c.pos.y);
			newState.push_back(c.angle);
			newState.push_back(c.accV.x);
			newState.push_back(c.accV.y);
			newState.push_back(c.vel.x);
			newState.push_back(c.vel.y);
			//max q
			std::vector<float> newActions = { 0,0,0,0, };
			network.decide(newState, newActions);
			float a = std::max(newActions[0], newActions[1]); float b = std::max(newActions[2], newActions[3]);
			float maxQ = std::max(a, b);

			xp.push_back(expRep(state, newState, actions, cumulativeReward, maxQ));
			cumulativeReward = 0;
		}

		if (over) {
				if (descent >= 10) {
					descent = 0;

					//std::sort(xp.begin(), xp.end(), [](const expRep &x, const expRep &y) {return x.reward < y.reward; });

					for (int n = 0; n < 10; n++) {
						std::uniform_int<int> rxp(0, xp.size() - 1);
						int random = rxp(engine);
						//calculate the new state
						std::vector<float> newS;
						network.decide(xp[random].newState, newS);

						//adjust last layer weights
						for (int i = 0; i < network.weights3.size(); i++) {
							for (int j = 0; j < network.weights3[0].size(); j++) {
								for (int input = 0; input < network.hiddenLayer2.size(); input++) {
									for (int a = 0; a < 4; a++) {
										network.weights3[i][j] += lr * -weightG1(xp[random].reward, xp[random].maxQ, newS[a], network.hiddenLayer2[input], network.bias[2], network.weights3[i][j]);
										network.bias[2] += lr * -biasG1(xp[random].reward, xp[random].maxQ, newS[a], network.hiddenLayer2[input], network.bias[2], network.weights3[i][j]);
									}
								}
							}
						}
						//ajdust second layer weights 
						for (int i = 0; i < network.weights2.size(); i++) {
							for (int j = 0; j < network.weights2[0].size(); j++) {
								for (int input = 0; input < network.hiddenLayer1.size(); input++) {
									for (int a = 0; a < 4; a++) {
										for (int k = 0; k < 4; k++) {
											network.weights2[i][j] += lr * -weightG2(xp[random].reward, xp[random].maxQ, newS[a], network.hiddenLayer1[input], network.bias[1], network.weights2[i][j], network.weights3[k][i]);
											network.bias[1] += lr * -biasG2(xp[random].reward, xp[random].maxQ, newS[a], network.hiddenLayer1[input], network.bias[1], network.weights2[i][j], network.weights3[k][i]);
										}
									}
								}
							}
						}
						//adjust first layer weights 
						for (int i = 0; i < network.weights1.size(); i++) {
							for (int j = 0; j < network.weights1[0].size(); j++) {
								for (int input = 0; input < 15; input++) {
									for (int a = 0; a < 4; a++) {
										for (int k = 0; k < 4; k++) {
											for (int k1 = 0; k1 < 16; k1++) {
												network.weights1[i][j] += lr * -weightG3(xp[random].reward, xp[random].maxQ, newS[a], xp[random].state[input], network.bias[1], network.weights2[i][j], network.weights3[k][i], network.weights2[k1][i]);
												network.bias[0] += lr * -biasG3(xp[random].reward, xp[random].maxQ, newS[a], network.hiddenLayer1[input], network.bias[1], network.weights2[i][j], network.weights3[k][i], network.weights2[k1][i]);
											}
										}
									}
								}
							}
						}
					}
					iteration++;
				}
				else descent++;
			    
			}

			

		//----------------------------------------------------
				//lines rotation and stuff
		{
			//draw
			window.clear(Color::Black);
			t.draw(window);
			c.draw(window, isRandom);
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
// ((- 2r - 2my + 2x) + 2i)

float weightG1(float reward, float maxQ, float newS, float input, float bias,float weight) {
	//partial derivate implementation
	float x = weight * input + bias;
	//partial in respect to z , a and c multiplied
	return  ((-2 * reward - 2 * maxQ * discount + 2 * newS) + 2 *input) * input * sigmoid(x) * (1 - sigmoid(x));
}

float weightG2(float reward, float maxQ, float newS, float input , float bias, float weight, float weight1) {
	//partial derivate implementation
	float x = weight * input + bias;
	//return gradient of w
	return ((-2 * reward - 2 * maxQ * discount + 2 * newS) + 2 * input) * input * weight1 * sigmoid(x) * (1 - sigmoid(x));
}
float weightG3(float reward, float maxQ, float newS, float input, float bias, float weight, float weight1, float weight2) {
	//partial derivate implementation
	float x = weight * input + bias;
	//return gradient of w
	return ((-2 * reward - 2 * maxQ * discount + 2 * newS) + 2 * input) * input * weight1 * weight2 * sigmoid(x) * (1 - sigmoid(x));

}
float biasG1(float reward, float maxQ, float newS, float input, float bias, float weight) {
	//partial derivate implementation
	float x = weight * input + bias;
	if (x != x) {
		int b = 0;
	}
	//partial in respect to z , a and c multiplied 
	return ((-2 * reward - 2 * maxQ * discount + 2 * newS) + 2 * input) * sigmoid(x) * (1 - sigmoid(x));
}

float biasG2(float reward, float maxQ, float newS, float input, float bias, float weight, float weight1) {
	//partial derivate implementation
	float x = weight * input + bias;
	//return gradient of the bias
	return ((-2 * reward - 2 * maxQ * discount + 2 * newS) + 2 * input) * weight1 * (sigmoid(x) * (1 - sigmoid(x)));
}

float biasG3(float reward, float maxQ, float newS, float input, float bias, float weight, float weight1, float weight2) {
	//partial derivate implementation
	float x = weight * input + bias;
	//return gradient of the bias
	return ((-2 * reward - 2 * maxQ * discount + 2 * newS) + 2 * input) * weight1 * weight2 * (sigmoid(x) * (1 - sigmoid(x)));
}

float sigmoid(float x) {
	int k = 0;
	if (x != 0) k = std::powf(2.718281, -x);
	float y = (1 / (1 + k));
	if (y != y) 
		return 0;
	else return y;
}