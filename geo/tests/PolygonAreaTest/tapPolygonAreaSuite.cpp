#include "tapPolygonAreaSuite.h"
#include "Polygon.h"
#include "Geometry.h"
#include "Result.h"
#include "../../tapPolygonArea.h"
#include "../lemon.h"

class tapPolygonAreaSuiteImpl
{
public:
    tapPolygonAreaSuiteImpl()
        : g0(10, 10, 0.0, 0.0, 1.0, 1.0)        // square tile
        , g1(50, 50, -0.3, -0.1, 1.0/9.0, 0.09) // rectangular tile
        , g2(1, 1, -0.3, -0.1, 10.3, 9.7)       // all polygons are within one tile
        , g3(1, 4, 0.0001, 0.001, 10.5, 2.7)    // the polygons enter and leave tile from the edge
        , g4(1, 4, 0.0001, 0.001, 2.6, 12.7)    // the polygons enter and leave tile from the edge
        , g5(10, 10, 10.0, 10.0, -1.0, -1.0)    // negative pixel spacing
        , g6(3, 3, 3.0, 3.0, -2.0, -2.0)
    {}

    // A square of 3.0x3.0 drawn counterclockwise
    static void Area_SquarePolygon_3x3_Counterclockwise(tapPolygonAreaSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;

        // A counterclockwise polygon of 3.0x3.0 square
        Polygon p;
        p.Append(Point(0.1, 0.1));
        p.Append(Point(1.1, 0.1));
        p.Append(Point(3.1, 0.1));
        p.Append(Point(3.1, 2.7));
        p.Append(Point(3.1, 3.1));
        p.Append(Point(0.1, 3.1));
        p.Append(Point(0.1, 3.0));

        Result r0(-1.0, 100.1, 10), r1(-1.0, 100.1, 10);
        Geometry::Path path0 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g0, p, r0);
        r0.ComputePathArea(me.g0, path0);
        t.is(r0.TotalArea(), 9.0, 0.0001, "Total area of a 3.0x3.0 square is 9.0");

