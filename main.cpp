#include<opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "rectangles_interface.hpp"
#include "xml_interface.hpp"
#include "world_interface.hpp"
#include "tinyxml2.h"
using namespace tinyxml2;
using namespace std;
using namespace cv;
/*
static void onMouse(int event, int x, int y, int flags, void* param) // now it's in param
{
    Mat &xyz = *((Mat*)param); //cast and deref the param

    if (event == EVENT_LBUTTONDOWN)
    {
        short val = xyz.at< short >(y,x); // opencv is row-major ! 
        cout << "x= " << x << " y= " << y << "val= "<<val<< endl;
    }
}
*/
int main(){
    
    cv::Mat image;
    cv::Mat binary_image;
    image = cv::imread("deneme.jpg");

    flip(image,image,0);
    int rows = image.rows;
    int cols = image.cols;

    threshold(image,binary_image,100,255,CV_THRESH_BINARY);

    double scale;
    cout<<"Enter the scale between the image and simulation world scale(meter per pixel)"<<endl;
    cin>>scale;

    int **ptr_to_rect=generate_borders(binary_image);
    
    parser_func(ptr_to_rect,scale,image);

    generator((double)rows, (double)cols,scale,image);

    flip(image,image,0);
    for(int i=0;i<rows;i++){

        image.at<Vec3b>(i,cols/2)=Vec3b(0,0,255);

    }for(int i=0;i<cols;i++){

        image.at<Vec3b>(rows/2,i)=Vec3b(0,0,255);
    }
    imshow("uras",image);
    waitKey(0);
    imwrite("result.jpg",image);

    
    return 0;
}