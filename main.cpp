#include <iostream>
#include <fstream> //std::ifstream, std::ofstream
#include <vector> //std::vector
#include <utility> //std::pair
#include <stdlib.h> //srand, rand 
#include <time.h> //time 
#include <algorithm> //std::find
#include <cmath> //abs
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::pair;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::find;
using std::string;

// -std=c++11 needed for Linux
using Point = pair<double, double>;

// create a Cluster class to store cluster data
// cluster data: center, set of points
class Cluster{
	private:
		Point center;
		vector<Point> setOfPoints;
	protected:
	public:
		Cluster(Point _center)
		{
			center = _center;
		}
		Point getCenter() const
		{
			return center;
		}
		void setCenter(const Point& p)
		{
			center = p;
		}
		vector<Point> getSetOfPoints() const
		{
			return setOfPoints;
		}
		void addPoint(const Point& p)
		{
			setOfPoints.push_back(p);
		}
		void printPoints() const
		{
			for(size_t i = 0; i < setOfPoints.size(); ++i)
			{
				cout<<setOfPoints[i].first<<" "<<setOfPoints[i].second<<endl;
			}
		}
		void clearCluster()
		{
			while(!setOfPoints.empty())
			{
				setOfPoints.pop_back();
			}
		}
};

double findDist(const Point& p1,const Point& p2)
{
	return (p2.first - p1.first)*(p2.first - p1.first) + (p2.second - p1.second)*(p2.second - p1.second);
}

size_t findClosestCluster(vector<Cluster> c, const Point& p)
{
	double currentDist = findDist(p, c[0].getCenter());
	size_t index = 0;
	//use for loop to find the index
	for(size_t i = 1; i < c.size(); ++i)
	{
		double dist = findDist(p,c[i].getCenter());
		if(currentDist > dist)
		{
			currentDist = dist;
			index = i;
		}
	}
	return index;
}

Point findCenter(vector<Point> points, Point center)
{
	//find the avarage point coordinates
	double avgX = center.first;
	double avgY = center.second;
	for(auto it = points.begin(); it != points.end(); ++it)
	{
		avgX += (*it).first;
		avgY += (*it).second;
	}
	avgX /= (points.size() + 1);
	avgY /= (points.size() + 1);

	return Point(avgX,avgY);
}

bool comparePoints(const Point& p1, const Point& p2)
{
	return (p1.first == p2.first) && (p1.second == p2.second);
}

int partition(vector<Cluster>& c,vector<Point>& points)
{
	//find the avarage Point coordinates
	vector<Point> centeres;
	for(size_t i = 0; i < c.size(); ++i)
	{
		Point currentCenter = c[i].getCenter();
		vector<Point> currentPoints = c[i].getSetOfPoints();
		Point newCenter = findCenter(currentPoints, currentCenter);
		if(comparePoints(newCenter, currentCenter))
		{
			centeres.push_back(currentCenter);
		}
		else
		{
			centeres.push_back(newCenter);
		}
	}
	//check if the center has a change
	bool centerChange = false;
	for(size_t i = 0; i < centeres.size(); ++i)
	{
		Point currentCenter = c[i].getCenter();
		if(!comparePoints(centeres[i], currentCenter))
		{
			centerChange = true;
			c[i].setCenter(centeres[i]);
			if(find(points.begin(), points.end(), currentCenter) == points.end())
			{
				points.push_back(currentCenter);
			}
		}
	}
	if(!centerChange)
	{
		//We have no change in the clusters center
		//Everything is done!
		return 0;
	}
	return -1;	
}

void clusterize(vector<Cluster>& clusters,vector<Point> points)
{
	for(size_t i = 0; i < clusters.size(); ++i)
	{
		clusters[i].clearCluster();
	}
	for(auto it = points.begin();it != points.end(); ++it)
	{
		bool hasPoint = false;
		//find if the point is a center of a cluster
		for(size_t i = 0; i < clusters.size(); ++i)
		{
			//Check if the point is already a center of a cluster
			//If the point is a center
			//Do not add it to the set of points of that cluster
			if(*it == clusters[i].getCenter())
			{
				hasPoint = true;
				break;
			}
		}
		if(hasPoint)
		{
			continue;
		}
		//add all the points only if they are not the center of a cluster
		//find the closest cluster center for that point
		//add the point to that cluster set of points
		size_t index = findClosestCluster(clusters, *it);
		clusters[index].addPoint(*it);
	}
	//Call the partition function
	int res = partition(clusters, points);
	if(res == 0)
	{
		return;
	}
	else
	{
		clusterize(clusters, points);
	}
}

void writeToFile(const string& filename,const vector<Cluster>& clusters)
{
	ofstream file(filename.c_str(), ofstream::out);
	for(size_t i = 0; i < clusters.size(); ++i)
	{
		file << i + 1<<": " << endl;
		file << "Center: " << clusters[i].getCenter().first<<" " << clusters[i].getCenter().second << endl;
		vector<Point> points = clusters[i].getSetOfPoints();
		for(auto it = points.begin(); it != points.end(); ++it)
		{
			file << (*it).first << " " << (*it).second << endl;
		}
		file << endl;
	}
}

int main()
{	
	bool fileExist = false;
	//Create a vector to store all the points that we will read from the file
	vector<Point> points;
	//Open a file that contains a set of points
	ifstream filePoints("test.txt",ifstream::in);
	if(filePoints != nullptr)
	{
		//The file exists and it is open
		fileExist = true;
	   	//Read the file content
	   	//We asume that the file data is correctly given
		Point temp;
		while(filePoints>>temp.first>>temp.second)
		{
			//Reading file line by line
			points.push_back(temp);
		}
		//We have successfully read the file
		//Close the file
		filePoints.close();
	}
	else
	{
		fileExist = false;
		cout<<"The file does not exists!"<<endl;
	}

	if(fileExist)
	{
		//The number of clusters
		size_t k = 0;
		do{
			cin>>k;
		}while(k <= 0 || k > points.size());

		//create a vector of clusters
		vector<Cluster> clusters;

		//create a vector of ints that cointains the indexes of the points
		vector<size_t> pointsIndexes;
		//find the center of each cluster
		//This algorithm will be slow for high number of clusters
		srand (time(NULL));
		for(size_t i = 0; i < k; ++i)
		{
			//get a random k points from the vector of points
			size_t pointIndex = rand()%points.size();
			if(find(pointsIndexes.begin(),pointsIndexes.end(),pointIndex) == pointsIndexes.end())
			{
				//The point is not a center of a cluster
				pointsIndexes.push_back(pointIndex);
				Cluster temp(points[pointIndex]);
				clusters.push_back(temp);
			}
			else
			{
				//search for another point
				//this one is already a center of a cluster
				i--;
			}
		}
		//We found our centeres and created k clusters
		clusterize(clusters, points);
		writeToFile("result.txt", clusters);
	}
	//if the file does not exist quit the program
	return 0;
}