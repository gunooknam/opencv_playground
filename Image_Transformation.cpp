#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요

using namespace std;
using namespace cv;

struct MouseParams {
	Mat img; 
	vector<Point2f> in, out;
};

static void onMouse(int event, int x, int y, int, void* param) {
	MouseParams * mp = (MouseParams*) param;
	Mat img = mp->img;
	if (event == EVENT_LBUTTONDOWN) {
		Mat result;
		mp->in.push_back(Point2f(x, y));
		if (mp->in.size() == 4) {
			// 점 (x,  y )의 해당하는 4개의 좌표
			// 점 (x', y')의 해당하는 4개의 좌표로 맵핑하는
			// Transform Matrix를 구함
			// 내가 찍은 in 좌표들을 out의 4점으로 변환하는 matrix 구함
			Mat homo_mat = getPerspectiveTransform(mp->in, mp->out);								
			warpPerspective(img, result, homo_mat, Size(img.cols, img.rows));
			// img를 homo_mat함수로 와핑 
			imshow("output", result);
			mp->in.clear();
		}
		else {
			result = img.clone();
			for (size_t i = 0; i < mp->in.size(); i++) {
				circle(result, mp->in[i], 3, Scalar(0, 0, 255), 5);
			}
			imshow("input", result);
		}
	}
	if (event == EVENT_RBUTTONDOWN) {
		mp->in.clear();
		imshow("input", img);
	}
}

int main() {
	Mat input = imread("hello.jpg");
	imshow("input", input);
	MouseParams mp;
	mp.out.push_back(Point2f(0, 0));
	mp.out.push_back(Point2f(500, 0));
	mp.out.push_back(Point2f(500, 500));
	mp.out.push_back(Point2f(0, 500));
	mp.img = input;
	setMouseCallback("input", onMouse, (void*)&mp);
	waitKey(0);
	return 0;
}