#include<opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "../include/rectangles_interface.hpp"
#include "../include/xml_interface.hpp"
#include "../include/world_interface.hpp"
#include "../tinyxml2.h"


//using namespace cv;

std::vector<double> gauss(std::vector< std::vector<double> > A) {
    int n = A.size();

    for (int i=0; i<n; i++) {
        // Search for maximum in this column
        double maxEl = abs(A[i][i]);
        int maxRow = i;
        for (int k=i+1; k<n; k++) {
            if (abs(A[k][i]) > maxEl) {
                maxEl = abs(A[k][i]);
                maxRow = k;
            }
        }
        // Swap maximum row with current row (column by column)
        for (int k=i; k<n+1;k++) {
            double tmp = A[maxRow][k];
            A[maxRow][k] = A[i][k];
            A[i][k] = tmp;
        }
        // Make all rows below this one 0 in current column
        for (int k=i+1; k<n; k++) {
            double c = -A[k][i]/A[i][i];
            for (int j=i; j<n+1; j++) {
                if (i==j) {
                    A[k][j] = 0;
                } else {
                    A[k][j] += c * A[i][j];
                }
            }
        }
    }
    // Solve equation Ax=b for an upper triangular matrix A
    std::vector<double> x(n);
    for (int i=n-1; i>=0; i--) {
        x[i] = A[i][n]/A[i][i];
        for (int k=i-1;k>=0; k--) {
            A[k][n] -= A[k][i] * x[i];
        }
    }
    return x;
}

int main(){
/*sil!!!!!!!!!!!!!!!!!!!!!!
    vector<double> line(3);
    vector< vector<double> > A(2,line);
    A[0][0]=1;A[0][1]=2;A[0][2]=0;
    A[1][0]=2;A[1][1]=-2;A[1][2]=6;
    vector<double>x(2);x=gauss(A);
    cout<<x[0]<<"  "<<x[1]<<endl;*/    
    cv::Mat image;
    cv::Mat binary_image;
    image = cv::imread("../deneme.jpg");

    flip(image,image,0);
    int rows = image.rows;
    int cols = image.cols;

    threshold(image,binary_image,100,255,CV_THRESH_BINARY);

    double scale;
    std::cout<<"Enter the scale between the image and simulation world scale(meter per pixel)"<<std::endl;
    std::cin>>scale;

    int **ptr_to_rect=generate_borders(binary_image);
    
    parser_func(ptr_to_rect,scale,image);

    generator((double)rows, (double)cols,scale,image);

    flip(image,image,0);

    for(int i=0;i<rows;i++){

        image.at<cv::Vec3b>(i,cols/2)=cv::Vec3b(0,0,255);

    }for(int i=0;i<cols;i++){

        image.at<cv::Vec3b>(rows/2,i)=cv::Vec3b(0,0,255);
    }
    imshow("result",image);
    cv::waitKey(0);
    imwrite("result.jpg",image);

    return 0;
}