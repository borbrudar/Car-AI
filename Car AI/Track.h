#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <iostream>

using namespace sf;

class Track {
public:
	
	Track() {
		if (!t.loadFromFile("res/track.png")) std::cout << " track FIle not loaded" << std::endl;
		//define the track
		int n = 0;
		//setting up the inner rim
		{
			tp(n, 104, 496, 104, 192);
			tp(n, 104, 192, 129, 140);
			tp(n, 129, 140, 170, 94);
			tp(n, 170, 94, 362, 94);
			tp(n, 362, 94, 389, 112);
			tp(n, 389, 112, 356, 134);
			tp(n, 356, 134, 319, 196);
			tp(n, 319, 196, 319, 228);
			tp(n, 319, 228, 337, 260);
			tp(n, 337, 260, 366, 280);
			tp(n, 366, 280, 565, 227);
			tp(n, 565, 227, 596, 200);
			tp(n, 596, 200, 614, 129);
			tp(n, 614, 129, 659, 118);
			tp(n, 659, 118, 675, 137);
			tp(n, 675, 137, 675, 368);
			tp(n, 675, 368, 644, 402);
			tp(n, 644, 402, 621, 444);
			tp(n, 621, 444, 584, 452);
			tp(n, 584, 452, 563, 483);
			tp(n, 563, 483, 556, 510);
			tp(n, 556, 510, 128, 510);
			tp(n, 128, 510, 104, 496);
		}
		//setting up outer rim
		{
			tp(n, 52, 193, 52, 517);
			tp(n, 52, 193, 64, 135);
			tp(n, 64, 135, 90, 92);
			tp(n, 90, 92, 120, 63);
			tp(n, 120, 63, 157, 42);
			tp(n, 157, 42, 397, 42);
			tp(n, 397, 42, 423, 57);
			tp(n, 423, 57, 447, 90);
			tp(n, 447, 90, 447, 130);
			tp(n, 447, 130, 420, 170);
			tp(n, 420, 170, 379, 194);
			tp(n, 379, 194, 373, 209);
			tp(n, 373, 209, 381, 221);
			tp(n, 381, 221, 551, 161);
			tp(n, 551, 161, 572, 84);
			tp(n, 572, 84, 605, 66);
			tp(n, 605, 66, 654, 55);
			tp(n, 654, 55, 731, 100);
			tp(n, 731, 100, 737, 333);
			tp(n, 737, 333, 714, 411);
			tp(n, 714, 411, 714, 442);
			tp(n, 714, 442, 683, 459);
			tp(n, 683, 459, 654, 487);
			tp(n, 654, 487, 618, 505);
			tp(n, 618, 505, 610, 537);
			tp(n, 610, 537, 583, 562);
			tp(n, 583, 562, 109, 563);
			tp(n, 109, 563, 60, 549);
			tp(n, 60, 549, 51, 517);
		}

	}
	bool isColliding(Vector2f q, Vector2f q2);
	void draw(RenderWindow &window);
	bool onSegment(Vector2f p, Vector2f q, Vector2f r);
	int orientation(Vector2f p, Vector2f q, Vector2f r);
	float lineIntersection(Vector2f p1, Vector2f p2,int j);
	void tp(int &i, int x1, int y1, int x2, int y2);
	std::vector < Vertex[2]> trackPart{ 52 };
	Texture t;
	Sprite s;
	bool drawLines = false;
};