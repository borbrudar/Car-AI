#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
class Car {
public:
	//user defined variables
	Vector2f shapeSize = Vector2f(20, 40);
	float speed = 0, maxSpeed = 0.8f, angle = 0, acc = 0.005f, dec = 0.002f, turnSpeed = .015f;
	Vector2f pos = { 320,240 }, vel = { 0, 0 }, accV = { 0,0 };

	Car() {
		if (!t.loadFromFile("res/car.jpg")) std::cout << "you fucked up pal";
		car.setTexture(t);
		car.setScale(0.0545454545454f, 0.04545454545f);
		car.setOrigin(shapeSize.x / 2, shapeSize.y / 2);
		shape.setPosition(320, 240);
		shape.setFillColor(Color::Red);
		shape.setSize(shapeSize);
		shape.setOrigin(shapeSize.x /2, shapeSize.y / 2);

		int width = shape.getSize().x;
		int height = shape.getSize().y;

		
	}
	void draw(RenderWindow &window, bool up, bool down, bool left, bool right);

	RectangleShape shape;
	Texture t;
	Sprite car;
};