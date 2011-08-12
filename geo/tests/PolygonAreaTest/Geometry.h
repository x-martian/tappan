#pragma once
#include "../../tapRectangularTile.h"
#include <vector>
#include <map>
#include <assert.h>

// This Geometry class represents rectanular pixels arranged in rows and
// columns.  Vertex is a interger pair identifies a pixels of this class by
// its column position and row position.  These position indices are zero based.
class Geometry
{
public:
    typedef int RowIndex;
    typedef int ColIndex;
    typedef std::pair<RowIndex, ColIndex> Vertex;
	typedef std::vector<Vertex> Path;
	typedef std::pair<double, double> Coordinate;
	typedef struct {char edge; double position;} Direction;
	typedef double Area;
	typedef double Weight;

	// Create a geometry with a weighted pixels.
	Geometry(unsigned w, unsigned h, double x0, double y0, double dx, double dy)
		: m_w(w), m_h(h), m_x0(x0), m_y0(y0), m_size(dx, dy)
	{
		m_array = new double[w*h];
		for (size_t i=0; i<w*h; ++i)
			m_array[i] = i*3.1415926;
	}

	~Geometry()
	{
		delete [] m_array;
	}

	Weight GetWeight(const Vertex& v) const
	{
		return m_array[v.second*m_w+v.first];
	}

	Area GetTileArea() const
	{
		return m_size.first * m_size.second;
	}

    void WorldToVertex(const Coordinate& coordinate, Vertex& vertex) const
    {
        double tmp = coordinate.first - m_x0;
        if (tmp > 0.0)
    		vertex.first = static_cast<RowIndex>(tmp/m_size.first);
        else if (tmp < 0.0)
            vertex.first = static_cast<RowIndex>(tmp/m_size.first)-1;
        else
            vertex.first = 0;
        tmp = coordinate.second - m_x0;
        if (tmp > 0.0)
            vertex.second = static_cast<ColIndex>(tmp/m_size.second);
        else if (tmp < 0.0)
            vertex.second = static_cast<ColIndex>(tmp/m_size.second)-1;
        else
            vertex.second = 0;
    }

    void VertexToWorld(const Vertex& v, Coordinate& coordinate) const
    {
        coordinate.first = m_x0+double(v.first)*m_size.first;
        coordinate.second = m_y0+double(v.second)*m_size.second;
    }

	// convert from world coordinate to the coordinate local to the current tile
    static void WorldToLocal(const Coordinate& origin, const Coordinate& world, Coordinate& local)
    {
        local.first = world.first-origin.first;
        local.second = world.second-origin.second;
    }

	// test if a point, specified by its local coordinate, is within the tile
    bool HitCurrentTile(const Vertex& reference, const Coordinate& local) const
    {
        return HitCurrentRow(reference, local) && HitCurrentColumn(reference, local);
    }

    bool HitCurrentRow(const Vertex& reference, const Coordinate& local) const
    {
        return tapRectangularTile<Geometry>::HitRow(m_size, local);
    }

    bool HitCurrentColumn(const Vertex& reference, const Coordinate& local) const
    {
        return tapRectangularTile<Geometry>::HitColumn(m_size, local);
    }

	// figure out from with direction does a point cross the title boundary
	Direction GetDirection(const Vertex& reference, const Coordinate& in, const Coordinate& out) const
	{
//		if (m_size.first>0) {
			return tapRectangularTile<Geometry>::GetDirection(m_size, in, out);
/*		} else {
			Coordinate tmpIn(-in.first, -in.second), tmpOut(-out.first, -out.second), tmpSize(-m_size.first, -m_size.second);
			Direction direction = hsRectangularTile<Geometry>::GetDirection(tmpSize, tmpIn, tmpOut);
			direction.position = -direction.position;
			return direction;
		}*/
	}

	Coordinate DirectionToLocal(const Vertex& reference, const Direction& direction) const
	{
		return tapRectangularTile<Geometry>::DirectionToLocal(m_size, direction);
	}

	Area ComputeArea(const Vertex& reference, const Coordinate& start, const Coordinate& end) const
	{
		return tapRectangularTile<Geometry>::ComputeArea(start, end);
	}

	Area ComputeIncrementalArea(const Vertex& reference, const Direction& direction) const
	{
		return tapRectangularTile<Geometry>::ComputeIncrementalArea(m_size, direction);
	}

	Area ComputeIncrementalArea(const Vertex& reference, const Direction& entry, const Direction& exit) const
	{
		return tapRectangularTile<Geometry>::ComputeIncrementalArea(m_size, entry, exit);
	}

	Vertex MoveReference(const Vertex& reference, const Direction& entry, Direction& exit) const
	{
		return tapRectangularTile<Geometry>::MoveReference(m_size, entry, exit, reference);
	}

	static bool EdgeMismatch(const Vertex& reference, const Direction& a, const Direction& b)
	{
		return a.edge != b.edge;
	}

	// Rotate the entry direction clockwise while extending the path
	void RotateDirectionCounterClockwise(const Vertex& reference, Direction& direction, Path& path, bool insertVertex) const
	{
		Vertex vertex = tapRectangularTile<Geometry>::RotateDirectionCounterClockwise(m_size, reference, direction);
		//if (path.empty() || vertex != path.back())
		//	path.push_back(vertex);
		if (insertVertex)
			path.push_back(vertex);
	}

	// find the displacement from b to a
	void TranslateCoordinate(const Vertex& a, const Vertex& b, Coordinate& coordinate) const
	{
		return tapRectangularTile<Geometry>::TranslateCoordinate(m_size, a, b, coordinate);
	}

private:
	double m_x0, m_y0;
	Coordinate m_size;
	unsigned m_w, m_h;
	double* m_array;
};
