#include "tapConcaveScanSuite.h"
#include "Geometry.h"
#include "Polygon.h"
#include "Processor.h"
#include "../../tapConcaveScan.h"
#include "../lemon.h"

class tapConcaveScanSuiteImpl
{
public:
    tapConcaveScanSuiteImpl()
    {
    }

    static void Test(tapConcaveScanSuiteImpl& me, lemon::test<>& t) {
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

        Geometry g(5,5);
        Geometry::Window win(Geometry::Vertex(0,0), Geometry::Vertex(9,9));
        tapConcaveScan<Geometry, Polygon, Processor<Geometry,'x'> > concaveScan(p);
        concaveScan.Fill(g, win);

        for (unsigned int i=0; i<4; ++i)
            for (unsigned int j=0; j<4; ++j) {
                if (i<3 && j<3) {
                    if (g.GetPixel(i,j) != 'x')
                        t.fail("False negative!");
                } else {
                    if (g.GetPixel(i,j) == 'x')
                        t.fail("False positive!");
                }
            }
    }

    /** test case where the polygon edges are rounded up in the grid. */
    static void RoundUp(tapConcaveScanSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;
        unsigned w=6, h=5;

        // A counterclockwise polygon
        //  (0.5,0.5)--
        //        |     --(3.6,0.6)
        //        |         |
        //         |        |
        //         |    --(3.5,3.5)
        //   (0.6,3.6)--
        t.diag("(0.5,0.5)--            ");
        t.diag("      |    --(3.6,0.6) ");
        t.diag("      |         |       ");
        t.diag("       |        |       ");
        t.diag("       |    --(3.5,3.5) ");
        t.diag(" (0.6,3.6)--            ");
        Polygon p;
        p.Append(Point(0.5, 0.5));
        p.Append(Point(3.6, 0.6));
        p.Append(Point(3.5, 3.5));
        p.Append(Point(0.6, 3.6));

        Geometry g(w, h);
        Geometry::Window win(Geometry::Vertex(0,0), Geometry::Vertex(9,9));
        tapConcaveScan<Geometry, Polygon, Processor<Geometry,'x'> > concaveScan(p);
        concaveScan.Fill(g, win);

        t.is(g.GetPixel(0,2), 'o', "Pixel[0][] whose x value range from 0.0 and 1.0 is not filled when lx > 0.5");
        t.is(g.GetPixel(1,2), 'x', "Pixel[1][] whose x value range from 1.0 and 2.0 is filled.");
        t.is(g.GetPixel(3,2), 'x', "Pixel[3][] whose x value range from 3.0 and 4.0 is filled when rx > 3.5");
        t.is(g.GetPixel(4,2), 'o', "Pixel[4][] whose x value range from 4.0 and 5.0 is not filled");

        t.is(g.GetPixel(2,0), 'o', "Pixel[][0] whose y value range from 0.0 and 1.0 is not filled when ly > 0.5");
        t.is(g.GetPixel(2,2), 'x', "Pixel[][1] whose y value range from 1.0 and 2.0 is filled.");
        t.is(g.GetPixel(2,3), 'x', "Pixel[][3] whose y value range from 3.0 and 4.0 is filled when uy > 3.5");
        t.is(g.GetPixel(2,4), 'o', "Pixel[][4] whose y value range from 4.0 and 5.0 is not filled");

        t.is(g.GetPixel(1,1), 'x', "Pixel[1][1] is filled, 0.5 at the left boundary is rounded off");
        t.is(g.GetPixel(3,3), 'x', "Pixel[3][3] is filled, 3.5 at the right boundary is rounded up");

        // output the map
        for (int j=0; j<h; ++j) {
            t.diag(g.GetNullTerminatedRow(j));
        }
    }

