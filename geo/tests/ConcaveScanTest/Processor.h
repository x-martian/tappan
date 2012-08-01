#pragma once

namespace {

    template <class G, char c> class Processor
    {
    public:
        static void Span(G& geo, int y, int xl, int xr)
        {
            for (int x=xl; x<=xr; ++x)
                geo.SetPixel(x, y, c);
        }
    };
}
