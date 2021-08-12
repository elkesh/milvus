
#include "model_interface.hpp"



	Shapes::Shapes(string x="0",string y="0",string z="0",string pitch="0",string yaw="0",string roll="0")
	: pose(x,y,z,pitch,yaw,roll)
	{
	}

	Shapes::basic_pose::basic_pose(string x,string y,string z,string pitch,string yaw,string roll):
	    x(x),y(y),z(z),pitch(pitch),yaw(yaw),roll(roll){}

	Box::basic_size::basic_size(string x,string y,string z):
		x_size(x),y_size(y),z_size(z){}

	
	Box::Box(string x="0",string y="0",string z="0",string pitch="0",
		string yaw="0",string roll="0",string x2,string y2,string z2)
		: size(x2,y2,z2),Shapes(x,y,z,pitch,yaw,roll)
		{
		}
		/*
	int Box::image_world_transform(int pt,bool row_colm,int number_of_rowcolumn){//0 if row , 1 if column

		if(row_colm==0){
			return number_of_rowcolumn/2-pt;
		}else{
			return number_of_rowcolumn/2+pt;
		}

	}*/

	void Box::put_map(cv::Mat &image,double scale){

		int row=image.rows;
		int colm=image.cols;



		Point pt0((stod(pose.x)-stod(size.x_size)/2.0)/scale,(stod(pose.y)+stod(size.y_size)/2.0)/scale);
		Point pt1((stod(pose.x)-stod(size.x_size)/2.0)/scale,(stod(pose.y)-stod(size.y_size)/2.0)/scale);
		Point pt2((stod(pose.x)+stod(size.x_size)/2.0)/scale,(stod(pose.y)+stod(size.y_size)/2.0)/scale);
		Point pt3((stod(pose.x)+stod(size.x_size)/2.0)/scale,(stod(pose.y)-stod(size.y_size)/2.0)/scale);

		line(image, pt0,pt1,(255,255,255),3);
		line(image, pt1,pt3,(255,255,255),3);		
		line(image, pt3,pt2,(255,255,255),3);
		line(image, pt2,pt0,(255,255,255),3);		
		
		

	}


	Sphere::Sphere(string x,string y,string z,string pitch,string yaw,string roll,
		string r)
		:radius(r),Shapes(x,y,z,pitch,yaw,roll)
		{}

	void Sphere::put_map(cv::Mat &image,double scale){

		circle(image,(stod(pose.x)/scale,stod(pose.y)/scale),radius/scale,(0,255,0),2);

	}


	Cylinder::Cylinder(string x,string y,string z,string pitch,string yaw,string roll,
		string h,string r)
		:height(h),radius(r),Shapes(x,y,z,pitch,yaw,roll)
		{}

	void Cylinder::put_map(cv::Mat &image,double scale){



		circle(image,(stod(pose.x)/scale,stod(pose.y)/scale),radius/scale,(0,255,0),2);

	}

