#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;


void generate_map(string model, const char* pose,Mat &image,double scale);