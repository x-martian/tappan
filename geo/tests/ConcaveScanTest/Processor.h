#pragma once

template <class G> class Processor
{
public:
    static void Span(G& geo, int y, int xl, int xr)
    {
        for (int x=xl; x<=xr; ++x)
            geo.SetPixel(x, y, 1);
    }
};