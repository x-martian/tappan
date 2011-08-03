#pragma once

#include <vector>
#include <assert.h>

template<typename G> class tapRectangularTile
{
	typedef typename G::Vertex Vertex;
	typedef typename G::Coordinate Coordinate;
	typedef typename G::Direction Direction;
	typedef typename G::Area Area;

	enum {PLUS=2, DIAG=4};	// i.e. the second and the third bits from the right
public:

	tapRectangularTile(void)
	{
	}

	~tapRectangularTile(void)
	{
	}

	// Determine if a point is inside the rectangle or not
	inline static bool Hit(const Coordinate& size, const Coordinate& pt)
	{
		return (
			   pt.first>=0.0 && pt.first<size.first
			&& pt.second>=0 && pt.second<size.second
			);
	}

	inline static bool Miss(const Coordinate& size, const Coordinate& pt)
	{
		return (
			   pt.first<0.0 || pt.first>=size.first
			|| pt.second<0 || pt.second>=size.second
			);
	}

	// direction are defined as the following: 
	//  13       12
	//     +-3-+
	//     0   2
	//     +-1-+
	//  10       11
	//
	// or: 0000-left, 0001-down, 0010-right, 0011-up
	inline static Direction GetDirection(const Coordinate& size, const Coordinate& in, const Coordinate& out)
	{
		Direction direction = {0, 0.0};
		if ((out.first<0.0||out.first>=size.first) && (out.second<0.0||out.second>=size.second)) {
			double fx = (out.first - (out.first<0.0?0.0:size.first))/(out.first-in.first);
			double fy = (out.second- (out.second<0.0?0.0:size.second))/(out.second-in.second);
			if (fy>fx) {
				// intersects horizontal boundary first (up or down)
				direction.position = out.first*(1.0-fy) + in.first*fy;
				direction.edge = 1;	// intersect with a horizontal edge
				if (out.second>=size.second) {
					direction.edge |= PLUS;
					direction.position = size.first - direction.position;
				}
			} else if (fx>fy) {
				direction.position = out.second*(1.0-fx) + in.second*fx;
				if (out.first>=size.first) {
					direction.edge |= PLUS;
				} else {
					direction.position = size.second - direction.position;
				}
			} else {
				// passing a tile corner, in which case direction.position->0
				assert(direction.position == 0);
				assert(direction.edge == 0);
				if (out.first>=size.first) {
					if (out.second>=size.second)
						direction.edge = 3;
					else
						direction.edge = 2;
				} else {
					if (out.second<0.0)
						direction.edge = 1;
				}
			}
		} else if (out.first<0.0||out.first>=size.first) {
			if (out.first<0.0) {
				double f = out.first==in.first ? 0.5 : (out.first)/(out.first-in.first);
				direction.position = size.second - out.second*(1.0-f) - in.second*f;
			} else {
				direction.edge |= PLUS;
				double f = out.first==in.first ? 0.5 : (out.first-size.first)/(out.first-in.first);
				direction.position = out.second*(1.0-f) + in.second*f;
			}
		} else if (out.second<0.0||out.second>=size.second) {
			if (out.second<0.0) {
				double f = out.second==in.second? 0.5 : (out.second)/(out.second-in.second);
				direction.position = out.first*(1.0-f) + in.first*f;
			} else {
				direction.edge |= PLUS;
				double f = out.second==in.second ? 0.5 : (out.second-size.second)/(out.second-in.second);
				direction.position = size.first - out.first*(1.0-f) - in.first*f;
			}
			direction.edge += 1;	// vertical crossing
		} else {
			assert(false);	// we know at least one of the indeces is not equal
		}
		assert(direction.position>=0.0);

		return direction;
	}

	inline static Coordinate DirectionToLocal(const Coordinate& size, const Direction& direction)
	{
		switch (direction.edge)
		{
		case 0:
			return Coordinate(0.0, size.second-direction.position);
			break;
		case 1:
			return Coordinate(direction.position, 0.0);
			break;
		case 2:
			return Coordinate(size.first, direction.position);
			break;
		case 3:
			return Coordinate(size.first-direction.position, size.second);
			break;
		}

		return Coordinate();	// this should not happen
	}

	static Area ComputeArea(const Coordinate& start, const Coordinate& end)
	{
		return (start.second+end.second)*(start.first-end.first)/2.0;
	}

	// rotate the given direction counter clockwise by one tile edge, return the vertex been passed
	inline static Vertex RotateDirectionCounterClockwise(const Coordinate& size, const Vertex& reference, Direction& direction)
	{
		Vertex v(reference);

		switch (direction.edge)
		{
		case 2:
			++v.second;
		case 1:
			++v.first;
		case 0:
			++direction.edge;
			break;
		case 3:
			++v.second;
			direction.edge = 0;
			break;
		}

		direction.position = 0;

		return v;
	}

	// rotate the given direction clockwise by one title edge, return the vertex been passed
	inline static Vertex RotateDirectionClockwise(const Coordinate& size, const Vertex& reference, Direction& direction)
	{
		Vertex v(reference);

		switch (direction.edge)
		{
		case 3:
			++v.first;
			++v.second;
			--direction.edge;
			direction.position = size.second;
			break;
		case 2:
			++v.first;
			--direction.edge;
			direction.position = size.first;
		case 1:
			--direction.edge;
			direction.position = size.second;
			break;
		case 0:
			++v.second;
			direction.edge = 3;
			direction.position = size.first;
			break;
		}

		return v;
	}

	inline static Area ComputeIncrementalArea(const Coordinate& size, const Direction& entry, const Direction& exit)
	{
		assert(exit.edge == entry.edge);
		if (exit.edge == 3)
			return (exit.position - entry.position) * size.second;
		else
			return 0.0;
	}

	inline static Area ComputeIncrementalArea(const Coordinate& size, const Direction& direction)
	{
		switch (direction.edge)
		{
		case 0:
			return 0.0;
		case 1:
			return 0.0;
		case 2:
			return 0.0;
		case 3:
			return (size.first-direction.position) * size.second;
		}

		return 0.0;
	}

	// move to the tile where the entry comes from and make
	// the entry point to be the exit point of that tile
	inline static Vertex MoveReference(const Coordinate& size, const Direction& entry, Direction& exit, const Vertex& v)
	{
		Vertex vertex(v);

		switch (entry.edge)
		{
		case 0:
			vertex.first -= 1;
			exit.edge = 2;
			exit.position = size.second - entry.position;
			break;
		case 1:
			vertex.second -= 1;
			exit.edge = 3;
			exit.position = size.first - entry.position;
			break;
		case 2:
			vertex.first += 1;
			exit.edge = 0;
			exit.position = size.second - entry.position;
			break;
		case 3:
			vertex.second += 1;
			exit.edge = 1;
			exit.position = size.first - entry.position;
			break;
		}

		return vertex;
	}

	inline static void TranslateCoordinate(const Coordinate& size, const Vertex& a, const Vertex& b, Coordinate& coordinate)
	{
		if (a.first > b.first)
			coordinate.first -= size.first;
		else if (a.first < b.first)
			coordinate.first += size.first;

		if (a.second > b.second)
			coordinate.second -= size.second;
		else if (a.second < b.second)
			coordinate.second += size.second;
	}
};
