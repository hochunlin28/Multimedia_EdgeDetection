#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(){
	Mat src = imread("hw2_1.png",CV_LOAD_IMAGE_COLOR);
	imshow("original",src);
	return 0;
}
