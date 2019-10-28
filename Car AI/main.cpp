#include <SFML/Graphics.hpp>
#include "Car.h"
#include "Track.h"
using namespace sf;

//function prototypes
void rotateCorner(float cx, float cy, float theta, Vector2f &c1);

int main() {
	RenderWindow window;
	window.create(VideoMode(800, 600), "Car AI");

	Event e;

	Car c;
	Track t;
	bool right = 0, left = 0, up = 0, down = 0;

	//game loop
	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
		}
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

		bool isBlue = false;
		for (int i = 0; i < 4; i++) rotateCorner(cx, cy, theta, corner[i]);
		if (t.isColliding(corner[0], corner[1])) isBlue = true;
		if (t.isColliding(corner[1], corner[2])) isBlue = true;
		if (t.isColliding(corner[2], corner[3])) isBlue = true;
		if (t.isColliding(corner[3], corner[0])) isBlue = true;
		if (isBlue) c.shape.setFillColor(Color::Blue); else c.shape.setFillColor(Color::Red);
		//----------------------------------------------------


		//draw
		window.clear(Color::Black);

		t.draw(window);
		c.draw(window, up, down, left, right);

		window.display();
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