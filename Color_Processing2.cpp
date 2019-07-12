#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
#include <time.h>

using namespace cv;
using namespace std;

// applyColorMap �� ���ؼ� 0~255 ��� ����
// Į�� ������ �����ϴ�. COLORMAP_JET�� ���� ������~
//    -> �� ~ ������ visualizing �����ϴ�.
 
int main() {
	Mat gray = imread("china.jpg", IMREAD_GRAYSCALE);
	Mat color;

	// 1. Color Mapping 
	applyColorMap(gray, color, COLORMAP_JET);
	imshow("gray", gray);
	imshow("image", color);
	waitKey(0);

	// 2. Color Balancing 
	Mat image = imread("china.jpg", IMREAD_COLOR);
	Mat result;
	vector<Mat> ch(3);
	int b_sum = 0, g_sum = 0, r_sum = 0;
	int b_avg = 0, g_avg = 0, r_avg = 0;
	if (image.empty()) {
		cerr << "read fail" << endl;
		exit(-1);
	}

	int rows = image.rows;
	int cols = image.cols;
	int pixno = rows * cols;




	return 0;
}