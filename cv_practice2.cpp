#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize 함수 사용을 위해
#include <iostream>

using namespace std;
using namespace cv;


void onMouse(int event, int x, int y, int flags, void*params);

int main() {

	Mat image;
	// imread 할 때는 반드시 파일명과 무슨 포맷으로 불러올지 를 정한다. 
	image = imread("hello.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("bonobono", image);
	setMouseCallback("bonobono", onMouse, reinterpret_cast<void*>(&image));
	waitKey(0);
}
// 마우스 이벤트
// x 와 y는 좌표 위치가 들어온다. 
void onMouse(int event, int x, int y, int flags, void* params) {
	Mat * im = reinterpret_cast<Mat*>(params);
	switch (event) {	
	case CV_EVENT_LBUTTONDOWN:
		cout << "x 좌표는 여기" << x << "y 좌표는 여기" << y << endl;
		cout << static_cast<int>(im->at<uchar>(50,50)) << endl; // static cast를 int로 안하면 아무것도 안뜬다.
		cout << "(x,y) is " << static_cast<int>(im->at<uchar>(Point(x, y))) << endl;
		break; // im->at(x,y) 이렇게 넣어주면 뽑아온다.
	case CV_EVENT_RBUTTONDOWN:
		std::cout << "Hello!" << std::endl; // Hello! 출력
		break;
	default :
		break;
	}
}