#include<opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "rectangles_interface.hpp"
#include "xml_interface.hpp"
using namespace std;
using namespace cv;
int main(){

    cv::Mat image;
    cv::Mat binary_image;
    image = cv::imread("deneme.jpg");
    threshold(image,binary_image,100,255,CV_THRESH_BINARY);


    int *a=generate_borders(binary_image);

    parser_func(a);



    imshow("Image",image);
    waitKey(0);
    
    
    return 0;
}