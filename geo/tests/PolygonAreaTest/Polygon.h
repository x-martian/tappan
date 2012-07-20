#pragma once
#include <vector>
#include "Geometry.h"

namespace {

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
			// clockwise Polygon
			points.push_back(Point(0.1, 3.1));
			points.push_back(Point(3.1, 3.1));
			points.push_back(Point(3.1, 2.7));
			points.push_back(Point(3.1, 0.1));
			points.push_back(Point(2.3, 0.1));
			points.push_back(Point(0.1, 0.1));
			break;
		case 3:
			// trianglar Polygon, with the diagnol line passes through vertices
			points.push_back(Point(0.1, 0.1));
			points.push_back(Point(1.1, 0.1));
			points.push_back(Point(3.1, 0.1));
			points.push_back(Point(3.1, 2.7));
			points.push_back(Point(3.1, 3.1));
			break;
		case 4:
			// clockwise Polygon triangluar, with all points on vertices
//			points.push_back(Point(3.0, 3.0));
//			points.push_back(Point(3.0, 2.7));
//			points.push_back(Point(3.0, 0.0));
//			points.push_back(Point(2.3, 0.0));
			points.push_back(Point(1.0, 1.0));	// *
			points.push_back(Point(1.0, 0.0));	// *
			points.push_back(Point(0.0, 0.0));
			break;
		case 5:
			// counter clockwise Polygon triangluar
			points.push_back(Point(1.0, 1.0));
			points.push_back(Point(0.0, 0.0));
			points.push_back(Point(1.0, 0.0));
			break;
		case 6:
			// Polygon causes exception
/*			points.push_back(Point(61.09, -8.98));
			points.push_back(Point(59.33, -8.00));
			points.push_back(Point(57.57, -7.49));
			points.push_back(Point(55.82, -7.49));
			points.push_back(Point(54.06, -8.67));
			//...
			points.push_back(Point(64.61, -17.22));
			points.push_back(Point(64.99, -16.26));
			points.push_back(Point(65.01, -14.50));
			points.push_back(Point(64.01, -13.04));
			points.push_back(Point(64.45, -12.74));
			points.push_back(Point(63.26, -10.99));
			points.push_back(Point(62.85, -10.22));
			points.push_back(Point(61.57, -9.23));*/
			points.push_back(Point( sqrt(2.0), 0.0));
			points.push_back(Point(0.0,  sqrt(2.0)));
			points.push_back(Point(-sqrt(2.0), 0.0));
			points.push_back(Point(0.0, -sqrt(2.0)));
			break;
		case 7:
			points.push_back(Point(0.0, -sqrt(2.0)));
			points.push_back(Point(-sqrt(2.0), 0.0));
			points.push_back(Point(0.0,  sqrt(2.0)));
			points.push_back(Point( sqrt(2.0), 0.0));
			break;
		case 8:
			points.push_back(Point(0.5, 0.5));
			points.push_back(Point(1.5, 0.5));
			points.push_back(Point(3.4, 0.7));
			points.push_back(Point(3.3, 2.9));
			points.push_back(Point(3.5, 3.5));
			points.push_back(Point(0.5, 3.5));
			break;
		case 9:
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

	size_t GetPointCount() const {
		return points.size();
	}

private:
	std::vector<Point> points;
};

}