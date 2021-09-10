
#include "model_interface.hpp"
#include <math.h>
#include <Eigen/Dense>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <fstream>
#include "xml_interface.hpp"
#include "map_generator.hpp"

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
		{}
	
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
		double** return_vertices=new double*[8];//8 yerine max sayıyı koy.
		return_vertices[0]=new double[1];
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
	return_vertices[0][0]=return_index-1;
	return_vertices[return_index]=new double[1];
	return_vertices[return_index][0]=-1;
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
				vertices[i][0]=(int)image_world_transform(colm,vertices[i][0]);
				vertices[i][1]=(int)image_world_transform(row,vertices[i][1]);				
			}
			bool break_flag=false;			

			for(int i=1;i<vertices[0][0]+1;i++)
				cout<<vertices[i][0]<<" "<<vertices[i][1]<<endl;

			Point* list=new Point[(int)vertices[0][0]];
			for(int i=0;i<vertices[0][0];i++){

				if(i==0){
					list[i].x=vertices[i+1][0];
					list[i].y=vertices[i+1][1];
					continue;
				}
				double angle[(int)vertices[0][0]-1][2];
				int angle_index=0;
				for(int j=1;j<vertices[0][0]+1;j++){

					if(list[i-1].x==vertices[j][0] && list[i-1].y==vertices[j][1])
						continue;
					angle[angle_index][0]=j;					
					double cosine=(vertices[j][0]-list[i-1].x)/sqrt(pow(list[i-1].x-vertices[j][0],2)+pow(list[i-1].y-vertices[j][1],2));
					double sine=(vertices[j][1]-list[i-1].y)/sqrt(pow(list[i-1].x-vertices[j][0],2)+pow(list[i-1].y-vertices[j][1],2));
					if(asin(sine)<0){
						angle[angle_index][1]=2*3.14-acos(cosine);
					}else
						angle[angle_index][1]=acos(cosine);
					angle_index++;
				}
				for(int j=0;j<vertices[0][0]-2;j++){
					int min=j;
					for(int k=j+1;k<vertices[0][0]-1;k++)
						if(angle[k][1]<angle[min][1])
							min=k;
					double tmp1=angle[j][0],tmp2=angle[j][1];
					angle[j][0]=angle[min][0];
					angle[j][1]=angle[min][1];
					angle[min][0]=tmp1;
					angle[min][1]=tmp2;
						
				}
				int number_of_angles_between=0;
							
				bool break_var=false;
				for(int j=0;j<vertices[0][0]-1;j++){
					for(int k=0;k<vertices[0][0]-1;k++){
						bool first_if=true;
						bool second_if=true;
						if(j==k)
							continue;
						for(int z=0;z<vertices[0][0]-1;z++){
							if(z==j || z==k)
								continue;
							if(angle[j][1]>angle[k][1]){
								if((angle[j][1]-angle[k][1])<3.14 && (angle[z][1]<angle[j][1] && angle[z][1]>angle[k][1]) && first_if){
									number_of_angles_between++;
									second_if=false;
								}
								if((angle[j][1]-angle[k][1])>3.14 && (angle[z][1]>angle[j][1] || angle[z][1]<angle[k][1]) && second_if){
									number_of_angles_between++;
									first_if=false;
								}
							}
							else{
								if((angle[k][1]-angle[j][1])<3.14 && (angle[z][1]<angle[k][1] && angle[z][1]>angle[j][1]) && first_if){
									number_of_angles_between++;
									second_if=false;
								}
								if((angle[k][1]-angle[j][1])>3.14 && (angle[z][1]>angle[k][1] || angle[z][1]<angle[j][1]) && second_if){
									number_of_angles_between++;
									first_if=false;
								}
							}
						}
						if(number_of_angles_between==(vertices[0][0]-3)){
							angle[0][0]=angle[j][0];
							angle[0][1]=angle[j][1];
							angle[(int)vertices[0][0]-2][0]=angle[k][0];
							angle[(int)vertices[0][0]-2][1]=angle[k][1];
							break_var=true;
						}
						number_of_angles_between=0;
						if(break_var)
							break;
					}
					if(break_var)
						break;
				}
				if(i>=2)
					if(list[i-2].x==vertices[(int)angle[0][0]][0] && list[i-2].y==vertices[(int)angle[0][0]][1]){
						list[i].x=vertices[(int)angle[(int)vertices[0][0]-2][0]][0];
						list[i].y=vertices[(int)angle[(int)vertices[0][0]-2][0]][1];
					}else{
						list[i].x=vertices[(int)angle[0][0]][0];
						list[i].y=vertices[(int)angle[0][0]][1];
					}
				else{
					list[i].x=vertices[(int)angle[0][0]][0];
					list[i].y=vertices[(int)angle[0][0]][1];
				}
			}
			for(int i=0;i<vertices[0][0];i++){
				line(image,list[i],list[(i+1)%(int)vertices[0][0]],(255,255,255),1);
			}
		}

		if(flag_from_image){
		
			Point pt0((stod(pose.x)-stod(size.x_size)/2.0)/scale,(stod(pose.y)+stod(size.y_size)/2.0)/scale);
			Point pt1((stod(pose.x)-stod(size.x_size)/2.0)/scale,(stod(pose.y)-stod(size.y_size)/2.0)/scale);
			Point pt2((stod(pose.x)+stod(size.x_size)/2.0)/scale,(stod(pose.y)+stod(size.y_size)/2.0)/scale);
			Point pt3((stod(pose.x)+stod(size.x_size)/2.0)/scale,(stod(pose.y)-stod(size.y_size)/2.0)/scale);

			line(image, pt0,pt1,(255,255,255),1);
			line(image, pt1,pt3,(255,255,255),1);		
			line(image, pt3,pt2,(255,255,255),1);
			line(image, pt2,pt0,(255,255,255),1);
		}	
	}

	Sphere::Sphere(string x,string y,string z,string pitch,string yaw,string roll,
		string r)
		:radius(r),Shapes(x,y,z,pitch,yaw,roll)
		{}

	void Sphere::put_map(cv::Mat &image,double scale,double height){

		int row=image.rows;
		int colm=image.cols;

		Point center(image_world_transform(colm,coord_scale_change(scale,stod(pose.x))),image_world_transform(row,coord_scale_change(scale,stod(pose.y))));
		double difference=abs(stod(pose.z)-height);
		double small_r;
		cout<<difference<<endl;
		cout<<pose.z<<endl;
		cout<<radius<<endl;
		if(difference<stod(radius)){
			cout<<"yes"<<endl;
			small_r=sqrt(pow(stod(radius),2)-pow(difference,2));
			cout<<small_r/scale<<endl;
			cout<<center<<endl;
			circle(image,center,small_r/scale,(255,255,255),2);
		}
	}


	Cylinder::Cylinder(string x,string y,string z,string pitch,string yaw,string roll,
		string h,string r)
		:height(h),radius(r),Shapes(x,y,z,pitch,yaw,roll)
		{}

	void Cylinder::put_map(cv::Mat &image,double scale){

		Point center((stod(pose.x)/scale,stod(pose.y)/scale));

		circle(image,center,stod(radius)/scale,(0,255,0),2);

	}
	Mesh::Mesh(string x,string y,string z,string roll,
		string pitch,string yaw)
		: Shapes(x,y,z,roll,pitch,yaw)
		{
		}

	void Mesh::put_map(cv::Mat &image,double scale,string filepath,double height){
		int row=image.rows;
		int colm=image.cols;

		char* whole_line=new char[200];
		ifstream infile;
		infile.open(caster(filepath));
		Point three_points[3];
		//double z_pose[3];
		bool start_flag=true;
		int count_of_vertexes=0;

		while(!infile.eof()){

			infile.getline(whole_line,100);
			char** line_disected=string_parser(whole_line,10);

			if(compare_string_with_ptrtochar("vertex",line_disected[0])){

				double x=stod(line_disected[1]);
				double y=stod(line_disected[2]);
				double z=stod(line_disected[3]);

				double* array_to_send=new double[3];
				array_to_send[0]=x;
				array_to_send[1]=y;
				array_to_send[2]=z;
				double array_to_return[3];
				for(int q=0;q<3;q++){
					array_to_return[q]=pointTransform(array_to_send)[q];
				}
				
				for(int q=0;q<2;q++){
					array_to_return[q]=coord_scale_change(scale,array_to_return[q]);
					if(q==0)
						array_to_return[q]=(int)image_world_transform(colm,array_to_return[q]);
					else if(q==1)
						array_to_return[q]=(int)image_world_transform(row,array_to_return[q]);
				}
				//z_pose[count_of_vertexes]=array_to_return[2];
				three_points[count_of_vertexes].x=array_to_return[0];
				three_points[count_of_vertexes].y=array_to_return[1];
				count_of_vertexes++;

				//bool under=false;
				//bool over=false;

				if(count_of_vertexes==3){/*
					for(int q=0;q<3;q++){
						if(z_pose[q]>height)
							bool over=true;
						else
							bool under=true;
					}
					if(over && under)*/
					for(int q=0;q<3;q++){
						line(image,three_points[q],three_points[(q+1)%3],(255,255,255),1);
					}
					count_of_vertexes=0;						
				}
			}
		}
	}

				

		



