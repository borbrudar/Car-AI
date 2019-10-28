#include "Car.h"

void Car::draw(RenderWindow & window,bool up, bool down,bool left,bool right)
{
	if (up && speed < maxSpeed) {
		if (speed < 0) speed += dec;
		else speed += acc;
	}
	if (down && speed > -maxSpeed) {
		if (speed > 0) speed -= dec; 
		else speed -= acc;
	}
	if (!up && !down) {
		if (speed - dec > 0) speed -= dec;
		else if (speed + dec < 0) speed += dec;
		else speed = 0;
	}
	if (right && speed != 0)angle += turnSpeed * (speed / maxSpeed);
	if (left && speed != 0) angle -= turnSpeed * (speed / maxSpeed);

	pos.x += std::sin(angle) * speed;
	pos.y -= std::cos(angle) * speed;

	shape.setPosition(pos);
	shape.setRotation(angle * 180 / 3.141592);
	window.draw(shape);

	//texture
	car.setPosition(pos);
	car.setRotation(angle * 180 / 3.141592);
	//window.draw(car);

}
