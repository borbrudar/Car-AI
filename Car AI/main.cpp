#include <SFML/Graphics.hpp>
#include <math.h>
#include "Car.h"
#include "Track.h"
#include "Neuron.h"
using namespace sf;

//function prototypes
void rotateCorner(float cx, float cy, float theta, Vector2f &c1);
int reward(Vector2f pos);
//struct for experience replay
struct expRep {
	expRep(std::vector<float> state, std::vector<float> newState, std::vector<float> actions, float reward) :
		state(state), newState(newState), actions(actions), reward(reward) {};
	std::vector<float> state;
	std::vector<float> newState;
	std::vector<float> actions;
	float reward;
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
	bool right = 0, left = 0, up = 0, down = 0;
	int iteration = 0, numberOfActions = 0;
	float epsilon = 1, discount = 0.9f, loss = 0;

	//for epsilon greedy strategy
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> dist(0, 1);
	

	//game loop
	while (window.isOpen()) {
		std::uniform_int_distribution<int> randomXP(0, xp.size());
		//update iteration count
		if (iteration == 10) {
			iteration = 0;
			network.copyNeuron(target_network);
			//remove xp replay
			if (xp.size() > 50) {
				for (int i = 0; i < 10; i++) xp.erase(xp.begin() + i);
			}
		}
		//Check for events
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::S) t.drawLines = !t.drawLines;
		}
		//updating the car n stuff
		{
			if (Keyboard::isKeyPressed(Keyboard::Left))  left = 1; else left = 0;
			if (Keyboard::isKeyPressed(Keyboard::Right)) right = 1; else right = 0;
			if (Keyboard::isKeyPressed(Keyboard::Up))    up = 1; else up = 0;
			if (Keyboard::isKeyPressed(Keyboard::Down))  down = 1; else down = 0;

			//TODO move to a separate car.h function
			float cx = c.shape.getPosition().x, cy = c.shape.getPosition().y, theta = c.shape.getRotation() * (3.14159f / 180.f);
			Vector2f corner[4];
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
			//Random action
		if (epsilon > dist(engine)) {
			//store for xp replay
			std::vector<float> state = c.distances;
			//choose an actions
			std::vector<float> actions{ 0,0,0,0 };
			std::vector<float> maxActions{ 0,0,0,0 };
			network.decide(c.distances, actions);
			target_network.decide(c.distances, maxActions);
			//perform the action
			if (actions[0] > 1.f) left = true;
			if (actions[1] > 1.f) right = true;
			if (actions[2] > 1.f) up = true;
			if (actions[3] > 1.f) down = true;
			c.update();
			for (int i = 0; i < c.lines.size(); i++) c.distances[i] = t.lineIntersection(c.lines[i][0].position, c.lines[i][1].position, i);
			//experience replay
			std::vector<float> newState = c.distances;
		    xp.push_back(expRep(state, newState, actions, reward(Vector2f(0, 0))));
			//loss function 
			float a = std::max(maxActions[0], maxActions[1]); float b = std::max(maxActions[2], maxActions[3]);
			float maxQ = std::max(a, b);
			for (int i = 0; i < actions.size(); i++) {
				loss += std::pow(reward(Vector2f(0, 0)) + discount * (maxQ - actions[i]), 2);
			}
			//TODO implement reward function and gradient to the loss function
		}//Chosen action
		else {

		}

		//----------------------------------------------------
		//lines rotation and stuff
		{
		//draw
		window.clear(Color::Black);
		t.draw(window);
		c.draw(window, up, down, left, right);
		//for(int i = 0; i < t.c.size(); i++) window.draw(t.c[i]);
		window.display();
	    }
		//variables update
		iteration++;
		numberOfActions++;
		//if(epsilon > 0.1f) epsilon = 1 / std::sqrt(numberOfActions + 1); 
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

int reward(Vector2f pos) {
	return -1;
}