
#include "model_interface.hpp"
#include <math.h>
#include <Eigen/Dense>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/algorithms/intersection.hpp>

namespace bg=boost::geometry;
using Eigen::MatrixXd;


	Shapes::Shapes(string x="0",string y="0",string z="0",string roll="0",string pitch="0",string yaw="0")
	: pose(x,y,z,roll,pitch,yaw)
	{
	}

	Shapes::basic_pose::basic_pose(string x,string y,string z,string roll,string pitch,string yaw):
	    x(x),y(y),z(z),roll(roll),pitch(pitch),yaw(yaw){}

	

	Box::basic_size::basic_size(string x,string y,string z):
		x_size(x),y_size(y),z_size(z){}

	
	Box::Box(string x,string y,string z,string roll,
		string pitch,string yaw,string x2,string y2,string z2)
		: size(x2,y2,z2),Shapes(x,y,z,roll,pitch,yaw)
		{
		}
	
	double Shapes::image_world_transform(int number_of_row_column,double x_or_y){//0 if row , 1 if column

		double last_coord=number_of_row_column/2.0+x_or_y;

		return last_coord;

	}
	double Shapes::coord_scale_change(double scale,double crd){

		return crd/scale;

	}
	double* Shapes::pointTransform(double* coordinate){

			double roll,pitch,yaw;
			roll=stod(pose.roll);
			pitch=stod(pose.pitch);
			yaw=stod(pose.yaw);
			double x,y,z;
			x=stod(pose.x);
			y=stod(pose.y);
			z=stod(pose.z);

			MatrixXd m(4,4);
			m(0,0)=cos(yaw)*cos(pitch);
			m(0,1)=cos(yaw)*sin(pitch)*sin(roll)-sin(yaw)*cos(roll);
			m(0,2)=cos(yaw)*sin(pitch)*cos(roll)+sin(yaw)*sin(roll);
			m(0,3)=x;
			///////////////////////
			m(1,0)=sin(yaw)*cos(pitch);
			m(1,1)=sin(yaw)*sin(pitch)*sin(roll)+cos(yaw)*cos(roll);
			m(1,2)=sin(yaw)*sin(pitch)*cos(roll)-cos(yaw)*sin(roll);
			m(1,3)=y;
			/////////////////////
			m(2,0)=-sin(pitch);
			m(2,1)=cos(pitch)*sin(roll);
			m(2,2)=cos(pitch)*cos(roll);
			m(2,3)=z;
			////////////////////
			m(3,0)=0;m(3,1)=0;m(3,2)=0;m(3,3)=1;

			MatrixXd point(4,1);
			point(0,0)=coordinate[0];
			point(1,0)=coordinate[1];
			point(2,0)=coordinate[2];
			point(3,0)=1;

			double* new_point=new double[3];
			MatrixXd result=m*point;

			new_point[0]=result(0,0);
			new_point[1]=result(1,0);
			new_point[2]=result(2,0);

			return new_point;


	}
	double** Box::find_disection_vertexes(double** coordinates,double height){

		int under=0;
		for(int i=0;i<8;i++){
			if(coordinates[i][2]<height){
				under++;
			}
		}
		int under_array[under];

		int j=0;
		for(int i=0;i<8;i++){
			if(coordinates[i][2]<height){
				under_array[j]=i;
				j++;
			}
		}
		double number_of_intersection_vertices;

		switch(under){

			case 1:
				number_of_intersection_vertices=3;
				break;
			case 2:
				number_of_intersection_vertices=4;
				break;
			case 3:
				number_of_intersection_vertices=5;
				break;
			case 4:
				number_of_intersection_vertices=4;
				break;
			case 5:
				number_of_intersection_vertices=5;
				break;
			case 6:
				number_of_intersection_vertices=4;
				break;
			case 7:
				number_of_intersection_vertices=3;
				break;
			default:
				number_of_intersection_vertices=0;
		}

		double** return_vertices=new double*[(int)number_of_intersection_vertices+1];
		return_vertices[0]=new double[1];
		return_vertices[0][0]=number_of_intersection_vertices;
		int return_index=1;

		for(int i=0;i<under;i++){

			for(int j=0;j<8;j++){

				int difference=0;

				bool cond1=(j%8<4 && under_array[i]%8<4)||(j%8>=4 && under_array[i]%8>=4);
				bool cond2=(j%4<2 && under_array[i]%4<2)||(j%4>=2 && under_array[i]%4>=2);
				bool cond3=(j%2<1 && under_array[i]%2<1)||(j%2>=1 && under_array[i]%2>=1);

				if(!cond1)
					difference++;
				if(!cond2)
					difference++;
				if(!cond3)
					difference++;

				if(difference==1){

					if(coordinates[j][2]>=height){

						return_vertices[return_index]=new double[2];
						for(int k=0;k<2;k++){

							return_vertices[return_index][k]=coordinates[under_array[i]][k]+(coordinates[j][k]-coordinates[under_array[i]][k])*((height
								-coordinates[under_array[i]][2])/(coordinates[j][2]-coordinates[under_array[i]][2]));

						}

						return_index++;
					}

				}
			}

		}

	return return_vertices;

	}

	void Box::put_map(cv::Mat &image,double scale,bool flag_from_image){

		int row=image.rows;
		int colm=image.cols;		

		double** coord=new double*[8];

		if(!flag_from_image){

			for(int i=0;i<8;i++){

				coord[i]=new double[3];

				if(i%8<4)
					coord[i][0]=stod(size.x_size)/2;
				else
					coord[i][0]=-stod(size.x_size)/2;
				if(i%4<2)
					coord[i][1]=stod(size.y_size)/2;
				else
					coord[i][1]=-stod(size.y_size)/2;
				if(i%2<1)
					coord[i][2]=stod(size.z_size)/2;
				else
					coord[i][2]=-stod(size.z_size)/2;

			}
			double** new_coord=new double*[8];

			for(int i=0;i<8;i++){
				new_coord[i]=new double[3];
				for(int j=0;j<3;j++){
					new_coord[i][j]=pointTransform(coord[i])[j];
				}
			}

			double** vertices=find_disection_vertexes(new_coord,0);

			for(int i=1;i<vertices[0][0]+1;i++){

				
				vertices[i][0]=coord_scale_change(scale,vertices[i][0]);
				vertices[i][1]=coord_scale_change(scale,vertices[i][1]);
				vertices[i][0]=image_world_transform(colm,vertices[i][0]);
				vertices[i][1]=image_world_transform(row,vertices[i][1]);
				
			}

			for(int i=1;i<vertices[0][0]+1;i++)
				cout<<vertices[i][0]<<" "<<vertices[i][1]<<endl;

			Point* list=new Point[(int)vertices[0][0]];
			for(int i=0;i<vertices[0][0];i++){
				int small;
				int small_vec_index;
				int big;
				int big_vector_index;
				bool small_changed=false;
				bool first_non_escape_iter=false;

				if(i==0){
					list[i].x=vertices[i+1][0];
					list[i].y=vertices[i+1][1];
					continue;
				}
				
				for(int j=1;j<vertices[0][0]+1;j++){

					bool escape=false;
					for(int k=0;k<i+1;k++){
						if(list[k].x==vertices[j][0] && list[k].y==vertices[j][1])
							escape=true;
					}
					if(escape){

						continue;
					}

					double cosine=(vertices[j][0]-list[i-1].x)/sqrt(pow(list[i-1].x-vertices[j][0],2)+pow(list[i-1].y-vertices[j][1],2));
					cout<<cosine<<endl;
					if(first_non_escape_iter==false){
						first_non_escape_iter=true;
						small=cosine;
						small_vec_index=j;
						big=cosine;
						big_vector_index=j;
						continue;
					}
					if(cosine<small){
						cout<<"small calisti"<<endl;
						small=cosine;
						small_vec_index=j;
						small_changed=true;
					}
					if(cosine>big){
						cout<<"big calisti"<<endl;
						big=cosine;
						big_vector_index=j;
					}
				}
				cout<<small<<" "<<vertices[small_vec_index][0]<<" "<<big<<" "<<vertices[big_vector_index][0]<<endl;
				cout<<"____________________________"<<endl;
				cout<<vertices[big_vector_index][0]<<"  "<<vertices[big_vector_index][1]<<endl;
				cout<<vertices[small_vec_index][0]<<"  "<<vertices[small_vec_index][1]<<endl;

				if(list[i-1].x==vertices[small_vec_index][0] && list[i-1].y==vertices[small_vec_index][1]){
					list[i].x=vertices[big_vector_index][0];
					list[i].y=vertices[big_vector_index][1];
				}else if(small_changed){
					list[i].x=vertices[small_vec_index][0];
					list[i].y=vertices[small_vec_index][1];
				}else{
					list[i].x=vertices[big_vector_index][0];
					list[i].y=vertices[big_vector_index][1];
				}
				cout<<"___________________________"<<endl;

			}

			for(int i=0;i<vertices[0][0];i++)
				cout<<list[i]<<endl;
			

			for(int i=0;i<vertices[0][0];i++){
				line(image,list[i],list[(i+1)%(int)vertices[0][0]],(255,255,255),3);
			}
		}


		
	

		if(flag_from_image){
		
			Point pt0((stod(pose.x)-stod(size.x_size)/2.0)/scale,(stod(pose.y)+stod(size.y_size)/2.0)/scale);
			Point pt1((stod(pose.x)-stod(size.x_size)/2.0)/scale,(stod(pose.y)-stod(size.y_size)/2.0)/scale);
			Point pt2((stod(pose.x)+stod(size.x_size)/2.0)/scale,(stod(pose.y)+stod(size.y_size)/2.0)/scale);
			Point pt3((stod(pose.x)+stod(size.x_size)/2.0)/scale,(stod(pose.y)-stod(size.y_size)/2.0)/scale);

			line(image, pt0,pt1,(255,255,255),3);
			line(image, pt1,pt3,(255,255,255),3);		
			line(image, pt3,pt2,(255,255,255),3);
			line(image, pt2,pt0,(255,255,255),3);
		}	

	}


	Sphere::Sphere(string x,string y,string z,string pitch,string yaw,string roll,
		string r)
		:radius(r),Shapes(x,y,z,pitch,yaw,roll)
		{}

	void Sphere::put_map(cv::Mat &image,double scale){

		Point center(stod(pose.x)/scale,stod(pose.y)/scale);

		circle(image,center,stod(radius)/scale,(0,255,0),2);

	}


	Cylinder::Cylinder(string x,string y,string z,string pitch,string yaw,string roll,
		string h,string r)
		:height(h),radius(r),Shapes(x,y,z,pitch,yaw,roll)
		{}

	void Cylinder::put_map(cv::Mat &image,double scale){

		Point center((stod(pose.x)/scale,stod(pose.y)/scale));

		circle(image,center,stod(radius)/scale,(0,255,0),2);

	}

