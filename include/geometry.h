// Geometry.h
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

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
	return Point({.x = lerp(p1.x, p2.x, t), .y = lerp(p1.y, p2.y, t)});
}

int diagonalDistance(Point p1, Point p2) {
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	return (dx<dy) ? dy : dx;
}

std::vector<Point> line(Point p1, Point p2) {
	int numPoints = diagonalDistance(p1, p2);
	std::vector<Point> points(numPoints);
	for (int step = 0; step < numPoints; step++) {
		float t = (step == 0) ? 0.0 : step/numPoints;
		points.push_back(lerp_point(p1, p2, t));
	}

	return points;
}


#endif