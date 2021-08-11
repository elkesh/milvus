
#include "model_interface.hpp"



	Shapes::Shapes(string &x="0",string &y="0",string &z="0",string &pitch="0",string &yaw="0",string &roll="0")
	: basic_pose.x{x},basic_pose.y{y},basic_pose.z{z},basic_pose.pitch{pitch},
	basic_pose.yaw{yaw},basic_pose.roll{roll}
	{
	}

	
	Box::Box(string x="0",string y="0",string z="0")
		: basic_size.x_size{x},basic_size.y_size{y},basic_size.z_size{z}
		{
		}
	int Box::image_world_transform(int pt,bool row_colm,int number_of_rowcolumn){//0 if row , 1 if column

		if(row_colm==0){
			return number_of_rowcolumn/2-pt;
		}else{
			return number_of_rowcolumn/2+pt;
		}

	}

	void Box::put_map(cv::Mat &image){

		int row=image.rows;
		int colm=image.cols;



		Point pt0(stoi(basic_pose.x-basic_size.x_size/2),stoi(basic_pose.y+basic_size.y_size/2));
		Point pt1(stoi(basic_pose.x-basic_size.x_size/2),stoi(basic_pose.y-basic_size.y_size/2));
		Point pt2(stoi(basic_pose.x+basic_size.x_size/2),stoi(basic_pose.y+basic_size.y_size/2));
		Point pt3(stoi(basic_pose.x+basic_size.x_size/2),stoi(basic_pose.y-basic_size.y_size/2));

		line(image, pt0,pt1,(255,255,255),3);
		line(image, pt1,pt2,(255,255,255),3);		
		line(image, pt2,pt3,(255,255,255),3);
		line(image, pt3,pt0,(255,255,255),3);		
		
		

	}


	Sphere::Sphere(string r="0")
		:radius{r}
		{}

	Cylinder::Cylinder(string h="0",string r="0")
		:height{h},radius{r}
		{}