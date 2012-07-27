#pragma once
#include <map>
#include <assert.h>

namespace {

    class Geometry
    {
    public:
        typedef int RowIndex;
        typedef int ColIndex;
        typedef std::pair<RowIndex, ColIndex> Vertex;
        typedef std::pair<Vertex, Vertex> Window;
        typedef std::pair<double, double> Coordinate;

        /** Create the geometric object with size, origin, and pixel resolution. */
        Geometry(unsigned w, unsigned h)
            : m_w(w), m_h(h)
            , m_array(new char[w*h])
        {
            memset(m_array, 'o', w*h*sizeof(unsigned char));
        }

        ~Geometry()
        {
            delete [] m_array;
        }

        void SetPixel(unsigned x, unsigned y, unsigned char v) {
            assert(x<m_w);
            assert(y<m_h);
            m_array[y*m_w+x] = v;
        }

        unsigned char GetPixel(unsigned x, unsigned y) const {
            return m_array[y*m_w+x];
        }

        char* GetNullTerminatedRow(unsigned y) {
            m_array[(y+1)*m_w-1] = 0;
            return m_array + y*m_w;
        }

    private:
        unsigned m_w, m_h;
        char* m_array;
    };
}
