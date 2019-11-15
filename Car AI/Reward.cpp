#include "Reward.h"

int Reward::reward(Vector2f p2, Vector2f q2)
{
	bool col = false;
	
	Vector2f p1 = rewards[canInteract][0].position;
	Vector2f q1 = rewards[canInteract][1].position;

	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case 
	if (o1 != o2 && o3 != o4) col = true;
	if (col) {
		int c;
	}
	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) col = true;
	if (col) {
		int c;
	}
	// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) col = true;
	if (col) {
		int c;
	}
	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) col = true;
	if (col) {
		int c;
	}
	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) col = true;


	if (col) {
		int c;
	}

	if (!col) return 0;
	else if(col){
		//reset with whom it can interact
		if (canInteract != 42) canInteract++; else canInteract = 0;
		return 10;
	}
}


void Reward::tp(int &i,int x1, int y1, int x2, int y2)
{
	rewards[i][0] = Vertex(Vector2f(x1, y1));
	rewards[i][1] = Vertex(Vector2f(x2, y2));
	i++;
}

bool Reward::onSegment(Vector2f p, Vector2f q, Vector2f r)
{
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int Reward::orientation(Vector2f p, Vector2f q, Vector2f r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
	// for details of below formula. 
	int val = ((q.y - p.y) * (r.x - q.x)) -
		((q.x - p.x) * (r.y - q.y));

	if (val == 0) return 0;  // colinear 

	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}