#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
#include <time.h>
#include <opencv2/video/background_segm.hpp>

using namespace cv;
using namespace std;

// Object의 개수를 세는 코드 
// 
int main() {
	// 1. grayscale로 변환
	Mat gray = imread("pic4.png", IMREAD_GRAYSCALE);
	Mat result;
	// 2. threshold를 주어서 180 넘는 것을 0으로
	threshold(gray, result, 180, 255, THRESH_BINARY_INV);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// contours에 result를 담는다. 
	findContours(result,
				 contours,
				 hierarchy, // 계층적 관계
				 CV_RETR_EXTERNAL,
				 CV_CHAIN_APPROX_SIMPLE);
	// contours가 담겨진다. 객체의 개체들이 담겨진다.
	putText(result,
		    format("contour count: %d", contours.size()),
		    Point(50, 80),
		    FONT_HERSHEY_SIMPLEX,
		    1,
		    Scalar(128),4);
	imshow("contours", result);
	waitKey(0);

}