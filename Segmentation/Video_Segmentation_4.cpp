#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
#include <time.h>
#include <opencv2/video/background_segm.hpp>

using namespace cv;
using namespace std;

// Object�� ������ ���� �ڵ� 
// 
int main() {
	// 1. grayscale�� ��ȯ
	Mat gray = imread("pic4.png", IMREAD_GRAYSCALE);
	Mat result;
	// 2. threshold�� �־ 180 �Ѵ� ���� 0����
	threshold(gray, result, 180, 255, THRESH_BINARY_INV);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// contours�� result�� ��´�. 
	findContours(result,
				 contours,
				 hierarchy, // ������ ����
				 CV_RETR_EXTERNAL,
				 CV_CHAIN_APPROX_SIMPLE);
	// contours�� �������. ��ü�� ��ü���� �������.
	putText(result,
		    format("contour count: %d", contours.size()),
		    Point(50, 80),
		    FONT_HERSHEY_SIMPLEX,
		    1,
		    Scalar(128),4);
	imshow("contours", result);
	waitKey(0);

}