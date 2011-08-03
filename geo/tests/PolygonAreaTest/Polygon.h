#pragma once
#include <vector>
#include "Geometry.h"

class Polygon
{
public:
	typedef Geometry::Coordinate Point;
	typedef std::vector<Point>::const_iterator const_iterator;

	Polygon() {
		points.reserve(10);
	}

	void Append(Point& point)
	{
		points.push_back(point);
	}

	void CreateTestPolygon(unsigned index)
	{
		switch (index)
		{
		case 0:
			points.push_back(Point(0.1, 0.1));
			points.push_back(Point(1.1, 0.1));
			points.push_back(Point(3.1, 0.1));
			points.push_back(Point(3.1, 2.7));
			points.push_back(Point(3.1, 3.1));
			points.push_back(Point(0.1, 3.1));
			break;
		case 1:
			points.push_back(Point(0.1, 0.1));
			points.push_back(Point(2.3, 0.1));
			points.push_back(Point(3.1, 0.1));
			points.push_back(Point(3.1, 2.7));
			points.push_back(Point(3.1, 3.1));
			points.push_back(Point(0.1, 3.1));
			break;
		case 2:
			// clockwise polygon
			points.push_back(Point(0.1, 3.1));
			points.push_back(Point(3.1, 3.1));
			points.push_back(Point(3.1, 2.7));
			points.push_back(Point(3.1, 0.1));
			points.push_back(Point(2.3, 0.1));
			points.push_back(Point(0.1, 0.1));
			break;
		case 3:
			// trianglar polygon, with the diagnol line passes through vertices
			points.push_back(Point(0.1, 0.1));
			points.push_back(Point(1.1, 0.1));
			points.push_back(Point(3.1, 0.1));
			points.push_back(Point(3.1, 2.7));
			points.push_back(Point(3.1, 3.1));
			break;
		case 4:
			// clockwise polygon triangluar, with all points on vertices
//			points.push_back(Point(3.0, 3.0));
//			points.push_back(Point(3.0, 2.7));
//			points.push_back(Point(3.0, 0.0));
//			points.push_back(Point(2.3, 0.0));
			points.push_back(Point(1.0, 1.0));	// *
			points.push_back(Point(1.0, 0.0));	// *
			points.push_back(Point(0.0, 0.0));
			break;
		case 5:
			// counter clockwise polygon triangluar
			points.push_back(Point(1.0, 1.0));
			points.push_back(Point(0.0, 0.0));
			points.push_back(Point(1.0, 0.0));
			break;
		case 6:
			points.push_back(Point(0.5, 0.5));
			points.push_back(Point(1.5, 0.5));
			points.push_back(Point(3.4, 0.7));
			points.push_back(Point(3.3, 2.9));
			points.push_back(Point(3.5, 3.5));
			points.push_back(Point(0.5, 3.5));
			break;
		case 7:
			// breaks the code
			points.push_back(Point(3.0, 3.0));
			points.push_back(Point(3.0, 2.7));
			points.push_back(Point(3.0, 0.0));
			points.push_back(Point(2.3, 0.0));
			points.push_back(Point(0.0, 0.0));

			points.push_back(Point(0.5, 0.5));
			points.push_back(Point(1.5, 0.5));
			points.push_back(Point(3.4, 0.7));
			points.push_back(Point(3.3, 2.9));
			points.push_back(Point(3.5, 3.5));
			points.push_back(Point(0.5, 3.5));
			break;
		}
	}

	const_iterator GetFirstPoint() const {
		return points.begin();
	}

	const_iterator GetPolygonEnd() const {
		return points.end();
	}

	unsigned GetPointCount() const {
		return points.size();
	}

private:
	std::vector<Point> points;
};
