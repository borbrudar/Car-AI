#include "Track.h"

bool Track::isColliding(Vector2f p2, Vector2f q2)
{
	for (int i = 0; i < trackPart.size(); i++) {
		Vector2f p1 = trackPart[i][0].position;
		Vector2f q1 = trackPart[i][1].position;

		int o1 = orientation(p1, q1, p2);
		int o2 = orientation(p1, q1, q2);
		int o3 = orientation(p2, q2, p1);
		int o4 = orientation(p2, q2, q1);

		// General case 
		if (o1 != o2 && o3 != o4)
			return true;

		// Special Cases 
		// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
		if (o1 == 0 && onSegment(p1, p2, q1)) return true;

		// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
		if (o2 == 0 && onSegment(p1, q2, q1)) return true;

		// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
		if (o3 == 0 && onSegment(p2, p1, q2)) return true;

		// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
		if (o4 == 0 && onSegment(p2, q1, q2)) return true;
	}
	return false; // Doesn't fall in any of the above cases 
}

void Track::draw(RenderWindow & window)
{
	s.setTexture(t);
	window.draw(s);
	if (drawLines) {
		for (int i = 0; i < trackPart.size(); i++) {
			window.draw(trackPart[i], 2, Lines);
		}
	}
}

bool Track::onSegment(Vector2f p, Vector2f q, Vector2f r)
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
int Track::orientation(Vector2f p, Vector2f q, Vector2f r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
	// for details of below formula. 
	int val = ((q.y - p.y) * (r.x - q.x)) -
		((q.x - p.x) * (r.y - q.y));

	if (val == 0) return 0;  // colinear 

	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}

float Track::lineIntersection(Vector2f p2, Vector2f q2, int j)
{
	std::vector<float> distances;
	distances.resize(10);
	int k = 0;
	bool col = false;
	Vector2f P;
	for (int i = 0; i < trackPart.size(); i++) {
		col = false;
		Vector2f p1 = trackPart[i][0].position;
		Vector2f q1 = trackPart[i][1].position;

		int o1 = orientation(p1, q1, p2);
		int o2 = orientation(p1, q1, q2);
		int o3 = orientation(p2, q2, p1);
		int o4 = orientation(p2, q2, q1);

		// General case 
		if (o1 != o2 && o3 != o4)
			col = true;

		// Special Cases 
		// p1, q1 and p2 are colinear and p2 lies on segment p1q1
		if (o1 == 0 && onSegment(p1, p2, q1)) col = true;

		// p1, q1 and q2 are colinear and q2 lies on segment p1q1
		if (o2 == 0 && onSegment(p1, q2, q1)) col = true;

		// p2, q2 and p1 are colinear and p1 lies on segment p2q2
		if (o3 == 0 && onSegment(p2, p1, q2)) col = true;

		// p2, q2 and q1 are colinear and q1 lies on segment p2q2
		if (o4 == 0 && onSegment(p2, q1, q2)) col = true;

		if (col) {
			//trackpart line points positions
			float x1 = trackPart[i][0].position.x;
			float y1 = trackPart[i][0].position.y;
			float x2 = trackPart[i][1].position.x;
			float y2 = trackPart[i][1].position.y;
			//line positions
			float x3 = p2.x;
			float y3 = p2.y;
			float x4 = q2.x;
			float y4 = q2.y;
			//calculate intersection point
			float t = ((x1 - x3) * (y3 - y4) - (y1 - y3)*(x3 - x4)) / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
			P = Vector2f(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
			float deltaX = p2.x - P.x;
			float deltaY = p2.y - P.y;

			distances[k] = std::sqrt((deltaX * deltaX) + (deltaY * deltaY));
			k++;
		}
    }	
	for (int i = 9; i > 0; i--) {
		if (distances[i] == 0) distances.erase(distances.begin() + i);
	}
	std::sort(distances.begin(), distances.end());
	return distances[0];
}

void Track::tp(int &i, int x1, int y1, int x2, int y2)
{
	trackPart[i][0] = Vertex(Vector2f(x1, y1));
	trackPart[i][1] = Vertex(Vector2f(x2, y2));
	i++;
}