    /** a concave polygon, where the cave open to the left. */
    static void ConcavePolygonSide(tapConcaveScanSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;
        unsigned w=9, h=5;

        // A clockwise concave polygon
        //  (0.5,0.5)-
        //        |      --
        //          |         --
        //            |     (4.6,1.9)
        //      (2.6,2.1)       - 
        //            |     --   
        //          |   --
        //    (1.1,3.9)
        t.diag("(0.5,0.5)-               ");
        t.diag("      |      --          ");
        t.diag("        |         --     ");
        t.diag("          |     (4.6,1.9)");
        t.diag("    (2.6,2.1)       -    ");
        t.diag("          |     --       ");
        t.diag("        |   --           ");
        t.diag("  (1.1,3.9)              ");
        Polygon p, q;
        p.Append(Point(0.5, 0.5));
        p.Append(Point(4.6, 1.9));
        p.Append(Point(1.1, 3.9));
        p.Append(Point(2.6, 2.1));

        q.Append(Point(w-0.5, 0.5));
        q.Append(Point(w-4.6, 1.9));
        q.Append(Point(w-1.1, 3.9));
        q.Append(Point(w-2.6, 2.1));

        Geometry g(w+1,h);
        Geometry::Window win(Geometry::Vertex(0,0), Geometry::Vertex(9,9));
        tapConcaveScan<Geometry, Polygon, Processor<Geometry,'x'> > concaveScanp(p);
        tapConcaveScan<Geometry, Polygon, Processor<Geometry,'+'> > concaveScanq(q);
        concaveScanp.Fill(g, win);
        concaveScanq.Fill(g, win);

        t.is(g.GetPixel(1,1), 'o', "Pixel[1][1] inside the cave is not filled");
        t.is(g.GetPixel(1,2), 'o', "Pixel[1][2] inside the cave is not filled");
        t.is(g.GetPixel(2,1), 'x', "Pixel[2][1] wall of the cave is filled");
        t.is(g.GetPixel(2,2), 'x', "Pixel[2][2] wall of the cave is filled");

        // output the map
        for (int j=0; j<h; ++j) {
            t.diag(g.GetNullTerminatedRow(j));
            for (int i=0; i<w/2; ++i) {
                if (g.GetPixel(i,j)!='o'&&g.GetPixel(w-1-i,j)=='o' ||
                    g.GetPixel(i,j)=='o'&&g.GetPixel(w-1-i,j)!='o' )
                    t.fail("Reflection symmetry broken");
            }
        }
    }

    /** a concave polygon, where the cave open to the bottom. */
    static void ConcavePolygonUp(tapConcaveScanSuiteImpl& me, lemon::test<>& t)
    {
        typedef Polygon::Point Point;
        unsigned w=6, h=5;

        Polygon p;
        p.Append(Point(0.5, 0.5));
        p.Append(Point(1.9, 4.6));
        p.Append(Point(3.9, 1.1));
        p.Append(Point(2.1, 2.6));

        Geometry g(w, h);
        Geometry::Window win(Geometry::Vertex(0,0), Geometry::Vertex(9,9));
        tapConcaveScan<Geometry, Polygon, Processor<Geometry,'x'> > concaveScan(p);
        concaveScan.Fill(g, win);

        t.is(g.GetPixel(1,1), 'o', "Pixel[1][1] inside the cave not filled");
        t.is(g.GetPixel(2,1), 'o', "Pixel[1][1] inside the case not filled");
        t.is(g.GetPixel(1,2), 'x', "Pixel[1][1] bottom of the cave is filled");
        t.is(g.GetPixel(2,2), 'x', "Pixel[1][1] bottom of the cave is filled");

        // output the map
        for (int j=0; j<h; ++j) {
            t.diag(g.GetNullTerminatedRow(j));
        }
    }
};

tapConcaveScanSuite::tapConcaveScanSuite(void)
{
}

tapConcaveScanSuite::~tapConcaveScanSuite(void)
{
}

bool tapConcaveScanSuite::Run()
{
    lemon::test<> t;
    tapConcaveScanSuiteImpl impl;
    tapConcaveScanSuiteImpl::ConcavePolygonSide(impl, t);
    tapConcaveScanSuiteImpl::ConcavePolygonUp(impl, t);
    tapConcaveScanSuiteImpl::RoundUp(impl, t);
    tapConcaveScanSuiteImpl::Test(impl, t);
    return t.done();
}
