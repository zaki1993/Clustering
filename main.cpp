#include <iostream>
#include <fstream> //std::ifstream, std::ofstream
#include <vector> //std::vector
#include <utility> //std::pair
#include <stdlib.h> //srand, rand 
#include <time.h> //time 
#include <algorithm> //std::find

using std::cout;
using std::cin;
using std::endl;
using std::pair;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::find;

// -std=c++11 needed for Linux
using Point = pair<double, double>;

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
};

void clusterize(int k)
{

}

void findCenter()
{

}

double findDist(const Point& p1,const Point& p2)
{
	return (p2.first - p1.first)*(p2.first - p1.first) + (p2.second - p1.second)*(p2.second - p1.second);
}

size_t findClosestCluster(vector<Cluster> c, const Point& p)
{
	double currentDist = findDist(p, c[0].getCenter());
	size_t index = 0;
	//using for loop to find the index
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

int main()
{	
	bool fileExist = false;
	//Create a vector to store all the points that we will read from the file
	vector<Point> points;
	//Open a file that contains a set of points
	ifstream filePoints("test.txt",ifstream::in);
	if(filePoints != nullptr)
	{
		//The file exists and it is opened
		fileExist = true;
	   	//Reading the file content
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
				//searching for another point
				//this one is already a center of a cluster
				i--;
			}
		}
		//We found our centeres and created k clusters
		//Adding points to clusters	
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

		for(size_t i = 0;i<clusters.size();i++){
			cout<<"The center is: "<<clusters[i].getCenter().first<<" "<<clusters[i].getCenter().second<<endl;
			clusters[i].printPoints();
		}
	}
	//otherwise quit the program
	return 0;
}