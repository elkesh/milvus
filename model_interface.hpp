#include <iostream>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

class Shapes{

public:
	struct basic_pose{
		string x;
	    string y;
	    string z;
	    string pitch;
	    string yaw;
	    string roll;

	    basic_pose(string x="0",string y="0",string z="0",string roll="0"
	    ,string pitch="0",string yaw="0");

	};
	basic_pose pose;

	Shapes(string x,string y,string z,string roll,string pitch,string yaw);
	double image_world_transform(int number_of_row_column,double x_or_y);
	double* pointTransform(double* coordinate);
	double coord_scale_change(double scale,double crd);

};

class Box : public Shapes{

public:

	struct basic_size{

		string x_size;
		string y_size;
		string z_size;

		basic_size(string x="0",string y="0",string z="0");

	};
	basic_size size;

	Box(string x="0",string y="0",string z="0",string roll="0",
		string pitch="0",string yaw="0", string x2="0",string y2="0",string z2="0");

	void put_map(cv::Mat &image,double,bool);
	double** find_disection_vertexes(double** coordinates,double height);
};

class Sphere : public Shapes{

public:

	string radius;

	Sphere(string x="0",string y="0",string z="0",string pitch="0",
		string yaw="0",string roll="0",string r="0");

	void put_map(cv::Mat &image,double);
};

class Cylinder : public Shapes{
public:

	string height;
	string radius;

	Cylinder(string x="0",string y="0",string z="0",string pitch="0",
		string yaw="0",string roll="0",string h="0",string r="0");

	void put_map(cv::Mat &image,double);


};