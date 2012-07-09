#pragma once
#include <map>

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
        , m_array(new unsigned char[w*h])
    {
        m_array = new unsigned char[w*h];
        memset(m_array, 0, w*h*sizeof(unsigned char));
    }

    ~Geometry()
    {
        delete [] m_array;
    }

    void SetPixel(unsigned x, unsigned y, unsigned char v) {
        m_array[y*m_w+x] = v;
    }

    unsigned char GetPixel(unsigned x, unsigned y) const {
        return m_array[y*m_w+x];
    }

private:
    unsigned m_w, m_h;
    unsigned char* m_array;
};
