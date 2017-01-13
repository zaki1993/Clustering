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
		Cluster(Point _center){
			center = _center;
		}
		Point getCenter(){
			return center;
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
		int k = 0;
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
		for(int i = 0; i < k; ++i)
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
		for(int i = 0;i<clusters.size();i++){
			cout<<clusters[i].getCenter().first<<" "<<clusters[i].getCenter().second<<endl;
		}
		
	}
	//otherwise quit the program
	return 0;
}