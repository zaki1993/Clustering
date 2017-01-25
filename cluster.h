#pragma once

#include <iostream>
#include <vector> //std::vector
#include <utility> //std::pair

using std::pair;
using std::vector;

// -std=c++11 needed for Unix
using Point = pair<double, double>;

bool comparePoints(const Point& p1, const Point& p2)
{
	return (p1.first == p2.first) && (p1.second == p2.second);
}

class Cluster{
	private:
		Point center;
		vector<Point> setOfPoints;
	protected:
	public:
		Cluster(Point);
		Point getCenter() const;
		void setCenter(const Point&);
		vector<Point> getSetOfPoints() const;
		void addPoint(const Point&);
		void removePoint(const Point&);
		void printPoints() const;
		void clearCluster();
};