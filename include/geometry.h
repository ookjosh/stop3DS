// Geometry.h
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <stdlib.h>

typedef struct Point {
	int x;
	int y;
};
float lerp(float start, float end, float t);
Point lerp_point(Point p1, Point p2, float t);
int diagonalDistance(Point p1, Point p2);


float lerp(float start, float end, float t) {
	return start + t * (end - start);
}

Point lerp_point(Point p1, Point p2, float t) {
	Point p;
	//printf("lerpP: %d, %d", p1.x, p2.x);
	float newX = lerp(p1.x, p2.x, t);
	float newY = lerp(p1.y, p2.y, t);
	//printf("lerpF: %d, %.2f", static_cast<int>(newX), newY);
	p.x = static_cast<int>(newX);
	p.y = static_cast<int>(newY);
	return p;
	//return Point{.x = static_cast<int>(newX), .y = static_cast<int>(newY)};
}

int diagonalDistance(Point p1, Point p2) {
	//printf("ahh %d, %d, %d, %d", p1.x, p1.y, p2.x, p2.y);
	int dx = abs(p2.x - p1.x);
	int dy = abs(p2.y - p1.y);
	return (dx<=dy) ? dy : dx;
}

int diagonalDistance(int x1, int y1, int x2, int y2) {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	return (dx < dy) ? dy : dx;
}

std::vector<Point> line(Point p1, Point p2) {
	int numPoints = diagonalDistance(p1, p2);
	if (numPoints == 0) numPoints = 1;
	//printf("Num points %d\n", numPoints);
	std::vector<Point> points(numPoints);
	//printf("capacity: %d\n", points.capacity());
	
	for (int step = 0; step < numPoints; step++) {
		float t = (step == 0) ? 0.0 : step/(float)numPoints;
		//printf("T: %.2f", t);
		int x = static_cast<int>(lerp(p1.x, p2.x, t));
		int y = static_cast<int>(lerp(p1.y, p2.y, t));
		//printf("S: %d, %d", x, y);
		points.push_back(Point{.x = x, .y = y});
		//printf("Pushed: %d, %d\n", points.at(step).x, points.at(step).y);
	}
	
	return points;
}


#endif