#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>

using namespace sf;

class Track {
public:
	
	Track() { 
		trackPart[0][0] = Vertex(Vector2f(100, 100));
		trackPart[0][1] = Vertex(Vector2f(300, 300));
	}
	bool isColliding(Vector2f q, Vector2f q2);
	void draw(RenderWindow &window);
	bool onSegment(Vector2f p, Vector2f q, Vector2f r);
	int orientation(Vector2f p, Vector2f q, Vector2f r);
	std::vector < Vertex[2]> trackPart{ 1 };
};