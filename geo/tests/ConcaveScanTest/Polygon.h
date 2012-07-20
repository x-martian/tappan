#pragma once
#include <vector>
#include "Geometry.h"

namespace {

    class Polygon
    {
    public:
        typedef Geometry::Coordinate Point;
        typedef Geometry::Coordinate Point;
        typedef std::vector<Point>::const_iterator PointID;
        typedef std::pair<double, double> Intersect;
        typedef std::pair<Intersect, PointID> Edge;

        Polygon() {
            points.reserve(10);
        }

        void Append(Point& point)
        {
            points.push_back(point);
        }

        PointID GetFirstPoint() const {
            return points.begin();
        }

        PointID GetPolygonEnd() const {
            return points.end();
        }

        size_t GetPointCount() const {
            return points.size();
        }

    private:
        std::vector<Point> points;
    };
}