        Geometry::Path path1 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g1, p, r1);
        r1.ComputePathArea(me.g1, path1);
        t.is(r1.TotalArea(), 9.0, 0.0001, "Total area of a 3.0x3.0 square is 9.0");
    }

    // A square with kinks of 3.0x3.0 by diagnal drawn counterclockwise
    static void Area_Bended_SquarePolygon_3x3_Counterclockwise(tapPolygonAreaSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;

        Polygon p;
        p.Append(Point(0.1, 0.1));
        p.Append(Point(2.3, 0.3));	// a upward kink here
        p.Append(Point(3.1, 0.1));
        p.Append(Point(3.1, 3.1));
        p.Append(Point(1.1, 3.3));	// and here
        p.Append(Point(0.1, 3.1));

        Result r0(-1.0, 100.1, 10), r1(-1.0, 100.1, 10);
        Geometry::Path path0 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g0, p, r0);
        r0.ComputePathArea(me.g0, path0);
        t.is(r0.TotalArea(), 9.0, 0.0001, "Total area of a 3.0x3.0 square with a bend is 9.0");

        Geometry::Path path1 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g1, p, r1);
        r1.ComputePathArea(me.g1, path1);
        t.is(r1.TotalArea(), 9.0, 0.0001, "Total area of a 3.0x3.0 square with a bend is 9.0");
    }

    // A square of 3.0x3.0 drawn clockwise
    static void Area_SquarePolygon_3x3_clockwise(tapPolygonAreaSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;

        // A clockwise polygon of 3.0x3.0 square
        Polygon p;
        p.Append(Point(0.1, 3.1));
        p.Append(Point(3.1, 3.1));
        p.Append(Point(3.1, 2.7));
        p.Append(Point(3.1, 0.1));
        p.Append(Point(2.3, 0.1));
        p.Append(Point(0.1, 0.1));

        Result r0(-1.0, 100.1, 10), r1(-1.0, 100.1, 10);
        Geometry::Path path0 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g0, p, r0);
        r0.ComputePathArea(me.g0, path0);
        t.is(r0.TotalArea(), -9.0, 0.0001, "Total area of a 3.0x3.0 square is -9.0");

        Geometry::Path path1 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g1, p, r1);
        r1.ComputePathArea(me.g1, path1);
        t.is(r1.TotalArea(), -9.0, 0.0001, "Total area of a 3.0x3.0 square is -9.0");
    }

    // trianglar polygon, with the diagnol line passes through vertices
    static void Area_TrianglarPolygon_3x3_counterclockwise(tapPolygonAreaSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;

        Polygon p;
        p.Append(Point(0.1, 0.1));
        p.Append(Point(1.1, 0.1));
        p.Append(Point(3.1, 0.1));
        p.Append(Point(3.1, 2.7));
        p.Append(Point(3.1, 3.1));

        Result r0(-1.0, 100.1, 10), r1(-1.0, 100.1, 10);
        Geometry::Path path0 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g0, p, r0);
        r0.ComputePathArea(me.g0, path0);
        t.is(r0.TotalArea(), 4.5, 0.0001, "Total area of a 3.0x3.0 triangle is 4.5");

        Geometry::Path path1 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g1, p, r1);
        r1.ComputePathArea(me.g1, path1);
        t.is(r1.TotalArea(), 4.5, 0.0001, "Total area of a 3.0x3.0 triangle is 4.5");
    }

    // clockwise polygon triangluar, with all points on vertices
    static void Area_TrianglarPolygon_Integer_Vertices(tapPolygonAreaSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;

        Polygon p;
        p.Append(Point(1.0, 1.0));
        p.Append(Point(1.0, 0.0));
        p.Append(Point(0.0, 0.0));

        Result r0(-1.0, 100.1, 10), r1(-1.0, 100.1, 10);
        Geometry::Path path0 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g0, p, r0);
        r0.ComputePathArea(me.g0, path0);
        t.is(r0.TotalArea(), -.5, 0.0001, "Total area of a 1.0x1.0 triangle is -0.5");

        Geometry::Path path1 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g1, p, r1);
        r1.ComputePathArea(me.g1, path1);
        t.is(r1.TotalArea(), -.5, 0.0001, "Total area of a 1.0x1.0 triangle is -0.5");
    }

    // small counterclockwise polygon triangluar, all points on vertices
    static void Area_TrianglarPolygon_Integer_Vertices_CCW(tapPolygonAreaSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;

        Polygon p;
        p.Append(Point(1.0, 1.0));
        p.Append(Point(0.0, 0.0));
        p.Append(Point(1.0, 0.0));

        Result r0(-1.0, 100.1, 10), r1(-1.0, 100.1, 10);
        Geometry::Path path0 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g0, p, r0);
        r0.ComputePathArea(me.g0, path0);
        t.is(r0.TotalArea(), .5, 0.0001, "Total area of a ccw 1.0x1.0 triangle is 0.5");

        Geometry::Path path1 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g1, p, r1);
        r1.ComputePathArea(me.g1, path1);
        t.is(r1.TotalArea(), .5, 0.0001, "Total area of a ccw 1.0x1.0 triangle is 0.5");
    }

    // ccw diamond
    static void Area_Diamond_CCW(tapPolygonAreaSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;
        Polygon p;
        p.Append(Point(5.0+sqrt(2.0), 5.0));
        p.Append(Point(5.0, 5.0+sqrt(2.0)));
        p.Append(Point(5.0-sqrt(2.0), 5.0));
        p.Append(Point(5.0, 5.0-sqrt(2.0)));

        Result r0(-1.0, 100.1, 10), r1(-100.0, 555.1, 10);
        Geometry::Path path0 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g0, p, r0);
        r0.ComputePathArea(me.g0, path0);
        t.is(r0.TotalArea(), 4.0, 0.0001, "Total area of a ccw 2.0x2.0 diamond is 4.0");

        Geometry::Path path1 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g1, p, r1);
        r1.ComputePathArea(me.g1, path1);
        t.is(r1.TotalArea(), 4.0, 0.0001, "Total area of a ccw 2.0x2.0 diamond is 4.0");
    }

    // cw diamond
    static void Area_Diamond_CW(tapPolygonAreaSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;
        Polygon p;
        p.Append(Point(5.0, 5.0-sqrt(2.0)));
        p.Append(Point(5.0-sqrt(2.0), 5.0));
        p.Append(Point(5.0, 5.0+sqrt(2.0)));
        p.Append(Point(5.0+sqrt(2.0), 5.0));

        Result r0(-1.0, 100.1, 10), r1(-100.0, 555.1, 10);
        Geometry::Path path0 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g0, p, r0);
        r0.ComputePathArea(me.g0, path0);
        t.is(r0.TotalArea(), -4.0, 0.0001, "Total area of a cw 2.0x2.0 diamond is -4.0");

        Geometry::Path path1 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g1, p, r1);
        r1.ComputePathArea(me.g1, path1);
        t.is(r1.TotalArea(), -4.0, 0.0001, "Total area of a cw 2.0x2.0 diamond is -4.0");
    }

    // irregularly shaped polygon
    static void Irregularly_Shaped_Polygon(tapPolygonAreaSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;
        Polygon p;
        p.Append(Point(0.5, 0.5));
        p.Append(Point(1.5, 0.5));
        p.Append(Point(3.4, 0.7));
        p.Append(Point(3.3, 2.9));
        p.Append(Point(3.5, 3.5));
        p.Append(Point(0.5, 3.5));

        Result r0(-1.0, 100.1, 10), r1(-1.0, 100.1, 10);
        Geometry::Path path0 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g0, p, r0);
        r0.ComputePathArea(me.g0, path0);

        Geometry::Path path1 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g1, p, r1);
        r1.ComputePathArea(me.g1, path1);
        t.is(r0.TotalArea(), r1.TotalArea(), 0.0001, "Total area of a irregular polygon from two grids");
    }

    // self intersecting shaped polygon
    static void Self_Intersecting_Polygon(tapPolygonAreaSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;
        Polygon p;
        p.Append(Point(3.0, 3.0));
        p.Append(Point(3.0, 2.7));
        p.Append(Point(3.0, 0.0));
        p.Append(Point(2.3, 0.0));
        p.Append(Point(0.0, 0.0));

        p.Append(Point(0.5, 0.5));
        p.Append(Point(1.5, 0.5));
        p.Append(Point(3.4, 0.7));
        p.Append(Point(3.3, 2.9));
        p.Append(Point(3.5, 3.5));
        p.Append(Point(0.5, 3.5));

        Result r0(-1.0, 100.1, 10), r1(-1.0, 100.1, 10);
        Geometry::Path path0 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g0, p, r0);
        r0.ComputePathArea(me.g0, path0);

        Geometry::Path path1 = tapPolygonArea<Geometry, Polygon, Result, true>::Compute(me.g1, p, r1);
        r1.ComputePathArea(me.g1, path1);
        t.is(r0.TotalArea(), r1.TotalArea(), 0.0001, "Total area of a self intersecting polygon from two grids");
    }

