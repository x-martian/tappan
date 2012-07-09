// tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConcaveScanTest/tapConcaveScanSuite.h"
#include "PolygonAreaTest/tapPolygonAreaSuite.h"

int _tmain(int argc, _TCHAR* argv[])
{
    tapConcaveScanSuite::Run();
    tapPolygonAreaSuite::Run();

    return 0;
}

