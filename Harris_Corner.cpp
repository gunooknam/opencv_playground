#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
Mat src, src_gray;
int thresh = 200;
const char* source_window = "Source image";
const char* corners_window = "Corners detected";
void cornerHarris_demo(int thresh);

int main()
{
	src = imread("hello.jpg");
	if (src.empty()) { cout << "Could not open or find the image!\n" << endl; return -1; }
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	namedWindow(source_window);
	imshow(source_window, src);
	cornerHarris_demo(200);
	waitKey();
	return 0;
}

void cornerHarris_demo(int thresh) 
{
	// thresh 가 인자는 thresh 그대로 들어옴
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	Mat dst = Mat::zeros(src.size(), CV_32FC1); //
	Mat dst_norm, dst_norm_scaled;
	cornerHarris(src_gray, dst, blockSize, apertureSize, k);

	// 0~255 범위로 바꾼다. 
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);
	
	for (int i = 0; i < dst_norm.rows; i++) {
		for (int j = 0; j < dst_norm.cols; j++) {
			if ((int)dst_norm.at<float>(i, j) > thresh) {
				circle( dst_norm_scaled, Point(j, i), 5, Scalar(0), 2, 8, 0);
			}
		}
	}
	namedWindow(corners_window);
	imshow(corners_window, dst_norm_scaled); 
}
