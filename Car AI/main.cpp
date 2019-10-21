#include <SFML/Graphics.hpp>
#include "Car.h"

using namespace sf;
int main() {
	RenderWindow window;
	window.create(VideoMode(800, 600), "Car AI");

	Event e;

	Car c;
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
		
		
		window.clear(Color::Black);
		c.draw(window,up,down,left,right);
		window.display();
	}


	return 0;
}