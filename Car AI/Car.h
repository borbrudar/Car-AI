#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
class Car {
public:
	//user defined variables
	Vector2f shapeSize = Vector2f(10, 20);
	float speed = 0, maxSpeed = 0.6f, angle = 0, acc = 0.004f, dec = 0.003f, turnSpeed = .017f;
	Vector2f pos, vel = { 0, 0 }, accV = { 0,0 }, startPos = { 75, 350 };

	Car() {
		if (!t.loadFromFile("res/car.png")) std::cout << "you fucked up pal";
		car.setTexture(t);
		car.setScale(shapeSize.x / 450, shapeSize.y / 780);
		car.setOrigin(shapeSize.x / 2, shapeSize.y / 2);

		shape.setFillColor(Color::Red);
		shape.setSize(shapeSize);
		shape.setOrigin(shapeSize.x /2, shapeSize.y / 2);

		pos = startPos;
	}
	void draw(RenderWindow &window, bool up, bool down, bool left, bool right);

	RectangleShape shape;
	Texture t;
	Sprite car;
};