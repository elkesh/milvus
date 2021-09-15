#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;


void generate_map(string model, const char* pose,Mat &image,double scale);
char** string_parser(const char* head, int size);
bool compare_string_with_ptrtochar(string a,const char* b);