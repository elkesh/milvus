#include <iostream>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

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
	};
	basic_pose pose;

	Shapes(string &x,string &y,string &z,string &pitch,string &yaw,string &roll);

};

class Box : public Shapes{

public:

	struct basic_size{

		string x_size;
		string y_size;
		string z_size;

	};
	basic_size size;

	Box(string x="0",string y="0",string z="0");
	int image_world_transform(int pt,bool row_colm,int number_of_rowcolumn);

	void put_map(cv::Mat &image);};

class Sphere : public Shapes{

public:

	string radius;

	Sphere(string r);
};

class Cylinder : public Shapes{
public:

	string height;
	string radius;

	Cylinder(string h,string r);


};