private:
    Geometry g0, g1, g2, g3, g4, g5, g6;
};

tapPolygonAreaSuite::tapPolygonAreaSuite(void)
{
}


tapPolygonAreaSuite::~tapPolygonAreaSuite(void)
{
}

bool tapPolygonAreaSuite::Run()
{
    lemon::test<> t;
    tapPolygonAreaSuiteImpl impl;
    tapPolygonAreaSuiteImpl::Area_SquarePolygon_3x3_Counterclockwise(impl, t);
    tapPolygonAreaSuiteImpl::Area_Bended_SquarePolygon_3x3_Counterclockwise(impl, t);
    tapPolygonAreaSuiteImpl::Area_SquarePolygon_3x3_clockwise(impl, t);
    tapPolygonAreaSuiteImpl::Area_TrianglarPolygon_3x3_counterclockwise(impl, t);
    tapPolygonAreaSuiteImpl::Area_TrianglarPolygon_Integer_Vertices(impl, t);
    tapPolygonAreaSuiteImpl::Area_TrianglarPolygon_Integer_Vertices_CCW(impl, t);
    tapPolygonAreaSuiteImpl::Area_Diamond_CCW(impl, t);
    tapPolygonAreaSuiteImpl::Area_Diamond_CW(impl, t);
    tapPolygonAreaSuiteImpl::Irregularly_Shaped_Polygon(impl, t);
    tapPolygonAreaSuiteImpl::Self_Intersecting_Polygon(impl, t);

    return t.done();
}
