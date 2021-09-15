#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

void generate_map(std::string model, const char* pose,cv::Mat &image,double scale);
char** string_parser(const char* head, int size);
bool compare_string_with_ptrtochar(std::string a,const char* b);