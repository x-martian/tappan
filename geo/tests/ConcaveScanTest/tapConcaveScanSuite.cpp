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

        Geometry g(10, 10);
        Geometry::Window win(Geometry::Vertex(0,0), Geometry::Vertex(9,9));
        tapConcaveScan<Geometry, Polygon, Processor<Geometry> > concaveScan(p);
        concaveScan.Fill(g, win);

        for (unsigned int i=0; i<4; ++i)
            for (unsigned int j=0; j<4; ++j) {
                if (i<3 && j<3) {
                    if (g.GetPixel(i,j) != 1)
                        t.fail("False negative!");
                } else {
                    if (g.GetPixel(i,j) == 1)
                        t.fail("False positive!");
                }
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
    tapConcaveScanSuiteImpl::Test(impl, t);
    return t.done();
}
