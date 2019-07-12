#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize �Լ� ����� ����
#include <iostream>

using namespace std;
using namespace cv;


void onMouse(int event, int x, int y, int flags, void*params);

int main() {

	Mat image;
	// imread �� ���� �ݵ�� ���ϸ�� ���� �������� �ҷ����� �� ���Ѵ�. 
	image = imread("hello.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("bonobono", image);
	setMouseCallback("bonobono", onMouse, reinterpret_cast<void*>(&image));
	waitKey(0);
}
// ���콺 �̺�Ʈ
// x �� y�� ��ǥ ��ġ�� ���´�. 
void onMouse(int event, int x, int y, int flags, void* params) {
	Mat * im = reinterpret_cast<Mat*>(params);
	switch (event) {	
	case CV_EVENT_LBUTTONDOWN:
		cout << "x ��ǥ�� ����" << x << "y ��ǥ�� ����" << y << endl;
		cout << static_cast<int>(im->at<uchar>(50,50)) << endl; // static cast�� int�� ���ϸ� �ƹ��͵� �ȶ��.
		cout << "(x,y) is " << static_cast<int>(im->at<uchar>(Point(x, y))) << endl;
		break; // im->at(x,y) �̷��� �־��ָ� �̾ƿ´�.
	case CV_EVENT_RBUTTONDOWN:
		std::cout << "Hello!" << std::endl; // Hello! ���
		break;
	default :
		break;
	}
}