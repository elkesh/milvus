#include <iostream>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
class Shapes{

public:
  struct basic_pose{
      std::string x;
      std::string y;
      std::string z;
      std::string pitch;
      std::string yaw;
      std::string roll;

      basic_pose(std::string x="0",std::string y="0",std::string z="0",std::string roll="0"
      ,std::string pitch="0",std::string yaw="0");

  };
  basic_pose pose;

  Shapes(std::string x,std::string y,std::string z,std::string roll,std::string pitch,std::string yaw);
  double image_world_transform(int number_of_row_column,double x_or_y);
  double* pointTransform(double* coordinate);
  double coord_scale_change(double scale,double crd);

};

class Box : public Shapes{

public:

  struct basic_size{

    std::string x_size;
    std::string y_size;
    std::string z_size;

    basic_size(std::string x="0",std::string y="0",std::string z="0");

  };
  basic_size size;

  Box(std::string x="0",std::string y="0",std::string z="0",std::string roll="0",
    std::string pitch="0",std::string yaw="0", std::string x2="0",std::string y2="0",std::string z2="0");

  void put_map(cv::Mat &image,double,bool);
  double** find_disection_vertexes(double** coordinates,double height);
};

class Sphere : public Shapes{

public:

  std::string radius;

  Sphere(std::string x="0",std::string y="0",std::string z="0",std::string pitch="0",
    std::string yaw="0",std::string roll="0",std::string r="0");

  void put_map(cv::Mat &image,double,double height);
};

class Cylinder : public Shapes{
public:

  std::string height;
  std::string radius;

  Cylinder(std::string x="0",std::string y="0",std::string z="0",std::string pitch="0",
    std::string yaw="0",std::string roll="0",std::string r="0",std::string h="0");

  void put_map(cv::Mat &image,double,double);


};

class Mesh : public Shapes{
public:
  Mesh(std::string x="0",std::string y="0",std::string z="0",std::string pitch="0",
    std::string yaw="0",std::string roll="0");

  void put_map(cv::Mat &image,double,std::string filepath,double height);
};