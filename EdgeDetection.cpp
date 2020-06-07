#include <cstdio>
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#define LEVELS 256
#define THRESHOLD 120

using namespace std;
using namespace cv;


int main(int argc, char* argv[]){
	Mat data_src; //source image
	Mat dst; //equalize image
	Mat dst_gx; //x gradient
	Mat dst_gy; //y gradient
	Mat dst_gradient; //x+y
	double hist[LEVELS]; //save number of each pixel value
	int gx[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}}; //mask
	int gy[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; //mask

	if(argc != 2){
		printf("Please enter correct code:\n./hw2 sample1.jpg\nor\n./hw2 sample2.jpg\n");
		return 0;
	}

	data_src = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	dst = Mat::zeros(data_src.rows, data_src.cols, CV_8UC1);
	dst_gx = Mat::zeros(data_src.rows, data_src.cols, CV_8UC1);
	dst_gy = Mat::zeros(data_src.rows, data_src.cols, CV_8UC1);
	dst_gradient = Mat::zeros(data_src.rows, data_src.cols, CV_8UC1);
	
	/*initialization*/
	for(int i=0; i<LEVELS; i++){
		hist[i] = 0;
	}
	
	/*count number of pixel value*/
	for(int i=0; i<data_src.rows; i++){
		for(int j=0; j<data_src.cols; j++){
			hist[data_src.at<uchar>(i,j)]++;
		}
	}

	/* count equalization number( hist[i] = j => number i is repace of j) */
	int pixelCount = data_src.rows * data_src.cols;

	//cumulation of his[]
	for(int i=1; i<LEVELS; i++){
		hist[i] = hist[i-1] + hist[i];
	}

	//calculate
	for(int i=0; i<LEVELS; i++){
		hist[i] = (hist[i]/pixelCount) * (LEVELS-1); //calculate replace number
		hist[i] = (int) (hist[i] + 0.5); //rounding to int
	}


	//replace
	for(int i=0; i<data_src.rows; i++){
		for(int j=0; j<data_src.cols; j++){
			//dst.at<uchar>(i,j) = hist[data_src.at<uchar>(i,j)];
			dst.at<uchar>(i,j) = data_src.at<uchar>(i,j);
		}
	}

	/*gaussian filter*/
	GaussianBlur(dst, dst, Size(3,3), 0, 0, BORDER_DEFAULT);


	/*implement sobel edge detection*/
	//calculate x gradient
	double tmp;	
	for(int i=1; i<data_src.rows-1; i++){
		for(int j=1; j<data_src.cols-1; j++){
			tmp = 0.0;
			for(int maskX=-1; maskX<=1; maskX++){
				for(int maskY=-1; maskY<=1; maskY++){
					tmp = tmp + (dst.at<uchar>(i+maskX,j+maskY) * gx[1+maskX][1+maskY]);
				}
			}
			dst_gx.at<uchar>(i,j) = abs(tmp);
		}
	}


	//calculate y gradient
	for(int i=1; i<data_src.rows-1; i++){
		for(int j=1; j<data_src.cols-1; j++){
			tmp = 0.0;
			for(int maskX=-1; maskX<=1; maskX++){
				for(int maskY=-1; maskY<=1; maskY++){
					tmp = tmp + (dst.at<uchar>(i+maskX,j+maskY) * gy[1+maskX][1+maskY]);
				}
			}
			dst_gy.at<uchar>(i,j) = abs(tmp);
		}
	}
	
	//add x gradient and y gradient
	for(int i=0; i<data_src.rows; i++){
		for(int j=0; j<data_src.cols; j++){
			if(dst_gx.at<uchar>(i,j) + dst_gy.at<uchar>(i,j) > THRESHOLD){
				dst_gradient.at<uchar>(i,j) = dst_gx.at<uchar>(i,j) + dst_gy.at<uchar>(i,j);
			}
		}
	}

	/*show histogram image and save*/
	imwrite("equalize.jpg",dst);
	imwrite("edge_detect.jpg",dst_gradient);

	namedWindow("origin",CV_WINDOW_AUTOSIZE);
	imshow("origin",data_src);
	imshow("equalize",dst);
	imshow("edge detection",dst_gradient);




	waitKey(0);


}
