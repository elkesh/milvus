#include "rectangles_interface.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int** generate_borders(cv::Mat image){



	int rows = image.rows;
    int cols = image.cols;

    int i;
    int j;
    int** rectangles=0;
    rectangles=new int*[500];//buraya bak
    rectangles[0]=new int[4];
    //int rectangles[500][4];//0:x1 1:y1 2:x2 3:y2
    int last_row[100][2]={0};
    int rectangle_index=0;
    int last_row_index=0;
    
    int count=0;

    for(i=0;i<rows;i++){

        for(j=0;j<cols;j++){

            if(image.at<Vec3b>(i,j)[0]==0){
                
                last_row[last_row_index][0]=j;

                while(true){

                    

                    j++;
                    if(image.at<Vec3b>(i,j+1)[0]!=0){

                        last_row[last_row_index][1]=j;
                        last_row_index++;
                        break;
                    }
                    if(j==cols){
                        last_row[last_row_index][1]=j;
                        last_row_index++;
                        break;
                    }
                     
               
                } 



            }


        }
        
        int k;
        for(j=0;j<last_row_index;j++){
            for(k=0;k<=rectangle_index;k++){

                
                
                if(rectangles[k][3]==(i-1) && rectangles[k][0]==last_row[j][0] && rectangles[k][2]==last_row[j][1]){                    
                    rectangles[k][3]++;
                    break;                    
                }else if(k==rectangle_index){
                    
                    rectangles[rectangle_index][0]=last_row[j][0];
                    rectangles[rectangle_index][1]=i;
                    rectangles[rectangle_index][2]=last_row[j][1];
                    rectangles[rectangle_index][3]=i;
                    rectangle_index++;
                    rectangles[rectangle_index]=new int[4];
                    break;
                }                                     

            }
             

        }

        

        
    
        last_row_index=0;

        
    }

    delete rectangles[rectangle_index];
    rectangle_index--;
    
    for(i=0;i<=rectangle_index;i++){

        cout<<i<<endl;
        cout<<"x1:"<<rectangles[i][0]<<endl;
        cout<<"y1:"<<rectangles[i][1]<<endl;
        cout<<"x2:"<<rectangles[i][2]<<endl;
        cout<<"y2:"<<rectangles[i][3]<<endl;
        int x1=rectangles[i][0];
        int y1=rectangles[i][1];
        int x2=rectangles[i][2];
        int y2=rectangles[i][3];


        Point pt1(x1,y1);
        Point pt2(x2,y2);

        //rectangle(image,pt1,pt2,cv::Scalar(0,255,0),1);

    }

    imwrite("result.jpg",image);
    
    
    rectangles[rectangle_index+1]=new int[4];
    rectangles[rectangle_index+1][0]=-123;

    
    
   
    return rectangles;


}