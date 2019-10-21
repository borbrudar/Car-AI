#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
class Car {
public:
	Car() {
		if (!t.loadFromFile("res/car.jpg")) std::cout << "you fucked up pal";
		car.setTexture(t);
		car.setScale(0.0545454545454f, 0.04545454545f);
		car.setOrigin(10, 20);
		shape.setPosition(320, 240);
		shape.setFillColor(Color::Red);
		shape.setSize(Vector2f(10.f, 20.f));
		shape.setOrigin(5, 10);
	}
	void draw(RenderWindow &window, bool up, bool down, bool left, bool right);

	float speed = 0, maxSpeed = 1.2f, angle = 0, acc = 0.01f, dec = 0.002f, turnSpeed = .008f;
	Vector2f pos = { 320,240 }, vel = { 0, 0 }, accV = { 0,0 };
	RectangleShape shape;
	Texture t;
	Sprite car;
};