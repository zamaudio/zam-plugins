#include "instrument.hpp"

LookupTable::LookupTable()
{
	m_Points = NULL;
	m_nPoints = 0;
}

void LookupTable::set(double *points, int num_points)
{
	// Note: Actual array size is 2*num_points
	
	m_Points = points;
	m_nPoints = num_points;
}

double LookupTable::getValue(double x)
{
	// Note: Assumes points are monotonically increasing in X!
	
	int i=0;
	while (x>m_Points[i*2] && i<m_nPoints)
		i++;
	
	if (i==0)
		return m_Points[1];
	
	if (i>=m_nPoints)
		return m_Points[(m_nPoints-1)*2+1];
	
	double ratio =
	(x - m_Points[(i-1)*2])
	/ (m_Points[i*2] - m_Points[(i-1)*2]);
	
	return m_Points[(i-1)*2+1]*(1-ratio) + m_Points[i*2+1]*(ratio);
}

