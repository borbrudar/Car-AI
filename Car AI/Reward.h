#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Reward {
public:
	Reward() {
		//initialize lines
		{
			int i = 0;
			tp(i, 50, 300, 120, 300);
			tp(i, 50, 250, 120, 250);
			tp(i, 50, 200, 120, 200);
			tp(i, 40, 150, 130, 175);
			tp(i, 50, 100, 130, 140);
			tp(i,100,  80, 170, 120);
			tp(i,175,  40, 175, 120);
			tp(i,225,  40, 225, 100);
			tp(i,275,  40, 275, 100);
			tp(i,325,  40, 325, 100);
			tp(i,375,  40, 375, 120);
			tp(i,450,  40, 380, 120);
			tp(i,340, 150, 440, 150);
			tp(i,320, 200, 390, 200);
			tp(i,360, 300, 390, 210);
			tp(i,420, 270, 420, 190);
			tp(i,480, 160, 480, 270);
			tp(i,540, 150, 540, 250);
			tp(i,545, 150, 610, 170);
			tp(i,550,  80, 620, 140);
			tp(i,630,  40, 630, 130);
			tp(i,670,  40, 670, 150);
			tp(i,720,  80, 670, 160);
			tp(i,670, 170, 740, 170);
			tp(i,670, 200, 740, 200);
			tp(i,670, 240, 740, 240);
			tp(i,670, 290, 740, 290);
			tp(i,670, 340, 740, 340);
			tp(i,650, 390, 740, 390);
			tp(i,620, 440, 740, 440);
			tp(i,550, 490, 680, 490);
			tp(i,530, 500, 530, 575);
			tp(i,480, 500, 480, 575);
			tp(i,430, 500, 430, 575);
			tp(i,390, 500, 390, 575);
			tp(i,340, 500, 340, 575);
			tp(i,290, 500, 290, 575);
			tp(i,240, 500, 240, 575);
			tp(i,190, 500, 190, 575);
			tp(i,130, 500, 130, 575);
			tp(i, 50, 510, 120, 490);
			tp(i, 50, 460, 120, 460);
			tp(i, 50, 410, 120, 410);
			tp(i, 50, 360, 120, 360);
		}
	}
	int reward(Vector2f p2, Vector2f q2);
	std::vector<Vertex[2]> rewards{ 44 };
	void tp(int &i,int x1, int y1, int x2, int y2);
	int orientation(Vector2f p, Vector2f q, Vector2f r);
	bool onSegment(Vector2f p, Vector2f q, Vector2f r);
	int canInteract = 0;
};
