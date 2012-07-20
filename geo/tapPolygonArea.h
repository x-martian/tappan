#pragma once
#include <assert.h>

// A class template to compute area enclosed within a 2D polygon over a
// given geometry type, G.  G contains information of grid size, means of
// moving around the grid, and attributes per grid tile to be used for
// weighting, etc.  R is the type that processes (uses) the result
// of the computation.
//     For example, to compute average intensity of a CT signal of a ROI
// which is delineated by a polygon.  P will be the class that represents
// the polygon, G contains intensity data and pixel shape and size, and R
// should be caculating the mean as the polygon is processed.
//
// Limitations:  Currently only rectangular grid data is supported
template<typename G, typename C, typename R, bool w=false>
class tapPolygonArea
{
public:
	tapPolygonArea(void)
	{
	}

	~tapPolygonArea(void)
	{
	}
};

template<typename G, typename C, typename R>
class tapPolygonArea<G, C, R, true>
{
	typedef typename G::Path Path;
	typedef typename G::Area Area;
	typedef typename G::Weight Weight;
	typedef typename G::Vertex Vertex;
	typedef typename G::Direction Direction;
	typedef typename G::Coordinate Coordinate;

public:
	tapPolygonArea(void)
	{
	}

	~tapPolygonArea(void)
	{
	}

	// As the polygon travels from one geometric tile to the other, we'll replace
	// the section of the polygon within the grid tile using the grid boundary
	// while keeping trak of the difference in the area due to the replacement.
	static typename Path Compute(const G& g, const C& c, R& r)
	{
		Path path;	// path of the grid aligned polygon

		typedef C::const_iterator const_iterator;
		const_iterator firstPolygonPoint = c.GetFirstPoint();
		size_t n = c.GetPointCount();
		if (n<3) return path;

		C::const_iterator head = firstPolygonPoint, tail = c.GetPolygonEnd();

		// find the world coordinate of the local origin, and transform
		// the current point from world to local coordinate
        Vertex vertex;
        g.WorldToVertex(*firstPolygonPoint, vertex);
        Coordinate origin;
        g.VertexToWorld(vertex, origin);
        Coordinate trailingPointCoordinate;
        g.WorldToLocal(origin, *firstPolygonPoint, trailingPointCoordinate);
		
		Weight w = g.GetWeight(vertex);

		// search forward for the next point in polygon that fails outside of the current tile.
		Coordinate leadingPointCoordinate;
		while (++head != c.GetPolygonEnd()) {
			g.WorldToLocal(origin, *head, leadingPointCoordinate);
			if (!g.HitCurrentTile(vertex, leadingPointCoordinate))
				break;
			else
				r.Accumulate(g.ComputeArea(vertex, trailingPointCoordinate, leadingPointCoordinate), w);

			// since we expect most of the times, the length of the polygon segments
			// exceeds the size of the grid tile, it is worthwhile to copy the local
			// coordiante instead of recomputing it, see the GetDirection step.
			trailingPointCoordinate = leadingPointCoordinate;
		}
		if (head == c.GetPolygonEnd()) {
			// the polygon has never stepped out of one tile, so we will
			// compute area contribution from the last segment and return
			g.WorldToLocal(origin, *firstPolygonPoint, leadingPointCoordinate);
			r.Accumulate(g.ComputeArea(vertex, trailingPointCoordinate, leadingPointCoordinate), w);
			return path;
		}

		// figure out from which way did the polygon exits the current tile.
		Direction exit = g.GetDirection(vertex, trailingPointCoordinate, leadingPointCoordinate);
		// compute the area contribution from the segment between the
		// trailing point to the boundary intersection.
		// leadingPointCoordinate used for temp storagge
//		leadingPointCoordinate = g.DirectionToLocal(vertex, exit);
//		r.Accumulate(g.ComputeArea(vertex, trailingPointCoordinate, leadingPointCoordinate), w);

		// at this point, head is pointing to the polygon point just outside 
		// of the current tile.  Now we're going to turn around and iterate
		// toward the opposite direction.  Before that, we let head take one
		// step back.
		--head;

		// now we'll turn to the tail and iterate backwards alone the polygon.
		g.WorldToLocal(origin, *firstPolygonPoint, trailingPointCoordinate);
		while (tail != head) {
			--tail;
			g.WorldToLocal(origin, *tail, leadingPointCoordinate);

			while (!g.HitCurrentTile(vertex, leadingPointCoordinate)) {
				// work off the tiles between points local and next
				// since we're iterating backwards, we refer the intersection the
				// entry point into this tile when the polygon is drawn
				Direction entry = g.GetDirection(vertex, trailingPointCoordinate, leadingPointCoordinate);

				Coordinate tmp = g.DirectionToLocal(vertex, entry);	// coordinate of the entry point
				r.Accumulate(g.ComputeArea(vertex, tmp, trailingPointCoordinate), w);

				// now close the section with the tile boundary
				// push the vertices into the replacement polygon
				bool insertVertex = true;
				while (g.EdgeMismatch(vertex, entry, exit)) {
					Area area = g.ComputeIncrementalArea(vertex, exit);
					r.Accumulate(area, w);
					g.RotateDirectionCounterClockwise(vertex, exit, path, insertVertex);
					insertVertex = true;
				}

				r.Accumulate(g.ComputeIncrementalArea(vertex, exit, entry), w);
				
				// move to the next tile, based on the point of exit
				Vertex destVertex = g.MoveReference(vertex, entry, exit);
				g.VertexToWorld(destVertex, origin);
				g.TranslateCoordinate(destVertex, vertex, leadingPointCoordinate);

				vertex = destVertex;
				w = g.GetWeight(vertex);
				trailingPointCoordinate = g.DirectionToLocal(vertex, exit);
			}

			r.Accumulate(g.ComputeArea(vertex, leadingPointCoordinate, trailingPointCoordinate), w);
			trailingPointCoordinate = leadingPointCoordinate;
		}

		return path;
	}
};
