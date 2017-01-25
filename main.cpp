#include "cluster.h"
#include <fstream> //std::ifstream, std::ofstream
#include <stdlib.h> //srand, rand 
#include <time.h> //time 
#include <algorithm> //std::find
#include <string> //std::string
#include <limits> //numeric_limits
#include <cmath> //abs

using std::ifstream;
using std::ofstream;
using std::find;
using std::string;
using std::numeric_limits;
using std::cout;
using std::cin;
using std::endl;

double findDist(const Point& p1, const Point& p2)
{
	return (p2.first - p1.first)*(p2.first - p1.first) + (p2.second - p1.second)*(p2.second - p1.second);
}

size_t findClosestCluster(vector<Cluster> clusters, const Point& p)
{
	//get the max value of double type
	double currentDist = findDist(p, clusters[0].getCenter());
	size_t index = 0;
	//use for loop to find the index
	for(size_t i = 1; i < clusters.size(); ++i)
	{
		double dist = findDist(p, clusters[i].getCenter());
		if(currentDist > dist)
		{
			currentDist = dist;
			index = i;
		}
	}
	return index;
}

Point findCenter(const vector<Point>& points)
{
	//calculate the center of points
	double minDist = numeric_limits<double>::max();
	int index = 0;
	for(size_t i = 0; i < points.size(); ++i)
	{
		double temp = 0;
		for(size_t j = 0; j < points.size(); ++j)
		{
			temp += findDist(points[i], points[j]);
		}
		if(minDist > temp)
		{
			minDist = temp;
			index = i;
		}
	}
	return points[index];
}

void clusterize(vector<Cluster>& clusters, vector<Point> points)
{
	bool swap = false;
	for(size_t i = 0; i < clusters.size(); ++i)
	{
		//hold the old center
		Point currentCenter = clusters[i].getCenter();

		//hold the set of points
		vector<Point> currentPoints = clusters[i].getSetOfPoints();

		//find the new center
		Point newCenter = findCenter(currentPoints);

		//check if the old center and the new center are the same point
		if(!comparePoints(newCenter, currentCenter))
		{
			//they are not the same point
			//set the new center
			clusters[i].setCenter(newCenter);	
			//check if a point will change it's cluster
			vector<Point> set = clusters[i].getSetOfPoints();
			for(auto it = set.begin(); it != set.end(); ++it)
			{
				//check if the cluster is the same
				size_t index = findClosestCluster(clusters, *it);
				if(i != index)
				{
					//the clusters are different
					//change the cluster
					clusters[index].addPoint(*it);
					clusters[i].removePoint(*it);
					//the point change it's cluster
					//we have a swap
					//repeat the algorithm
					swap = true;
				}
			}
		}
	}
	if(swap)
	{
		//Call the function recursively
		clusterize(clusters, points);
	}
	//No change in the points
	//Everything is done!
}

void writeToFile(const string& filename, const vector<Cluster>& clusters)
{
	ofstream file(filename.c_str(), ofstream::out);
	for(size_t i = 0; i < clusters.size(); ++i)
	{
		file << i + 1<<": " << endl;
		vector<Point> points = clusters[i].getSetOfPoints();
		for(auto it = points.begin(); it != points.end(); ++it)
		{
			file << (*it).first << ' ' << (*it).second << endl;
		}
		file << endl;
	}
}

bool readFile(vector<Point>& points, const string& filename)
{
	ifstream filePoints(filename.c_str(), ifstream::in);
	if(filePoints != nullptr)
		{
		//The file exists and it is open
	   	//Read the file content
	   	//We asume the file data is correctly given
		Point temp;
		while(filePoints >> temp.first >> temp.second)
		{
			//Reading file line by line
			points.push_back(temp);
		}
		//We have successfully read the file
		//Close the file
		filePoints.close();
		return true;
	}
	else
	{
		cout<<"The file does not exists!"<<endl;
		return false;
	}
}

int main()
{	
	//Create a vector to store all the points that we will read from the file
	vector<Point> points;
	if(readFile(points, "test.txt"))
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
		srand (time(NULL));
		for(size_t i = 0; i < k; ++i)
		{
			//get a random k points from the vector of points
			size_t pointIndex = rand()%points.size();
			if(find(pointsIndexes.begin(), pointsIndexes.end(), pointIndex) == pointsIndexes.end())
			{
				//The point is not a center of a cluster
				pointsIndexes.push_back(pointIndex);
				Cluster temp(points[pointIndex]);

				//add the center to the set of points
				temp.addPoint(points[pointIndex]);
				clusters.push_back(temp);
			}
			else
			{
				//search for another point
				//this one is already a center of a cluster
				i--;
			}
		}
		for(auto it = points.begin(); it != points.end(); ++it)
		{
			//find the closest cluster center for that point
			//add the point to that cluster set of points
			size_t index = findClosestCluster(clusters, *it);

			//get the current cluster set of points
			vector<Point> currentPoints = clusters[index].getSetOfPoints();

			//check if the point is added to the set of points when we search for cluster center
			if(find(currentPoints.begin(), currentPoints.end(), *it) == currentPoints.end()){
				//the point is not in the set
				//add the point to the set
				clusters[index].addPoint(*it);
			}
		}
		//We found our centeres and created k clusters

		clusterize(clusters, points);
		writeToFile("result.txt", clusters);
	}
	//if the file does not exist quit the program
	return 0;
}