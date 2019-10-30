#include "Car.h"

void Car::draw(RenderWindow & window,bool up, bool down,bool left,bool right)
{
	std::vector<int> directions; directions.resize(8);
	n.decide(distances, directions);
	if (directions[0] == 1) up = true;
	if (directions[0] == -1) down = true;
	if (directions[1] == 1) right = true;
	if (directions[1] == -1) left = true;
	directions.resize(0);

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
	//for(int i = 0; i < lines.size(); i++) window.draw(lines[i], 2, Lines);
}

void Car::update()
{
	float cx = shape.getPosition().x, cy = shape.getPosition().y, theta = shape.getRotation() * (3.14159f / 180.f);
	float sizex = shape.getSize().x / 2; float sizey = shape.getSize().y / 2;
	{
	//line 1
	lines[0][0] = Vector2f(cx, cy - sizey);
	lines[0][1] = Vector2f(cx, cy - sizey - 800);
	//line 2						
	lines[1][0] = Vector2f(cx + sizex, cy - sizey);
	lines[1][1] = Vector2f(cx + sizex + 800, cy - sizey - 800);
	//line 3						
	lines[2][0] = Vector2f(cx + sizex, cy);
	lines[2][1] = Vector2f(cx + sizex + 800, cy);
	//line 4						
	lines[3][0] = Vector2f(cx + sizex, cy + sizey);
	lines[3][1] = Vector2f(cx + sizex + 800, cy + sizey + 800);
	//line 5						
	lines[4][0] = Vector2f(cx, cy + sizey);
	lines[4][1] = Vector2f(cx, cy + sizey + 800);
	//line 6						
	lines[5][0] = Vector2f(cx - sizex, cy + sizey);
	lines[5][1] = Vector2f(cx - sizex - 800, cy + sizey + 800);
	//line 7						
	lines[6][0] = Vector2f(cx - sizex, cy);
	lines[6][1] = Vector2f(cx - sizex - 800, cy);
	//line 8						
	lines[7][0] = Vector2f(cx - sizex, cy - sizey);
	lines[7][1] = Vector2f(cx - sizex - 800, cy - sizey - 800);
}
	for (int i = 0; i < lines.size(); i++) {
		rotateCorner(lines[i][0].position);
		rotateCorner(lines[i][1].position);
	}
}


void Car::rotateCorner(Vector2f &c1) {
	float cx = shape.getPosition().x, cy = shape.getPosition().y, theta = shape.getRotation() * (3.14159f / 180.f);
	//translate to origin
	float tempx1 = c1.x - cx;
	float tempy1 = c1.y - cy;
	//rotation
	float rotatedx1 = (tempx1 * std::cos(theta)) - (tempy1 * std::sin(theta));
	float rotatedy1 = (tempx1 * std::sin(theta)) + (tempy1 * std::cos(theta));
	//translate back
	c1.x = rotatedx1 + cx, c1.y = rotatedy1 + cy;
}
