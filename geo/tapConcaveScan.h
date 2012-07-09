#pragma once
#include <vector>
#include <algorithm>
#include <assert.h>

// Based on concavescan.c by Paul Heckert in Graphics Gems
//
// class template to scan convert nvert-sided concave non-simple polygon with
// vertices at (point[i].x, point[i].y) for i in [0..nvert-1] within the window
// win by calling spanproc for each visible span of pixels.
// Polygon can be clockwise or counterclockwise.
// Algorithm does uniform point sampling at pixel centers.
// Inside-outside test done by Jordan's rule: a point is considered inside if
// an emanating ray intersects the polygon an odd number of times.
// drawproc should fill in pixels from xl to xr inclusive on scanline y,
template<typename G, typename P, typename W>
class tapConcaveScan
{
    typedef typename G::Window Window;
    typedef typename P::Intersect Intersect;
    typedef typename P::Edge Edge;
    typedef typename P::PointID PointID;

    class CompareIndices
    {
    public:
        CompareIndices(const P& polygon) : p(polygon.GetFirstPoint()) {}
        bool operator() (PointID u, PointID v) {return u->second < v->second;}

    private:
        PointID p;
    };

    class CompareActiveEdges
    {
    public:
        CompareActiveEdges() {};
        bool operator() (const Edge& u, const Edge& v)
        {
            return u.first.first < v.first.first;
        }
    };

public:
    tapConcaveScan(const P& source) : polygon(source)
    {
    }

    ~tapConcaveScan(void)
    {
    }

    void Fill(G& geometry, const Window& win)
    {
        unsigned n = polygon.GetPointCount();
        std::vector<PointID> indices(n);

        // Create y-sorted array of indices ind[k] into vertex list
        PointID id = polygon.GetFirstPoint();
        for (unsigned k=0; k<n; ++k, ++id)
            indices[k] = id;

        CompareIndices compareIndices(polygon);
        std::sort(indices.begin(), indices.end(), compareIndices);

        activeEdgeTable.reserve(n);
        unsigned nextInLineToProcess;	// i.e. indices[nextInLineToProcess]
        double y0 = win.first.second > ceil(indices.front()->second-0.5) ? win.first.second : ceil(indices.front()->second-0.5);
        double y1 = win.second.second < floor(indices.back()->second+0.5) ? win.second.second : floor(indices.back()->second+0.5);

        unsigned k = 0;
        for (double y=y0; y<=y1; ++y) {	// step through scanlines
            // scanline y is at y+.5 in continuous coordinates

            // check vertices between previous scanline and current one, if any
            while (k<n && indices[k]->second<=y+0.5) {
                PointID i = indices[k];
                PointID j = i;
                if (j == polygon.GetFirstPoint())
                    j = polygon.GetPolygonEnd();
                --j;
                if (j->second <= y-0.5)
                    Delete(j);
                else if (j->second > y+0.5)
                    Insert(j, y);
                j = i;
                ++j;
                if (j == polygon.GetPolygonEnd())
                    j = polygon.GetFirstPoint();
                if (j->second <= y-0.5)
                    Delete(i);
                else if (j->second > y+0.5)
                    Insert(i, y);
                ++k;
            }

            // sort active edge list by active[j].x
            CompareActiveEdges compareActiveEdges;
            std::sort(activeEdgeTable.begin(), activeEdgeTable.end(), compareActiveEdges);

            // draw horizontal segments for scanline y
            assert(activeEdgeTable.size()%2 == 0);
            for (std::vector<Edge>::iterator j=activeEdgeTable.begin(); j != activeEdgeTable.end(); ++j)
            {
                double xl = ceil(j->first.first - 0.5);
                if (xl<win.first.first) xl = win.first.first;
                j->first.first += j->first.second;

                double xr = floor((++j)->first.first - 0.5);
                if (xr>win.second.first) xr = win.second.first;
                j->first.first += j->first.second;

                if (xl <= xr)
                    W::Span(geometry, static_cast<int>(y), static_cast<int>(xl), static_cast<int>(xr));
            }
        }
    }

private:
    // help function, delete an edge from the edge table
    void Delete(PointID i)
    {
        std::vector<Edge>::iterator j = activeEdgeTable.begin();
        while (j != activeEdgeTable.end() && j->second != i) ++j;
        activeEdgeTable.erase(j);
    }

    // helper function, add a new point (edge) to the edge table
    void Insert(PointID i, double y)
    {
        PointID j = i;
        ++j;
        if (j == polygon.GetPolygonEnd()) j = polygon.GetFirstPoint();

        double dx;
        if (i->second < j->second) {
            dx = (j->first-i->first)/(j->second-i->second);
            activeEdgeTable.push_back(Edge(Intersect(dx*(y+0.5-i->second)+i->first, dx), i));
        } else {
            dx = (i->first-j->first)/(i->second-j->second);
            activeEdgeTable.push_back(Edge(Intersect(dx*(y+0.5-j->second)+j->first, dx), i));
        }
    }

    const P& polygon;
    std::vector<Edge> activeEdgeTable;
};
