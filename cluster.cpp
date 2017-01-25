#include "cluster.h"

Cluster::Cluster(Point _center)
{
	center = _center;
}

Point Cluster::getCenter() const
{
	return center;
}

void Cluster::setCenter(const Point& p)
{
	center = p;
}

vector<Point> Cluster::getSetOfPoints() const
{
	return setOfPoints;
}

void Cluster::addPoint(const Point& p)
{
	setOfPoints.push_back(p);
}

void Cluster::removePoint(const Point& p)
{
	//find the element position
	for(size_t i = 0; i < setOfPoints.size(); ++i)
	{
		if(comparePoints(setOfPoints[i], p))
		{
			setOfPoints.erase(setOfPoints.begin() + i);
			return;
		}
	}
}

void Cluster::clearCluster()
{
	while(!setOfPoints.empty())
	{
		setOfPoints.pop_back();
	}
}