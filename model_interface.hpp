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

	    basic_pose(string x="0",string y="0",string z="0",string pitch="0"
	    ,string yaw="0",string roll="0");

	};
	basic_pose pose;

	Shapes(string x,string y,string z,string pitch,string yaw,string roll);

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

	Box(string x="0",string y="0",string z="0",string pitch="0",
		string yaw="0",string roll="0", string x2="0",string y2="0",string z2="0");
	int image_world_transform(int pt,bool row_colm,int number_of_rowcolumn);

	void put_map(cv::Mat &image,double);
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