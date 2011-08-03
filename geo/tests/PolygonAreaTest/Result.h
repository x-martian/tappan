#pragma once
#include <map>
#include "Geometry.h"

class Result
{
	typedef Geometry::Weight Weight;
	typedef Geometry::Vertex Vertex;
	typedef Geometry::Area Area;
	typedef Geometry::Path Path;

public:
	Result(const Weight& lo, const Weight& hi, unsigned bins)
		: m_low(lo), m_high(hi), m_bins(bins), m_total(0)
	{
	}

	void Accumulate(const Area& a, const Weight w)
	{
		m_total += a;
	}

	void ComputePathArea(const Geometry& g, Path& path)
	{
		if (path.size()<=2) return;

		Area tileArea = g.GetTileArea();

		std::map<unsigned, std::map<unsigned, int> > counts;
		Path::iterator a = path.end();
		--a;
		Path::iterator b = path.begin();

		int length = b->first - a->first;
		if (length != 0) {
			assert(length==1 || length==-1);
			assert(a->second == b->second);
			std::map<unsigned, int>& column = counts[length>0?a->first:b->first];
			std::map<unsigned, int>::iterator itr = column.end();
			for (unsigned j=0; j<a->second; ++j)
				if ((itr=column.find(j)) != column.end())
					itr->second += length;
				else
					column.insert(std::pair<unsigned, int>(j, length));
		}

		do {
			b = a;
			--a;
			length = b->first - a->first;
			if (length != 0) {
				assert(length==1 || length==-1);
				assert(a->second == b->second);
				std::map<unsigned, int>& column = counts[length>0?a->first:b->first];
				std::map<unsigned, int>::iterator itr = column.end();
				for (unsigned j=0; j<a->second; ++j)
					if ((itr=column.find(j)) != column.end())
						itr->second += length;
					else
						column.insert(std::pair<unsigned, int>(j, length));
			}
			path.pop_back();
		} while (a!=path.begin());

		std::map<unsigned, std::map<unsigned, int> >::iterator p = counts.begin();
		while (p!=counts.end()) {
			std::map<unsigned, int>::iterator q = p->second.begin();
			while (q!=p->second.end()) {
				Weight w = g.GetWeight(Vertex(p->first, q->first));
				if (q->second != 0)
					Accumulate(tileArea*q->second, w);
				++q;
			}
			++p;
		}
	}

	double TotalArea() const
	{
		return m_total;
	}

private:
	Weight m_weight;
	Weight m_low, m_high;
	unsigned m_bins;
	Area m_total;
};
