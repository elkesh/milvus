#include<opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "rectangles_interface.hpp"
#include "xml_interface.hpp"
#include "world_interface.hpp"
using namespace std;
using namespace cv;
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

    generator((double)rows, (double)cols,scale);


    imwrite("result.jpg",image);
    
    
    
    
    return 0;
}